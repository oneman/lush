#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <net/if.h>
#include <krad/app/debug.h>
#include <krad/loop/loop.h>
#include "server_internal.h"

static void tls_clear(kr_web_path *path);
static void tls_init(kr_web_path *path);

static void tls_clear(kr_web_path *path) {
  kr_web_server *server;
  server = &path->server;
  kr_tls_domain_destroy(server->tls);
  server->tls = NULL;
}

static void tls_init(kr_web_path *path) {
  kr_tls_domain_setup setup;
  kr_web_server *server;
  server = &path->server;
  setup.certfile = getenv("KR_WEB_TLS_CERTFILE");
  setup.keyfile = getenv("KR_WEB_TLS_KEYFILE");
  if (!setup.certfile) {
    printk("TLS: no cert file");
  }
  if (!setup.keyfile) {
    printk("TLS: no key file");
  }
  if (!setup.certfile || !setup.keyfile) {
    server->tls = NULL;
    return;
  }
  printk("TLS: Found cert and key files");
  server->tls = kr_tls_domain_create(&setup);
}

static int set_socket_defer_accept(int sd) {
  int ret;
  const int val = 6;
  ret = setsockopt(sd, IPPROTO_TCP, TCP_DEFER_ACCEPT, (const void *)&val,
   (socklen_t)sizeof(int));
  if (ret != 0) {
    printke("Web Server: Could not set socket TCP_DEFER_ACCEPT");
  }
  return ret;
}

static int set_socket_reuseaddr(int sd) {
  int ret;
  const int val = 1;
  ret = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (const void *)&val,
   (socklen_t)sizeof(int));
  if (ret != 0) {
    printke("Web Server: Could not set socket SO_REUSEADDR");
  }
  return ret;
}

static int server_accept(kr_event *event) {
  int err;
  int flags;
  kr_web_path *path;
  kr_web_server_info *server_info;
  kr_web_path *client_path;
  int sd;
  struct sockaddr_in sin;
  socklen_t slen;
  slen = sizeof(sin);
  memset(&sin, 0, slen);
  path = (kr_web_path *)event->user;
  server_info = &path->info.server;
  flags = SOCK_NONBLOCK | SOCK_CLOEXEC;
  if (path->server.tls) {
    /* openssl / tls brain damnage is to much for nonblocking right now */
    flags = SOCK_CLOEXEC;
  }
  sd = accept4(event->fd, &sin, &slen, flags);
  if (sd == -1) {
    err = errno;
    printke("Web Server: server accept4 %s", strerror(err));
    return 0;
  }
  server_info->conn_total++;
  client_path = web_path(path->web);
  if (!client_path) {
    close(sd);
    return 0;
  }
  client_path->client.num = server_info->conn_total;
  client_path->info.type = KR_WEB_CLIENT;
  client_path->info.client.type = KR_WEB_CLIENT_ACCEPT;
  client_path->client.sd = sd;
  if (path->server.accept_cb) client_path->event_cb = path->server.accept_cb;
  if (path->server.accept_user) client_path->user = path->server.accept_user;
  if (path->server.tls) {
    client_path->client.tls = kr_tls_domain_client_init(path->server.tls);
    kr_tls_domain_client_io_init(client_path->client.tls, sd);
  } else {
    client_path->client.tls = NULL;
  }
  client_path_accept(client_path);
  return 0;
}

int server_path_close(kr_web_path *path) {
  int ret;
  kr_web_server *server;
  kr_web_server_info *info;
  server = &path->server;
  info = &path->info.server;
  printk("Web Server: Stopping on port %d", info->port);
  ret = kr_loop_del(path_loop(path), server->sd);
  close(server->sd);
  server->sd = -1;
  tls_clear(path);
  return ret;
}

int server_path_open(kr_web_path *path, kr_web_server_setup_info *setup) {
  kr_web_server *server;
  kr_web_server_info *info;
  int ret;
  char port_string[8];
  struct addrinfo hints;
  struct addrinfo *result;
  struct addrinfo *rp;
  char *interface;
  char *interface_actual;
  kr_event harness;
  tls_init(path);
  server = &path->server;
  info = &path->info.server;
  info->conn_total = 0;
  memcpy(info, setup, sizeof(*setup));
  interface = info->iface;
  interface_actual = interface;
  printk("Web Server: socket setup on interface: %s port %d", interface,
   info->port);
  snprintf(port_string, 8, "%d", info->port);
  memset(&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
  hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
  hints.ai_flags = AI_PASSIVE;     /* All interfaces */
  if ((strlen(interface) == 7) && (strncmp(interface, "0.0.0.0", 7) == 0)) {
    hints.ai_family = AF_INET;
    interface_actual = NULL;
  }
  if ((strlen(interface) == 4) && (strncmp(interface, "[::]", 4) == 0)) {
    hints.ai_family = AF_INET6;
    interface_actual = NULL;
  }
  ret = getaddrinfo(interface_actual, port_string, &hints, &result);
  if (ret != 0) {
    printke ("Web Server getaddrinfo: %s\n", gai_strerror(ret));
    return -1;
  }
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    server->sd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (server->sd == -1) continue;
    set_socket_reuseaddr(server->sd);
    ret = bind(server->sd, rp->ai_addr, rp->ai_addrlen);
    if (ret == 0) {
      /* We managed to bind successfully! */
      break;
    }
    close(server->sd);
  }
  if (rp == NULL) {
    printke("Web Server: Could not bind %d\n", info->port);
    return -1;
  }
  freeaddrinfo(result);
  listen(server->sd, SOMAXCONN);
  set_socket_defer_accept(server->sd);
  printk("Web Server: Listen on interface %s port %d", interface, info->port);
  harness.fd = server->sd;
  harness.user = path;
  harness.handler = server_accept;
  harness.events = EPOLLIN;
  ret = kr_loop_add(path_loop(path), &harness);
  path_event(path, KR_WEB_OPEN);
  return ret;
}
