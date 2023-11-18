#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sched.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <pthread.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>

#include "osc.h"

#define KR_OSC_DEBUG 1

static inline void rcopy4(void *dst, void *src);
static void msg_debug(kr_osc_msg *msg);
static void packet_debug(kr_osc_packet *packet);
static int decode_payload(kr_osc_msg *msg, uint8_t *buf, size_t len);
static int decode_typetags(kr_osc_msg *msg, uint8_t *buf, size_t len);
static int decode_msg(kr_osc_msg *msg, uint8_t *buf, size_t len);
static int decode_bundle(kr_osc_bundle *bundle, uint8_t *buf, size_t len);

static inline void rcopy4(void *dst, void *src) {
  uint8_t *a_dst;
  uint8_t *a_src;
  a_dst = dst;
  a_src = src;
  a_dst[0] = a_src[3];
  a_dst[1] = a_src[2];
  a_dst[2] = a_src[1];
  a_dst[3] = a_src[0];
}

static void msg_debug(kr_osc_msg *msg) {
  int i;
  printk("  Address: %s", msg->address);
  printk("  %zu values", msg->len);
  for (i = 0; i < msg->len; i++) {
    printk("  %d: <type> <val>", i);
  }
}

static void bundle_debug(kr_osc_bundle *bundle) {
  int i;
  printk("OSC Bundle: %zu Messages", bundle->len);
  for (i = 0; i < bundle->len; i++) {
    printk(" Message %d", i);
    msg_debug(&bundle->msg[i]);
  }
}

static void packet_debug(kr_osc_packet *packet) {
  if (packet->type == KR_OSC_MSG) {
    return msg_debug(&packet->msg);
  } else if (packet->type == KR_OSC_BUNDLE) {
    return bundle_debug(&packet->bundle);
  }
}

static int decode_payload(kr_osc_msg *msg, uint8_t *buf, size_t len) {
  int i;
  int pos;
  pos = 0;
  for (i = 0; i < msg->len; i++) {
    pos = len + i * 4;
    if (msg->type[i] == KR_OSC_UNKNOWN) return -1;
    if (msg->type[i] == KR_OSC_FLOAT) {
      rcopy4(&msg->val[i], buf + pos);
    }
    if (msg->type[i] == KR_OSC_INT) {
      memcpy(&msg->val[i], buf + pos, 4);
    }
  }
  return 0;
}

static int decode_typetags(kr_osc_msg *msg, uint8_t *buf, size_t len) {
  int i;
  char *typetag;
  int cnt;
  typetag = (char *)buf + len;
  printk("Krad OSC buf typetags: %s", typetag);
  len += 1;
  msg->len = 0;
  cnt = strlen(typetag);
  if (cnt == 0 || cnt > 4) {
    return -1;
  }
  for (i = 0; typetag[i] != '\0'; i++) {
    if (msg->len == KR_OSC_MSG_NVALS) return -1;
    if ((typetag[i] != 'f') && (typetag[i] != 'i')) {
      return -1;
    }
    msg->len++;
  }
  return 0;
}

static int decode_msg(kr_osc_msg *msg, uint8_t *buf, size_t len) {
  int ret;
  /* Get address */
  printk("OSC buf address pattern: %s\n", buf);
  len = strlen((char *)buf) + 1;
  if (len > KR_OSC_ADDRESS_MAX) {
    printke("OSC: address too long");
  }
  strncpy(msg->address, (char *)buf, sizeof(msg->address) - 1);
  /* Move to typetags */
  while ((len % 4) != 0) {
    len++;
  }
  len += 1;
  ret = decode_typetags(msg, buf + len, len);
  if (ret < 1) return -1;
  /* Move to payload */
  //len += pos;
  while ((len % 4) != 0) {
    len++;
  }
  ret = decode_payload(msg, buf + len, len);
  if (ret < 1) return -1;
  return 0;
}

static int decode_bundle(kr_osc_bundle *bundle, uint8_t *buf, size_t len) {
  int i;
  int ret;
  for (i = 0; i < bundle->len; i++) {
    ret = decode_msg(&bundle->msg[i], buf, len);
    if (ret) return -1;
  }
  return 0;
}

int kr_osc_parse(kr_osc_packet *packet, uint8_t *buf, size_t len) {
  int ret;
  if (!packet) return -1;
  if (!buf) return -1;
  if (len == 0) return -1;
  ret = -1;
  if ((len > 20) && (memcmp(buf, "#bundle", 8) == 0)) {
    packet->type = KR_OSC_BUNDLE;
    printke("OSC: packet is a %zu byte bundle, we don't handle :/", len);
    return -1;
  }
  packet->type = KR_OSC_MSG;
  if (packet->type == KR_OSC_MSG) {
    ret = decode_msg(&packet->msg, buf, len);
  } else if (packet->type == KR_OSC_BUNDLE) {
    ret = decode_bundle(&packet->bundle, buf, len);
  }
  if (KR_OSC_DEBUG) packet_debug(packet);
  return ret;
}
