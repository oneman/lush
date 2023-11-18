#include <krad/radio/version.h>
#include <krad/app/debug.h>
#include <krad/io/buffer.h>
#include <krad/hash/sha1.h>
#include <krad/hash/base64.h>
#include <krad/mem/pool.h>
#include <krad/loop/loop.h>
#include <krad/system/cmdr.h>
#include "common.h"
#include "types.h"

#if !defined(_app_server_H)
# define _app_server_H (1)

#define KR_APP_CLIENT_DOCTYPE "kr_app_client"
#define KR_APP_SERVER_DOCTYPE "kr_app_server"
#define KR_APP_DOCTYPE_VERSION KR_VERSION
#define KR_APP_DOCTYPE_READ_VERSION KR_VERSION

#define KR_APP_SERVER_CLIENTS_MAX 32
#define KR_APP_SERVER_BROADCASTS_MAX 128
#define KR_APP_SERVER_BROADCASTERS_MAX 16

typedef struct kr_app_server_setup kr_app_server_setup;
typedef struct kr_app_server_info kr_app_server_info;
typedef struct kr_app_server_client_setup kr_app_server_client_setup;
typedef struct kr_app_server kr_app_server;
typedef struct kr_app_server_client kr_app_server_client;
typedef struct kr_app_server_client_event kr_app_server_client_event;
typedef int (kr_app_startup_handler)(kr_app_server *, void *);
typedef int (kr_app_shutdown_handler)(void *);
typedef int (kr_app_server_client_handler)(kr_app_server_client_event *e);

typedef enum {
  KR_APP_SERVER_CLIENT_CONNECT = 1,
  KR_APP_SERVER_CLIENT_DATA,
  KR_APP_SERVER_CLIENT_DISCONNECT
} kr_app_server_client_event_type;

struct kr_app_server_client_event {
  kr_app_server_client *client;
  kr_app_server *server;
  kr_app_server_client_event_type type;
};

struct kr_app_server_setup {
  void *user;
  kr_app_server_client_handler *client_handler;
  kr_app_startup_handler *startup_handler;
  kr_app_shutdown_handler *shutdown_handler;
  kr_handler *signal_handler;
  int cmdr;
  char appname[32];
  char sysname[64];
};

struct kr_app_server_info {
  uint64_t clients;
  uint64_t uptime;
};

void *kr_app_client_get_user(kr_app_server_client *c);
int kr_app_client_set_user(kr_app_server_client *c, void *user);
ssize_t kr_app_client_write(kr_app_server_client *c, uint8_t *buf, size_t sz);
ssize_t kr_app_client_read(kr_app_server_client *c, uint8_t *buf, size_t sz);
ssize_t kr_app_client_peek(kr_app_server_client *c, uint8_t *buf, size_t sz);
ssize_t kr_app_client_took(kr_app_server_client *c, size_t sz);
ssize_t kr_app_client_close(kr_app_server_client *c);
int kr_app_server_fds_get(kr_app_server *s, int *fds, int n);
int kr_app_server_info_get(kr_app_server *server, kr_app_server_info *info);
kr_loop *kr_app_server_mainloop_get(kr_app_server *server);
kr_cmdr *kr_app_server_get_cmdr(kr_app_server *s);
void *kr_app_server_get_user(kr_app_server *s);
void krad_app_server(kr_app_server_setup *setup);
#endif
