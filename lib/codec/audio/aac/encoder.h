#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/codec/types.h>
#include <krad/codec/header.h>
#include <krad/audio/audio.h>

#if !defined(_codec_audio_aac_encoder_H)
# define _codec_audio_aac_encoder_H (1)

typedef struct kr_aac_encoder kr_aac_encoder;

typedef struct {
  void *mem;
  kr_aac_info info;
} kr_aac_encoder_setup;

size_t kr_aac_encoder_sizeof();
kr_aac_encoder *kr_aac_encoder_init(kr_aac_encoder_setup *setup);
int kr_aac_encoder_get_info(kr_aac_encoder *enc, kr_aac_info *info);
int kr_aac_encode(kr_aac_encoder *enc, kr_coded_audio *out, kr_audio *in);
int kr_aac_encoder_destroy(kr_aac_encoder *enc);

#endif
