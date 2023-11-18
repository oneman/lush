#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>
#include <krad/app/debug.h>
#include "hashids.h"

#define ALPHABET_LEN 48
#define SEPARATORS_LEN 14
#define SALT_LEN 4
#define HASH_MIN_LEN 2
#define GUARD_DIV 12
#define SEP_DIV 3.5f

static const char alphabet[] = "abdegjklmnopqrvwxyz"
  "ABDEGJKLMNOPQRVWXYZ1234567890";
static const char separators[] = "cfhistuCFHISTU";

struct hashids {
  char salt[32];
  uint8_t salt_len;
  uint64_t init;
  uint64_t count;
};

static void hids_shuffle(char *alphabet, size_t alen,
 char *salt, size_t slen) {
  char integer;
  int i;
  int j;
  int k;
  int l;
  char temp;
  for (i = alen - 1, k = 0, l = 0; i > 0; i--, k++) {
    k %= slen;
    integer = salt[k];
    l += integer;
    j = (integer + k + l) % i;
    temp = alphabet[j];
    alphabet[j] = alphabet[i];
    alphabet[i] = temp;
  }
}

static ssize_t hids_hash(char *hash, size_t hlen,
  uint64_t num, char *alphabet, size_t len) {
  ssize_t i;
  i = 0;
  do {
    if (i == hlen) return -1;
    memmove(hash + 1, hash, i++);
    hash[0] = alphabet[num % len];
    num = num / len;
  } while(num);
  return i;
}

static ssize_t hashids_encode(kr_hashids *hids, char *out,
 size_t len, uint64_t in) {
  char hash[ALPHABET_LEN];
  char alpha[ALPHABET_LEN];
  char seps[SEPARATORS_LEN];
  char buffer[ALPHABET_LEN * 2];
  char alpha_copy[ALPHABET_LEN];
  char guards[(ALPHABET_LEN / GUARD_DIV) + 1];
  ssize_t ret;
  uint64_t nhash;
  uint64_t guard_index;
  size_t gc;
  size_t excess;
  size_t out_len;
  size_t alpha_len;
  size_t half_len_c;
  size_t half_len_f;
  char guard;
  char lottery;
  memcpy(alpha, alphabet, ALPHABET_LEN);
  memcpy(seps, separators, SEPARATORS_LEN);
  hids_shuffle(alpha, ALPHABET_LEN, hids->salt, SALT_LEN);
  hids_shuffle(seps, SEPARATORS_LEN, hids->salt, SALT_LEN);
  gc = ceil((float)ALPHABET_LEN / GUARD_DIV);
  memcpy(guards, alpha, gc);
  memmove(alpha, alpha + gc, ALPHABET_LEN - gc);
  alpha_len = ALPHABET_LEN - gc;
  if (!out || !len) return -1;
  nhash = in % 100;
  lottery = alpha[nhash % alpha_len];
  out[0] = lottery;
  out_len = 1;
  buffer[0] = lottery;
  memcpy(buffer + 1, hids->salt, SALT_LEN);
  memcpy(buffer + 1 + SALT_LEN, alpha, alpha_len);
  hids_shuffle(alpha, alpha_len, buffer,alpha_len);
  memcpy(alpha_copy, alpha, alpha_len);
  ret = hids_hash(hash, 64, in, alpha, alpha_len);
  if (ret < 0) return -1;
  memcpy(out + out_len, hash, ret);
  out_len += ret;
  if (out_len < HASH_MIN_LEN) {
    guard_index = (nhash + out[0]) % gc;
    guard =guards[guard_index];
    memmove(out + 1, out, out_len);
    out[0] = guard;
    out_len++;
    if (out_len < HASH_MIN_LEN) {
      guard_index = (nhash + out[2]) % gc;
      guard = guards[guard_index];
      out[out_len++] = guard;
    }
  }
  half_len_c = ceil((float)alpha_len / 2);
  half_len_f = floor((float)alpha_len / 2);
  while (out_len < HASH_MIN_LEN) {
    hids_shuffle(alpha, alpha_len,
     alpha_copy, alpha_len);
    memcpy(out + out_len, alpha, half_len_f);
    out_len += half_len_f;
    memmove(out + half_len_c, out, out_len);
    memcpy(out, alpha + half_len_f, half_len_c);
    out_len += half_len_c;
    excess = out_len - HASH_MIN_LEN;
    if (excess > 0) {
      memmove(out, out + (excess / 2), HASH_MIN_LEN);
      out_len = HASH_MIN_LEN;
    }
  }
  return out_len;
}

ssize_t kr_hashids_filename(char *name, size_t sz, size_t len,
 kr_hash_id *hid) {
  char *ext;
  char buffer[32];
  size_t ext_len;
  if (!name || !hid) return -1;
  if (!sz || !len || !hid->len) return -1;
  if (sz < (hid->len + len + 1)) return -1;
  ext_len = 0;
  ext = strrchr(name, '.');
  if (ext) {
    ext_len = len - (ext - name);
    if (ext_len > sizeof(buffer)) return -1;
    memcpy(buffer, ext, ext_len);
  }
  name[len - ext_len] = '-';
  memcpy(name + len - ext_len + 1, hid->uid, hid->len);
  memcpy(name + len - ext_len + 1 + hid->len, buffer, ext_len);
  len += hid->len + 1;
  return len;
}

ssize_t kr_hashids_filepath(char *name, size_t sz, size_t len,
 kr_hash_id *hid) {
  char buffer[512];
  char *slash;
  size_t offset;
  if (!name || !hid) return -1;
  if (!sz || !len || !hid->len) return -1;
  if (sz < (hid->len + len + 1)) return -1;
  if (len > sizeof(buffer)) return -1;
  offset = 0;
  memcpy(buffer, name, len);
  if ( (slash = strrchr(buffer, '/')) ) {
    offset = slash - buffer;
    offset++;
  }
  memcpy(name + offset, hid->uid, hid->len);
  name[offset + hid->len] = '/';
  memcpy(name + offset + hid->len + 1, buffer + offset, len - offset);
  len += hid->len + 1;
  return len;
}

size_t kr_hashids_sizeof() {
  return sizeof(struct hashids);
}

kr_hashids *kr_hashids_init(kr_hashids_setup *setup) {
  kr_hashids *hids;
  struct timespec ts;
  if (!setup || !setup->mem || !setup->len) return NULL;
  if (setup->len >= sizeof(setup->salt)) return NULL;
  hids = (kr_hashids *)setup->mem;
  memset(hids, 0, sizeof(*hids));
  clock_gettime(CLOCK_REALTIME, &ts);
  hids->init = ts.tv_nsec;
  memcpy(hids->salt, setup->salt, setup->len);
  return hids;
}

int kr_hashids_gen(kr_hashids *hids, kr_hash_id *hid) {
  ssize_t ret;
  hid->id = hids->init + __atomic_fetch_add(&hids->count, 1, __ATOMIC_SEQ_CST);
  ret = hashids_encode(hids, hid->uid, sizeof(hid->uid), hid->id);
  if (ret <= 0) return -1;
  hid->len = ret;
  return 0;
}
