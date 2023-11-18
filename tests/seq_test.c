#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <krad/mem/mem.h>
#include <krad/app/debug.h>
#include <krad/app/seq.h>

static int seq_test() {
  int ret;
  uint64_t len;
  uint64_t pos;
  uint64_t apos; 
  uint64_t horizon;
  kr_seq *seq;
  ret = 0;
  pos = 0;
  horizon = -1;
/*size_t kr_seq_sizeof();
int kr_seq_init(kr_seq *seq, uint64_t len);
int kr_seq_reset(kr_seq *seq);
uint64_t kr_seq_advance(kr_seq *seq);
uint64_t kr_seq_seek(kr_seq *seq, uint64_t n);
uint64_t kr_seq_pos(kr_seq *seq);
int kr_seq_horizon(kr_seq *seq, uint64_t *horizon);
uint64_t kr_seq_len(kr_seq *seq);
uint64_t kr_seq_total_pos(kr_seq *seq);*/
  seq = kr_allocz(1, kr_seq_sizeof());
  if (!seq) return -1;
  len = 5;
  printk("Testing seq len %d", len);
  kr_seq_init(seq, len);
  for (;;) {
    pos = kr_seq_pos(seq);
    apos = kr_seq_total_pos(seq);
    kr_seq_horizon(seq, &horizon);
    printk("seq position %zu %zu horizon %zu", apos, pos, horizon);
    kr_seq_advance(seq);
    if (apos > 20 + len) break;
  }
  return ret;
}

int main(int argc, char const *argv[]) {
  int ret;
  ret = seq_test();
  if (ret) fprintf(stderr, "Failure.\n");
  return ret;
}
