#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <stdio.h>

typedef struct addr_t {
  sa_family_t family;
  union {
    struct in_addr in_addr;
    struct in6_addr in6_addr;
  } u;
} addr_t;

static ssize_t
netlink_request(int fd, void *req, size_t reqlen)
{
  struct sockaddr_nl nladdr;
  struct msghdr msg;
  struct iovec vec;

  memset(&nladdr, 0, sizeof(nladdr));
  nladdr.nl_family = AF_NETLINK;
  nladdr.nl_groups = 0;

  memset(&msg, 0, sizeof(msg));
  msg.msg_name = &nladdr;
  msg.msg_namelen = sizeof(nladdr);
  vec.iov_base = req;
  vec.iov_len = reqlen;
  msg.msg_iov = &vec;
  msg.msg_iovlen = 1;

  return sendmsg(fd, &msg, 0);
}

static ssize_t
netlink_response(int fd, void *resp, size_t resplen)
{
  struct sockaddr_nl nladdr;
  struct msghdr msg;
  struct iovec vec;
  ssize_t ret;

  memset(&nladdr, 0, sizeof(nladdr));
  nladdr.nl_family = AF_NETLINK;
  nladdr.nl_groups = 0;

  memset(&msg, 0, sizeof(msg));
  msg.msg_name = &nladdr;
  msg.msg_namelen = sizeof(nladdr);
  vec.iov_base = resp;
  vec.iov_len = resplen;
  msg.msg_iov = &vec;
  msg.msg_iovlen = 1;

  ret = recvmsg(fd, &msg, 0);
  if (msg.msg_flags & MSG_TRUNC)
    return -1;
  return ret;
}

static int
netlink_get_external_ifi_index(int fd, sa_family_t family)
{
  static const size_t route_buffer_size = 8192;
  void *req = NULL;
  void *resp = NULL;
  struct nlmsghdr *hdr;
  struct rtmsg *rt;
  ssize_t len;
  size_t reqlen;
  int ifidx = -1;

  /* Allocate space for the request. */
  reqlen = NLMSG_SPACE(sizeof(*rt));
  if ((req = calloc(1, reqlen)) == NULL)
    goto out; /* ENOBUFS */

  /* Build the RTM_GETROUTE request. */
  hdr = (struct nlmsghdr *)req;
  hdr->nlmsg_len = NLMSG_LENGTH(sizeof(*rt));
  hdr->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
  hdr->nlmsg_type = RTM_GETROUTE;
  rt = (struct rtmsg *)NLMSG_DATA(hdr);
  rt->rtm_family = family;
  rt->rtm_table = RT_TABLE_MAIN;

  /* Issue the query. */
  if (netlink_request(fd, req, reqlen) < 0)
    goto out;

  /* Allocate space for the response. */
  if ((resp = calloc(1, route_buffer_size)) == NULL)
    goto out; /* ENOBUFS */

  /* Read the response(s).
   * 
   * WARNING: All the packets generated by the request must be consumed (as in,
   * consume responses till NLMSG_DONE/NLMSG_ERROR is encountered).
   */
  do {
    len = netlink_response(fd, resp, route_buffer_size);
    if (len < 0)
      goto out;

    /* Parse the response payload into netlink messages. */
    for (hdr = (struct nlmsghdr *)resp; NLMSG_OK(hdr, len); hdr = NLMSG_NEXT(hdr, len)){
      struct rtattr *attr;
      ssize_t attrlen;
      int temp_idx = -1;
      int has_destination = 0;
      int has_gateway = 0;

      if (hdr->nlmsg_type == NLMSG_DONE)
        goto out;
      if (hdr->nlmsg_type == NLMSG_ERROR) {
        /* Even if we found the interface index, something is broken with the
         * netlink socket, so return an error.
         */
        ifidx = -1;
        goto out;
      }

      /* If we found the correct answer, skip parsing the attributes. */
      if (ifidx != -1)
        continue;

      /* Find the message(s) concerting the main routing table, each message
       * corresponds to a single routing table entry.
       */
      rt = (struct rtmsg *)NLMSG_DATA(hdr);
      if (rt->rtm_table != RT_TABLE_MAIN)
        continue;

      /* Parse all the attributes for a single routing table entry. */
      attr = RTM_RTA(rt);
      attrlen = RTM_PAYLOAD(hdr);
      for (; RTA_OK(attr, attrlen); attr = RTA_NEXT(attr, attrlen)) {
        /* Determine if this routing table entry corresponds to the default
         * route by seeing if it has a gateway, and if a destination addr is
         * set, that it is all 0s.
         */

        switch (attr->rta_type) {
        case RTA_GATEWAY:
          /* The gateway of the route.
           *
           * If someone every needs to figure out the gateway address as well,
           * it's here as the attribute payload.
           */
          has_gateway = 1;
          break;
        case RTA_DST:
        {
          /* The destination address.
           * Should be either missing, or maybe all 0s.  Accept both.
           */
          const size_t nr_zeroes = (family == AF_INET) ? 4 : 16;
          unsigned char c = 0;
          size_t dstlen = RTA_PAYLOAD(attr);
          size_t i;

          if (dstlen != nr_zeroes)
            break;

          for (i = 0; i < dstlen; i++) {
            c |= *(unsigned char *)(RTA_DATA(attr) + i);
          }
          has_destination = (c == 0);
          break;
        }
        case RTA_OIF:
          /* The output interface index. */
          temp_idx = *(int*)RTA_DATA(attr);
          break;
        default:
          /* Ignore. */
          break;
        }
      }

      /* If this is the default route, and we know the interface index,
       * we can stop parsing this message.
       */
      if (has_gateway && !has_destination && temp_idx != -1) {
        ifidx = temp_idx;
        break;
      }
    }
  } while(1);

out:
  if (req)
    free(req);
  if (resp)
    free(resp);
  return ifidx;
}

static int
netlink_get_ifi_address(int fd, int ifidx, sa_family_t family, addr_t *addr)
{
  static const size_t addr_buffer_size = 1024;
  void *req = NULL;
  void *resp = NULL;
  struct nlmsghdr *hdr;
  struct ifaddrmsg *ifa;
  ssize_t len;
  size_t reqlen;
  int ret = -1;

  /* Allocate space for the request. */
  reqlen = NLMSG_SPACE(sizeof(*ifa));
  if ((req = calloc(1, reqlen)) == NULL)
    goto out; /* ENOBUFS */

  /* Build the RTM_GETADDR request.
   * 
   * Querying specific indexes is broken, so do this the hard way.
   */
  hdr = (struct nlmsghdr *)req;
  hdr->nlmsg_len = NLMSG_LENGTH(sizeof(*ifa));
  hdr->nlmsg_flags = NLM_F_REQUEST | NLM_F_ROOT;
  hdr->nlmsg_type = RTM_GETADDR;
  ifa = (struct ifaddrmsg *)NLMSG_DATA(hdr);
  ifa->ifa_family = family;
  ifa->ifa_index = ifidx;

  /* Issue the query. */
  if (netlink_request(fd, req, reqlen) < 0)
    goto out;

  /* Allocate space for the response. */
  if ((resp = calloc(1, addr_buffer_size)) == NULL)
    goto out; /* ENOBUFS */

  /* Read the response(s).
   *
   * WARNING: All the packets generated by the request must be consumed (as in,
   * consume responses till NLMSG_DONE/NLMSG_ERROR is encountered).
   */
  do {
    len = netlink_response(fd, resp, addr_buffer_size);
    if (len < 0)
      goto out;

    /* Parse the response payload into netlink messages. */
    for (hdr = (struct nlmsghdr *)resp; NLMSG_OK(hdr, len); hdr = NLMSG_NEXT(hdr, len)){
      struct rtattr *attr;
      ssize_t attrlen;

      if (hdr->nlmsg_type == NLMSG_DONE) {
        goto out;
      }
      if (hdr->nlmsg_type == NLMSG_ERROR) {
        /* Even if we may have found the answer, something went wrong in the
         * netlink layer, so return an error.
         */
        ret = -1;
        goto out;
      }

      /* If we found the correct answer, skip parsing the attributes. */
      if (ret != -1)
        continue;

      /* Filter the interface based on family, index, and if it scoped
       * correctly.
       */
      ifa = (struct ifaddrmsg *)NLMSG_DATA(hdr);
      if (ifa->ifa_family != family || (int)ifa->ifa_index != ifidx)
        continue;
      if (ifa->ifa_scope != RT_SCOPE_UNIVERSE)
        continue;

      /* Parse all the attributes for a single interface entry. */
      attr = IFA_RTA(ifa);
      attrlen = RTM_PAYLOAD(hdr);
      for (; RTA_OK(attr, attrlen); attr = RTA_NEXT(attr, attrlen)) {
        if (attr->rta_type == IFA_ADDRESS) {
          const size_t addrlen = (family == AF_INET) ? sizeof(struct in_addr) :
              sizeof(struct in6_addr);
          addr->family = family;
          memcpy(&addr->u, RTA_DATA(attr), addrlen);
          ret = 0;
          break;
        }
      }
    }
  } while(1);

out:
  if (req)
    free(req);

  return ret;
}

int
get_external_address(sa_family_t family, addr_t *addr)
{
  struct sockaddr_nl nladdr;
  int fd;
  int ret = -1;
  int ifidx;

  if (!addr)
    goto out_noclose;
  if (family != AF_INET && family != AF_INET6)
    goto out_noclose;

  memset(addr, 0, sizeof(*addr));

  /* Create an AF_NETLINK socket for querying the routing table. */
  fd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
  if (fd < 0)
    goto out_noclose;

  memset(&nladdr, 0, sizeof(nladdr));
  nladdr.nl_family = AF_NETLINK;
  if (bind(fd, (struct sockaddr *) &nladdr, sizeof(nladdr)) != 0)
    goto out;

  /* Figure out the index for the interface that has a default route for the
   * specified family.
   */
  ifidx = netlink_get_external_ifi_index(fd, family);
  if (ifidx < 0)
    goto out;

  /* Now that we have the interface, query the kernel again, to get the address
   * associated with the interface.
   */
  ret = netlink_get_ifi_address(fd, ifidx, family, addr);

out:
  close(fd);
out_noclose:
  return ret;
}

int
main(int argc, char *argv[]) {
  char addrstr[128];
  int ret;
  addr_t addr;

  (void)argc;
  (void)argv;

  ret = get_external_address(AF_INET, &addr);
  fprintf(stdout, "AF_INET: %d\n", ret);
  if (ret == 0) {
    inet_ntop(addr.family, &addr.u.in_addr, addrstr, sizeof(addrstr));
    fprintf(stdout, " Addr: %s\n", addrstr);
  }

  ret = get_external_address(AF_INET6, &addr);
  fprintf(stdout, "AF_INET6: %d\n", ret);
  if (ret == 0) {
    inet_ntop(addr.family, &addr.u.in_addr, addrstr, sizeof(addrstr));
    fprintf(stdout, " Addr: %s\n", addrstr);
  }

  return 0;
}