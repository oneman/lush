#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>

#include <krad/codec/types.h>
#include <krad/codec/header.h>

#if !defined(_codec_video_daala_daala_H)
# define _codec_video_daala_daala_H (1)

typedef struct kr_daala_encoder kr_daala_encoder;

typedef struct {
  uint8_t *mem;
  kr_daala_encoder_info info;
} kr_daala_encoder_setup;

size_t kr_daala_encoder_sizeof();
kr_daala_encoder *kr_daala_encoder_init(kr_daala_encoder_setup *setup);
int kr_daala_encoder_get_headers(kr_daala_encoder *dh, kr_codec_header *h);
int kr_daala_encoder_get_info(kr_daala_encoder *dh,
 kr_daala_encoder_info *info);
int kr_daala_encode(kr_daala_encoder *dh, kr_frame *out, kr_frame *in);
int kr_daala_encoder_ctl(kr_daala_encoder *dh, kr_patchset *patchset);
int kr_daala_encoder_destroy(kr_daala_encoder *dh);

#endif
