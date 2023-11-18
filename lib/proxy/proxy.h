#include <sys/types.h>

#if !defined(_proxy_proxy_H)
# define _proxy_proxy_H (1)

#include <krad/web/web.h>
#include "types.h"

typedef struct kr_proxy kr_proxy;

typedef struct {
  uint8_t *mem;
  kr_proxy_info info;
  kr_web *web;
} kr_proxy_setup;

int kr_proxy_accept(kr_proxy *p, kr_web_path *path, kr_path *addr);
int kr_proxy_destroy(kr_proxy *proxy);
kr_proxy *kr_proxy_create(kr_proxy_setup *setup);
size_t kr_proxy_sizeof();

#endif
