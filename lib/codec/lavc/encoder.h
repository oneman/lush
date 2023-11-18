#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>

#include <krad/codec/types.h>
#include <krad/codec/header.h>

#if !defined(_codec_audio_lavc_lavc_H)
# define _codec_audio_lavc_lavc_H (1)

typedef struct kr_lavc_encoder kr_lavc_encoder;

typedef struct {
  uint8_t *mem;
  kr_lavc_encoder_info info;
} kr_lavc_encoder_setup;

size_t kr_lavc_encoder_sizeof();
kr_lavc_encoder *kr_lavc_encoder_init(kr_lavc_encoder_setup *setup);
int kr_lavc_encoder_get_headers(kr_lavc_encoder *e, kr_iov *v, size_t len);
int kr_lavc_encoder_get_info(kr_lavc_encoder *e, kr_lavc_encoder_info *info);
int kr_lavc_encode_audio(kr_lavc_encoder *e, kr_coded_audio *out, kr_audio *in);
int kr_lavc_encode_video(kr_lavc_encoder *e, kr_frame *out, kr_frame *in);
int kr_lavc_encoder_ctl(kr_lavc_encoder *e, kr_patchset *patchset);
int kr_lavc_encoder_destroy(kr_lavc_encoder *e);

#endif
