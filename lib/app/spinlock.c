#include "spinlock.h"

#define MM __ATOMIC_SEQ_CST

void kr_spin_lock(kr_spinlock *lock) {
  int exp;
  for (;;) {
    exp = 0;
    if (__atomic_compare_exchange_n(lock, &exp, 1, 0, MM, MM)) break;
  }
}

void kr_spin_unlock(kr_spinlock *lock) {
  __atomic_store_n(lock, 0, MM);
}
