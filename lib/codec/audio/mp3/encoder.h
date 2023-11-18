#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/codec/types.h>
#include <krad/codec/header.h>
#include <krad/audio/audio.h>

#if !defined(_codec_audio_mp3_encoder_H)
# define _codec_audio_mp3_encoder_H (1)

typedef struct kr_mp3_encoder kr_mp3_encoder;

typedef struct {
  void *mem;
  kr_mp3_info info;
} kr_mp3_encoder_setup;

size_t kr_mp3_encoder_sizeof();
kr_mp3_encoder *kr_mp3_encoder_init(kr_mp3_encoder_setup *setup);
int kr_mp3_encoder_get_info(kr_mp3_encoder *enc, kr_mp3_info *info);
int kr_mp3_encode(kr_mp3_encoder *enc, kr_coded_audio *out, kr_audio *in);
int kr_mp3_encoder_destroy(kr_mp3_encoder *enc);

#endif
