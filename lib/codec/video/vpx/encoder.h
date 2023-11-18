#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>
#include <krad/codec/types.h>
#include <krad/codec/header.h>

#if !defined(_codec_video_vpx_vpx_H)
# define _codec_video_vpx_vpx_H (1)

typedef struct kr_vpx_encoder kr_vpx_encoder;

typedef struct {
  uint8_t *mem;
  kr_vpx_encoder_info info;
} kr_vpx_encoder_setup;

size_t kr_vpx_encoder_sizeof();
kr_vpx_encoder *kr_vpx_encoder_init(kr_vpx_encoder_setup *setup);
int kr_vpx_encoder_get_headers(kr_vpx_encoder *vpx, kr_codec_header *h);
int kr_vpx_encoder_get_info(kr_vpx_encoder *vpx,
 kr_vpx_encoder_info *info);
ssize_t kr_vpx_encode(kr_vpx_encoder *vpx, kr_frame *out, kr_frame *in);
int kr_vpx_encoder_ctl(kr_vpx_encoder *vpx, kr_patchset *patchset);
int kr_vpx_encoder_destroy(kr_vpx_encoder *vpx);

#endif
