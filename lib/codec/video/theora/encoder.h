#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>

#include <krad/codec/types.h>
#include <krad/codec/header.h>

#if !defined(_codec_video_theora_theora_H)
# define _codec_video_theora_theora_H (1)

typedef struct kr_theora_encoder kr_theora_encoder;

typedef struct {
  uint8_t *mem;
  kr_theora_encoder_info info;
} kr_theora_encoder_setup;

size_t kr_theora_encoder_sizeof();
kr_theora_encoder *kr_theora_encoder_init(kr_theora_encoder_setup *setup);
int kr_theora_encoder_get_headers(kr_theora_encoder *th, kr_codec_header *h);
int kr_theora_encoder_get_info(kr_theora_encoder *th,
 kr_theora_encoder_info *info);
int kr_theora_encode(kr_theora_encoder *th, kr_frame *out, kr_frame *in);
int kr_theora_encoder_ctl(kr_theora_encoder *th, kr_patchset *patchset);
int kr_theora_encoder_destroy(kr_theora_encoder *th);

#endif
