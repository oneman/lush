#if !defined(_app_spinlock_H)
# define _app_spinlock_H (1)

typedef int kr_spinlock;

void kr_spin_lock(kr_spinlock *lock);
void kr_spin_unlock(kr_spinlock *lock);
#define kr_spin_init kr_spin_unlock

#endif
