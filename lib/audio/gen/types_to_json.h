#if !defined(_AUDIO_GEN_TYPES_TO_JSON_H)
#define _AUDIO_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_eq_band_info_pack_json(char *json, void *info, int max);
int kr_eq_info_pack_json(char *json, void *info, int max);
int kr_lowpass_info_pack_json(char *json, void *info, int max);
int kr_highpass_info_pack_json(char *json, void *info, int max);
int kr_pass_info_pack_json(char *json, void *info, int max);
int kr_volume_info_pack_json(char *json, void *info, int max);
int kr_analog_info_pack_json(char *json, void *info, int max);
int kr_meter_info_pack_json(char *json, void *info, int max);
int kr_afx_info_pack_json(char *json, void *info, int max);
#endif

