#include <krad/mem/mem.h>
#include "encoder.h"

typedef struct {
  int w;
  int h;
} plane_sz;

struct kr_y4m_encoder {
  size_t frame_sz;
  plane_sz plane[3];
  kr_y4m_encoder_info info;
  uint8_t header[KR_Y4M_HEADER_SIZE_MAX];
  size_t header_sz;  
};

static ssize_t generate_header(uint8_t *header, kr_y4m_encoder_info *info) {
  int n;
  char *fmt;
  fmt = NULL;
  n = 0;
  switch (info->pixel_fmt) {
  case KR_PIXELS_YUV420:
    fmt = "";
    break;
  case KR_PIXELS_YUV422:
    fmt = "C422";
    break;
  case KR_PIXELS_YUV444:
    fmt = "C444";
    break;
  default:
    return 0;
  }
  n = snprintf((char *)header, KR_Y4M_HEADER_SIZE_MAX,
   "%s W%d H%d F%d:%d Ip A1:1 %s\n", KR_Y4M_MAGIC, info->width, info->height,
   info->fps_num, info->fps_den, fmt);
  return n;
}

size_t kr_y4m_encoder_sizeof() {
  return sizeof(kr_y4m_encoder);
}

kr_y4m_encoder *kr_y4m_encoder_init(kr_y4m_encoder_setup *setup) {
  kr_y4m_encoder *e;
  if (!setup || !setup->mem) return NULL;
  e = (kr_y4m_encoder *)setup->mem;
  e->info = setup->info;
  e->plane[0].w = e->info.width;
  e->plane[0].h = e->info.height;
  switch (e->info.pixel_fmt) {
  case KR_PIXELS_YUV420:
    e->plane[1].w = e->info.width / 2;
    e->plane[1].h = e->info.height / 2;
    e->plane[2] = e->plane[1];
    break;
  case KR_PIXELS_YUV422:
    e->plane[1].w = e->info.width / 2;
    e->plane[1].h = e->info.height;
    e->plane[2] = e->plane[1];
    break;
  case KR_PIXELS_YUV444:
    e->plane[1] = e->plane[0];
    e->plane[2] = e->plane[1];
    break;
  default:
    return NULL;
  }
  e->frame_sz = e->plane[0].w * e->plane[0].h;
  e->frame_sz += e->plane[1].w * e->plane[1].h;
  e->frame_sz += e->plane[2].w * e->plane[2].h;
  e->header_sz = generate_header(e->header, &e->info);
  return e;
}

int kr_y4m_encoder_get_headers(kr_y4m_encoder *e, kr_codec_header *h) {
  if (!e || !h) return -1;
  h->data[0].buf = e->header;
  h->data[0].sz = e->header_sz;
  h->len = 1;
  return 0;
}

int kr_y4m_encode(kr_y4m_encoder *e, kr_frame *out, kr_frame *in) {
  int p;
  int y;
  uint8_t *src;
  size_t pos;
  if (!e || !out || !in) return -1;
  if (in->image.info.w != e->info.width
   || in->image.info.h != e->info.height
   || in->image.info.fmt != e->info.pixel_fmt) {
     /*printke("Y4M: Frame !match In: %"PRIu16"x%"PRIu16" %s Expected: %dx%d %s",
     in->image.info.w, in->image.info.h, kr_pixel_fmt_to_str(in->image.info.fmt),
     e->info.width, e->info.height, kr_pixel_fmt_to_str(e->info.pixel_fmt));*/
    printke("ffffffffffffffff");
    return -1;
  }
  out->info.ts = in->info.ts;
  out->info.keyframe = 1;
  out->type = KR_IMAGE_ENC;
  out->coded.info.sz = e->frame_sz;
  out->coded.info.fmt = KR_CODED_IMAGE_Y4M;
  out->coded.info.image.w = e->info.width;
  out->coded.info.image.h = e->info.height;
  out->coded.info.image.fmt = e->info.pixel_fmt;
  pos = 0;
  for (p = 0; p < 3; p++) {
    src = in->image.px[p];
    for (y = 0; y < e->plane[p].h; y++) {
      memcpy(out->coded.data + pos, src, e->plane[p].w);
      pos += e->plane[p].w;
      src += in->image.ps[p];
    }
  }
  if (pos != e->frame_sz) {
    printke("Y4M: frame data size !match: expected: %zu got %zu",
     e->frame_sz, pos);
  }
  return 0;
}
