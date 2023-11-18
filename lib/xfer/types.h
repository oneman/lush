#if !defined(_xfer_types_H)
# define _xfer_types_H (1)

#include <krad/web/types.h>

/*
*      teleport files
* ===================
*       download  !<<  download a file
*         upload  !>>  upload a file somewhere
*         accept  <<!  accepting - accept a file upload
*          serve  >>!  sharing   - serve a file like an apache would
*
*   tranceive streams
*  ==================
*       downlink  !<<  request, tune into remote stream
*         uplink  !>>  send stream to another server
*         source  <<!  receiver - incoming source connection
*       transmit  >>!  transmission - typical user connects and requests stream
*
*/

typedef enum {
  KR_UPLOAD_FILE = 1,
  KR_UPLOAD_STASH
} kr_upload_type;

typedef enum {
  KR_DOWNLOAD_FILE = 1,
  KR_DOWNLOAD_STASH
} kr_download_type;

typedef struct {
  char url[512];
  int url_len;
  char path[512];
  int path_len;
} kr_upload_file_info;

typedef struct {
  char url[512];
  int url_len;
  char id[16];
  int id_len;
} kr_upload_stash_info;

typedef struct {
  char url[512];
  int url_len;
  char path[512];
  int path_len;
} kr_download_file_info;

typedef struct {
  char url[512];
  int url_len;
  char id[16];
  int id_len;
} kr_download_stash_info;

// maybe we can reduce redundancy later

typedef struct {
  kr_upload_type type;
  union {
    kr_upload_file_info file;
    kr_upload_stash_info stash;
  };
} kr_upload_info;

typedef struct {
  kr_download_type type;
  union {
    kr_download_file_info file;
    kr_download_stash_info stash;
  };
} kr_download_info;

typedef struct {
  char path[256];
  uint64_t path_len;
  uint64_t pos;
  uint64_t len;
} kr_accept_info;

typedef struct {
  uint64_t start;
  uint64_t end;
} kr_serve_range_info;

#define KR_XFER_NRANGES 4

typedef struct {
  uint64_t pos;
  uint64_t len;
  uint64_t sz;
  int nranges;
  kr_serve_range_info range[KR_XFER_NRANGES];
} kr_serve_ranges_info;

typedef struct {
  uint64_t pos;
  uint64_t len;
} kr_serve_full_info;

typedef enum {
  KR_SERVE_INDEX_JSON = 1,
  KR_SERVE_INDEX_HTML
} kr_serve_index_format;

typedef struct {
  uint64_t cnt;
  kr_serve_index_format format;
} kr_serve_index_info;

typedef struct {
  int coconut;
} kr_serve_redirect_info;

typedef enum {
  KR_SERVE_FULL = 1,
  KR_SERVE_RANGES,
  KR_SERVE_INDEX,
  KR_SERVE_REDIRECT
} kr_serve_type;

typedef struct {
  kr_serve_type type;
  union {
    kr_serve_full_info full;
    kr_serve_ranges_info ranges;
    kr_serve_index_info index;
    kr_serve_redirect_info redirect;
  };
} kr_serve_info;

typedef struct {
  char url[256];
  int len;
} kr_downlink_info;

typedef struct {
  char url[256];
  int len;
  uint8_t legacy; /* opt */
} kr_uplink_info;

typedef struct {
  int coconut;
} kr_source_info;

typedef struct {
  int packed_header;
} kr_transmit_info;

typedef enum {
  KR_DOWNLOAD = 1,
  KR_UPLOAD,
  KR_ACCEPT,
  KR_SERVE,
  KR_DOWNLINK,
  KR_UPLINK,
  KR_SOURCE,
  KR_TRANSMIT
} kr_xfer_client_type;

typedef struct {
  int max_tries;
  uint64_t ms;
} kr_xfer_reconnect_info;

typedef struct {
  uint64_t rx;
  uint64_t tx;
  kr_xfer_client_type type;
  union {
    kr_download_info download;
    kr_upload_info upload;
    kr_accept_info accept;
    kr_serve_info serve;
    kr_downlink_info downlink;
    kr_uplink_info uplink;
    kr_source_info source;
    kr_transmit_info transmit;
  };
  char path[512];
  kr_web_client_info client;
  kr_xfer_reconnect_info reconnect;
} kr_xfer_client_info;

typedef struct {
  char path[512];
  int len;
} kr_xfer_acceptor_dir_info;

typedef struct {
  int coconut;
} kr_xfer_acceptor_file_info;

typedef struct {
  int coconut;
} kr_xfer_acceptor_buf_info;

typedef struct {
  char path[512];
  int len;
} kr_xfer_share_dir_info;

typedef struct {
  int coconut;
} kr_xfer_share_file_info;

typedef struct {
  int coconut;
} kr_xfer_share_buf_info;

typedef enum {
  KR_XFER_ACCEPT_DIR = 1,
  KR_XFER_ACCEPT_FILE,
  KR_XFER_ACCEPT_BUF
} kr_xfer_acceptor_type;

typedef struct {
  kr_xfer_acceptor_type type;
  union {
    kr_xfer_acceptor_dir_info dir;
    kr_xfer_acceptor_file_info file;
    kr_xfer_acceptor_buf_info buf;
  };
} kr_xfer_acceptor_info;

typedef struct {
  int sources;
} kr_xfer_receiver_info;

typedef enum {
  KR_XFER_TRANSMITTER_HTTP = 1,
  KR_XFER_TRANSMITTER_WS
} kr_xfer_transmitter_type;

typedef struct {
  kr_xfer_transmitter_type type;
  int clients; /* opt */
} kr_xfer_transmitter_info;

typedef enum {
  KR_XFER_SHARE_DIR = 1,
  KR_XFER_SHARE_FILE,
  KR_XFER_SHARE_BUF
} kr_xfer_share_type;

typedef struct {
  kr_xfer_share_type type;
  union {
    kr_xfer_share_dir_info dir;
    kr_xfer_share_file_info file;
    kr_xfer_share_buf_info buf;
  };
} kr_xfer_share_info;

typedef enum {
  KR_XFER_SHARING = 1,
  KR_XFER_ACCEPTING,
  KR_XFER_TRANSMITTER,
  KR_XFER_RECEIVER
} kr_xfer_server_type;

typedef struct {
  char path[512];
  int len;
  kr_xfer_server_type type;
  union {
    kr_xfer_share_info share;
    kr_xfer_acceptor_info acceptor;
    kr_xfer_transmitter_info transmitter;
    kr_xfer_receiver_info receiver;
  };
} kr_xfer_server_info;

typedef enum {
  KR_XFER_SERVER = 1,
  KR_XFER_CLIENT
} kr_xfer_path_type;

typedef struct {
  kr_xfer_path_type type;
  union {
    kr_xfer_server_info server;
    kr_xfer_client_info client;
  };
} kr_xfer_path_info;

#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_helpers.h"
#include "gen/types_to_text.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"

#endif
