#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "diff.h"
#include <krad/gen/metainfo.h>

static int diff_to_json_patch(char *json, size_t max,
 kr_diff *diff, void *info) {
  int res;
  res = 0;
  if (diff->type == DIFF_STRING) {
    res += snprintf(json + res, max - res,
     "{\"op\":\"replace\",\"path\":\"%s\",\"value\":\"%s\"}",
      diff->path, (char *)(info + diff->off));
  }
  if (diff->type == DIFF_INTEGER) {
    res += snprintf(json + res, max - res,
     "{\"op\":\"replace\",\"path\":\"%s\",\"value\":%d}",
      diff->path, *(int32_t *)(info + diff->off));
  }
  if (diff->type == DIFF_REAL) {
    res += snprintf(json + res, max - res,
     "{\"op\":\"replace\",\"path\":\"%s\",\"value\":%0.2f}",
      diff->path, *(float *)(info + diff->off));
  }
  if (diff->type == DIFF_ENUM) {
    res += snprintf(json + res, max - res,
     "{\"op\":\"replace\",\"path\":\"%s\",\"value\":\"%s\"}", diff->path,
      enum_to_str(diff->enum_idx , *(int32_t *)(info + diff->off)));
  }
  return res;
}

static void diff_apply(kr_diff *diff, void *dst_info, void *src_info) {
  memcpy(dst_info + diff->off, src_info + diff->off, diff->sz);
}

int kr_diffset_to_patchset(kr_patchset *ps, kr_diffset *ds) {
  int i;
  int j;
  if (!ps || !ds) return -1;
  if (ds->len > KR_NPATCHES) return -2;
  if (!ds->on) return -3;
  ps->len = ds->len;
  ps->type = ds->type;
  ps->on = ds->on;
  for (i = j = 0; i < ds->len; i++) {
    if (ds->diffs[i].sz <= sizeof(ps->patch[j].value)) {
      ps->patch[j].sz = ds->diffs[i].sz;
      ps->patch[j].offset = ds->diffs[i].off;
      memcpy(ps->patch[j].value,
       ps->on + ps->patch[j].offset, ps->patch[j].sz);
      ++j;
    }
  }
  ps->len = j;
  return 0;
}

int kr_diffset_to_json_patches(char *json, size_t max,
 kr_diffset *diffset, void *info) {
  int i;
  int res;
  res = 0;
  res += snprintf(json + res, max - res, "[\n");
  if (res >= max) return -1;
  for (i = 0; i < diffset->len; i++) {
    res += diff_to_json_patch(json + res, max - res,
     &diffset->diffs[i], info);
    if (res >= max) return -1;
    if (i != (diffset->len - 1)) {
      res += snprintf(json + res, max - res, ",");
      if (res >= max) return -1;
    }
    res += snprintf(json + res, max - res, "\n");
    if (res >= max) return -1;
  }
  res += snprintf(json + res, max - res, "]\n");
  if (res >= max) return -1;
  return 0;
}

void kr_diffset_apply(kr_diffset *diffset, void *dst_info, void *src_info) {
  int i;
  for (i = 0; i < diffset->len; i++) {
    diff_apply(&diffset->diffs[i], dst_info, src_info);
  }
}
