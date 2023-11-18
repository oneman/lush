#if !defined(_TRACK_GEN_TYPES_FROM_JSON_H)
#define _TRACK_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_track_name_unpack_json(char *json, void *info);
int kr_mux_name_unpack_json(char *json, void *info);
int kr_demux_info_unpack_json(char *json, void *info);
int kr_mux_info_unpack_json(char *json, void *info);
int kr_video_encode_info_unpack_json(char *json, void *info);
int kr_audio_encode_info_unpack_json(char *json, void *info);
int kr_encode_info_unpack_json(char *json, void *info);
int kr_record_info_unpack_json(char *json, void *info);
int kr_transmission_info_unpack_json(char *json, void *info);
int kr_output_info_unpack_json(char *json, void *info);
int kr_capture_audio_info_unpack_json(char *json, void *info);
int kr_capture_video_info_unpack_json(char *json, void *info);
int kr_capture_image_info_unpack_json(char *json, void *info);
int kr_capture_info_unpack_json(char *json, void *info);
int kr_source_file_info_unpack_json(char *json, void *info);
int kr_source_remote_file_info_unpack_json(char *json, void *info);
int kr_source_stream_info_unpack_json(char *json, void *info);
int kr_studio_source_info_unpack_json(char *json, void *info);
int kr_decode_info_unpack_json(char *json, void *info);
int kr_play_info_unpack_json(char *json, void *info);
int kr_render_image_info_unpack_json(char *json, void *info);
int kr_render_video_info_unpack_json(char *json, void *info);
int kr_render_audio_info_unpack_json(char *json, void *info);
int kr_render_info_unpack_json(char *json, void *info);
int kr_generate_audio_info_unpack_json(char *json, void *info);
int kr_generate_color_info_unpack_json(char *json, void *info);
int kr_generate_video_info_unpack_json(char *json, void *info);
int kr_generate_info_unpack_json(char *json, void *info);
int kr_studio_path_info_unpack_json(char *json, void *info);
int kr_track_info_unpack_json(char *json, void *info);
int kr_track_block_info_unpack_json(char *json, void *info);
#endif

