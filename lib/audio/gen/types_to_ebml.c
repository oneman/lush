#include "types_to_ebml.h"

#include <krad/util/util.h>
int kr_eq_band_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_EQ_BAND_INFO_META_INFO, 0);
  return res;
}

int kr_eq_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_EQ_INFO_META_INFO, 0);
  return res;
}

int kr_lowpass_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_LOWPASS_INFO_META_INFO, 0);
  return res;
}

int kr_highpass_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_HIGHPASS_INFO_META_INFO, 0);
  return res;
}

int kr_pass_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_PASS_INFO_META_INFO, 0);
  return res;
}

int kr_volume_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_VOLUME_INFO_META_INFO, 0);
  return res;
}

int kr_analog_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_ANALOG_INFO_META_INFO, 0);
  return res;
}

int kr_meter_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_METER_INFO_META_INFO, 0);
  return res;
}

int kr_afx_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_AFX_INFO_META_INFO, 0);
  return res;
}

