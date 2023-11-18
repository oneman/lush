#include <krad/web/web.h>

#if !defined(_web_protocol_osc_H)

#define KR_OSC_BUNDLE_NELEMS 8
#define KR_OSC_PACKET_NBUNDLES 8
#define KR_OSC_MSG_NVALS 16
#define KR_OSC_ADDRESS_MAX 128

typedef enum {
  KR_OSC_UNKNOWN,
  KR_OSC_FLOAT,
  KR_OSC_INT
} kr_osc_val_type;

typedef enum {
  KR_OSC_MSG = 1,
  KR_OSC_BUNDLE,
  KR_OSC_BUNDLES
} kr_osc_content_type;

typedef union {
  float real;
  int integer;
  char str[4];
} kr_osc_val;

typedef struct {
  char address[KR_OSC_ADDRESS_MAX];
  size_t address_len;
  size_t len;
  kr_osc_val_type type[KR_OSC_MSG_NVALS];
  kr_osc_val val[KR_OSC_MSG_NVALS];
} kr_osc_msg;

typedef struct {
  kr_osc_msg msg[KR_OSC_BUNDLE_NELEMS];
  size_t len;
} kr_osc_bundle;

typedef struct {
  kr_osc_content_type type;
  int nbundles;
  union {
    kr_osc_bundle bundle;
    kr_osc_bundle bundles[KR_OSC_PACKET_NBUNDLES];
    kr_osc_msg msg;
  };
} kr_osc_packet;

/*
 * this all doesn't actually work yet
 * plan: parse buffer into packet info structure
 *       that includes content type, bundle count
 *
 * add functions to get bundles/messages/values
 */

int kr_osc_parse(kr_osc_packet *packet, uint8_t *buf, size_t size);

#endif
