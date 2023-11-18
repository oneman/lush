#if !defined(_xfer_xfer_H)
# define _xfer_xfer_H (1)

typedef struct kr_xfer kr_xfer;
typedef struct kr_xfer_setup kr_xfer_setup;
typedef struct kr_xfer_path kr_xfer_path;
typedef struct kr_xfer_event kr_xfer_event;
typedef int (kr_xfer_event_cb)(kr_xfer_event *);

#include <krad/mem/mem.h>
#include <krad/web/web.h>
#include <krad/io/file2.h>
#include "types.h"

typedef enum {
  KR_XFER_CREATE = 1,
  KR_XFER_PATCH,
  KR_XFER_DESTROY
} kr_xfer_event_type;

struct kr_xfer_event {
  kr_xfer_path *path;
  void *user;
  void *user_path;
  kr_xfer_event_type type;
  kr_patch patch;
  kr_xfer_path_info info;
};

struct kr_xfer_setup {
  kr_web *web;
  kr_xfer_event_cb *event_cb;
  void *user;
  int npaths;
};

typedef struct {
  void *user;
  int backbuf;
  kr_amem(mem, 512)
} kr_stream_client;

typedef kr_stream_client kr_transmit_client;
typedef kr_stream_client kr_source_client;

typedef ssize_t (kr_stream_client_write_cb)(kr_stream_client *client,
 uint8_t *buf, size_t len, void *user);
typedef ssize_t (kr_stream_client_read_cb)(kr_stream_client *client,
 uint8_t *buf, size_t len);
typedef int (kr_stream_client_add_cb)(kr_stream_client *client, void *user);
typedef int (kr_stream_client_del_cb)(kr_stream_client *client, void *user);
typedef ssize_t (kr_uplink_read_cb)(void *user,
 uint8_t *buf, size_t len);
typedef ssize_t (kr_downlink_write_cb)(void *user,
 uint8_t *buf, size_t len);
typedef int (kr_xmit_destroy_cb)(void *user);
typedef int (kr_xf_connect_cb)(void *user);
typedef int (kr_xf_disconnect_cb)(void *user);
typedef int (kr_accepted_cb)(kr_file2 *file, void *user);

typedef struct {
  int max_tries;
  int duration;
  kr_xf_connect_cb *connect_cb;
  kr_xf_disconnect_cb *disconnect_cb;
} kr_xfer_reconnect_setup;

typedef struct {
  kr_uplink_info info;
  kr_uplink_read_cb *read_cb;
  void *user;
  kr_xfer_reconnect_setup reconnect;
} kr_uplink_setup;

typedef struct {
  char url[256];
  size_t len;
  kr_downlink_write_cb *write_cb;
  void *user;
  kr_xfer_reconnect_setup reconnect;
} kr_downlink_setup;

typedef struct {
  void *user;
  kr_accepted_cb *accepted_cb;
} kr_xfer_acceptor_setup;

typedef struct {
  void *user;
  kr_stream_client_add_cb *add_client;
  kr_stream_client_del_cb *del_client;
  kr_stream_client_write_cb *client_write;
} kr_xfer_receiver_setup;

typedef struct {
  void *user;
  kr_stream_client_add_cb *add_client;
  kr_stream_client_del_cb *del_client;
  kr_stream_client_read_cb *client_read;
  kr_xmit_destroy_cb *destroy_cb;
} kr_xfer_transmitter_setup;

typedef struct {
  kr_xfer_server_info info;
  union {
    void *user;
    kr_xfer_transmitter_setup txmtr;
    kr_xfer_receiver_setup rcvr;
    kr_xfer_acceptor_setup acptr;
  };
} kr_xfer_server_setup;

typedef struct {
  kr_xfer_client_info info;
  void *user;
  union {
    kr_upload_info upload;
    kr_download_info download;
    kr_uplink_setup uplink;
    kr_downlink_setup downlink;
  };
  kr_handler *reconnect_cb;
} kr_xfer_client_setup;

/*
  create serve:
    src: dir | file | buffer
    dst: xfer pathname
  create accept:
    src: xfer pathname
    dst: dir | file | buffer
  create upload:
    src: file | buffer
    dst: url
  create download:
    src: url
    dst: file | buffer
*/

int kr_xfer_path_web_client_set(kr_xfer_path *path, kr_web_client *client);

kr_loop *kr_xfer_loop(kr_xfer *xfer);
int kr_xfer_path_info_get(kr_xfer_path *path, kr_xfer_path_info *info);
int kr_xfer_ctl(kr_xfer_path *path, kr_patchset *patchset);

int kr_xfer_done(kr_xfer_path *path);
int kr_xfer_remove(kr_xfer_path *path);

int kr_xfer_match(kr_xfer *x, kr_http_method m, kr_path *addr);
int kr_xfer_headers(kr_xfer_path *path);
int kr_xfer_data(kr_xfer_path *path);

int kr_xfer_accept(kr_xfer *xfer, kr_web_client *client, void *user);
kr_xfer_path *kr_xfer_server(kr_xfer *xfer, kr_xfer_server_setup *setup);
kr_xfer_path *kr_xfer_connect(kr_xfer *xfer, kr_xfer_client_setup *setup);

kr_file_set *kr_xfer_get_file_set(kr_xfer *xfer);
int kr_xfer_destroy(kr_xfer *xfer);
kr_xfer *kr_xfer_create(kr_xfer_setup *setup);

#endif
