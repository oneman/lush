#include <string.h>
#include <krad/app/debug.h>
#include "fdshare.h"

#define KR_FDSHARE_NFDS 32

ssize_t recv_fds(int ud, uint8_t *data, size_t len, int *fds, int *nfds) {
  ssize_t ret;
  int *fdptr;
  int *end;
  struct msghdr msg;
  struct iovec iov;
  struct cmsghdr *cmsg;
  union {
    uint8_t buf[CMSG_SPACE(sizeof(fds[0]) * KR_FDSHARE_NFDS)];
    struct cmsghdr align;
  } u;
  *nfds = 0;
  memset(&msg, 0, sizeof(msg));
  iov.iov_base = data;
  iov.iov_len = len;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = u.buf;
  msg.msg_controllen = sizeof(u.buf);
  ret = recvmsg(ud, &msg, MSG_CMSG_CLOEXEC);
  if (ret == -1) return -1;
  if (!msg.msg_control || msg.msg_controllen == 0) return ret;
  cmsg = CMSG_FIRSTHDR(&msg);
  for (; cmsg != NULL; cmsg = CMSG_NXTHDR(&msg, cmsg)) {
    if (cmsg->cmsg_level != SOL_SOCKET || cmsg->cmsg_type != SCM_RIGHTS) {
      continue;
    }
    data = CMSG_DATA(cmsg);
    end = (int *)(data + cmsg->cmsg_len - CMSG_LEN(0));
    for (fdptr = (int *)data; fdptr < end; ++fdptr) {
      fds[*nfds] = *fdptr;
      (*nfds)++;
    }
  }
  return ret;
}

ssize_t send_fds(int ud, char *buf, size_t sz, const int *fds, int nfds) {
  int ret;
  struct msghdr msg;
  struct iovec iov;
  struct cmsghdr *cmsg;
  int *fdptr;
  union {
    uint8_t buf[CMSG_SPACE(sizeof(fds[0]) * KR_FDSHARE_NFDS)];
    struct cmsghdr align;
  } u;
  memset(&msg, 0, sizeof(msg));
  msg.msg_control = u.buf;
  msg.msg_controllen = sizeof(u.buf);
  cmsg = CMSG_FIRSTHDR(&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN(sizeof(int) * nfds);
  fdptr = (int *)CMSG_DATA(cmsg);
  memcpy(fdptr, fds, nfds * sizeof(int));
  msg.msg_controllen = cmsg->cmsg_len;
  iov.iov_base = buf;
  iov.iov_len = sz;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_flags = 0;
  printk("Sending %d fds msg_controllen %d cmsg_len %d", nfds,
   msg.msg_controllen, cmsg->cmsg_len);
  ret = sendmsg(ud, &msg, 0);
  if (ret != sz) exit(1);
  return 0;
}
