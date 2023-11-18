#include <zlib.h>
#include <krad/app/debug.h>
#include "gzip.h"

struct kr_gzip {
  z_stream ctx;
  kr_gzip_type type;
  uint8_t mem[65536 * 5];
  size_t allocated;
  int init;
};

static void *gz_alloc(void *user, uint n, uint sz) {
  uint8_t *p;
  kr_gzip *gz;
  if (!user || !n || !sz) return NULL;
  gz = (kr_gzip *)user;
  if ((sizeof(gz->mem) - gz->allocated) < (n * sz)) {
    printke("kr_gzip: not enough mem, %zu vs %zu",
     sizeof(gz->mem) - gz->allocated, n * sz);
    return NULL;
  }
  p = gz->mem + gz->allocated;
  gz->allocated += (n * sz);
  return p;
}

static void gz_free(void *user, void *p) {
  return;
}

size_t kr_gzip_sizeof() {
  return sizeof(kr_gzip);
}

int kr_gzip_inflate(kr_gzip *gz, kr_gzip_data *data, int flush) {
  int ret;
  if (!gz || !data) return -1;
  if (gz->type != KR_GZIP_INFLATE) return -2;
  if (data->wrote != data->out_sz) {
    gz->ctx.avail_in = (uInt)data->in_sz;
    gz->ctx.next_in = (Bytef *)data->in;
  }
  gz->ctx.avail_out = (uInt)data->out_sz;
  gz->ctx.next_out = (Bytef *)data->out;
  if (!gz->init) {
    inflateInit2(&gz->ctx, 16 + MAX_WBITS);
    gz->init = 1;
  }
  if (!flush) {
    ret = inflate(&gz->ctx, Z_NO_FLUSH);
  } else {
    ret = inflate(&gz->ctx, Z_FINISH);
  }
  if (ret == Z_STREAM_END && flush) {
    data->read = data->in_sz - gz->ctx.avail_in;
    data->wrote = data->out_sz - gz->ctx.avail_out;
    inflateEnd(&gz->ctx);
    gz->init = 0;
    gz->allocated = 0;
    return 0;
  }
  if (ret < 0 && ret != Z_BUF_ERROR) {
    printke("kr_gzip: inflate failed %d", ret);
    return -3;
  }
  data->read = data->in_sz - gz->ctx.avail_in;
  data->wrote = data->out_sz - gz->ctx.avail_out;
  return 0;
}

int kr_gzip_deflate(kr_gzip *gz, kr_gzip_data *data, int flush) {
  int ret;
  if (!gz || !data) return -1;
  if (gz->type != KR_GZIP_DEFLATE) return -2;
  if (data->wrote != data->out_sz) {
    gz->ctx.avail_in = (uInt)data->in_sz;
    gz->ctx.next_in = (Bytef *)data->in;
  }
  gz->ctx.avail_out = (uInt)data->out_sz;
  gz->ctx.next_out = (Bytef *)data->out;
  if (!gz->init) {
    deflateInit2(&gz->ctx, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
     16 + MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
    gz->init = 1;
  }
  if (gz->ctx.avail_out < deflateBound(&gz->ctx, gz->ctx.avail_in)) {
    printke("kr_gzip: prolly not enough space in outbut buffer %zu vs %zu",
      gz->ctx.avail_out, deflateBound(&gz->ctx, gz->ctx.avail_in));
    return -3;
  }
  if (!flush) {
    ret = deflate(&gz->ctx, Z_NO_FLUSH);
  } else {
    ret = deflate(&gz->ctx, Z_FINISH);
  }
  if (ret == Z_STREAM_END && flush) {
    data->read = data->in_sz - gz->ctx.avail_in;
    data->wrote = data->out_sz - gz->ctx.avail_out;
    deflateEnd(&gz->ctx);
    gz->init = 0;
    gz->allocated = 0;
    return 0;
  }
  if (ret < 0 && ret != Z_BUF_ERROR) {
    printke("kr_gzip: inflate failed %d", ret);
    return -3;
  }
  data->read = data->in_sz - gz->ctx.avail_in;
  data->wrote = data->out_sz - gz->ctx.avail_out;
  return 0;
}

kr_gzip *kr_gzip_init(kr_gzip_setup *setup) {
  kr_gzip *gz;
  if (!setup || !setup->mem) return NULL;
  if (!setup->type) return NULL;
  gz = (kr_gzip *)setup->mem;
  memset(gz, 0, sizeof(kr_gzip));
  gz->ctx.zalloc = gz_alloc;
  gz->ctx.zfree = gz_free;
  gz->ctx.opaque = gz;
  gz->type = setup->type;
  return gz;
}
