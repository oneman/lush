#include "seq.h"

#define MM __ATOMIC_SEQ_CST

struct kr_seq {
  uint64_t pos;
  uint64_t len;
};

size_t kr_seq_sizeof() {
  return sizeof(kr_seq);
}

int kr_seq_init(kr_seq *seq, uint64_t len) {
  uint64_t pos;
  if (!seq || !len) return -1;
  pos = 0;
  __atomic_store(&seq->pos, &pos, MM);
  seq->len = len;
  return 0;
}

int kr_seq_reset(kr_seq *seq) {
  if (!seq) return -1;
  return kr_seq_init(seq, seq->len);
}

uint64_t kr_seq_advance(kr_seq *seq) {
  if (!seq) return 0;
  uint64_t pos;
  pos = __atomic_add_fetch(&seq->pos, 1, MM);
  return pos;
}

uint64_t kr_seq_next(kr_seq *seq) {
  uint64_t pos;
  if (!seq) return 0;
  __atomic_load(&seq->pos, &pos, MM);
  return (pos + 1) % seq->len;
}

uint64_t kr_seq_prev(kr_seq *seq) {
  uint64_t pos;
  if (!seq) return 0;
  __atomic_load(&seq->pos, &pos, MM);
  if (pos == 0) return 0;
  return (pos - 1) % seq->len;
}

uint64_t kr_seq_seek(kr_seq *seq, uint64_t n) {
  if (!seq) return 0;
  uint64_t pos;
  pos = __atomic_add_fetch(&seq->pos, n, MM);
  return pos;
}

uint64_t kr_seq_pos(kr_seq *seq) {
  uint64_t pos;
  if (!seq) return 0;
  __atomic_load(&seq->pos, &pos, MM);
  return pos % seq->len;
}

int kr_seq_horizon(kr_seq *seq, uint64_t *horizon) {
  uint64_t pos;
  uint64_t len;
  if (!seq) return 0;
  __atomic_load(&seq->pos, &pos, MM);
  __atomic_load(&seq->len, &len, MM);
  if (pos < (len - 1) ) {
    return 0;
  }
  *horizon = ((pos + 1) - len) % seq->len;
  return 1;
}

int kr_seq_sync_check(kr_seq *seq, uint64_t pos) {
  uint64_t tpos;
  uint64_t len;
  __atomic_load(&seq->pos, &tpos, MM);
  __atomic_load(&seq->len, &len, MM);
  if (tpos < (len - 1)) return 0;
  if (pos < (tpos + 1 - len)) return ((tpos + 1 - len) - pos);
  return 0;
}

uint64_t kr_seq_len(kr_seq *seq) {
  uint64_t len;
  if (!seq) return 0;
  __atomic_load(&seq->len, &len, MM);
  return len;
}

uint64_t kr_seq_total_pos(kr_seq *seq) {
  uint64_t pos;
  if (!seq) return 0;
  __atomic_load(&seq->pos, &pos, MM);
  return pos;
}
