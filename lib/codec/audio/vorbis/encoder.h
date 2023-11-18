#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>

#include <krad/codec/types.h>
#include <krad/codec/header.h>

#if !defined(_codec_audio_vorbis_vorbis_H)
# define _codec_audio_vorbis_vorbis_H (1)

typedef struct kr_vorbis_encoder kr_vorbis_encoder;

typedef struct {
  uint8_t *mem;
  kr_vorbis_info info;
} kr_vorbis_encoder_setup;

size_t kr_vorbis_encoder_sizeof();
kr_vorbis_encoder *kr_vorbis_encoder_init(kr_vorbis_encoder_setup *setup);
int kr_vorbis_encoder_get_headers(kr_vorbis_encoder *ve, kr_codec_header *h);
int kr_vorbis_encoder_get_info(kr_vorbis_encoder *ve, kr_vorbis_info *info);
int kr_vorbis_encode(kr_vorbis_encoder *ve, kr_coded_audio *out, kr_audio *in);
int kr_vorbis_encoder_ctl(kr_vorbis_encoder *ve, kr_patchset *patchset);
int kr_vorbis_encoder_destroy(kr_vorbis_encoder *ve);

#endif
