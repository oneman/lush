#if !defined(_AUDIO_GEN_TYPES_TO_EBML_H)
#define _AUDIO_GEN_TYPES_TO_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/ebml/ebml.h>
int kr_eq_band_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_eq_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_lowpass_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_highpass_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_pass_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_volume_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_analog_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_meter_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_afx_info_pack_ebml(kr_ebml *ebml, void *info);
#endif

