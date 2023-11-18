#include <stdlib.h>
#include <string.h>
#include <krad/app/debug.h>
#include "occ.h"

int kr_occ_mux_ok(kr_codec *codecs, int ntracks) {
  if (!codecs) return 0;
  if (ntracks != 1) return 0;
  if (codecs[0].type != KR_CODEC_AUDIO) return 0;
  if (codecs[0].audio != KR_AUDIO_CODEC_OPUS) return 0;
  return 1;
}

int kr_occ_sync_check(uint8_t *buf, size_t len) {
  if (!buf) return -1;
  if (len < KR_OCC_HEADER_LEN) return -2;
  if (memcmp(buf, "OCC", 3)) return -3;
  return 0;
}

int kr_occ_pack(uint8_t *buf, size_t len, kr_occ_data *data) {
  if (!buf || !len || !data) return -1;
  data->pkts[0].hdr[0] = 'O';
  data->pkts[0].hdr[1] = 'C';
  data->pkts[0].hdr[2] = 'C';
  data->pkts[0].hdr[3] = (len >> 8) & 0xFF;
  data->pkts[0].hdr[4] = len & 0xFF;
  data->pkts[0].buf = buf;
  data->pkts[0].len = len;
  data->npkts = 1;
  return 0;
}

int kr_occ_unpack(uint8_t *buf, size_t len, kr_occ_data *data) {
  size_t pkt_len;
  if (!buf || !len || !data) return -1;
  data->npkts = 0;
  for (;;) {
    if (kr_occ_sync_check(buf, len) != 0) {
      printke("OCC: sync check failed at pkt %d!", data->npkts);
      return -2;
    }
    if (data->npkts == KR_OCC_MAX_PKTS) {
      printke("OCC: too many packets!");
      return -3;
    }
    memcpy(data->pkts[data->npkts].hdr, buf, KR_OCC_HEADER_LEN);
    data->pkts[data->npkts].buf = buf + KR_OCC_HEADER_LEN;
    pkt_len = (buf[3] << 8) + buf[4];
    data->pkts[data->npkts].len = pkt_len;
    data->npkts++;
    buf += (KR_OCC_HEADER_LEN + pkt_len);
    if (len < (pkt_len + KR_OCC_HEADER_LEN) ) {
      printke("OCC: wrong len!");
      return -4;
    }
    len -= (pkt_len + KR_OCC_HEADER_LEN);
    if (len == 0) break;
  }
  return 0;
}
