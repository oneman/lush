#include <krad/web/web.h>

#if !defined(_web_http_http_H)
# define _web_http_http_H (1)

#include "types.h"

typedef enum {
  KR_HTTP_VERSION_UNKNOWN,
  KR_HTTP_VERSION_1_1
} kr_http_version;

#define KR_HTTP_METHOD_FIRST KR_HTTP_GET
#define KR_HTTP_METHOD_LAST KR_HTTP_TRACE

#define KR_HTTP_HEADERS_MAX 32
#define KR_HTTP_PARAMS_MAX 32

typedef kr_web_key_value kr_http_header;
typedef kr_web_key_value kr_http_param;

typedef struct {
  kr_http_header hdr;
  char str[32];
} kr_http_date_header;

typedef enum {
  KR_HTTP_RANGE_START_FROM = 1,
  KR_HTTP_RANGE_SPEC
} kr_http_range_type;

typedef struct {
  uint16_t port; /* 0, 65535 */
  char *host;
  int host_len;
  char *path;
  size_t path_len;
  char *user;
  int user_len;
  char *pwd;
  int pwd_len;
  kr_http_param param[KR_HTTP_PARAMS_MAX];
  int param_count;
} kr_http_url;

typedef struct {
  kr_http_range_type type;
  int64_t start;
  int64_t end;
} kr_http_range;

typedef struct {
  kr_http_method method;
  char *path;
  size_t path_len;
  kr_http_param param[KR_HTTP_PARAMS_MAX];
  int param_count;
  kr_http_version version;
} kr_http_request_line;

typedef struct {
  kr_http_request_line req_line;
  kr_http_header header[KR_HTTP_HEADERS_MAX];
  int count;
} kr_http_request;

typedef struct {
  kr_http_response_type type;
  kr_http_header headers[KR_HTTP_HEADERS_MAX];
  int count;
} kr_http_response;

typedef struct {
  kr_http_request request;
  kr_http_response response;
} kr_http;

int kr_http_url_parse(kr_http_url *url, char *buf, size_t len);
ssize_t kr_http_url_decode(char *out, size_t out_len,
 char *in, size_t in_len);
int kr_http_req_indata(kr_http_request *req);
int kr_http_upgrade_wanted(kr_http_request *req);
int kr_http_get_header(kr_http_header *header, char *name, size_t len,
 kr_http_request *req);
int kr_http_request_get_ranges(kr_http_request *req, kr_http_range *ranges,
 int n);
ssize_t kr_http_request_parse(kr_http_request *req, char *buf, size_t sz);
ssize_t kr_http_response_parse(kr_http_response *res, char *buf, size_t sz);
ssize_t kr_http_response_string(char *str, kr_http_response *res, size_t max);
ssize_t kr_http_request_string(char *str, kr_http_request *req, size_t max);
int kr_http_length_header_init(kr_http_header *hdr, size_t len, uint8_t *buf,
 size_t max);
int kr_http_range_header_init(kr_http_header *hdr, size_t start, size_t end,
 size_t sz, uint8_t *buf, size_t max);
int kr_http_response_any_origin(kr_http_response *res);
int kr_http_response_zero_content(kr_http_response *res);
int kr_http_response_transfer_chunked(kr_http_response *res);
int kr_http_response_content_encoding(kr_http_response *res);
int kr_http_response_connection_close(kr_http_response *res);
int kr_http_response_accept_ranges(kr_http_response *res);
int kr_http_response_get_header(kr_http_header *header, char *name, size_t len,
 kr_http_response *res);
ssize_t kr_http_response_add_header(kr_http_response *response,
 kr_http_header *header);
ssize_t kr_http_request_add_header(kr_http_request *request,
 kr_http_header *header);
ssize_t kr_http_request_add_param(kr_http_request *r, kr_http_param *param);
int kr_http_request_init(kr_http_request *req);
int kr_http_request_set_method(kr_http_request *req, kr_http_method meth);
int kr_http_request_set_path(kr_http_request *req, char *path, size_t len);
int kr_http_response_add_version_rand(kr_http_response *res);
int kr_http_response_init(kr_http_response *res, kr_http_response_type type);

int kr_http_lastmod_header_at(kr_http_date_header *dh, time_t *now);
int kr_http_date_header_at(kr_http_date_header *dh, time_t *now);
int kr_http_lastmod_header_now(kr_http_date_header *dh);
int kr_http_date_header_now(kr_http_date_header *dh);

#endif
