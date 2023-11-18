#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <inttypes.h>
#include <sys/utsname.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <sys/wait.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/mman.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/ebml/ebml.h>
#include <krad/io/buffer.h>
#include <krad/util/strton.h>
#include <krad/system/fdshare.h>

#include "client.h"

struct kr_app_client {
  char address[256];
  int sd;
  int port;
  char host[256];
  char api_path[107];
  int api_path_pos;
};

static int get_host_and_port(char *string, char *host, int *port);
static void kr_app_connect_local(kr_app_client *client);
static void kr_app_connect_remote(kr_app_client *client);

static int get_host_and_port(char *string, char *host, int *port) {
  int res;
  char *str;
  str = strrchr(string, ':') + 1;
  res = kr_strtoi32(port, str, strlen(str));
  if (res != 0) return -1;
  memset(host, '\0', 128);
  memcpy(host, string, strlen(string) - strlen(strrchr(string, ':')));
  return 0;
}

static void kr_app_connect_local(kr_app_client *client) {
  int ret;
  socklen_t socket_sz;
  struct sockaddr_un unix_saddr;
  client->sd = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
  if (client->sd == -1) {
    printke("App Client: socket fail");
    return;
  }
  memset(&unix_saddr, 0, sizeof(unix_saddr));
  unix_saddr.sun_family = AF_UNIX;
  snprintf(unix_saddr.sun_path, sizeof(unix_saddr.sun_path), "%s", client->api_path);
  socket_sz = sizeof(unix_saddr) - sizeof(unix_saddr.sun_path) + strlen(unix_saddr.sun_path);
  unix_saddr.sun_path[0] = '\0';
  ret = connect(client->sd, (struct sockaddr *)&unix_saddr, socket_sz);
  if (ret != 0) {
    if (errno != EINPROGRESS) {
      close(client->sd);
      client->sd = -1;
      printke("App Client: Can't connect to socket %s", client->api_path);
    }
  }
}

static void kr_app_connect_remote(kr_app_client *client) {
  int ret;
  int flags;
  char port_string[6];
  struct in6_addr serveraddr;
  struct addrinfo hints;
  struct addrinfo *res;
  res = NULL;
  if (client->port == 0) return;
  //FIXME hrm we don't know the sysname of a remote connect! crazy ?
  //printf("Krad APP Client: Connecting to remote %s:%d", client->host, client->port);
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_NUMERICSERV;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  ret = inet_pton(AF_INET, client->host, &serveraddr);
  if (ret == 1) {
    hints.ai_family = AF_INET;
    hints.ai_flags |= AI_NUMERICHOST;
  } else {
    ret = inet_pton(AF_INET6, client->host, &serveraddr);
    if (ret == 1) {
      hints.ai_family = AF_INET6;
      hints.ai_flags |= AI_NUMERICHOST;
    }
  }
  snprintf(port_string, 6, "%d", client->port);
  ret = getaddrinfo(client->host, port_string, &hints, &res);
  if (ret != 0) {
    printf("App Client: Host not found: %s\n", gai_strerror(ret));
    return;
  }
  flags = res->ai_socktype;
  flags |= SOCK_NONBLOCK | SOCK_CLOEXEC;
  client->sd = socket(res->ai_family, flags, res->ai_protocol);
  if (client->sd == -1) {
    printf("App Client: Socket Error\n");
  } else {
    ret = connect(client->sd, res->ai_addr, res->ai_addrlen);
    if ((ret != 0) && (errno != EINPROGRESS)) {
      close(client->sd);
      client->sd = -1;
      printf("App Client: Remote Connect Error\n");
    }
  }
  if (res != NULL) {
    freeaddrinfo(res);
  }
}

int kr_app_client_fd(kr_app_client *client) {
  if (client == NULL) return -1;
  return client->sd;
}

int kr_app_local(kr_app_client *client) {
  if (client == NULL) return -1;
  if (client->port == 0) return 1;
  return 0;
}

int kr_app_disconnect(kr_app_client *client) {
  if (client == NULL) return -1;
  if (client->sd != -1) {
    close(client->sd);
    client->sd = -1;
  }
  return 0;
}

int kr_app_connect(kr_app_client *client, char *address) {
  if (client == NULL) return -1;
  strncpy(client->address, address, sizeof(client->address) - 1);
  if (kr_valid_host_and_port(address)) {
    if (get_host_and_port(address, client->host, &client->port) != 0) return -1;
    kr_app_connect_remote(client);
  } else {
    client->api_path_pos = sprintf(client->api_path, "@krad_radio_%s_api", address);
    kr_app_connect_local(client);
  }
  return 0;
}

int kr_app_client_destroy(kr_app_client *client) {
  if (client == NULL) return -1;
  kr_app_disconnect(client);
  free(client);
  return 0;
}

kr_app_client *kr_app_client_create() {
  kr_app_client *client;
  client = kr_allocz(1, sizeof(*client));
  client->sd = -1;
  return client;
}

static char *evoid = "\xEC\x80"; /* EBML VOID */

int kr_app_client_send_fds(kr_app_client *client, const int *fds, int nfds) {
  if (nfds > KR_RWFDS_MAX) return -1;
  return send_fds(client->sd, evoid, 2, fds, nfds);
}

int kr_app_client_send_fd(kr_app_client *client, int fd) {
  return send_fds(client->sd, evoid, 2, &fd, 1);
}
