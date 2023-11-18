#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/hash/hashids.h>

int hids_test(int64_t n) {
  int i;
  int ret;
  kr_hashids *hids;
  kr_hashids_setup setup;
  kr_hash_id hid;
  if (n <= 0) return -1;
  setup.mem = kr_alloc(kr_hashids_sizeof());
  memcpy(setup.salt, "test", 4);
  setup.len = 4;
  hids = kr_hashids_init(&setup);
  if (!hids) return -2;
  for (i = 0; i < n; i++) {
    ret = kr_hashids_gen(hids, &hid);
    if (ret != 0) return -3;
    printk("%"PRIu64" uid: %.*s", hid.id, hid.len, hid.uid);
  }
  free(hids);
  return 0;
}

int main(int argc, char const *argv[]) {
  int ret;
  if (argc != 2) {
    fprintf(stderr, "Give me a natural number...\n");
    return -1;
  }
  ret = hids_test(atoi(argv[1]));
  if (ret) fprintf(stderr, "Failure.\n");
  return ret;
}
