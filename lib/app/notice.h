#if !defined(_app_notice_H)
# define _app_notice_H (1)

#include <stdint.h>

uint64_t kr_take_notice(int fd);
int kr_give_notice(int fd);
int kr_notifier_destroy(int fd);
int kr_notifier_create();

#endif
