#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>

#include <krad/codec/types.h>
#include <krad/codec/header.h>
#include <krad/container/y4m/common.h>

#if !defined(_codec_video_y4m_y4m_H)
# define _codec_video_y4m_y4m_H (1)

typedef struct kr_y4m_encoder kr_y4m_encoder;

typedef struct {
  uint8_t *mem;
  kr_y4m_encoder_info info;
} kr_y4m_encoder_setup;

size_t kr_y4m_encoder_sizeof();
kr_y4m_encoder *kr_y4m_encoder_init(kr_y4m_encoder_setup *setup);
int kr_y4m_encoder_get_headers(kr_y4m_encoder *e, kr_codec_header *h);
int kr_y4m_encode(kr_y4m_encoder *e, kr_frame *out, kr_frame *in);

#endif
