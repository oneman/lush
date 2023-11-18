#if !defined(_AUDIO_GEN_TYPES_HELPERS_H)
#define _AUDIO_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_afx_effect_type(char *str);
char *kr_afx_effect_type_to_str(int val);
int str_to_kr_afx_effect_control(char *str);
char *kr_afx_effect_control_to_str(int val);
int kr_eq_band_info_init(void *info);
int kr_eq_band_info_valid(void *info);
int kr_eq_band_info_random(void *info);
int kr_eq_band_info_patch_path(kr_patch *patch, kr_path *path);
int kr_eq_band_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_eq_info_init(void *info);
int kr_eq_info_valid(void *info);
int kr_eq_info_random(void *info);
int kr_eq_info_patch_path(kr_patch *patch, kr_path *path);
int kr_eq_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_lowpass_info_init(void *info);
int kr_lowpass_info_valid(void *info);
int kr_lowpass_info_random(void *info);
int kr_lowpass_info_patch_path(kr_patch *patch, kr_path *path);
int kr_lowpass_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_highpass_info_init(void *info);
int kr_highpass_info_valid(void *info);
int kr_highpass_info_random(void *info);
int kr_highpass_info_patch_path(kr_patch *patch, kr_path *path);
int kr_highpass_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_pass_info_init(void *info);
int kr_pass_info_valid(void *info);
int kr_pass_info_random(void *info);
int kr_pass_info_patch_path(kr_patch *patch, kr_path *path);
int kr_pass_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_volume_info_init(void *info);
int kr_volume_info_valid(void *info);
int kr_volume_info_random(void *info);
int kr_volume_info_patch_path(kr_patch *patch, kr_path *path);
int kr_volume_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_analog_info_init(void *info);
int kr_analog_info_valid(void *info);
int kr_analog_info_random(void *info);
int kr_analog_info_patch_path(kr_patch *patch, kr_path *path);
int kr_analog_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_meter_info_init(void *info);
int kr_meter_info_valid(void *info);
int kr_meter_info_random(void *info);
int kr_meter_info_patch_path(kr_patch *patch, kr_path *path);
int kr_meter_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_afx_info_init(void *info);
int kr_afx_info_valid(void *info);
int kr_afx_info_random(void *info);
int kr_afx_info_patch_path(kr_patch *patch, kr_path *path);
int kr_afx_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

