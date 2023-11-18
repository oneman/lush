#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <krad/app/debug.h>
#include "parse.h"

#define ADTS_SYNC_LENGTH 2
#define ADTS_HEADER_SIZE 7

static const int32_t adts_rates[16] = {
  96000,
  88200,
  64000,
  48000,
  44100,
  32000,
  24000,
  22050,
  16000,
  12000,
  11025,
  8000,
  7350,
  -1,
  -1,
  -1
};

static const int32_t adts_channels[16] = {0, 1, 2, 3, 4, 5, 6, 8, -1,
 -1, -1, -1, -1, -1, -1, -1};

static int adts_check_sync(uint8_t *buf) {
  if (buf[0] == 0xFF && (buf[1] >> 4) == 0x0F) return 1;
  return 0;
}

static uint8_t adts_get_version(uint8_t *buf) {
  uint8_t val;
  val = ((buf[1] >> 3) & 0x01);
  if (val == 0) return 4;
  else return 2;
}

static uint8_t adts_get_layer(uint8_t *buf) {
  return ((buf[1] >> 1) & 0x03);
}

static uint8_t adts_get_protection(uint8_t *buf) {
  return !((buf[1]) & 0x01);
}

static uint8_t adts_get_profile(uint8_t *buf) {
  return ((buf[2] >> 6) & 0x03) + 1;
}

static int adts_get_sample_rate(uint8_t *buf) {
  uint8_t idx;
  idx = (buf[2] >> 2) & 0x0F;
  return adts_rates[idx];
}

static int adts_get_channels(uint8_t *buf) {
  uint8_t idx;
  idx = (buf[2] >> 7) & 0x07;
  return adts_channels[idx];
}

static int adts_get_frame_length(uint8_t *buf) {
  return ((buf[3] & 0x03) << 11) + (buf[4] << 3) + (buf[5] >> 5);
}

static int adts_get_frames(uint8_t *buf) {
  return (buf[6] & 0x03) + 1;
}

ssize_t kr_adts_scan(uint8_t *buf, size_t sz, size_t *read) {
  int i;
  if (!read) return -1;
  *read = 0;
  if (!buf) return -2;
  if (sz < ADTS_SYNC_LENGTH) return -3;
  for (i = 0; i <= (sz - ADTS_SYNC_LENGTH); i++) {
    if (adts_check_sync(buf + i)) {
      *read = i + ADTS_SYNC_LENGTH;
      return i;
    }
  }
  *read = i;
  return -4;
}

int kr_adts_parse(kr_adts_info *info, uint8_t *buf, size_t sz) {
  int ret;
  size_t frames;
  size_t frame_sz;
  if (!info || !buf) return -1;
  if (sz < ADTS_HEADER_SIZE) return -2;
  memset(info, 0, sizeof(kr_adts_info));
  frames = 0;
  frame_sz = 0;
  for (;;) {
    if (!adts_check_sync(buf)) return -3;
    info->version = adts_get_version(buf);
    if (adts_get_layer(buf)) return -4;
    info->protected = adts_get_protection(buf);
    info->profile = adts_get_profile(buf);
    ret = adts_get_sample_rate(buf);
    if (ret < 0) return -5;
    info->sample_rate = ret;
    ret = adts_get_channels(buf);
    if (ret < 0) return -6;
    info->channels = ret;
    frames = adts_get_frames(buf);
    frame_sz = adts_get_frame_length(buf);
    if (sz < frame_sz || !frame_sz) break;
    sz -= frame_sz;
    buf += frame_sz;
    info->frames_sz += frame_sz;
    info->frames += frames;
    if (sz < ADTS_HEADER_SIZE) break;
  }
  return 0;
}
