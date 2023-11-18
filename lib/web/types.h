#if !defined(_web_types_H)
# define _web_types_H (1)

/* Shoved over here because of some radio/type.h infinite loop horizon */

typedef enum {
  KR_RADIO_MODE_AUDIO = 1,
  KR_RADIO_MODE_AUDIOVIDEO
} kr_radio_mode;

typedef struct {
  char callsign[16];
  char name[128];
  int64_t started_on;
  kr_radio_mode mode;
  int version;
  char commit[48];
} kr_radio_info;

typedef enum {
  KR_PROTOCOL_HTTP = 1,
  KR_PROTOCOL_WEBSOCKET,
  KR_PROTOCOL_TCP
} kr_web_client_protocol;

typedef enum {
  KR_WEB_CLIENT_ACCEPT = 1,
  KR_WEB_CLIENT_CONNECT
} kr_web_client_type;

typedef enum {
  KR_WEB_RESOLVING = 1,
  KR_WEB_CONNECTING,
  KR_WEB_ACTIVE,
  KR_WEB_FLUSHING
} kr_web_client_state;

typedef enum {
  KR_WEB_SERVER_HTTP = 1,
  KR_WEB_SERVER_WEBSOCKET,
  KR_WEB_SERVER_HTTP_WEBSOCKET
} kr_web_server_type;

typedef enum {
  KR_MULTIPART_DONE = 0,
  KR_MULTIPART_INIT,
  KR_MULTIPART_INFO,
  KR_MULTIPART_DATA,
} kr_multipart_state;

typedef struct {
  kr_web_server_type type;
  uint16_t port; /* 0, 65535 */
  int addr[16];
  char iface[128];
  uint64_t conn_total;
  uint64_t created_on;
} kr_web_server_info;

typedef struct {
  kr_web_server_type type;
  uint16_t port; /* 0, 65535 */
  int addr[16];
  char iface[128];
} kr_web_server_setup_info;

typedef struct {
  uint16_t port; /* 0, 65535 */
  int addr[16];
  char host[256];
  int host_len;
} kr_web_remote_info;

typedef struct {
  uint64_t frame;
  uint64_t sz;
  uint64_t pos;
} kr_web_socket_io_state;

typedef enum {
  KR_WEBSOCKET_BINARY = 1,
  KR_WEBSOCKET_TEXT
} kr_websocket_frame_type;

typedef enum {
  KR_WS_PROTOCOL_UNKNOWN = 0,
  KR_WS_PROTOCOL_KRAD,
  KR_WS_PROTOCOL_STREAM,
  KR_WS_PROTOCOL_XMMS
} kr_websocket_protocol;

typedef struct {
  kr_websocket_frame_type type;
  kr_websocket_protocol proto;
  kr_web_socket_io_state input;
  kr_web_socket_io_state output;
} kr_web_socket_state;

typedef enum {
  KR_HTTP_IN_HEADER,
  KR_HTTP_IN_CONTENT
} kr_http_message_state;

typedef struct {
  kr_http_message_state state;
  uint64_t sz;
  uint64_t pos;
  uint64_t num;
  uint8_t chunked;
  kr_multipart_state multipart;
} kr_web_http_io_state;

typedef struct {
  kr_web_http_io_state input;
  kr_web_http_io_state output;
} kr_web_http_state;

typedef struct {
  kr_web_client_type type;
  kr_web_client_state state;
  kr_web_client_protocol protocol;
  union {
    kr_web_http_state http;
    kr_web_socket_state ws;
  };
  uint64_t created_on;
  uint64_t bytes_in;
  uint64_t bytes_out;
  int port; /* 0, 65535 */
  kr_web_remote_info remote;
} kr_web_client_info;

typedef struct {
  kr_web_client_protocol protocol;
  kr_web_remote_info remote;
} kr_web_client_setup_info;

typedef enum {
  KR_WEB_SERVER = 1,
  KR_WEB_CLIENT,
  KR_WEB_PROXY
} kr_web_path_type;

typedef struct {
  kr_web_path_type type;
  union {
    kr_web_server_info server;
    kr_web_client_info client;
  };
} kr_web_path_info;

typedef struct {
  kr_web_path_type type;
  union {
    kr_web_server_setup_info server;
    kr_web_client_setup_info client;
  };
} kr_web_path_setup_info;

typedef struct {
  char name[64];
} kr_path_info;

typedef struct {
  int len;
  kr_path_info path[64]; /* not null */
} kr_path_list;

#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"

#endif
