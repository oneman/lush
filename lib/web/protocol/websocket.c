#include <krad/app/debug.h>
#include "websocket.h"

#define KR_WS_MASK_BIT 0x80
#define KR_WS_FIN_BIT 0x80
#define KR_WS_CONT_OPCODE 0x00
#define KR_WS_TEXT_OPCODE 0x01
#define KR_WS_BIN_OPCODE 0x02
#define KR_WS_CLOSE_OPCODE 0x08
#define KR_WS_PING_OPCODE 0x09
#define KR_WS_PONG_OPCODE 0x0a

int kr_ws_unpack_header(kr_ws_frame *frm, uint8_t *buf, size_t sz) {
  if (!frm) return -1;
  if (!buf) return -1;
  uint8_t *bytes;
  uint16_t payload_len_16;
  uint64_t payload_len_64;
  uint8_t b;
  if (sz < 2) {
    return -1;
  }
  payload_len_16 = 0;
  payload_len_64 = 0;
  b = buf[0];
  if (b & KR_WS_FIN_BIT) {
    b ^= KR_WS_FIN_BIT;
    frm->fin = 1;
  } else {
    frm->fin = 0;
  }
  for (;;) {
    if (b == KR_WS_TEXT_OPCODE) {
      frm->opcode = KR_WS_TEXT;
      frm->type = KR_WS_NONCONTROL;
      break;
    }
    if (b == KR_WS_PONG_OPCODE) {
      frm->opcode = KR_WS_PONG;
      frm->type = KR_WS_CONTROL;
      break;
    }
    if (b == KR_WS_PING_OPCODE) {
      frm->opcode = KR_WS_PING;
      frm->type = KR_WS_CONTROL;
      break;
    }
    if (b == KR_WS_CLOSE_OPCODE) {
      frm->opcode = KR_WS_CLOSE;
      frm->type = KR_WS_CONTROL;
      break;
    }
    if (b == KR_WS_BIN_OPCODE) {
      frm->opcode = KR_WS_BIN;
      frm->type = KR_WS_NONCONTROL;
      break;
    }
    if (b == KR_WS_CONT_OPCODE) {
      /*frm->opcode = KR_WS_CONT;*/
      frm->type = KR_WS_CONTINUATION;
      break;
    }
    printke("Websocket: header parsing messup");
    return -1;
  }
  frm->pos = 0;
  b = buf[1];
  if (b & KR_WS_MASK_BIT) {
    b ^= KR_WS_MASK_BIT;
    frm->masking = KR_WS_MASKED;
  } else {
    frm->masking = KR_WS_UNMASKED;
  }
  if (b < 126) {
    frm->len = b;
    if (frm->masking == KR_WS_UNMASKED) return 2;
    if (sz < 6) return -1;
    frm->mask[0] = buf[2];
    frm->mask[1] = buf[3];
    frm->mask[2] = buf[4];
    frm->mask[3] = buf[5];
    return 6;
  }
  if (b == 126) {
    if (sz < 4) return -1;
    bytes = (uint8_t *)&payload_len_16;
    bytes[1] = buf[2];
    bytes[0] = buf[3];
    if (frm->masking == KR_WS_UNMASKED) return 4;
    if (sz < 8) return -1;
    frm->mask[0] = buf[4];
    frm->mask[1] = buf[5];
    frm->mask[2] = buf[6];
    frm->mask[3] = buf[7];
    frm->len = payload_len_16;
    return 8;
  }
  if (sz < 10) return -1;
  bytes = (uint8_t *)&payload_len_64;
  bytes[7] = buf[2];
  bytes[6] = buf[3];
  bytes[5] = buf[4];
  bytes[4] = buf[5];
  bytes[3] = buf[6];
  bytes[2] = buf[7];
  bytes[1] = buf[8];
  bytes[0] = buf[9];
  if (frm->masking == KR_WS_UNMASKED) return 10;
  if (sz < 14) return -1;
  frm->mask[0] = buf[10];
  frm->mask[1] = buf[11];
  frm->mask[2] = buf[12];
  frm->mask[3] = buf[13];
  frm->len = payload_len_64;
  return 14;
}

int kr_ws_unmask(kr_ws_frame *frm, uint8_t *buf, size_t sz) {
  int i;
  size_t len;
  if (!frm) return -1;
  if (!buf) return -1;
  if (sz == 0) return -1;
  len = frm->len - frm->pos;
  if (sz < len) len = sz;
  for (i = 0; i < len; i++) {
    buf[i] = buf[i] ^ frm->mask[frm->pos % 4];
    frm->pos++;
  }
  return len;
}

int kr_websocket_pack_ctl_header(uint8_t *buf, kr_ws_opcode opcode, size_t max,
 uint32_t size) {
  uint16_t size_16;
  uint64_t size_64;
  uint8_t *bytes;
  uint8_t mask_bit;
  if (opcode == KR_WS_BIN) {
    buf[0] = KR_WS_FIN_BIT | KR_WS_BIN_OPCODE;
  }
  if (opcode == KR_WS_TEXT) {
    buf[0] = KR_WS_FIN_BIT | KR_WS_TEXT_OPCODE;
  }
  if (opcode == KR_WS_PING) {
    buf[0] = KR_WS_FIN_BIT | KR_WS_PING_OPCODE;
  }
  if (opcode == KR_WS_PONG) {
    buf[0] = KR_WS_FIN_BIT | KR_WS_PONG_OPCODE;
  }
  if (opcode == KR_WS_CLOSE) {
    buf[0] = KR_WS_FIN_BIT | KR_WS_CLOSE_OPCODE;
  }
  if (max < 2) return -1;
  if (1) {
    mask_bit = 0;
  } else {
    mask_bit = KR_WS_MASK_BIT;
  }
  if (size < 126) {
    buf[1] = mask_bit | size;
    return 2;
  } else {
    if (max < 4) return -1;
    if (size < 65536) {
      buf[1] = mask_bit | 126;
      size_16 = size;
      bytes = (uint8_t *)&size_16;
      buf[2] = bytes[1];
      buf[3] = bytes[0];
      return 4;
    } else {
      if (max < 10) return -1;
      buf[1] = 127;
      size_64 = size;
      bytes = (uint8_t *)&size_64;
      buf[2] = bytes[7];
      buf[3] = bytes[6];
      buf[4] = bytes[5];
      buf[5] = bytes[4];
      buf[6] = bytes[3];
      buf[7] = bytes[2];
      buf[8] = bytes[1];
      buf[9] = bytes[0];
      return 10;
    }
  }
}

int kr_websocket_pack_header(uint8_t *buf, size_t max, uint32_t size) {
  return kr_websocket_pack_ctl_header(buf, KR_WS_TEXT, max, size);
}

int kr_websocket_pack_binary_header(uint8_t *buf, size_t max, uint32_t size) {
  return kr_websocket_pack_ctl_header(buf, KR_WS_BIN, max, size);
}

int kr_websocket_accept_key(char *akey, size_t max, char *ckey, size_t len) {
  static char *ws_guid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  int ret;
  char string[128];
  uint8_t hash[20];
  if ((akey == NULL) || (ckey == NULL)) {
    return -1;
  }
  snprintf(string, sizeof(string), "%.*s%s", (int)len, ckey, ws_guid);
  string[127] = '\0';
  kr_sha1((uint8_t *)string, strlen(string), hash);
  ret = kr_base64((uint8_t *)akey, hash, 20, 64);
  return ret;
}
