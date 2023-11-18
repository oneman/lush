#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include <krad/app/debug.h>
#include "pack.h"

static void write_le32(uint8_t *buf, int i32) {
  buf[0]=(unsigned  char)(i32&0xFF);
  buf[1]=(unsigned  char)(i32>>8&0xFF);
  buf[2]=(unsigned  char)(i32>>16&0xFF);
  buf[3]=(unsigned  char)(i32>>24&0xFF);
}

static void write_le16(uint8_t *buf, int i16) {
  buf[0]=(unsigned  char)(i16&0xFF);
  buf[1]=(unsigned  char)(i16>>8&0xFF);
}

ssize_t kr_wave_pack_header(uint8_t *hdr, size_t sz, kr_wave_info *info) {
  uint8_t *start;
  size_t chunk_sz;
  if (!hdr || !sz || !info) return -1;
  if (sz < WAVE_HEADER_MIN_SIZE) return -2;
  memset(hdr, 0, sz);
  start = hdr;
  memcpy(hdr, "RIFF", 4);
  hdr += 4;
  write_le32(hdr, -1);
  hdr += 4;
  memcpy(hdr, "WAVE", 4);
  hdr += 4;
  memcpy(hdr, "fmt ", 4);
  hdr += 4;
  if (info->extensible) chunk_sz = 40;
  else chunk_sz = 16;
  write_le32(hdr, chunk_sz);
  hdr += 4;
  write_le16(hdr, info->fmt);
  write_le16(hdr + 2, info->audio.channels);
  write_le32(hdr + 4, info->audio.sample_rate);
  write_le32(hdr + 12, info->audio.bit_depth * info->audio.channels);
  write_le16(hdr + 14, info->audio.bit_depth);
  if (info->extensible) write_le16(hdr + 24, info->fmt);
  hdr += chunk_sz;
  if (info->fmt != KR_WAVE_PCM) {
    memcpy(hdr, "fact", 4);
    write_le32(hdr + 4, 4);
    hdr += 12;
  }
  memcpy(hdr, "data", 4);
  hdr += 4;
  write_le32(hdr, -1);
  hdr += 4;
  return hdr - start;
}
