#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <krad/app/debug.h>
#include <krad/util/util.h>
#include <krad/util/strton.h>

#define JSON_SZ 1024*1024

void ps_diff(kr_patchset *a, kr_patchset *b) {
  int i;
  if (a->len != b->len) {
    printk("len diffs %zu %zu", a->len, b->len);
  }
  if (a->type != b->type) {
    printk("type diffs %d %d", a->type, b->type);
  }
  for (i = 0; i < KR_NPATCHES; i++) {
    if (a->patch[i].sz != b->patch[i].sz) {
      printk("patch[%d].sz diffs %zu %zu",
       i, a->patch[i].sz, b->patch[i].sz);
    }
    if (a->patch[i].offset != b->patch[i].offset) {
      printk("patch[%d].offset diffs %zu %zu",
       i, a->patch[i].offset, b->patch[i].offset);
    }
    if (memcmp(a->patch[i].value, b->patch[i].value,
     sizeof(a->patch[i].value))) {
      printk("patch[%d].value diffs", i);
    }
  }
}

void cosistencty_test(uint64_t *count, uint64_t *fails) {
  int ret;
  kr_info in;
  kr_info out;
  kr_patchset ps;
  kr_patchset ps2;
  kr_diffset ds;
  struct timeval tv;
  const char *name;
  char *json;
  int i;
  json = malloc(JSON_SZ);
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);
  for (i = 0; i < KR_META_INFO_LAST; i++) {
    if (meta_info[i].type != ST_STRUCT) continue;
    (*count)++;
    name = meta_info[i].name;
    //printf("  Testing: %s\n", name);
    memset(json, 0, JSON_SZ);
    memset(&in, 0, sizeof(in));
    memset(&out, 0, sizeof(out));
    ret = info_helpers(&in, i, 3);
    if (ret != 0) {
      printk("  Fail on random %s", name);
      (*fails)++;
      continue;
    }
    ret = info_pack_json(json, &in, JSON_SZ, i);
    if (ret >= JSON_SZ) {
      printk("  Fail on pack json %s", name);
      (*fails)++;
      printk("%s", json);
      continue;
    }
    ret = info_unpack_json(json, &out, i);
    if (ret != 0) {
      printk("  Fail on unpack json %s", name);
      printk("\n%s", json);
      (*fails)++;
      continue;
    }
    ret = memcmp(&in, &out, sizeof(in));
    if (ret != 0) {
      printk("  Fail on memcmp %s", name);
      (*fails)++;
      continue;
    }
    memset(&ps, 0, sizeof(ps));
    ret = kr_patchset_gen(&ps, &in, kr_metainfo_type_to_info_type(i));
    if (ret != 0) {
      printk("  Fail on patchset gen %s %d", name, ret);
      (*fails)++;
      continue;
    }
    memset(&out, 0, sizeof(out));
    ret = kr_patchset_apply(&out, &ps);
    if (ret != 0) {
      printk("  Fail on patchset apply %s %d", name, ret);
      (*fails)++;
      continue;
    }
    memset(&ds, 0, sizeof(ds));
    ret = info_diff(&ds, &in, &out, i);
    if (ret != 0 || ds.len) {
      printk("  Fail on diff %s", name);
      (*fails)++;
      continue;
    }
    ret = kr_patchset_to_json(json, JSON_SZ, &ps);
    if (ret <= 0) {
      printk("  Fail on patchset json %s %d", name, ret);
      (*fails)++;
      continue;
    }
    memset(&ps2, 0, sizeof(ps2));
    ps2.type = ps.type;
    ret = kr_json_to_patchset(&ps2, json, strlen(json));
    if (ret < 0) {
      printk("  Fail on json patchset %s %d", name, ret);
      (*fails)++;
      continue;
    }
    if (memcmp(&ps, &ps2, offsetof(kr_patchset, on))) {
      printk("  Fail on patchset memcmp %s", name);
      ps_diff(&ps, &ps2);
      (*fails)++;
      break;
      //continue;
    }
  }
  free(json);
  printf("  %zu info structures tested, %zu failed\r", *count, *fails);
  fflush(stdout);
}

int main(int argc, char *argv[]) {
  int i;
  int res;
  uint64_t iter;
  uint64_t count;
  uint64_t failures;
  iter = 1;
  count = 0;
  failures = 0;
  if (argc == 2) {
    res = kr_strtou64(&iter, argv[1], strlen(argv[1]));
    if (res != 0) iter = 1;
  }
  for (i = 0; i < iter; i++) {
    cosistencty_test(&count, &failures);
  }
  printf("\n  %"PRIu64"/%"PRIu64" failures\n",
   failures, count);
  return 0;
}
