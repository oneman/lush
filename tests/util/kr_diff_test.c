#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <krad/app/diff.h>
#include <krad/gen/metainfo.h>
#include "../tools/json/pretty.h"

static void print_diffs(kr_diffset *diffset, void *info1, void *info2) {
  int i;
  int32_t val1;
  int32_t val2;
  kr_diff *diff;
  printf("Found %zu diffs!:\n", diffset->len);
  for (i = 0; i < diffset->len; i++) {
    diff = &diffset->diffs[i];
    printf("  %s\n", diff->path);
    if (diff->type == DIFF_STRING) {
      printf("    info1: %s info2: %s\n", (char *)(info1 + diff->off), (char *)(info2+ diff->off));
    }
    if (diff->type == DIFF_REAL) {
      printf("    info1: %0.2f info2: %0.2f\n", *(float *)(info1 + diff->off), *(float *)(info2 + diff->off));
    }
    if (diff->type == DIFF_INTEGER) {
      printf("    info1: %d info2: %d\n", *(int32_t *)(info1 + diff->off), *(int32_t *)(info2 + diff->off));
    }
    if (diff->type == DIFF_ENUM) {
      val1 = *(int32_t *)(info1 + diff->off);
      val2 = *(int32_t *)(info2 + diff->off);
      printf("    info1: %s info2: %s\n", enum_to_str(diff->enum_idx, val1),  enum_to_str(diff->enum_idx, val2));
    }
  }
}

int diff_test() {
  int i;
  int res;
  char json[4096];
  kr_diffset diffset;
  kr_mixer_path_info info1;
  kr_mixer_path_info info2;
  memset(&diffset, 0, sizeof(diffset));
  memset(&info1, 0, sizeof(info1));
  memset(&info2, 0, sizeof(info2));

  info1.type = 1;
  info2.type = 1;
  info1.channels = 2;
  info2.channels = 4;

  for (i = 0; i < info1.channels; i++) {
    kr_analog_info_random(&info1.afx[i].analog);
  }

  for (i = 0; i < info2.channels; i++) {
    kr_lowpass_info_random(&info2.afx[i].pass.low);
    kr_highpass_info_random(&info2.afx[i].pass.high);
  }

  res = kr_mixer_path_info_diff(&diffset, &info1, &info2);
  if (res != 0) return -1;

  res = kr_mixer_path_info_pack_json(json, &info1, sizeof(json));
  if (res <= 0) return -1;
  printf("info1 (JSON): \n");
  pretty_json(json);
  printf("\n\n");

  res = kr_mixer_path_info_pack_json(json, &info2, sizeof(json));
  if (res <= 0) return -1;
  printf("info2 (JSON): \n");
  pretty_json(json);
  printf("\n\n");
  print_diffs(&diffset, &info1, &info2);
  printf("\nJSON patches:\n\n");
  res = kr_diffset_to_json_patches(json, sizeof(json), &diffset, &info2);
  if (res != 0) return -1;
  printf("%s\n", json);
  kr_diffset_apply(&diffset, &info1, &info2);
  res = kr_mixer_path_info_pack_json(json, &info1, sizeof(json));
  if (res <= 0) return -1;
  printf("\nResult (JSON): \n\n");
  pretty_json(json);
  printf("\n");
  if (memcmp(&info1, &info2, sizeof(info1))) return -1;
  return 0;
}

int main(int argc, char const *argv[]) {
  int res;
  res = diff_test();
  if (res == 0) {
    printf("Success.\n");
  } else {
    printf("Failure.\n");
  }
  return res;
}
