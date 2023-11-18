#if !defined(_app_interval_H)
# define _app_interval_H (1)

typedef struct kr_interval kr_interval;

typedef struct {
  int num;
  int den;
  uint64_t ms;
  uint8_t *mem;
} kr_interval_setup;

int kr_interval_update(kr_interval *t);
int kr_interval_restart(kr_interval *t);
int kr_interval_get_fd(kr_interval *t);
int kr_interval_destroy(kr_interval *t);
kr_interval *kr_interval_create(kr_interval_setup *setup);
int kr_interval_set(kr_interval *t, uint64_t ms);
size_t kr_interval_sizeof();

#endif
