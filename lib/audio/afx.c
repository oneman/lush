#include "afx.h"

struct kr_afx {
  kr_volume_info *volume_info;
  kr_volume *volume;
  kr_analog_info *analog_info;
  kr_analog *analog;
  kr_pass_info *pass_info;
  kr_pass *pass;
  kr_eq_info *eq_info;
  kr_eq *eq;
  kr_meter_info *meter_info;
  kr_meter *meter;
};

static void *afxgather(void *dst, kr_afx_info *info, off_t os, size_t sz, int n) {
  int i;
  uint8_t *src;
  for (i = 0; i < n; i++) {
    src = (uint8_t *)&info[i] + os;
    memcpy((uint8_t *)dst + (sz * i), src, sz);
  }
  return dst;
}

size_t kr_afx_sizeof(int nc) {
  size_t sz;
  sz = 0;
  sz += sizeof(kr_afx);
  sz += nc * sizeof(kr_volume_info);
  sz += kr_volume_sizeof(nc);
  sz += nc * sizeof(kr_analog_info);
  sz += kr_analog_sizeof(nc);
  sz += nc * sizeof(kr_pass_info);
  sz += kr_pass_sizeof(nc);
  sz += nc * sizeof(kr_eq_info);
  sz += kr_eq_sizeof(nc);
  sz += nc * sizeof(kr_meter_info);
  sz += kr_meter_sizeof(nc);
  return sz;
}

kr_afx *kr_afx_init(void *memi, int nc, int sample_rate,
 kr_afx_info *info) {
  size_t os;
  kr_afx *a;
  uint8_t *mem;
  a = NULL;
  os = 0;
  mem = (uint8_t *)memi;
  a = (kr_afx *)mem;
  memset(a, 0, sizeof(*a));
  os += sizeof(*a);
  a->volume_info = afxgather(mem + os, info, offsetof(kr_afx_info, volume), sizeof(info->volume), nc);
  os += nc * sizeof(kr_volume_info);
  a->volume = kr_volume_init(mem + os, nc, sample_rate, a->volume_info);
  os += kr_volume_sizeof(nc);
  a->analog_info = afxgather(mem + os, info, offsetof(kr_afx_info, analog), sizeof(info->analog), nc);
  os += nc * sizeof(kr_analog_info);
  a->analog = kr_analog_init(mem + os, nc, sample_rate, a->analog_info);
  os += kr_analog_sizeof(nc);
  a->pass_info = afxgather(mem + os, info, offsetof(kr_afx_info, pass), sizeof(info->pass), nc);
  os += nc * sizeof(kr_pass_info);
  a->pass = kr_pass_init(mem + os, nc, sample_rate, a->pass_info);
  os += kr_pass_sizeof(nc);
  a->eq_info = afxgather(mem + os, info, offsetof(kr_afx_info, eq), sizeof(info->eq), nc);
  os += nc * sizeof(kr_eq_info);
  a->eq = kr_eq_init(mem + os, nc, sample_rate, a->eq_info);
  os += kr_eq_sizeof(nc);
  a->meter_info = afxgather(mem + os, info, offsetof(kr_afx_info, meter), sizeof(info->meter), nc);
  os += nc * sizeof(kr_meter_info);
  a->meter = kr_meter_init(mem + os, nc);
  os += kr_meter_sizeof(nc);
  return a;
}

int kr_afx_process(kr_afx *afx, kr_audio *out, kr_audio *in,
 kr_afx_info *info) {
  int i;
  int nc;
  nc = in->channels;
  afxgather(afx->volume_info, info, offsetof(kr_afx_info, volume), sizeof(info->volume), nc);
  kr_volume_process(afx->volume, out, in, afx->volume_info);
  afxgather(afx->analog_info, info, offsetof(kr_afx_info, analog), sizeof(info->analog), nc);
  kr_analog_process(afx->analog, out, out, afx->analog_info);
  afxgather(afx->pass_info, info, offsetof(kr_afx_info, pass), sizeof(info->pass), nc);
  kr_pass_process(afx->pass, out, out, afx->pass_info);
  afxgather(afx->eq_info, info, offsetof(kr_afx_info, eq), sizeof(info->eq), nc);
  kr_eq_process(afx->eq, out, out, afx->eq_info);
  kr_meter_process(afx->meter, out, afx->meter_info);
  for (i = 0; i < nc; i++) {
    info[i].meter = afx->meter_info[i];
  }
  return nc;
}
