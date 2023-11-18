#include <inttypes.h>
#include <math.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <krad/mem/mem.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>
#include "internal.h"
#include "decoder.h"

struct kr_audio_dec {
  AVCodecContext *ctx;
  AVFrame *frm;
  int future_bongos;
};

struct kr_video_dec {
  AVCodecContext *ctx;
  AVFrame *frm;
  int future_bongos;
};

int kr_avc_decode_audio(kr_audio_dec *dec, uint8_t *data, size_t sz) {
  int decoded;
  int len;
  AVPacket pkt;
  if (!dec) return -1;
  av_init_packet(&pkt);
  pkt.data = data;
  pkt.size = sz;
  /*ya*/
  len = 0;
  /*len = avcodec_decode_audio4(dec->ctx, dec->frm, &decoded, &pkt);*/
  if (len < 0) return -2;
  if (!decoded) return 0;
  //av_samples_get_buffer_size(NULL, dec->ctx->channels, dec->frm->nb_samples,
    //dec->ctx->sample_fmt, 1);
  return len;
}

int kr_avc_decode_video(kr_video_dec *dec, kr_frame *frame,
 kr_coded_image *img) {
  int decoded;
  int len;
  AVPacket pkt;
  if (!dec || !frame || !img) return -1;
  //av_frame_unref(dec->frm);
  av_init_packet(&pkt);
  pkt.data = img->data;
  pkt.size = img->info.sz;
  /*FAIL len = avcodec_decode_video2(dec->ctx, dec->frm, &decoded, &pkt);*/
  /*ya*/
  len = 0;
  if (len < 0) return -2;
  if (!decoded) return 0;
  frame->image.px[0] = dec->frm->data[0];
  frame->image.px[1] = dec->frm->data[1];
  frame->image.px[2] = dec->frm->data[2];
  frame->image.ps[0] = dec->frm->linesize[0];
  frame->image.ps[1] = dec->frm->linesize[1];
  frame->image.ps[2] = dec->frm->linesize[2];
  frame->image.info.w = dec->frm->width;
  frame->image.info.h = dec->frm->height;
  if (dec->frm->format == AV_PIX_FMT_YUV420P)
    frame->image.info.fmt = KR_PIXELS_YUV420;
  /*FAIL frame->info.tc = av_frame_get_best_effort_timestamp(dec->frm);*/
  frame->info.keyframe = dec->frm->key_frame;
  return len;
}

kr_audio_dec *kr_avc_audio_decoder_create() {
  int ret;
  kr_audio_dec *dec;
  if (kr_lavc_global_init() < 0) return NULL;
  dec = kr_allocz(1, sizeof(*dec));
  dec->frm = av_frame_alloc();
  dec->ctx = avcodec_alloc_context3(aac);
  ret = avcodec_open2(dec->ctx, aac, NULL);
  if (ret < 0) return NULL;
  return dec;
}

int kr_avc_audio_decoder_destroy(kr_audio_dec *dec) {
  if (!dec) return -1;
  avcodec_close(dec->ctx);
  avcodec_free_context(&dec->ctx);
  av_frame_free(&dec->frm);
  free(dec);
  return 0;
}

kr_video_dec *kr_avc_video_decoder_create() {
  int ret;
  kr_video_dec *dec;
  if (kr_lavc_global_init() < 0) return NULL;
  dec = kr_allocz(1, sizeof(*dec));
  dec->frm = av_frame_alloc();
  dec->ctx = avcodec_alloc_context3(h264);
  ret = avcodec_open2(dec->ctx, h264, NULL);
  if (ret < 0) return NULL;
  return dec;
}

int kr_avc_video_decoder_destroy(kr_video_dec *dec) {
  if (!dec) return -1;
  avcodec_close(dec->ctx);
  avcodec_free_context(&dec->ctx);
  av_frame_free(&dec->frm);
  free(dec);
  return 0;
}

