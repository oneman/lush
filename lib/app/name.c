#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/mem/pool.h>
#include "name.h"

#define KR_NAME_NODE_NCTX 4

typedef struct {
  kr_name name;
  kr_name_ctx ctx[KR_NAME_NODE_NCTX];
  int nctx;
  int dnode;
  kr_name *dname[2];
} name_node;

struct kr_names {
  kr_pool *pool;
};

ssize_t kr_name_snprint(kr_name *name, char *str, size_t max) {
  int len;
  name_node *n;
  n = (name_node *)name;
  if (!name || !str) return -1;
  if (!n->dnode) {
    len = n->name.len;
    if (max < len) return -1;
    memcpy(str, n->name.buf, n->name.len);
    str[len] = '\0';
    return len;
  } else {
    len = n->dname[0]->len + n->dname[1]->len + 1;
    if (max < len) return -1;
    memcpy(str, n->dname[0]->buf, n->dname[0]->len);
    str += n->dname[0]->len;
    *str++ = '/';
    memcpy(str, n->dname[1]->buf, n->dname[1]->len);
    str += n->dname[1]->len;
    *str = '\0';
    return len;
  }
}

int kr_name_has_ctx_type(kr_name *name, int type) {
  int c;
  name_node *cur;
  kr_name_ctx *ctx;
  if (!name) return 0;
  cur = (name_node *)name;
  for (c = 0; c < KR_NAME_NODE_NCTX; c++) {
    ctx = &cur->ctx[c];
    if (ctx->user != NULL && type == ctx->type) return 1;
  }
  return 0;
}

kr_name *kr_names_iter(kr_names *names, int *pos) {
  if (!names || !pos) return NULL;
  return kr_pool_iterate_active(names->pool, pos);
}

kr_name *kr_name_find(kr_names *names, char *str, size_t sz) {
  int i;
  name_node *cur;
  if (!names || !str || sz < 1 || sz > KR_NAME_LEN) return NULL;
  if (sz > sizeof(cur->name.buf)) return NULL;
  i = 0;
  while ((cur = kr_pool_iterate_active(names->pool, &i))) {
    if (sz != cur->name.len) continue;
    if (memcmp(cur->name.buf, str, sz) == 0) {
      return &cur->name;
    }
  }
  return NULL;
}

kr_name *kr_name_reverse(kr_names *names, void *user) {
  int i;
  int c;
  name_node *cur;
  kr_name_ctx *ctx;
  if (!names || !user) return NULL;
  i = 0;
  while ((cur = kr_pool_iterate_active(names->pool, &i))) {
    for (c = 0; c < KR_NAME_NODE_NCTX; c++) {
      ctx = &cur->ctx[c];
      if (ctx->user == user) {
        return &cur->name;
      }
    }
  }
  return NULL;
}

kr_name_ctx *kr_name_resolve_type(kr_names *names, int type, char *str,
 size_t sz) {
  int i;
  int c;
  name_node *cur;
  kr_name_ctx *ctx;
  if (!names || !str) return NULL;
  if (sz > sizeof(cur->name.buf)) return NULL;
  i = 0;
  while ((cur = kr_pool_iterate_active(names->pool, &i))) {
    if (sz != cur->name.len) continue;
    if (memcmp(cur->name.buf, str, sz) == 0) {
      for (c = 0; c < KR_NAME_NODE_NCTX; c++) {
        ctx = &cur->ctx[c];
        if (ctx->user != NULL) {
          if ((type == 0) || (type == ctx->type)) {
            return ctx;
          }
        }
      }
      break;
    }
  }
  return NULL;
}

kr_name_ctx *kr_name_resolve(kr_names *names, char *str, size_t sz) {
  return kr_name_resolve_type(names, 0, str, sz);
}

kr_name_ctx *kr_name_resolve2(kr_names *names, char *str, size_t sz,
 char *str2, size_t sz2) {
  int i;
  int c;
  name_node *cur;
  kr_name_ctx *ctx;
  if (!names || !str || !str2) return NULL;
  if (sz > sizeof(cur->name.buf)) return NULL;
  if (sz2 > sizeof(cur->name.buf)) return NULL;
  i = 0;
  while ((cur = kr_pool_iterate_active(names->pool, &i))) {
    if (!cur->dnode) continue;
    if (sz != cur->dname[0]->len) continue;
    if (sz2 != cur->dname[1]->len) continue;
    if ((memcmp(cur->dname[0]->buf, str, sz) == 0)
     && (memcmp(cur->dname[1]->buf, str2, sz2) == 0)) {
      for (c = 0; c < KR_NAME_NODE_NCTX; c++) {
        ctx = &cur->ctx[c];
        if (ctx->user != NULL) {
          return ctx;
        }
      }
      break;
    }
  }
  return NULL;
}

int kr_name_unlink(kr_names *names, kr_name *name, kr_name_ctx *ctx) {
  int ret;
  int i;
  name_node *node;
  kr_name_ctx *cur;
  ret = -1;
  if (!names || !name || !ctx) return -1;
  node = (name_node *)name;
  if (ctx->user == NULL) return -1;
  for (i = 0; i < KR_NAME_NODE_NCTX; i++) {
    cur = &node->ctx[i];
    if (cur->user == ctx->user && cur->type == ctx->type) {
      memset(cur, 0, sizeof(*cur));
      node->nctx--;
      ret = 0;
      break;
    }
  }
  if (ret == 0 && node->nctx == 0) {
    ret = kr_name_remove(names, name);
  }
  return ret;
}

int kr_name_link(kr_names *names, kr_name *name, kr_name_ctx *ctx) {
  int ret;
  int i;
  name_node *node;
  kr_name_ctx *cur;
  ret = -1;
  if (!names || !name || !ctx) return -1;
  node = (name_node *)name;
  if (node->nctx == KR_NAME_NODE_NCTX) return -1;
  for (i = 0; i < KR_NAME_NODE_NCTX; i++) {
    cur = &node->ctx[i];
    if (cur->user == NULL) {
      *cur = *ctx;
      node->nctx++;
      ret = 0;
      break;
    }
  }
  return ret;
}

kr_name *kr_name_attach(kr_names *names, kr_name *name, kr_name *name2) {
  name_node *new;
  if (!names || !name || !name2) return NULL;
  new = kr_pool_slice(names->pool);
  if (!new) return NULL;
  memset(new, 0, sizeof(*new));
  new->dnode = 1;
  new->dname[0] = name;
  new->dname[1] = name2;
  return &new->name;
}

int kr_name_remove(kr_names *names, kr_name *name) {
  int ret;
  ret = 0;
  if (!names || !name) return -1;
  ret = kr_pool_release(names->pool, name);
  if (ret != 0) printke("Name: pool release failed %d", ret);
  return ret;
}

kr_name *kr_name_add_str(kr_names *names, char *name, size_t sz) {
  name_node *n;
  if (!names || !name || sz < 1 || sz > KR_NAME_LEN) return NULL;
  n = kr_pool_slice(names->pool);
  if (!n) return NULL;
  memset(n, 0, sizeof(*n));
  n->name.len = sz;
  memcpy(n->name.buf, name, n->name.len);
  return &n->name;
}

kr_name *kr_name_add(kr_names *names, kr_name *name) {
  if (!names || !name) return NULL;
  return kr_name_add_str(names, name->buf, name->len);
}

int kr_names_destroy(kr_names *names) {
  if (names == NULL) return -1;
  kr_pool_destroy(names->pool);
  return 0;
}

kr_names *kr_names_create(kr_names_setup *setup) {
  kr_names *names;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  pool_setup.size = sizeof(name_node);
  pool_setup.slices = setup->nnodes;
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(*names);
  pool = kr_pool_create(&pool_setup);
  if (!pool) return NULL;
  names = kr_pool_overlay_get(pool);
  memset(names, 0, sizeof(*names));
  names->pool = pool;
  return names;
}
