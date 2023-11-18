#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>
#include <krad/codec/types.h>
#include <krad/codec/header.h>

#if !defined(_codec_video_h264_h264_H)
# define _codec_video_h264_h264_H (1)

typedef struct kr_h264_encoder kr_h264_encoder;

typedef struct {
  uint8_t *mem;
  kr_h264_encoder_info info;
} kr_h264_encoder_setup;

size_t kr_h264_encoder_sizeof();
kr_h264_encoder *kr_h264_encoder_init(kr_h264_encoder_setup *setup);
int kr_h264_encoder_get_headers(kr_h264_encoder *h264, kr_codec_header *h);
int kr_h264_encoder_get_info(kr_h264_encoder *h264,
 kr_h264_encoder_info *info);
ssize_t kr_h264_encode(kr_h264_encoder *h264, kr_frame *out, kr_frame *in);
int kr_h264_encoder_ctl(kr_h264_encoder *h264, kr_patchset *patchset);
int kr_h264_encoder_destroy(kr_h264_encoder *h264);

#endif
