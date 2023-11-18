#include <math.h>
#include <vorbis/vorbisenc.h>
#include <krad/mem/mem.h>
#include <krad/radio/version.h>
#include "encoder.h"

#define KR_VORBIS_HEADER_SZ_MAX 4096 * 4

#define KR_VORBIS_NHEADERS 3
#define KR_VORBIS_NCHANNELS 2

struct kr_vorbis_encoder {
  vorbis_dsp_state vdsp;
  vorbis_block vblock;
  vorbis_info vi;
  vorbis_comment vc;
  kr_vorbis_info info;
  int bo_ret;
  uint8_t header[KR_VORBIS_HEADER_SZ_MAX];
  kr_codec_header headers;
};

size_t kr_vorbis_encoder_sizeof() {
  printk("Vorbis Encoder Version: %s", vorbis_version_string());
  return sizeof(kr_vorbis_encoder);
}

kr_vorbis_encoder *kr_vorbis_encoder_init(kr_vorbis_encoder_setup *setup) {
  int ret;
  int i;
  size_t pos;
  kr_vorbis_encoder *ve;
  kr_vorbis_info *info;
  ogg_packet hdr[KR_VORBIS_NHEADERS];
  if (!setup) return NULL;
  ve = (kr_vorbis_encoder *)setup->mem;
  memset(ve, 0, sizeof(*ve));
  ve->info = setup->info;
  info = &ve->info;
  ve->headers.len = 0;
  vorbis_info_init(&ve->vi);
  ret = vorbis_encode_init_vbr(&ve->vi, info->channels, info->sample_rate,
   info->quality);
  if (ret < 0) {
    if (ret == OV_EIMPL) printke("Vorbis Encoder: mode not supported");
    if (ret == OV_EINVAL) printke("Vorbis Encoder: invalid mode");
    return NULL;
  }
  printk("Vorbis Encoder Setup: Channels %d Sample Rate: %d Quality: %f",
   info->channels, info->sample_rate, info->quality);
  ve->info.small_block_sz = vorbis_info_blocksize(&ve->vi, 0);
  ve->info.large_block_sz = vorbis_info_blocksize(&ve->vi, 1);
  vorbis_analysis_init(&ve->vdsp, &ve->vi);
  ve->bo_ret = 0;
  vorbis_block_init(&ve->vdsp, &ve->vblock);
  vorbis_comment_init(&ve->vc);
  vorbis_comment_add_tag(&ve->vc, "ENCODER", KR_VERSION_STR_FULL);
  vorbis_analysis_headerout(&ve->vdsp, &ve->vc, &hdr[0], &hdr[1], &hdr[2]);
  ve->headers.len = KR_VORBIS_NHEADERS;
  pos = 0;
  for (i = 0; i < ve->headers.len; i++) {
    ve->headers.data[i].sz = hdr[i].bytes;
    memcpy(ve->header + pos, hdr[i].packet, ve->headers.data[i].sz);
    ve->headers.data[i].buf = ve->header + pos;
    pos += ve->headers.data[i].sz;
  }
  return ve;
}

int kr_vorbis_encoder_get_headers(kr_vorbis_encoder *ve, kr_codec_header *h) {
  if (!ve || !h) return -1;
  *h = ve->headers;
  return 0;
}

int kr_vorbis_encoder_get_info(kr_vorbis_encoder *ve, kr_vorbis_info *info) {
  if (!ve || !info) return -1;
  *info = ve->info;
  return 0;
}

int kr_vorbis_encode(kr_vorbis_encoder *ve, kr_coded_audio *out, kr_audio *in) {
  int32_t ret;
  int c;
  float **pcm;
  ogg_packet op;
  if (!ve->bo_ret) {
    if (in && in->count > 0) {
      pcm = vorbis_analysis_buffer(&ve->vdsp, in->count);
      for (c = 0; c < ve->info.channels; c++) {
        memcpy(pcm[c], in->samples[c], in->count * sizeof(float));
      }
      ret = vorbis_analysis_wrote(&ve->vdsp, in->count);
      if (ret < 0) {
        printke("Vorbis Encoder: vorbis_analysis_wrote");
        return -1;
      }
    } else {
      printk ("KR Vorbis Encoder: Got finish notice");
      ret = vorbis_analysis_wrote(&ve->vdsp, 0);
    }
  }
  ve->bo_ret = vorbis_analysis_blockout(&ve->vdsp, &ve->vblock);
  if (ve->bo_ret < 0) {
    printke("Vorbis Encoder: vorbis_analysis_blockout");
    return -1;
  }
  ret = vorbis_analysis(&ve->vblock, &op);
  if (ret < 0) {
    printke("Vorbis Encoder: vorbis_analysis");
    return -1;
  }
  if (op.bytes > out->sz) {
    printke("Vorbis Encoder: not enough space in out buffer!");
    return -1;
  }
  out->frames = op.granulepos - ve->info.total_frames;
  out->len = op.bytes;
  out->tc = op.granulepos;
  ve->info.total_frames = op.granulepos;
  ve->info.total_bytes += out->len;
  memcpy(out->data, op.packet, out->len);
  return ve->bo_ret;
}

int kr_vorbis_encoder_ctl(kr_vorbis_encoder *ve, kr_patchset *patchset) {
  if (!ve || !patchset) return -1;
  return 0;
}

int kr_vorbis_encoder_destroy(kr_vorbis_encoder *ve) {
  if (!ve) return -1;
  vorbis_block_clear(&ve->vblock);
  vorbis_comment_clear(&ve->vc);
  vorbis_dsp_clear(&ve->vdsp);
  vorbis_info_clear(&ve->vi);
  return 0;
}
