#include "stash_types_helpers.h"

#include <krad/util/util.h>
int kr_image_stash_item_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_IMAGE_STASH_ITEM_INFO_META_INFO, 1);
  return res;
}

int kr_image_stash_item_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_IMAGE_STASH_ITEM_INFO_META_INFO, 2);
  return res;
}

int kr_image_stash_item_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_IMAGE_STASH_ITEM_INFO_META_INFO, 3);
  return res;
}

int kr_image_stash_item_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_IMAGE_STASH_ITEM_INFO_META_INFO);
  return res;
}

int kr_image_stash_item_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_IMAGE_STASH_ITEM_INFO_META_INFO);
  return res;
}

