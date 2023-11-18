
#define MM __ATOMIC_SEQ_CST

typedef struct {
  void *rd;
  void *wr;
  void *cur; 
  uint64_t rv;
  uint64_t wv;
} kr_xnfo;

size_t kr_xnfo_sizeof() {
  return sizeof(kr_xinfo);
}

int kr_xnfo_init(kr_xnfo *x, void *p1, void *p2, void *p3) {
  if (!x || !p1 || !p2 || !p3) return -1;
  x->rd = p1;
  x->wr = p2;
  x->cur = p3;
  x->rv = 0;
  x->wv = 0;
  return 0;
}

void *kr_xnfo_push(kr_xnfo *x) {
  x->wr = __atomic_exchange_n(x->rd, x->wr, MM);
  return x->wr;
}

void *kr_xnfo_pull(kr_xnfo *x) {
  x->cur = __atomic_exchange_n(x->rd, x->cur, MM);
  return x->cur;
} 
