#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <krad/app/debug.h>
#include <bitstream/mpeg/mpga.h>
#include "parse.h"

#define MPGA_SYNC_LENGTH 2

static const int16_t mpga_bit_rates[3][3][16] = {
  {
    {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, -1},
    {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, -1},
    {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, -1}
  },
  {
    {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, -1},
    {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1},
    {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1}
  },
  {
    {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, -1},
    {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1},
    {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1}
  }
};

static const int32_t mpga_sample_rates[3][4] = {
  {44100, 48000, 32000, -1},
  {22050, 24000, 16000, -1},
  {11025, 12000, 8000, -1},
};

static uint8_t mpga_get_protection(uint8_t *buf) {
  return !(buf[1] & 0x01);
}

static uint8_t mpga_get_version(uint8_t *buf) {
  uint8_t val;
  val = (buf[1] >> 3) & 0x03;
  if (val == 0) return 3;
  if (val == 2) return 2;
  if (val == 3) return 1;
  return 0;
}

static int mpga_check_sync(uint8_t *buf) {
  if (buf[0] == 0xFF && (buf[1] >> 5) == 0x07) return 1;
  return 0;
}

ssize_t kr_mpga_scan(uint8_t *buf, size_t sz, size_t *read) {
  int i;
  if (!read) return -1;
  *read = 0;
  if (!buf) return -2;
  if (sz < MPGA_SYNC_LENGTH) return -3;
  for (i = 0; i <= (sz - MPGA_SYNC_LENGTH); i++) {
    if (mpga_check_sync(buf + i)) {
      *read = i + MPGA_SYNC_LENGTH;
      return i;
    }
  }
  *read = i;
  return -4;
}

int kr_mpga_parse(kr_mpga_info *info, uint8_t *buf, size_t sz) {
  size_t frame_sz;
  uint8_t val;
  uint8_t padding;
  int ret;
  if (!info || !buf) return -1;
  if (sz < MPGA_HEADER_SIZE) return -2;
  memset(info, 0, sizeof(kr_mpga_info));
  for (;;) {
    ret = mpga_check_sync(buf);
    if (!ret) return -3;
    info->version = mpga_get_version(buf);
    if (!info->version) return -4;
    val = mpga_get_layer(buf);
    if (val == 1) info->layer = 3;
    else if (val == 2) info->layer = 2;
    else if (val == 3) info->layer = 1;
    else return -5;
    info->protected = mpga_get_protection(buf);
    ret = mpga_get_bitrate_index(buf);
    ret = mpga_bit_rates[info->version - 1][info->layer - 1][ret];
    if (ret < 0) return -6;
    info->bit_rate = ret * 1000;
    ret = mpga_get_sampling_freq(buf);
    ret = mpga_sample_rates[info->version - 1][ret];
    if (ret < 0) return -7;
    info->sample_rate = ret;
    padding = mpga_get_padding(buf);
    val = mpga_get_mode(buf);
    if (val == 3) info->channels = 1;
    else info->channels = 2;
    if (info->layer == 1) {
      frame_sz = (((12 * info->bit_rate) / info->sample_rate) + padding) * 4;
    } else {
      frame_sz = ((144 * info->bit_rate) / info->sample_rate) + padding;
    }
    if (sz < frame_sz || !frame_sz) break;
    sz -= frame_sz;
    buf += frame_sz;
    info->frames_sz += frame_sz;
    info->frames++;
    if (sz < 4) break;
  }
  return 0;
}
