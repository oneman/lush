#include "types_from_json.h"

#include <krad/util/util.h>
int kr_eq_band_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_EQ_BAND_INFO_META_INFO);
  return res;
}

int kr_eq_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_EQ_INFO_META_INFO);
  return res;
}

int kr_lowpass_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_LOWPASS_INFO_META_INFO);
  return res;
}

int kr_highpass_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_HIGHPASS_INFO_META_INFO);
  return res;
}

int kr_pass_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_PASS_INFO_META_INFO);
  return res;
}

int kr_volume_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_VOLUME_INFO_META_INFO);
  return res;
}

int kr_analog_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_ANALOG_INFO_META_INFO);
  return res;
}

int kr_meter_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_METER_INFO_META_INFO);
  return res;
}

int kr_afx_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_AFX_INFO_META_INFO);
  return res;
}

