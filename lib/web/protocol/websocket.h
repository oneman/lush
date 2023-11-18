#include <krad/web/web.h>

#if !defined(_web_protocol_websocket_H)
# define _web_protocol_websocket_H (1)

#define KR_WS_CONTROL_FRAME_MAX 125

typedef enum {
  KR_WS_NONCONTROL = 1,
  KR_WS_CONTROL,
  KR_WS_CONTINUATION
} kr_ws_opcode_type;

typedef enum {
  KR_WS_UNMASKED,
  KR_WS_MASKED
} kr_ws_masking;

typedef enum {
  KR_WS_CONT,
  KR_WS_TEXT,
  KR_WS_BIN,
  KR_WS_CLOSE,
  KR_WS_PING,
  KR_WS_PONG
} kr_ws_opcode;

typedef struct {
  uint8_t mask[4];
  size_t pos;
  size_t len;
  int fin;
  kr_ws_opcode opcode;
  kr_ws_opcode_type type;
  kr_ws_masking masking;
} kr_ws_frame;

int kr_ws_unpack_header(kr_ws_frame *frm, uint8_t *buf, size_t sz);
int kr_ws_unmask(kr_ws_frame *frm, uint8_t *buf, size_t sz);

/* needed server funcs..
TODO: int kr_ws_pack_pong(uint8_t *buf, size_t max);
TODO: int kr_ws_pack_close(uint8_t *buf, size_t max);
*/

/* future client funcs ..
TODO: int kr_ws_pack_ping(uint8_t *buf, size_t max);
TODO: int kr_ws_mask(kr_ws_frame *frm, uint8_t *buf, size_t max);
*/

int kr_websocket_pack_ctl_header(uint8_t *buf, kr_ws_opcode opcode, size_t max,
 uint32_t size);
int kr_websocket_pack_header(uint8_t *buf, size_t max, uint32_t size);
int kr_websocket_pack_binary_header(uint8_t *buf, size_t max, uint32_t size);
int kr_websocket_accept_key(char *akey, size_t max, char *ckey, size_t len);

#endif
