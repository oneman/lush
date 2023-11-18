#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <krad/app/debug.h>
#include <krad/mem/pool.h>

typedef enum {
	XMMSV_TYPE_NONE,
	XMMSV_TYPE_ERROR,
	XMMSV_TYPE_INT64,
	XMMSV_TYPE_STRING,
	XMMSV_TYPE_COLL,
	XMMSV_TYPE_BIN,
	XMMSV_TYPE_LIST,
	XMMSV_TYPE_DICT,
	XMMSV_TYPE_BITBUFFER,
	XMMSV_TYPE_FLOAT,
	XMMSV_TYPE_END
} xmmsv_type_t;

struct xmmsv_St {
	union {
		char *error;
		int64_t int64;
		float flt32;
		char *string;
		/*xmmsv_coll_internal_t *coll;
		xmmsv_list_internal_t *list;
		xmmsv_dict_internal_t *dict;*/

		struct {
			unsigned char *data;
			uint32_t len;
		} bin;

		struct {
			bool ro;
			unsigned char *buf;
			int alloclen; /* in bits */
			int len; /* in bits */
			int pos; /* in bits */
		} bit;
	} value;
	xmmsv_type_t type;

	int ref;  /* refcounting */
};

#define XMMSV_TYPE_INT32 XMMSV_TYPE_INT64

typedef struct xmmsv_St xmmsv_t;

int bitbuffer_put_bits(xmmsv_t *v, int bits, int64_t d) {
	unsigned char t;
	int pos;
	int i;
  if (bits < 1) {
    printke("zero bits fail!");
    exit(1); 
  }
	if (bits == 1) {
		pos = v->value.bit.pos;

		if (pos >= v->value.bit.alloclen) {
			int ol, nl;
			nl = v->value.bit.alloclen * 2;
			ol = v->value.bit.alloclen;
			nl = nl < 128 ? 128 : nl;
			nl = (nl + 7) & ~7;
			v->value.bit.buf = realloc (v->value.bit.buf, nl / 8);
			memset (v->value.bit.buf + ol / 8, 0, (nl - ol) / 8);
			v->value.bit.alloclen = nl;
		}
		t = v->value.bit.buf[pos / 8];

		t = (t & (~(1<<(7-(pos % 8))))) | (d << (7-(pos % 8)));

		v->value.bit.buf[pos / 8] = t;

		v->value.bit.pos += 1;
		if (v->value.bit.pos > v->value.bit.len)
			v->value.bit.len = v->value.bit.pos;
		return 1;
	}

	for (i = 0; i < bits; i++) {
		if (!bitbuffer_put_bits (v, 1, !!(d & (1LL << (bits-i-1)))))
			return 0;
	}

	return 1;
}

static ssize_t bitstr(char *str, uint8_t byte, size_t max) {
  int i;
  uint8_t mask;
  if (max < 8) return -1;
  mask = 0;
  mask = 1;
  //printk("byte: 0x%02X :: %d", byte, byte);
  for (i = 7; i > -1; i--) {
    //printk("Mask: %02X", mask);
    //printk("i  is %d", i);
    str[i] = '0';
    if (mask & byte) str[i] = '1';
    mask = mask << 1;
  }
  if (max > 8) str[8] = '\0';
  return 8;
}

static void bitstr_test() {
  uint8_t byte;
  char str[128];
  byte = 0;
  for (;;) {
    memset(str, 0, sizeof(str));
    bitstr(str, byte, sizeof(str));
    printk("bitstr %3d: %s", byte, str);
    if (byte == 255) break;
    byte++;
  }
}

static void bitbuffer_debug(xmmsv_t *v) {
  int i;
  int len;
  uint8_t byte;
  char str[128];
  printk("bb# pos: %d len: %d alloclen: %d", v->value.bit.pos,
   v->value.bit.len, v->value.bit.alloclen);
  len = v->value.bit.len;
  for (i = 0; i < len/8; i++) {
    byte = v->value.bit.buf[i];
    memset(str, 0, sizeof(str));
    bitstr(str, byte, sizeof(str));
    printk("bitstr %3d: %s", byte, str);
  }
}

static int bb_test() {
  int ret;
  int number;
  xmmsv_t xv;
  bitstr_test();
  memset(&xv, 0, sizeof(xv));
  xv.type = XMMSV_TYPE_BITBUFFER;
  xv.value.bit.alloclen = 4096;
  xv.value.bit.buf = kr_allocz(1, xv.value.bit.alloclen/8);
  /* setup done */
  /*number = 120;
  ret = bitbuffer_put_bits(&xv, 32, number);
  if (ret != 1) return -1;
  bitbuffer_debug(&xv);*/
  number = 666;
  ret = bitbuffer_put_bits(&xv, 32, number);
  if (ret != 1) return -1;
  bitbuffer_debug(&xv);
  return 0;
}

int main(int argc, char const *argv[]) {
  int ret;
  ret = bb_test();
  if (ret == 0) {
    printk("bb Success!\n");
    exit(0);
  }
  fprintf(stderr, "bb Fail!\n");
  return 1;
}
