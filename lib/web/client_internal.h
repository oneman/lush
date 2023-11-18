#if !defined(_web_client_internal_H)
# define _web_client_internal_H (1)

#include "protocol/tls.h"

#include "client.h"
#include "chunk.h"

struct kr_web_client {
  int sd;
  int more_data;
  kr_io2_t *in;
  kr_io2_t *out;
  kr_tls_conn *tls;
  uint64_t num;
  kr_chunked_tracker ct;
  uint8_t boundary[70];
  uint8_t boundary_len;
  //union {
    kr_http http;
    kr_ws_frame ws;
    int binary_ws;
  //};
};

#include "internal.h"

int client_path_close(kr_web_path *path);
int client_path_accept(kr_web_path *path);
int client_path_connect(kr_web_path *path, kr_web_client_setup_info *setup);

#endif
