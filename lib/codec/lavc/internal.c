#include "internal.h"

AVCodec *h264;
AVCodec *aac;

int kr_lavc_global_init() {
  static int happened = 0;
  if (happened) return 0;
  happened = 1;
  /*avcodec_register_all();*/
  /* FAIL
  h264 = avcodec_find_decoder(AV_CODEC_ID_H264);
  aac = avcodec_find_decoder(AV_CODEC_ID_AAC);
  */
  if (!h264 || !aac) return -1;
  return 0;
}
