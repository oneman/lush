#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/codec/types.h>
#include <krad/codec/header.h>
#include <krad/audio/audio.h>

#if !defined(_codec_audio_flac_decoder_H)
# define _codec_audio_flac_decoder_H (1)

typedef struct kr_flac_decoder kr_flac_decoder;

typedef struct {
  void *mem;
} kr_flac_decoder_setup;

size_t kr_flac_decoder_sizeof();
kr_flac_decoder *kr_flac_decoder_init(kr_flac_decoder_setup *setup);
int kr_flac_decoder_get_info(kr_flac_decoder *dec, kr_flac_info *info);
int kr_flac_decode(kr_flac_decoder *dec, kr_audio *out, kr_coded_audio *in);
int kr_flac_decoder_flush(kr_flac_decoder *dec, kr_audio *out);
int kr_flac_decoder_destroy(kr_flac_decoder *dec);

#endif
