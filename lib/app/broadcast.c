#include <inttypes.h>
#include <string.h>
#include <stddef.h>
#include <krad/app/debug.h>
#include <krad/mem/pool.h>
#include "broadcast.h"

struct kr_broadcast {
  kr_pool *subs;
  void *user;
};

int kr_broadcast_emit(kr_broadcast *broadcast,
 kr_broadcast_emitter *emitter) {
  if (broadcast == NULL) return -1;
  if (emitter == NULL) return -1;
  void *slice;
  void *sub_user;
  int i;
  int res;
  i = 0;
  while ((slice = kr_pool_iterate_active(broadcast->subs, &i))) {
    memcpy(&sub_user, slice, sizeof(void *));
    res = emitter(broadcast->user, sub_user);
    if (res != 0) return -1;
  }
  return 0;
}

int kr_broadcast_subscribe(kr_broadcast *broadcast, void *user) {
  void *slice;
  if (broadcast == NULL) return -1;
  if (user == NULL) return -1;
  slice = kr_pool_slice(broadcast->subs);
  if (slice == NULL) return -1;
  memcpy(slice, &user, sizeof(void *));
  return 0;
}

int kr_broadcast_unsubscribe(kr_broadcast *broadcast, void *user) {
  void *slice;
  void *sub_user;
  int i;
  if (broadcast == NULL) return -1;
  if (user == NULL) return -1;
  i = 0;
  while ((slice = kr_pool_iterate_active(broadcast->subs, &i))) {
    memcpy(&sub_user, slice, sizeof(void *));
    if (sub_user == user)
      return kr_pool_release(broadcast->subs, slice);
  }
  return -1;
}

int kr_broadcast_destroy(kr_broadcast *broadcast) {
  if (broadcast == NULL) return -1;
  return kr_pool_destroy(broadcast->subs);
}

kr_broadcast *kr_broadcast_create(kr_broadcast_setup *setup) {
  int res;
  if (setup == NULL) return NULL;
  kr_pool_setup psetup;
  kr_broadcast bc;
  kr_broadcast *broadcast;
  memset(&psetup, 0, sizeof(psetup));
  memset(&bc, 0, sizeof(bc));
  psetup.slices = setup->nsubs;
  psetup.size = sizeof(void *);
  psetup.overlay_sz = sizeof(bc);
  bc.user = setup->user;
  bc.subs = kr_pool_create(&psetup);
  if (bc.subs == NULL) return NULL;
  res = kr_pool_overlay_set(bc.subs, &bc);
  if (res != 0) return NULL;
  broadcast = kr_pool_overlay_get(bc.subs);
  return broadcast;
}
