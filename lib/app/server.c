#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <ifaddrs.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <sys/signalfd.h>
#include <signal.h>
#include <krad/system/cmdr.h>
#include <krad/loop/loop.h>
#include <krad/mem/pool.h>
#include <krad/system/fdset.h>
#include <krad/system/fdshare.h>
#include "server.h"

#define KR_APP_EVENTS_MAX 16

typedef enum {
  KR_APP_STARTING = -1,
  KR_APP_RUNNING,
  KR_APP_DO_SHUTDOWN,
  KR_APP_SHUTINGDOWN,
} app_state;

struct kr_app_server {
  int sd;
  void *user;
  kr_pool *client_pool;
  uint64_t num_clients;
  uint64_t max_clients;
  kr_app_server_client_handler *client_handler;
  kr_app_startup_handler *startup_handler;
  kr_app_shutdown_handler *shutdown_handler;
  kr_handler *signal_handler;
  int socket_count;
  kr_app_server_client *current_client;
  kr_app_server_client *new_clients[KR_APP_SERVER_CLIENTS_MAX];
  uint64_t start_time;
  app_state state;
  kr_loop *loop;
  pid_t pid;
  kr_cmdr *cmdr;
};

typedef enum {
  KR_APP_CLIENT_LOCAL_NEW = 1,
  KR_APP_CLIENT_LOCAL_VALID
} kr_app_client_type;

struct kr_app_server_client {
  int sd;
  kr_app_server *server;
  kr_app_client_type type;
  kr_info_format info_format;
  kr_io2_t *in;
  kr_io2_t *out;
  void *user;
  kr_fdset *fdset;
  void *transport_user;
  kr_crate_loader *load;
  kr_crate_unloader *unload;
};

typedef struct kr_app_server_client kr_app_client;

#include "local.c"

static int client_data(kr_app_client *client);
static int client_write(kr_app_client *client);
static int client_read(kr_app_client *client);
static void disconnect_client(kr_app_client *client);
static int accept_client(kr_event *event);
static int client_event(kr_event *event);
static int signal_event(kr_event *event);
static int systemd_notify(const char *data);
static void setup_system(kr_app_server *server);
static int setup_socket(char *appname, char *sysname);
static int shutdown_app_server(void *user);
static int start_app_server(kr_loop *loop, void *user);
static kr_app_server *create_app_server(kr_app_server_setup *setup);

static int run_client_handler(kr_app_client *client,
 kr_app_server_client_event_type type) {
  kr_app_server_client_event e;
  if (!client) return -1;
  e.client = client;
  e.server = client->server;
  e.type = type;
  client->server->client_handler(&e);
  return 0;
}

static int client_data(kr_app_client *client) {
  int ret;
  client->server->current_client = client;
  ret = run_client_handler(client, KR_APP_SERVER_CLIENT_DATA);
  client->server->current_client = NULL;
  return ret;
}

static int client_write(kr_app_client *client) {
  ssize_t ret;
  ssize_t bytes;
  ret = kr_io2_output(client->out, &bytes);
  printkd("WROTE %zu", bytes);
  return ret;
}

static int client_read(kr_app_server_client *client) {
  int ret;
  int fds[KR_RWFDS_MAX];
  int nfds;
  int i;
  nfds = KR_RWFDS_MAX;
  for (i = 0; i < nfds; i++) fds[i] = -1;
  if (client->type == KR_APP_CLIENT_LOCAL_VALID) {
    ret = recv_fds(client->sd, client->in->buf, client->in->space, fds, &nfds);
    if (ret > 0) {
      kr_io2_advance(client->in, ret);
      printkd("Local client READ %d", ret);
      if (nfds > 0) {
        kr_fdset_put(client->fdset, fds, nfds);
        printk("App Server: Added %d fds to client", nfds);
      }
    }
  } else {
    ret = kr_io2_read(client->in);
    printkd("Local client READ %d", ret);
  }
  return ret;
}

static void disconnect_client(kr_app_client *client) {
  int ret;
  kr_app_server *server;
  server = client->server;
  ret = run_client_handler(client, KR_APP_SERVER_CLIENT_DISCONNECT);
  if (ret != 0) {
    printke("App Server: Problem happened in client_handler");
  }
  ret = kr_io2_buffered_bytes(client->out);
  if (ret > 0) {
    printk("App Server: %zu bytes remained for client at disconnect");
  }
  if (client->sd != -1) {
    kr_loop_del(server->loop, client->sd);
    close(client->sd);
    client->sd = -1;
  }
  ret = kr_fdset_count(client->fdset);
  if (ret) printke("App Server: Client had %d leftover fds", ret);
  kr_fdset_close(client->fdset);
  free(client->fdset);
  client->user = NULL;
  kr_io2_destroy(&client->in);
  kr_io2_destroy(&client->out);
  kr_pool_release(server->client_pool, client);
  server->num_clients--;
  printk("App Server: Client disconnected");
}

static int accept_client(kr_event *event) {
  kr_app_server *server;
  kr_app_server_client *client;
  kr_event watch;
  struct sockaddr_un sin;
  socklen_t slen;
  int ret;
  server = (kr_app_server *)event->user;
  if (!(event->events & EPOLLIN)) return -1;
  slen = sizeof(sin);
  client = kr_pool_slice(server->client_pool);
  if (client == NULL) {
    printke("App Server: Overloaded can't accept client");
    return -1;
  }
  client->sd = accept4(server->sd, (struct sockaddr *)&sin, &slen,
   SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (client->sd < 0) {
    kr_pool_release(server->client_pool, client);
    printke("App Server: accept4() failed!");
    return -1;
  }
  client->fdset = calloc(1, kr_fdset_sizeof());
  kr_fdset_init(client->fdset);
  watch.fd = client->sd;
  watch.events = EPOLLIN;
  watch.user = client;
  watch.handler = client_event;
  ret = kr_loop_add(server->loop, &watch);
  if (ret != 0) {
    kr_pool_release(server->client_pool, client);
    printke("App Server: Adding client to epoll after accept4 failed");
    close(client->sd);
    return -1;
  }
  client->type = KR_APP_CLIENT_LOCAL_NEW;
  client->info_format = KR_FORMAT_EBML;
  client->in = kr_io2_create();
  client->out = kr_io2_create();
  kr_io2_set_fd(client->in, client->sd);
  kr_io2_set_fd(client->out, client->sd);
  server->num_clients++;
  client->server = server;
  printk("App Server: Local client accepted");
  ret = run_client_handler(client, KR_APP_SERVER_CLIENT_CONNECT);
  if (ret != 0) {
    disconnect_client(client);
  }
  return 0;
}

static int client_event(kr_event *event) {
  int ret;
  kr_app_client *client;
  kr_app_server *server;
  client = (kr_app_client *)event->user;
  server = client->server;
  if (event->events & EPOLLIN) {
    ret = client_read(client);
    if (ret > 0) {
      printk("App Server: Got %d bytes from client", ret);
      /* temp ebml kludge */
      if (client->type == KR_APP_CLIENT_LOCAL_NEW) {
        ret = validate_local_client(client);
        if (ret != 0) return ret;
      }
      /* end temp ebml kludge */
      if (kr_io2_has_in(client->in)) {
        ret = client_data(client);
      } else {
        ret = 0;
      }
      if (ret < 0) {
        disconnect_client(client);
        return 0;
      } else {
        if (kr_io2_want_out(client->out)) {
          printk("App Server: Have bytes to send to client");
          event->events = EPOLLIN | EPOLLOUT;
          kr_loop_mod(server->loop, event);
        }
      }
    } else {
      if (ret == -1) {
        printk("App Server: Handle client ret %d", ret);
      }
      if (ret == 0) {
        printk("App Server: Client EOF");
      }
      disconnect_client(client);
      return 0;
    }
  }
  if (event->events & EPOLLOUT) {
    ret = client_write(client);
    if (ret != 0) {
      printke("App Server: Client write error");
      disconnect_client(client);
      return 0;
    }
    if (!(kr_io2_want_out(client->out))) {
      printk("App Server: Client has no more bytes for wire");
      event->events = EPOLLIN;
      kr_loop_mod(server->loop, event);
    }
  }
  if (event->events & EPOLLHUP) {
    printk("App Server: Client Hangup");
    disconnect_client(client);
    return 0;
  }
  if (event->events & EPOLLERR) {
    printke("App Server: Client ERR");
    disconnect_client(client);
    return 0;
  }
  return 0;
}

static int signal_event(kr_event *event) {
  kr_app_server *server;
  kr_event user_event;
  user_event = *event;
  server = (kr_app_server *)event->user;
  user_event.user = server->user;
  return server->signal_handler(&user_event);
}

static int systemd_notify(const char *data) {
  const char *e;
  struct sockaddr_un un = { .sun_family = AF_UNIX };
  int fd;
  e = getenv("NOTIFY_SOCKET");
  if (e) {
    strncpy(un.sun_path, e, sizeof(un.sun_path) - 1);
    if (un.sun_path[0] == '@') {
      un.sun_path[0] = 0;
    }
    fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd < 0) {
      return -1;
    }
    sendto(fd, data, strlen(data), MSG_NOSIGNAL, (struct sockaddr*) &un,
     sizeof(un) - sizeof(un.sun_path) + strlen(e));
    close(fd);
    return 0;
  }
  return -1;
}

static void setup_system(kr_app_server *server) {
  srand(time(NULL));
}

static int setup_socket(char *appname, char *sysname) {
  int sd;
  struct sockaddr_un saddr;
  socklen_t socket_sz;
  sd = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
  if (sd == -1) {
    printke("App Server: Socket failed.");
    return -1;
  }
  saddr.sun_family = AF_UNIX;
  snprintf(saddr.sun_path, sizeof(saddr.sun_path),
   "@%s_%s_api", appname, sysname);
  socket_sz = sizeof(saddr) - sizeof(saddr.sun_path);
  socket_sz += strlen(saddr.sun_path);
  saddr.sun_path[0] = '\0';
  if (connect(sd, (struct sockaddr *)&saddr, socket_sz) != -1) {
    printke("App Server: Socket name in use.");
    close(sd);
    return -1;
  }
  if (bind(sd, (struct sockaddr *)&saddr, socket_sz) == -1) {
    printke("App Server: Can't bind to socket.");
    close(sd);
    return -1;
  }
  listen(sd, SOMAXCONN);
  return sd;
}

static int shutdown_app_server(void *user) {
  int i;
  int ret;
  kr_app_server *server;
  kr_app_server_client *client;
  server = (kr_app_server *)user;
  if (server == NULL) return -1;
  printk("App Server: Destroying");
  server->state = KR_APP_DO_SHUTDOWN;
  if (server->sd != 0) {
    close(server->sd);
  }
  ret = server->shutdown_handler(server->user);
  i = 0;
  while ((client = kr_pool_iterate_active(server->client_pool, &i))) {
    disconnect_client(client);
  }
  if (server->cmdr) {
    ret = kr_cmdr_destroy(server->cmdr);
    server->cmdr = NULL;
    if (ret != 0) printk("App Server: cmdr destroy");
  }
  kr_pool_destroy(server->client_pool);
  printk("App Server: Destroyed");
  return ret;
}

static int start_app_server(kr_loop *loop, void *user) {
  int ret;
  kr_app_server *server;
  kr_event harness;
  if (loop == NULL) return -1;
  server = (kr_app_server *)user;
  if (server == NULL) return -1;
  if (server->state != KR_APP_STARTING) return -2;
  printk("App Server: Enabling");
  server->pid = getpid();
  server->state = KR_APP_RUNNING;
  server->start_time = time(NULL);
  server->loop = loop;
  ret = server->startup_handler(server, server->user);
  if (ret != 0) return ret;
  harness.user = server;
  harness.events = EPOLLIN;
  harness.handler = accept_client;
  harness.fd = server->sd;
  kr_loop_add(server->loop, &harness);
  systemd_notify("READY=1");
  return 0;
}

static void cmdr_started(void *user) {
  int *sd;
  sd = user;
  close(*sd);
}

static kr_app_server *create_app_server(kr_app_server_setup *setup) {
  int sd;
  kr_cmdr_setup cmdr_setup;
  kr_app_server *server;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  char *home_path;
  if (setup == NULL) return NULL;
  home_path = getenv("HOME");
  if (home_path == NULL) {
    printke("App Server: $HOME not set!");
    return NULL;
  }
  if (chdir(home_path) != 0) {
    printke("App Server: chdir to %s ($HOME) failed!", home_path);
    return NULL;
  }
  printk("App Server: chdir to %s", home_path);
  printk("App Server: Creating");
  sd = setup_socket(setup->appname, setup->sysname);
  if (sd < 0) {
    return NULL;
  }
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(kr_app_server);
  pool_setup.size = sizeof(kr_app_server_client);
  pool_setup.slices = KR_APP_SERVER_CLIENTS_MAX;
  pool = kr_pool_create(&pool_setup);
  server = kr_pool_overlay_get(pool);
  memset(server, 0, sizeof(kr_app_server));
  server->client_pool = pool;
  server->user = setup->user;
  server->client_handler = setup->client_handler;
  server->signal_handler = setup->signal_handler;
  server->startup_handler = setup->startup_handler;
  server->shutdown_handler = setup->shutdown_handler;
  server->sd = sd;
  server->state = KR_APP_STARTING;
  server->max_clients = KR_APP_SERVER_CLIENTS_MAX;
  if (setup->cmdr == 1) {
    cmdr_setup.user = &sd;
    cmdr_setup.startup_cb = cmdr_started;
    server->cmdr = kr_cmdr_create(&cmdr_setup);
    if (!server->cmdr) {
      printk("frook");
    } 
  }
  setup_system(server);
  printk("App Server: Created");
  return server;
}

kr_cmdr *kr_app_server_get_cmdr(kr_app_server *s) {
  if (!s) return NULL;
  return s->cmdr;  
}

void *kr_app_client_get_user(kr_app_server_client *c) {
  if (!c) return NULL;
  return c->user;
}

int kr_app_client_set_user(kr_app_server_client *c, void *user) {
  if (!c || !user) return -1;
  c->user = user;
  return 0;
}

ssize_t kr_app_client_write(kr_app_server_client *c, uint8_t *buf, size_t sz) {
  ssize_t ret;
  if (!c || !buf || !sz) return -1;
  ret = kr_io2_pack(c->out, buf, sz);
  return ret;
}

ssize_t kr_app_client_read(kr_app_server_client *c, uint8_t *buf, size_t sz) {
  ssize_t ret;
  if (!c || !buf || !sz) return -1;
  ret = kr_io2_unpack(c->in, buf, sz);
  return ret;
}

ssize_t kr_app_client_peek(kr_app_server_client *c, uint8_t *buf, size_t sz) {
  ssize_t ret;
  if (!c || !buf || !sz) return -1;
  ret = kr_io2_unpeek(c->in, buf, sz);
  return ret;
}

ssize_t kr_app_client_took(kr_app_server_client *c, size_t sz) {
  if (!c || !sz) return -1;
  kr_io2_pulled(c->in, sz);
  return sz;
}

ssize_t kr_app_client_close(kr_app_server_client *c) {
  ssize_t ret;
  ret = 0;
  if (!c) return -1;
  if (kr_io2_want_out(c->out)) {
    ret = client_write(c);
    if (ret > 0) {
      printk("App Server: Wrote %zu bytes before closing client");
    }
  }
  disconnect_client(c);
  return ret;
}

int kr_app_server_fds_get(kr_app_server *s, int *fds, int n) {
  if (!s) return -1;
  if (!s->current_client) return -2;
  int ret;
  ret = kr_fdset_get(s->current_client->fdset, fds, n);
  return ret;
}

int kr_app_server_info_get(kr_app_server *server, kr_app_server_info *info) {
  uint64_t now;
  if ((server == NULL) || (info == NULL)) return -1;
  if (server->state != KR_APP_RUNNING) {
   info->clients = 0;
   info->uptime = 0;
  } else {
    now = time(NULL);
    info->uptime = now - server->start_time;
    info->clients = server->num_clients;
  }
  return 0;
}

kr_loop *kr_app_server_mainloop_get(kr_app_server *server) {
  if (server == NULL) return NULL;
  return server->loop;
}

void *kr_app_server_get_user(kr_app_server *s) {
  if (!s) return NULL;
  return s->user;
}

void krad_app_server(kr_app_server_setup *setup) {
  static kr_mainloop_setup loop_setup;
  static kr_app_server *server;
  server = create_app_server(setup);
  if (server == NULL) exit(1);
  loop_setup.user = server;
  loop_setup.signal_handler = signal_event;
  loop_setup.startup_handler = start_app_server;
  loop_setup.shutdown_handler = shutdown_app_server;
  kr_mainloop(&loop_setup);
}
