#include <stdlib.h>
#include <krad/app/debug.h>
#include "unpack.h"

struct kr_y4m_demuxer {
  kr_image_info image_info;
  kr_fps_info fps;
  uint64_t nframes;
};

ssize_t parse_frame_header(kr_y4m_demuxer *y, uint8_t *buf,
 size_t sz) {
  int pos;
  if (sz < KR_Y4M_FRAME_HEADER_SIZE) return -1;
  if (memcmp(buf, KR_Y4M_FRAME_HEADER, 5) != 0) return -1;  
  pos = 5;
  while (pos < sz) {
    if (buf[pos] == '\n') return pos + 1;
    pos++;
  }
  return -1;
}

static int parse_header(uint8_t *buffer, size_t sz, uint16_t *w, uint16_t *h,
 int32_t *fps_num, int32_t *fps_den, kr_pixel_fmt *color_depth) {
  int i;
  int len;
  char *tokstart, *tokend, *header_end;
  char header[KR_Y4M_HEADER_SIZE_MAX];
  int found_end;
  found_end = 0;
  i = 0;
  *w = 0;
  *h = 0;
  *fps_num = 0;
  *fps_den = 0;
  *color_depth = 0;
  for (i = 0; i < sz; i++) {
    header[i] = buffer[i];
    if (header[i] == '\n') {
      len = i + 1;
      header[i + 1] = 0x20;
      header[i + 2] = 0;
      found_end = 1;
      break;
    }
  }
  if (strncmp(header, KR_Y4M_MAGIC, KR_Y4M_MAGIC_SZ)) return -1;
  if (!found_end) return -1;
  header_end = &header[i + 1]; /* Include space */
  for (tokstart = &header[KR_Y4M_MAGIC_SZ]; tokstart < header_end; tokstart++) {
    if (*tokstart == 0x20) continue;
    switch (*tokstart++) {
    case 'W': // Width. Required.
      *w = strtol(tokstart, &tokend, 10);
      tokstart = tokend;
      break;
    case 'H': // Height. Required.
      *h = strtol(tokstart, &tokend, 10);
      tokstart = tokend;
      break;
    case 'C': // Color space
      if (strncmp("420jpeg", tokstart, 7) == 0) {
        *color_depth = KR_PIXELS_YUV420;
      } else if (strncmp("420mpeg2", tokstart, 8) == 0) {
        *color_depth = KR_PIXELS_YUV420;
      } else if (strncmp("420paldv", tokstart, 8) == 0) {
        *color_depth = KR_PIXELS_YUV420;
      } else if (strncmp("420", tokstart, 3) == 0) {
        *color_depth = KR_PIXELS_YUV420;
      } else if (strncmp("422", tokstart, 3) == 0) {
        *color_depth = KR_PIXELS_YUV422;
      } else if (strncmp("444", tokstart, 3) == 0) {
        *color_depth = KR_PIXELS_YUV444;
      } else {
        return -2;
      }
      while (tokstart < header_end && *tokstart != 0x20) tokstart++;
      break;
    case 'I': // Interlace type
      while (tokstart < header_end && *tokstart != 0x20) tokstart++;
      break;
    case 'F': // Frame rate
      sscanf(tokstart, "%u:%u", fps_num, fps_den); // 0:0 if unknown
      while (tokstart < header_end && *tokstart != 0x20) tokstart++;
      break;
    case 'A': // Pixel aspect
      //sscanf(tokstart, "%d:%d", &aspectn, &aspectd); // 0:0 if unknown
      while (tokstart < header_end && *tokstart != 0x20) tokstart++;
      break;
    case 'X': // Vendor extensions
      while (tokstart < header_end && *tokstart != 0x20) tokstart++;
      break;
    }
  }
  if (*w == 0 || *h == 0) {
      return -1;
  }
  if (*color_depth == 0) {
    *color_depth = KR_PIXELS_YUV420;
  }
  if (*fps_num == 0 || *fps_den == 0) {
      // Frame rate unknown
      *fps_num = 30;
      *fps_den = 1;
  }
  return len;
}

size_t kr_y4m_demuxer_sizeof() {
  return sizeof(kr_y4m_demuxer);
}

ssize_t kr_y4m_demuxer_read_frame_header(kr_y4m_demuxer *y, uint8_t *buf,
 size_t sz) {
  ssize_t ret;
  if (!y || !buf || sz < KR_Y4M_FRAME_HEADER_SIZE) return -1;
  ret = parse_frame_header(y, buf, sz);
  if (ret < 1) printke("Y4M Unpack: parse frame ret %d", ret);
  return ret;

}

ssize_t kr_y4m_demuxer_read_header(kr_y4m_demuxer *y, uint8_t *header,
 size_t sz) {
  ssize_t ret;
  if (!y || !header || sz < KR_Y4M_MAGIC_SZ) return -1;
  ret = parse_header(header, sz, &y->image_info.w, &y->image_info.h,
   &y->fps.num, &y->fps.den, &y->image_info.fmt);
  if (ret < 1) printke("Y4M Unpack: parse header ret %d", ret);
  return ret;
}

kr_y4m_demuxer *kr_y4m_demuxer_init(kr_y4m_demuxer_setup *setup) {
  ssize_t ret;
  kr_y4m_demuxer *d;
  if (!setup || !setup->mem) return NULL;
  d = (kr_y4m_demuxer *)setup->mem;
  memset(d, 0, sizeof(kr_y4m_demuxer));
  if (setup->header && setup->header_sz > 0) {
    ret = kr_y4m_demuxer_read_header(d, setup->header, setup->header_sz);
    if (ret < 1) return NULL;
  }
  return d;
}

int kr_y4m_demuxer_get_image_info(kr_y4m_demuxer *d, kr_image_info *info) {
  if (!d || !info) return -1;
  *info = d->image_info;
  return 0;
}

int kr_y4m_demuxer_get_fps(kr_y4m_demuxer *d, kr_fps_info *fps) {
  if (!d || !fps) return -1;
  *fps = d->fps;
  return 0;
}
