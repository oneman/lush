#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/codec/types.h>
#include <krad/codec/header.h>
#include <krad/audio/audio.h>

#if !defined(_codec_audio_opus_decoder_H)
# define _codec_audio_opus_decoder_H (1)

typedef struct kr_opus_decoder kr_opus_decoder;

typedef struct {
  void *mem;
  kr_opus_info info;
} kr_opus_decoder_setup;

size_t kr_opus_decoder_sizeof();
kr_opus_decoder *kr_opus_decoder_init(kr_opus_decoder_setup *setup);
int kr_opus_decoder_get_info(kr_opus_decoder *dec, kr_opus_info *info);
int kr_opus_decode(kr_opus_decoder *dec, kr_audio *out, kr_coded_audio *in);
int kr_opus_decoder_destroy(kr_opus_decoder *dec);

#endif
