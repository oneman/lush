#if !defined(_AUDIO_GEN_TYPES_FROM_JSON_H)
#define _AUDIO_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_eq_band_info_unpack_json(char *json, void *info);
int kr_eq_info_unpack_json(char *json, void *info);
int kr_lowpass_info_unpack_json(char *json, void *info);
int kr_highpass_info_unpack_json(char *json, void *info);
int kr_pass_info_unpack_json(char *json, void *info);
int kr_volume_info_unpack_json(char *json, void *info);
int kr_analog_info_unpack_json(char *json, void *info);
int kr_meter_info_unpack_json(char *json, void *info);
int kr_afx_info_unpack_json(char *json, void *info);
#endif

