#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/codec/types.h>
#include <krad/codec/header.h>
#include <krad/audio/audio.h>

#if !defined(_codec_audio_mp3_decoder_H)
# define _codec_audio_mp3_decoder_H (1)

typedef struct kr_mp3_decoder kr_mp3_decoder;

typedef struct {
  void *mem;
} kr_mp3_decoder_setup;

size_t kr_mp3_decoder_sizeof();
kr_mp3_decoder *kr_mp3_decoder_init(kr_mp3_decoder_setup *setup);
int kr_mp3_decoder_get_info(kr_mp3_decoder *dec, kr_mp3_info *info);
int kr_mp3_decode(kr_mp3_decoder *dec, kr_audio *out, kr_coded_audio *in);
int kr_mp3_decoder_destroy(kr_mp3_decoder *dec);

#endif
