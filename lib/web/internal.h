#if !defined(_web_internal_H)
# define _web_internal_H (1)

#include "web.h"
#include "client_internal.h"
#include "server_internal.h"

struct kr_web_path {
  kr_web_path_info info;
  union {
    kr_web_server server;
    kr_web_client client;
  };
  void *user;
  int user_type;
  kr_web_event_cb *event_cb;
  kr_web *web;
};

int path_event(kr_web_path *path, kr_web_event_type type);
kr_web_path *web_path(kr_web *web);
kr_loop *path_loop(kr_web_path *path);

#endif
