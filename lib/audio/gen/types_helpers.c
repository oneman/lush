#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_afx_effect_type(char *str) {
  return str_to_enum(KR_KR_AFX_EFFECT_TYPE_META_INFO, str);
}

char *kr_afx_effect_type_to_str(int val) {
  return enum_to_str(KR_KR_AFX_EFFECT_TYPE_META_INFO, val);
}

int str_to_kr_afx_effect_control(char *str) {
  return str_to_enum(KR_KR_AFX_EFFECT_CONTROL_META_INFO, str);
}

char *kr_afx_effect_control_to_str(int val) {
  return enum_to_str(KR_KR_AFX_EFFECT_CONTROL_META_INFO, val);
}

int kr_eq_band_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_EQ_BAND_INFO_META_INFO, 1);
  return res;
}

int kr_eq_band_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_EQ_BAND_INFO_META_INFO, 2);
  return res;
}

int kr_eq_band_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_EQ_BAND_INFO_META_INFO, 3);
  return res;
}

int kr_eq_band_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_EQ_BAND_INFO_META_INFO);
  return res;
}

int kr_eq_band_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_EQ_BAND_INFO_META_INFO);
  return res;
}

int kr_eq_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_EQ_INFO_META_INFO, 1);
  return res;
}

int kr_eq_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_EQ_INFO_META_INFO, 2);
  return res;
}

int kr_eq_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_EQ_INFO_META_INFO, 3);
  return res;
}

int kr_eq_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_EQ_INFO_META_INFO);
  return res;
}

int kr_eq_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_EQ_INFO_META_INFO);
  return res;
}

int kr_lowpass_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_LOWPASS_INFO_META_INFO, 1);
  return res;
}

int kr_lowpass_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_LOWPASS_INFO_META_INFO, 2);
  return res;
}

int kr_lowpass_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_LOWPASS_INFO_META_INFO, 3);
  return res;
}

int kr_lowpass_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_LOWPASS_INFO_META_INFO);
  return res;
}

int kr_lowpass_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_LOWPASS_INFO_META_INFO);
  return res;
}

int kr_highpass_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HIGHPASS_INFO_META_INFO, 1);
  return res;
}

int kr_highpass_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HIGHPASS_INFO_META_INFO, 2);
  return res;
}

int kr_highpass_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HIGHPASS_INFO_META_INFO, 3);
  return res;
}

int kr_highpass_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_HIGHPASS_INFO_META_INFO);
  return res;
}

int kr_highpass_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_HIGHPASS_INFO_META_INFO);
  return res;
}

int kr_pass_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PASS_INFO_META_INFO, 1);
  return res;
}

int kr_pass_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PASS_INFO_META_INFO, 2);
  return res;
}

int kr_pass_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PASS_INFO_META_INFO, 3);
  return res;
}

int kr_pass_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_PASS_INFO_META_INFO);
  return res;
}

int kr_pass_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_PASS_INFO_META_INFO);
  return res;
}

int kr_volume_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VOLUME_INFO_META_INFO, 1);
  return res;
}

int kr_volume_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VOLUME_INFO_META_INFO, 2);
  return res;
}

int kr_volume_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VOLUME_INFO_META_INFO, 3);
  return res;
}

int kr_volume_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_VOLUME_INFO_META_INFO);
  return res;
}

int kr_volume_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_VOLUME_INFO_META_INFO);
  return res;
}

int kr_analog_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_ANALOG_INFO_META_INFO, 1);
  return res;
}

int kr_analog_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_ANALOG_INFO_META_INFO, 2);
  return res;
}

int kr_analog_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_ANALOG_INFO_META_INFO, 3);
  return res;
}

int kr_analog_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_ANALOG_INFO_META_INFO);
  return res;
}

int kr_analog_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_ANALOG_INFO_META_INFO);
  return res;
}

int kr_meter_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_METER_INFO_META_INFO, 1);
  return res;
}

int kr_meter_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_METER_INFO_META_INFO, 2);
  return res;
}

int kr_meter_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_METER_INFO_META_INFO, 3);
  return res;
}

int kr_meter_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_METER_INFO_META_INFO);
  return res;
}

int kr_meter_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_METER_INFO_META_INFO);
  return res;
}

int kr_afx_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AFX_INFO_META_INFO, 1);
  return res;
}

int kr_afx_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AFX_INFO_META_INFO, 2);
  return res;
}

int kr_afx_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AFX_INFO_META_INFO, 3);
  return res;
}

int kr_afx_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_AFX_INFO_META_INFO);
  return res;
}

int kr_afx_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_AFX_INFO_META_INFO);
  return res;
}

