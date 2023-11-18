#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <krad/mem/pool.h>
#include <krad/app/broadcast.h>

static int bc_emitter(void *user, void *sub_user) {
  printf("  %s got the broadcast from %s!\n",
   (char *)sub_user, (char *)user);
  return 0;
};

static int bc_test() {
  int res;
  char *user;
  char *streamer45;
  kr_broadcast *bc;
  kr_broadcast_setup setup;
  memset(&setup, 0, sizeof(setup));
  user = "coconut_island";
  streamer45 = "streamer45";
  setup.nsubs = 4;
  setup.user = user;
  bc = kr_broadcast_create(&setup);
  if (bc == NULL) return -1;
  printf("Broadcast created.\n");

  res = kr_broadcast_subscribe(bc, "oneman");
  if (res != 0) return -1;
  printf("  added subscriber oneman.\n");
  res = kr_broadcast_subscribe(bc, "six");
  if (res != 0) return -1;
  printf("  added subscriber six.\n");
  res = kr_broadcast_subscribe(bc, streamer45);
  if (res != 0) return -1;
  printf("  added subscriber %s.\n", streamer45);
  res = kr_broadcast_subscribe(bc, "juca");
  if (res != 0) return -1;
  printf("  added subscriber juca.\n");

  printf("Ready to emit broadcast.\n");
  res = kr_broadcast_emit(bc, bc_emitter);
  if (res != 0) return -1;
  printf("Broadcast was emitted succesfully.\n");

  printf("Unsubscribing %s.\n", streamer45);
  res = kr_broadcast_unsubscribe(bc, streamer45);
  if (res != 0) return -1;
  printf("%s unsubscribed.\n", streamer45);

  printf("Ready to emit a new broadcast.\n");
  res = kr_broadcast_emit(bc, bc_emitter);
  if (res != 0) return -1;
  printf("Broadcast was emitted succesfully.\n");

  res = kr_broadcast_destroy(bc);
  if (res != 0) return -1;
  printf("Broadcast destroyed.\n");
  return 0;
}

int main(int argc, char const *argv[]) {
  int res;
  res = bc_test();
  if (res == 0)
    printf("Success!\n");
  else
    fprintf(stderr, "Failure!\n");
  return res;
}