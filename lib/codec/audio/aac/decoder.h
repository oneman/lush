#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/codec/types.h>
#include <krad/codec/header.h>
#include <krad/audio/audio.h>

#if !defined(_codec_audio_aac_decoder_H)
# define _codec_audio_aac_decoder_H (1)

typedef struct kr_aac_decoder kr_aac_decoder;

typedef struct {
  void *mem;
} kr_aac_decoder_setup;

size_t kr_aac_decoder_sizeof();
kr_aac_decoder *kr_aac_decoder_init(kr_aac_decoder_setup *setup);
int kr_aac_decoder_get_info(kr_aac_decoder *dec, kr_aac_info *info);
int kr_aac_decode(kr_aac_decoder *dec, kr_audio *out, kr_coded_audio *in);
int kr_aac_decoder_destroy(kr_aac_decoder *dec);

#endif
