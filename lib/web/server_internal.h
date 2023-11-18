#if !defined(_web_server_internal_H)
# define _web_server_internal_H (1)

#include "web.h"
#include "server.h"
#include "protocol/tls.h"

struct kr_web_server {
  int sd;
  kr_tls_domain *tls;
  kr_web_event_cb *accept_cb;
  void *accept_user;
};

#include "internal.h"
#include "client_internal.h"

int server_path_close(kr_web_path *path);
int server_path_open(kr_web_path *path, kr_web_server_setup_info *setup);

#endif
