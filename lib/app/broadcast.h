#if !defined(_app_broadcast_H)
# define _app_broadcast_H (1)

#include <inttypes.h>
#include <stddef.h>

typedef struct kr_broadcast kr_broadcast;
typedef int (kr_broadcast_emitter)(void *, void *);

typedef struct {
  int nsubs;
  void *user;
} kr_broadcast_setup;

int kr_broadcast_destroy(kr_broadcast *broadcast);
kr_broadcast *kr_broadcast_create(kr_broadcast_setup *setup);
int kr_broadcast_subscribe(kr_broadcast *broadcast, void *user);
int kr_broadcast_unsubscribe(kr_broadcast *broadcast, void *user);
int kr_broadcast_emit(kr_broadcast *broadcast, kr_broadcast_emitter *emitter);

#endif
