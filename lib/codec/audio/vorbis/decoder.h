#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/codec/types.h>
#include <krad/codec/header.h>
#include <krad/audio/audio.h>

#if !defined(_codec_audio_vorbis_decoder_H)
# define _codec_audio_vorbis_decoder_H (1)

typedef struct kr_vorbis_decoder kr_vorbis_decoder;

typedef struct {
  void *mem;
  kr_codec_header header;
} kr_vorbis_decoder_setup;

size_t kr_vorbis_decoder_sizeof();
kr_vorbis_decoder *kr_vorbis_decoder_init(kr_vorbis_decoder_setup *setup);
int kr_vorbis_decoder_get_info(kr_vorbis_decoder *dec, kr_vorbis_info *info);
int kr_vorbis_decode(kr_vorbis_decoder *dec, kr_audio *out, kr_coded_audio *in);
int kr_vorbis_decoder_destroy(kr_vorbis_decoder *dec);

#endif
