#if !defined(_TRACK_GEN_TYPES_TO_JSON_H)
#define _TRACK_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_track_name_pack_json(char *json, void *info, int max);
int kr_mux_name_pack_json(char *json, void *info, int max);
int kr_demux_info_pack_json(char *json, void *info, int max);
int kr_mux_info_pack_json(char *json, void *info, int max);
int kr_video_encode_info_pack_json(char *json, void *info, int max);
int kr_audio_encode_info_pack_json(char *json, void *info, int max);
int kr_encode_info_pack_json(char *json, void *info, int max);
int kr_record_info_pack_json(char *json, void *info, int max);
int kr_transmission_info_pack_json(char *json, void *info, int max);
int kr_output_info_pack_json(char *json, void *info, int max);
int kr_capture_audio_info_pack_json(char *json, void *info, int max);
int kr_capture_video_info_pack_json(char *json, void *info, int max);
int kr_capture_image_info_pack_json(char *json, void *info, int max);
int kr_capture_info_pack_json(char *json, void *info, int max);
int kr_source_file_info_pack_json(char *json, void *info, int max);
int kr_source_remote_file_info_pack_json(char *json, void *info, int max);
int kr_source_stream_info_pack_json(char *json, void *info, int max);
int kr_studio_source_info_pack_json(char *json, void *info, int max);
int kr_decode_info_pack_json(char *json, void *info, int max);
int kr_play_info_pack_json(char *json, void *info, int max);
int kr_render_image_info_pack_json(char *json, void *info, int max);
int kr_render_video_info_pack_json(char *json, void *info, int max);
int kr_render_audio_info_pack_json(char *json, void *info, int max);
int kr_render_info_pack_json(char *json, void *info, int max);
int kr_generate_audio_info_pack_json(char *json, void *info, int max);
int kr_generate_color_info_pack_json(char *json, void *info, int max);
int kr_generate_video_info_pack_json(char *json, void *info, int max);
int kr_generate_info_pack_json(char *json, void *info, int max);
int kr_studio_path_info_pack_json(char *json, void *info, int max);
int kr_track_info_pack_json(char *json, void *info, int max);
int kr_track_block_info_pack_json(char *json, void *info, int max);
#endif

