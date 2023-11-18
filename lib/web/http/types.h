#if !defined(_web_http_types_H)
# define _web_http_types_H (1)

typedef enum {
  KR_HTTP_GET = 1,
  KR_HTTP_PUT,
  KR_HTTP_SOURCE,
  KR_HTTP_POST,
  KR_HTTP_HEAD,
  KR_HTTP_PATCH,
  KR_HTTP_DELETE,
  KR_HTTP_OPTIONS,
  KR_HTTP_TRACE
} kr_http_method;

typedef enum {
  KR_HTTP_ERROR,
  KR_HTTP_OK,
  KR_HTTP_CREATED,
  KR_HTTP_PARTIAL,
  KR_HTTP_MOVED,
  KR_HTTP_FOUND,
  KR_HTTP_NOT_FOUND,
  KR_HTTP_FORBIDDEN,
  KR_HTTP_BAD_REQUEST,
  KR_HTTP_METHOD_NOPE,
  KR_HTTP_RANGE_NOPE,
  KR_HTTP_CONFLICT,
  KR_HTTP_CONTINUE,
  KR_HTTP_UNKNOWN
} kr_http_response_type;

#endif
