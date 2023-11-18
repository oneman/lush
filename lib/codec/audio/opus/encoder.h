#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>

#include <krad/codec/types.h>
#include <krad/codec/header.h>

#if !defined(_codec_audio_opus_encoder_H)
# define _codec_audio_opus_encoder_H (1)

typedef struct kr_opus_encoder kr_opus_encoder;

typedef struct {
  uint8_t *mem;
  kr_opus_info info;
} kr_opus_encoder_setup;

size_t kr_opus_encoder_sizeof();
kr_opus_encoder *kr_opus_encoder_init(kr_opus_encoder_setup *setup);
int kr_opus_encoder_get_headers(kr_opus_encoder *oe, kr_codec_header *h);
int kr_opus_encoder_get_info(kr_opus_encoder *oe, kr_opus_info *info);
int kr_opus_encode(kr_opus_encoder *oe, kr_coded_audio *out, kr_audio *in);
int kr_opus_encoder_ctl(kr_opus_encoder *oe, kr_patchset *patchset);
int kr_opus_encoder_destroy(kr_opus_encoder *oe);

#endif
