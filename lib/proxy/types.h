#if !defined(_proxy_types_H)
# define _proxy_types_H (1)

#include <krad/web/types.h>

typedef enum {
  KR_PROXY_HTTP = 1,
  KR_PROXY_WEBSOCKET,
  KR_PROXY_WEBSOCKET_TO_TCP
} kr_proxy_type;

typedef struct {
  kr_proxy_type type;
  kr_web_remote_info dst;
  char path[32];
} kr_proxy_info;

#endif
