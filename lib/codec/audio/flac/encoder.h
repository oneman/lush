#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/audio/audio.h>

#include <krad/codec/types.h>
#include <krad/codec/header.h>

#if !defined(_codec_audio_flac_encoder_H)
# define _codec_audio_flac_encoder_H (1)

typedef struct kr_flac_encoder kr_flac_encoder;

typedef struct {
  uint8_t *mem;
  kr_flac_info info;
} kr_flac_encoder_setup;

size_t kr_flac_encoder_sizeof();
kr_flac_encoder *kr_flac_encoder_init(kr_flac_encoder_setup *setup);
int kr_flac_encoder_get_headers(kr_flac_encoder *fe, kr_codec_header *h);
int kr_flac_encoder_get_info(kr_flac_encoder *fe, kr_flac_info *info);
int kr_flac_encode(kr_flac_encoder *fe, kr_coded_audio *out, kr_audio *in);
int kr_flac_encoder_ctl(kr_flac_encoder *fe, kr_patchset *patchset);
int kr_flac_encoder_destroy(kr_flac_encoder *fe);

#endif
