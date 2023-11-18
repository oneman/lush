#if !defined(_app_seq_H)
# define _app_seq_H (1)

#include <sys/types.h>
#include <inttypes.h>

typedef struct kr_seq kr_seq;

size_t kr_seq_sizeof();
int kr_seq_init(kr_seq *seq, uint64_t len);
int kr_seq_reset(kr_seq *seq);
uint64_t kr_seq_advance(kr_seq *seq);
uint64_t kr_seq_next(kr_seq *seq);
uint64_t kr_seq_prev(kr_seq *seq);
uint64_t kr_seq_seek(kr_seq *seq, uint64_t n);
uint64_t kr_seq_pos(kr_seq *seq);
int kr_seq_horizon(kr_seq *seq, uint64_t *horizon);
uint64_t kr_seq_len(kr_seq *seq);
uint64_t kr_seq_total_pos(kr_seq *seq);
int kr_seq_sync_check(kr_seq *seq, uint64_t pos);

#endif
