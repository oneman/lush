#if !defined(_gen_info_functions_H)
# define _gen_info_functions_H (1)

typedef union kr_info kr_info;

#include <krad/adapter/hw/alsa/types.h>
#include <krad/adapter/hw/decklink/types.h>
#include <krad/adapter/hw/v4l2/types.h>
#include <krad/adapter/sw/aux/types.h>
#include <krad/adapter/sw/jack/types.h>
#include <krad/adapter/sw/wayland/types.h>
#include <krad/adapter/sw/x11/types.h>
#include <krad/app/types.h>
#include <krad/audio/types.h>
#include <krad/codec/types.h>
#include <krad/compositor/types.h>
#include <krad/container/mpeg/ts/types.h>
#include <krad/container/types.h>
#include <krad/draw/text_types.h>
#include <krad/draw/vector_types.h>
#include <krad/feat/types.h>
#include <krad/image/stash_types.h>
#include <krad/image/types.h>
#include <krad/io/types.h>
#include <krad/mixer/types.h>
#include <krad/ops/media/types.h>
#include <krad/ops/types.h>
#include <krad/perspective/types.h>
#include <krad/playlist/m3u_types.h>
#include <krad/radio/limit_types.h>
#include <krad/radio/types.h>
#include <krad/system/types.h>
#include <krad/tasks/avconv/types.h>
#include <krad/tasks/avprobe/types.h>
#include <krad/tasks/curl/types.h>
#include <krad/tasks/livestreamer/types.h>
#include <krad/tasks/quvi/types.h>
#include <krad/tasks/transmission_cli/types.h>
#include <krad/tasks/types.h>
#include <krad/tasks/valgrind_stressor/types.h>
#include <krad/tasks/xmms/types.h>
#include <krad/tasks/ydl/types.h>
#include <krad/time/types.h>
#include <krad/track/media_types.h>
#include <krad/track/types.h>
#include <krad/tuner/hls_types.h>
#include <krad/video/types.h>
#include <krad/web/types.h>
#include <krad/xfer/types.h>
#include <krad/xpdr/types.h>
#include "info_types.h"
typedef int (kr_info_type_to_text)(char *text, void *st, int max);
typedef int (kr_info_type_pack_json)(char *json, void *st, int max);
typedef int (kr_info_type_unpack_json)(char *json, void *st);
typedef int (kr_info_type_pack_ebml)(kr_ebml *ebml, void *st);
typedef int (kr_info_type_unpack_ebml)(kr_ebml *ebml, void *st);
typedef int (kr_info_type_init)(void *st);
typedef int (kr_info_type_valid)(void *st);
typedef int (kr_info_type_random)(void *st);
typedef int (kr_info_type_patch_path)(kr_patch *patch, kr_path *path);
typedef int (kr_info_type_diff)(kr_diffset *diffset, void *info1, void *info2);
union kr_info {
  uint8_t buf[8];
  kr_alsa_info alsa_info;
  kr_alsa_path_info alsa_path_info;
  kr_decklink_info decklink_info;
  kr_decklink_path_info decklink_path_info;
  kr_v4l2_path_info v4l2_path_info;
  kr_v4l2_info v4l2_info;
  kr_aux_info aux_info;
  kr_aux_path_info aux_path_info;
  kr_jack_info jack_info;
  kr_jack_path_info jack_path_info;
  kr_wayland_info wayland_info;
  kr_wayland_path_info wayland_path_info;
  kr_x11_info x11_info;
  kr_x11_path_info x11_path_info;
  struct kr_eq_band_info eq_band_info;
  struct kr_eq_info eq_info;
  struct kr_lowpass_info lowpass_info;
  struct kr_highpass_info highpass_info;
  struct kr_pass_info pass_info;
  struct kr_volume_info volume_info;
  struct kr_analog_info analog_info;
  struct kr_meter_info meter_info;
  struct kr_afx_info afx_info;
  kr_codec codec;
  kr_audio_info audio_info;
  kr_video_info video_info;
  kr_av_info av_info;
  kr_daala_encoder_info daala_encoder_info;
  kr_vpx_encoder_info vpx_encoder_info;
  kr_h264_encoder_info h264_encoder_info;
  kr_theora_encoder_info theora_encoder_info;
  kr_y4m_encoder_info y4m_encoder_info;
  kr_flac_info flac_info;
  kr_opus_info opus_info;
  kr_vorbis_info vorbis_info;
  kr_mp3_info mp3_info;
  kr_aac_info aac_info;
  kr_lavc_encoder_info lavc_encoder_info;
  struct kr_compositor_output_info compositor_output_info;
  struct kr_compositor_bus_info compositor_bus_info;
  struct kr_compositor_input_info compositor_input_info;
  struct kr_compositor_source_info compositor_source_info;
  struct kr_compositor_overlay_info compositor_overlay_info;
  struct kr_compositor_path_info compositor_path_info;
  kr_mts_packet_info mts_packet_info;
  kr_mts_stream_info mts_stream_info;
  kr_mts_program_info mts_program_info;
  kr_mts_info mts_info;
  struct kr_text_info text_info;
  struct kr_vector_info vector_info;
  kr_feature_aspect feature_aspect;
  struct kr_feature feature;
  kr_image_stash_item_info image_stash_item_info;
  kr_rect rect;
  kr_image_info image_info;
  kr_coded_image_info coded_image_info;
  kr_file2_info file2_info;
  struct kr_mixer_path_info mixer_path_info;
  kr_media_file_info media_file_info;
  kr_media_metadata_info media_metadata_info;
  kr_media_url_info media_url_info;
  kr_media_item_info media_item_info;
  kr_media_info media_info;
  kr_ops_info ops_info;
  struct kr_pos pos;
  struct kr_perspective_view perspective_view;
  kr_m3u_tag_prim_info m3u_tag_prim_info;
  kr_m3u_tag_alist_info m3u_tag_alist_info;
  m3u_tag_info m3u_tag_info;
  m3u_el_info m3u_el_info;
  kr_m3u_info m3u_info;
  struct kr_limit_info limit_info;
  struct kr_limits_info limits_info;
  kr_avconv_status_info avconv_status_info;
  kr_avconv_params_info avconv_params_info;
  kr_avconv_info avconv_info;
  kr_avprobe_format_info avprobe_format_info;
  kr_avprobe_stream_info avprobe_stream_info;
  kr_avprobe_media_info avprobe_media_info;
  kr_avprobe_params_info avprobe_params_info;
  kr_avprobe_status_info avprobe_status_info;
  kr_avprobe_info avprobe_info;
  kr_curl_status_info curl_status_info;
  kr_curl_params_info curl_params_info;
  kr_curl_info curl_info;
  kr_livestreamer_params_info livestreamer_params_info;
  kr_livestreamer_status_info livestreamer_status_info;
  kr_livestreamer_info livestreamer_info;
  kr_media_version media_version;
  kr_quvi_media_info quvi_media_info;
  kr_quvi_status_info quvi_status_info;
  kr_quvi_params_info quvi_params_info;
  kr_quvi_info quvi_info;
  kr_transmission_cli_status_info transmission_cli_status_info;
  kr_transmission_cli_params_info transmission_cli_params_info;
  kr_transmission_cli_info transmission_cli_info;
  kr_fail_info fail_info;
  kr_task_status task_status;
  kr_task_params_info task_params_info;
  kr_task_info task_info;
  kr_valgrind_stressor_status_info valgrind_stressor_status_info;
  kr_valgrind_stressor_params_info valgrind_stressor_params_info;
  kr_valgrind_stressor_info valgrind_stressor_info;
  kr_xmms_params_info xmms_params_info;
  kr_xmms_status_info xmms_status_info;
  kr_xmms_info xmms_info;
  kr_ydl_format_info ydl_format_info;
  kr_ydl_video_info ydl_video_info;
  kr_ydl_params_info ydl_params_info;
  kr_ydl_status_info ydl_status_info;
  kr_ydl_info ydl_info;
  kr_timestamp_info timestamp_info;
  kr_media_source_info media_source_info;
  kr_media_packet media_packet;
  kr_track_name track_name;
  kr_mux_name mux_name;
  kr_demux_info demux_info;
  kr_mux_info mux_info;
  kr_video_encode_info video_encode_info;
  kr_audio_encode_info audio_encode_info;
  kr_encode_info encode_info;
  kr_record_info record_info;
  kr_transmission_info transmission_info;
  kr_output_info output_info;
  kr_capture_audio_info capture_audio_info;
  kr_capture_video_info capture_video_info;
  kr_capture_image_info capture_image_info;
  kr_capture_info capture_info;
  kr_source_file_info source_file_info;
  kr_source_remote_file_info source_remote_file_info;
  kr_source_stream_info source_stream_info;
  kr_studio_source_info studio_source_info;
  kr_decode_info decode_info;
  kr_play_info play_info;
  kr_render_image_info render_image_info;
  kr_render_video_info render_video_info;
  kr_render_audio_info render_audio_info;
  kr_render_info render_info;
  kr_generate_audio_info generate_audio_info;
  kr_generate_color_info generate_color_info;
  kr_generate_video_info generate_video_info;
  kr_generate_info generate_info;
  kr_studio_path_info studio_path_info;
  kr_track_info track_info;
  kr_track_block_info track_block_info;
  kr_hls_variant hls_variant;
  kr_hls_rendition hls_rendition;
  kr_hls_master_playlist hls_master_playlist;
  kr_hls_media_segment hls_media_segment;
  kr_hls_media_playlist hls_media_playlist;
  kr_hls_playlist hls_playlist;
  kr_hls_tuner_info hls_tuner_info;
  kr_fps_info fps_info;
  kr_radio_info radio_info;
  kr_web_server_info web_server_info;
  kr_web_server_setup_info web_server_setup_info;
  kr_web_remote_info web_remote_info;
  kr_web_socket_io_state web_socket_io_state;
  kr_web_socket_state web_socket_state;
  kr_web_http_io_state web_http_io_state;
  kr_web_http_state web_http_state;
  kr_web_client_info web_client_info;
  kr_web_client_setup_info web_client_setup_info;
  kr_web_path_info web_path_info;
  kr_web_path_setup_info web_path_setup_info;
  kr_path_info path_info;
  kr_path_list path_list;
  kr_upload_file_info upload_file_info;
  kr_upload_stash_info upload_stash_info;
  kr_download_file_info download_file_info;
  kr_download_stash_info download_stash_info;
  kr_upload_info upload_info;
  kr_download_info download_info;
  kr_accept_info accept_info;
  kr_serve_range_info serve_range_info;
  kr_serve_ranges_info serve_ranges_info;
  kr_serve_full_info serve_full_info;
  kr_serve_index_info serve_index_info;
  kr_serve_redirect_info serve_redirect_info;
  kr_serve_info serve_info;
  kr_downlink_info downlink_info;
  kr_uplink_info uplink_info;
  kr_source_info source_info;
  kr_transmit_info transmit_info;
  kr_xfer_reconnect_info xfer_reconnect_info;
  kr_xfer_client_info xfer_client_info;
  kr_xfer_acceptor_dir_info xfer_acceptor_dir_info;
  kr_xfer_acceptor_file_info xfer_acceptor_file_info;
  kr_xfer_acceptor_buf_info xfer_acceptor_buf_info;
  kr_xfer_share_dir_info xfer_share_dir_info;
  kr_xfer_share_file_info xfer_share_file_info;
  kr_xfer_share_buf_info xfer_share_buf_info;
  kr_xfer_acceptor_info xfer_acceptor_info;
  kr_xfer_receiver_info xfer_receiver_info;
  kr_xfer_transmitter_info xfer_transmitter_info;
  kr_xfer_share_info xfer_share_info;
  kr_xfer_server_info xfer_server_info;
  kr_xfer_path_info xfer_path_info;
  struct kr_xpdr_path_info xpdr_path_info;
};

typedef struct {
  size_t size;
  kr_info_type_to_text *to_text;
  kr_info_type_pack_json *pack_json;
  kr_info_type_unpack_json *unpack_json;
  kr_info_type_pack_ebml *pack_ebml;
  kr_info_type_unpack_ebml *unpack_ebml;
  kr_info_type_init *init;
  kr_info_type_valid *valid;
  kr_info_type_random *random;
  kr_info_type_patch_path *patch_path;
  kr_info_type_diff *diff;
} kr_info_functions_spec;

static const kr_info_functions_spec info_functions[] = {
  [KR_ALSA_INFO] = {
    .size = sizeof(kr_alsa_info),
    .to_text = kr_alsa_info_to_text,
    .pack_json = kr_alsa_info_pack_json,
    .unpack_json = kr_alsa_info_unpack_json,
    .pack_ebml = kr_alsa_info_pack_ebml,
    .unpack_ebml = kr_alsa_info_unpack_ebml,
    .init = kr_alsa_info_init,
    .valid = kr_alsa_info_valid,
    .random = kr_alsa_info_random,
    .patch_path = kr_alsa_info_patch_path,
    .diff = kr_alsa_info_diff
  },
  [KR_ALSA_PATH_INFO] = {
    .size = sizeof(kr_alsa_path_info),
    .to_text = kr_alsa_path_info_to_text,
    .pack_json = kr_alsa_path_info_pack_json,
    .unpack_json = kr_alsa_path_info_unpack_json,
    .pack_ebml = kr_alsa_path_info_pack_ebml,
    .unpack_ebml = kr_alsa_path_info_unpack_ebml,
    .init = kr_alsa_path_info_init,
    .valid = kr_alsa_path_info_valid,
    .random = kr_alsa_path_info_random,
    .patch_path = kr_alsa_path_info_patch_path,
    .diff = kr_alsa_path_info_diff
  },
  [KR_DECKLINK_INFO] = {
    .size = sizeof(kr_decklink_info),
    .to_text = kr_decklink_info_to_text,
    .pack_json = kr_decklink_info_pack_json,
    .unpack_json = kr_decklink_info_unpack_json,
    .pack_ebml = kr_decklink_info_pack_ebml,
    .unpack_ebml = kr_decklink_info_unpack_ebml,
    .init = kr_decklink_info_init,
    .valid = kr_decklink_info_valid,
    .random = kr_decklink_info_random,
    .patch_path = kr_decklink_info_patch_path,
    .diff = kr_decklink_info_diff
  },
  [KR_DECKLINK_PATH_INFO] = {
    .size = sizeof(kr_decklink_path_info),
    .to_text = kr_decklink_path_info_to_text,
    .pack_json = kr_decklink_path_info_pack_json,
    .unpack_json = kr_decklink_path_info_unpack_json,
    .pack_ebml = kr_decklink_path_info_pack_ebml,
    .unpack_ebml = kr_decklink_path_info_unpack_ebml,
    .init = kr_decklink_path_info_init,
    .valid = kr_decklink_path_info_valid,
    .random = kr_decklink_path_info_random,
    .patch_path = kr_decklink_path_info_patch_path,
    .diff = kr_decklink_path_info_diff
  },
  [KR_V4L2_PATH_INFO] = {
    .size = sizeof(kr_v4l2_path_info),
    .to_text = kr_v4l2_path_info_to_text,
    .pack_json = kr_v4l2_path_info_pack_json,
    .unpack_json = kr_v4l2_path_info_unpack_json,
    .pack_ebml = kr_v4l2_path_info_pack_ebml,
    .unpack_ebml = kr_v4l2_path_info_unpack_ebml,
    .init = kr_v4l2_path_info_init,
    .valid = kr_v4l2_path_info_valid,
    .random = kr_v4l2_path_info_random,
    .patch_path = kr_v4l2_path_info_patch_path,
    .diff = kr_v4l2_path_info_diff
  },
  [KR_V4L2_INFO] = {
    .size = sizeof(kr_v4l2_info),
    .to_text = kr_v4l2_info_to_text,
    .pack_json = kr_v4l2_info_pack_json,
    .unpack_json = kr_v4l2_info_unpack_json,
    .pack_ebml = kr_v4l2_info_pack_ebml,
    .unpack_ebml = kr_v4l2_info_unpack_ebml,
    .init = kr_v4l2_info_init,
    .valid = kr_v4l2_info_valid,
    .random = kr_v4l2_info_random,
    .patch_path = kr_v4l2_info_patch_path,
    .diff = kr_v4l2_info_diff
  },
  [KR_AUX_INFO] = {
    .size = sizeof(kr_aux_info),
    .to_text = kr_aux_info_to_text,
    .pack_json = kr_aux_info_pack_json,
    .unpack_json = kr_aux_info_unpack_json,
    .pack_ebml = kr_aux_info_pack_ebml,
    .unpack_ebml = kr_aux_info_unpack_ebml,
    .init = kr_aux_info_init,
    .valid = kr_aux_info_valid,
    .random = kr_aux_info_random,
    .patch_path = kr_aux_info_patch_path,
    .diff = kr_aux_info_diff
  },
  [KR_AUX_PATH_INFO] = {
    .size = sizeof(kr_aux_path_info),
    .to_text = kr_aux_path_info_to_text,
    .pack_json = kr_aux_path_info_pack_json,
    .unpack_json = kr_aux_path_info_unpack_json,
    .pack_ebml = kr_aux_path_info_pack_ebml,
    .unpack_ebml = kr_aux_path_info_unpack_ebml,
    .init = kr_aux_path_info_init,
    .valid = kr_aux_path_info_valid,
    .random = kr_aux_path_info_random,
    .patch_path = kr_aux_path_info_patch_path,
    .diff = kr_aux_path_info_diff
  },
  [KR_JACK_INFO] = {
    .size = sizeof(kr_jack_info),
    .to_text = kr_jack_info_to_text,
    .pack_json = kr_jack_info_pack_json,
    .unpack_json = kr_jack_info_unpack_json,
    .pack_ebml = kr_jack_info_pack_ebml,
    .unpack_ebml = kr_jack_info_unpack_ebml,
    .init = kr_jack_info_init,
    .valid = kr_jack_info_valid,
    .random = kr_jack_info_random,
    .patch_path = kr_jack_info_patch_path,
    .diff = kr_jack_info_diff
  },
  [KR_JACK_PATH_INFO] = {
    .size = sizeof(kr_jack_path_info),
    .to_text = kr_jack_path_info_to_text,
    .pack_json = kr_jack_path_info_pack_json,
    .unpack_json = kr_jack_path_info_unpack_json,
    .pack_ebml = kr_jack_path_info_pack_ebml,
    .unpack_ebml = kr_jack_path_info_unpack_ebml,
    .init = kr_jack_path_info_init,
    .valid = kr_jack_path_info_valid,
    .random = kr_jack_path_info_random,
    .patch_path = kr_jack_path_info_patch_path,
    .diff = kr_jack_path_info_diff
  },
  [KR_WAYLAND_INFO] = {
    .size = sizeof(kr_wayland_info),
    .to_text = kr_wayland_info_to_text,
    .pack_json = kr_wayland_info_pack_json,
    .unpack_json = kr_wayland_info_unpack_json,
    .pack_ebml = kr_wayland_info_pack_ebml,
    .unpack_ebml = kr_wayland_info_unpack_ebml,
    .init = kr_wayland_info_init,
    .valid = kr_wayland_info_valid,
    .random = kr_wayland_info_random,
    .patch_path = kr_wayland_info_patch_path,
    .diff = kr_wayland_info_diff
  },
  [KR_WAYLAND_PATH_INFO] = {
    .size = sizeof(kr_wayland_path_info),
    .to_text = kr_wayland_path_info_to_text,
    .pack_json = kr_wayland_path_info_pack_json,
    .unpack_json = kr_wayland_path_info_unpack_json,
    .pack_ebml = kr_wayland_path_info_pack_ebml,
    .unpack_ebml = kr_wayland_path_info_unpack_ebml,
    .init = kr_wayland_path_info_init,
    .valid = kr_wayland_path_info_valid,
    .random = kr_wayland_path_info_random,
    .patch_path = kr_wayland_path_info_patch_path,
    .diff = kr_wayland_path_info_diff
  },
  [KR_X11_INFO] = {
    .size = sizeof(kr_x11_info),
    .to_text = kr_x11_info_to_text,
    .pack_json = kr_x11_info_pack_json,
    .unpack_json = kr_x11_info_unpack_json,
    .pack_ebml = kr_x11_info_pack_ebml,
    .unpack_ebml = kr_x11_info_unpack_ebml,
    .init = kr_x11_info_init,
    .valid = kr_x11_info_valid,
    .random = kr_x11_info_random,
    .patch_path = kr_x11_info_patch_path,
    .diff = kr_x11_info_diff
  },
  [KR_X11_PATH_INFO] = {
    .size = sizeof(kr_x11_path_info),
    .to_text = kr_x11_path_info_to_text,
    .pack_json = kr_x11_path_info_pack_json,
    .unpack_json = kr_x11_path_info_unpack_json,
    .pack_ebml = kr_x11_path_info_pack_ebml,
    .unpack_ebml = kr_x11_path_info_unpack_ebml,
    .init = kr_x11_path_info_init,
    .valid = kr_x11_path_info_valid,
    .random = kr_x11_path_info_random,
    .patch_path = kr_x11_path_info_patch_path,
    .diff = kr_x11_path_info_diff
  },
  [KR_EQ_BAND_INFO] = {
    .size = sizeof(struct kr_eq_band_info),
    .to_text = kr_eq_band_info_to_text,
    .pack_json = kr_eq_band_info_pack_json,
    .unpack_json = kr_eq_band_info_unpack_json,
    .pack_ebml = kr_eq_band_info_pack_ebml,
    .unpack_ebml = kr_eq_band_info_unpack_ebml,
    .init = kr_eq_band_info_init,
    .valid = kr_eq_band_info_valid,
    .random = kr_eq_band_info_random,
    .patch_path = kr_eq_band_info_patch_path,
    .diff = kr_eq_band_info_diff
  },
  [KR_EQ_INFO] = {
    .size = sizeof(struct kr_eq_info),
    .to_text = kr_eq_info_to_text,
    .pack_json = kr_eq_info_pack_json,
    .unpack_json = kr_eq_info_unpack_json,
    .pack_ebml = kr_eq_info_pack_ebml,
    .unpack_ebml = kr_eq_info_unpack_ebml,
    .init = kr_eq_info_init,
    .valid = kr_eq_info_valid,
    .random = kr_eq_info_random,
    .patch_path = kr_eq_info_patch_path,
    .diff = kr_eq_info_diff
  },
  [KR_LOWPASS_INFO] = {
    .size = sizeof(struct kr_lowpass_info),
    .to_text = kr_lowpass_info_to_text,
    .pack_json = kr_lowpass_info_pack_json,
    .unpack_json = kr_lowpass_info_unpack_json,
    .pack_ebml = kr_lowpass_info_pack_ebml,
    .unpack_ebml = kr_lowpass_info_unpack_ebml,
    .init = kr_lowpass_info_init,
    .valid = kr_lowpass_info_valid,
    .random = kr_lowpass_info_random,
    .patch_path = kr_lowpass_info_patch_path,
    .diff = kr_lowpass_info_diff
  },
  [KR_HIGHPASS_INFO] = {
    .size = sizeof(struct kr_highpass_info),
    .to_text = kr_highpass_info_to_text,
    .pack_json = kr_highpass_info_pack_json,
    .unpack_json = kr_highpass_info_unpack_json,
    .pack_ebml = kr_highpass_info_pack_ebml,
    .unpack_ebml = kr_highpass_info_unpack_ebml,
    .init = kr_highpass_info_init,
    .valid = kr_highpass_info_valid,
    .random = kr_highpass_info_random,
    .patch_path = kr_highpass_info_patch_path,
    .diff = kr_highpass_info_diff
  },
  [KR_PASS_INFO] = {
    .size = sizeof(struct kr_pass_info),
    .to_text = kr_pass_info_to_text,
    .pack_json = kr_pass_info_pack_json,
    .unpack_json = kr_pass_info_unpack_json,
    .pack_ebml = kr_pass_info_pack_ebml,
    .unpack_ebml = kr_pass_info_unpack_ebml,
    .init = kr_pass_info_init,
    .valid = kr_pass_info_valid,
    .random = kr_pass_info_random,
    .patch_path = kr_pass_info_patch_path,
    .diff = kr_pass_info_diff
  },
  [KR_VOLUME_INFO] = {
    .size = sizeof(struct kr_volume_info),
    .to_text = kr_volume_info_to_text,
    .pack_json = kr_volume_info_pack_json,
    .unpack_json = kr_volume_info_unpack_json,
    .pack_ebml = kr_volume_info_pack_ebml,
    .unpack_ebml = kr_volume_info_unpack_ebml,
    .init = kr_volume_info_init,
    .valid = kr_volume_info_valid,
    .random = kr_volume_info_random,
    .patch_path = kr_volume_info_patch_path,
    .diff = kr_volume_info_diff
  },
  [KR_ANALOG_INFO] = {
    .size = sizeof(struct kr_analog_info),
    .to_text = kr_analog_info_to_text,
    .pack_json = kr_analog_info_pack_json,
    .unpack_json = kr_analog_info_unpack_json,
    .pack_ebml = kr_analog_info_pack_ebml,
    .unpack_ebml = kr_analog_info_unpack_ebml,
    .init = kr_analog_info_init,
    .valid = kr_analog_info_valid,
    .random = kr_analog_info_random,
    .patch_path = kr_analog_info_patch_path,
    .diff = kr_analog_info_diff
  },
  [KR_METER_INFO] = {
    .size = sizeof(struct kr_meter_info),
    .to_text = kr_meter_info_to_text,
    .pack_json = kr_meter_info_pack_json,
    .unpack_json = kr_meter_info_unpack_json,
    .pack_ebml = kr_meter_info_pack_ebml,
    .unpack_ebml = kr_meter_info_unpack_ebml,
    .init = kr_meter_info_init,
    .valid = kr_meter_info_valid,
    .random = kr_meter_info_random,
    .patch_path = kr_meter_info_patch_path,
    .diff = kr_meter_info_diff
  },
  [KR_AFX_INFO] = {
    .size = sizeof(struct kr_afx_info),
    .to_text = kr_afx_info_to_text,
    .pack_json = kr_afx_info_pack_json,
    .unpack_json = kr_afx_info_unpack_json,
    .pack_ebml = kr_afx_info_pack_ebml,
    .unpack_ebml = kr_afx_info_unpack_ebml,
    .init = kr_afx_info_init,
    .valid = kr_afx_info_valid,
    .random = kr_afx_info_random,
    .patch_path = kr_afx_info_patch_path,
    .diff = kr_afx_info_diff
  },
  [KR_CODEC] = {
    .size = sizeof(kr_codec),
    .to_text = kr_codec_to_text,
    .pack_json = kr_codec_pack_json,
    .unpack_json = kr_codec_unpack_json,
    .pack_ebml = kr_codec_pack_ebml,
    .unpack_ebml = kr_codec_unpack_ebml,
    .init = kr_codec_init,
    .valid = kr_codec_valid,
    .random = kr_codec_random,
    .patch_path = kr_codec_patch_path,
    .diff = kr_codec_diff
  },
  [KR_AUDIO_INFO] = {
    .size = sizeof(kr_audio_info),
    .to_text = kr_audio_info_to_text,
    .pack_json = kr_audio_info_pack_json,
    .unpack_json = kr_audio_info_unpack_json,
    .pack_ebml = kr_audio_info_pack_ebml,
    .unpack_ebml = kr_audio_info_unpack_ebml,
    .init = kr_audio_info_init,
    .valid = kr_audio_info_valid,
    .random = kr_audio_info_random,
    .patch_path = kr_audio_info_patch_path,
    .diff = kr_audio_info_diff
  },
  [KR_VIDEO_INFO] = {
    .size = sizeof(kr_video_info),
    .to_text = kr_video_info_to_text,
    .pack_json = kr_video_info_pack_json,
    .unpack_json = kr_video_info_unpack_json,
    .pack_ebml = kr_video_info_pack_ebml,
    .unpack_ebml = kr_video_info_unpack_ebml,
    .init = kr_video_info_init,
    .valid = kr_video_info_valid,
    .random = kr_video_info_random,
    .patch_path = kr_video_info_patch_path,
    .diff = kr_video_info_diff
  },
  [KR_AV_INFO] = {
    .size = sizeof(kr_av_info),
    .to_text = kr_av_info_to_text,
    .pack_json = kr_av_info_pack_json,
    .unpack_json = kr_av_info_unpack_json,
    .pack_ebml = kr_av_info_pack_ebml,
    .unpack_ebml = kr_av_info_unpack_ebml,
    .init = kr_av_info_init,
    .valid = kr_av_info_valid,
    .random = kr_av_info_random,
    .patch_path = kr_av_info_patch_path,
    .diff = kr_av_info_diff
  },
  [KR_DAALA_ENCODER_INFO] = {
    .size = sizeof(kr_daala_encoder_info),
    .to_text = kr_daala_encoder_info_to_text,
    .pack_json = kr_daala_encoder_info_pack_json,
    .unpack_json = kr_daala_encoder_info_unpack_json,
    .pack_ebml = kr_daala_encoder_info_pack_ebml,
    .unpack_ebml = kr_daala_encoder_info_unpack_ebml,
    .init = kr_daala_encoder_info_init,
    .valid = kr_daala_encoder_info_valid,
    .random = kr_daala_encoder_info_random,
    .patch_path = kr_daala_encoder_info_patch_path,
    .diff = kr_daala_encoder_info_diff
  },
  [KR_VPX_ENCODER_INFO] = {
    .size = sizeof(kr_vpx_encoder_info),
    .to_text = kr_vpx_encoder_info_to_text,
    .pack_json = kr_vpx_encoder_info_pack_json,
    .unpack_json = kr_vpx_encoder_info_unpack_json,
    .pack_ebml = kr_vpx_encoder_info_pack_ebml,
    .unpack_ebml = kr_vpx_encoder_info_unpack_ebml,
    .init = kr_vpx_encoder_info_init,
    .valid = kr_vpx_encoder_info_valid,
    .random = kr_vpx_encoder_info_random,
    .patch_path = kr_vpx_encoder_info_patch_path,
    .diff = kr_vpx_encoder_info_diff
  },
  [KR_H264_ENCODER_INFO] = {
    .size = sizeof(kr_h264_encoder_info),
    .to_text = kr_h264_encoder_info_to_text,
    .pack_json = kr_h264_encoder_info_pack_json,
    .unpack_json = kr_h264_encoder_info_unpack_json,
    .pack_ebml = kr_h264_encoder_info_pack_ebml,
    .unpack_ebml = kr_h264_encoder_info_unpack_ebml,
    .init = kr_h264_encoder_info_init,
    .valid = kr_h264_encoder_info_valid,
    .random = kr_h264_encoder_info_random,
    .patch_path = kr_h264_encoder_info_patch_path,
    .diff = kr_h264_encoder_info_diff
  },
  [KR_THEORA_ENCODER_INFO] = {
    .size = sizeof(kr_theora_encoder_info),
    .to_text = kr_theora_encoder_info_to_text,
    .pack_json = kr_theora_encoder_info_pack_json,
    .unpack_json = kr_theora_encoder_info_unpack_json,
    .pack_ebml = kr_theora_encoder_info_pack_ebml,
    .unpack_ebml = kr_theora_encoder_info_unpack_ebml,
    .init = kr_theora_encoder_info_init,
    .valid = kr_theora_encoder_info_valid,
    .random = kr_theora_encoder_info_random,
    .patch_path = kr_theora_encoder_info_patch_path,
    .diff = kr_theora_encoder_info_diff
  },
  [KR_Y4M_ENCODER_INFO] = {
    .size = sizeof(kr_y4m_encoder_info),
    .to_text = kr_y4m_encoder_info_to_text,
    .pack_json = kr_y4m_encoder_info_pack_json,
    .unpack_json = kr_y4m_encoder_info_unpack_json,
    .pack_ebml = kr_y4m_encoder_info_pack_ebml,
    .unpack_ebml = kr_y4m_encoder_info_unpack_ebml,
    .init = kr_y4m_encoder_info_init,
    .valid = kr_y4m_encoder_info_valid,
    .random = kr_y4m_encoder_info_random,
    .patch_path = kr_y4m_encoder_info_patch_path,
    .diff = kr_y4m_encoder_info_diff
  },
  [KR_FLAC_INFO] = {
    .size = sizeof(kr_flac_info),
    .to_text = kr_flac_info_to_text,
    .pack_json = kr_flac_info_pack_json,
    .unpack_json = kr_flac_info_unpack_json,
    .pack_ebml = kr_flac_info_pack_ebml,
    .unpack_ebml = kr_flac_info_unpack_ebml,
    .init = kr_flac_info_init,
    .valid = kr_flac_info_valid,
    .random = kr_flac_info_random,
    .patch_path = kr_flac_info_patch_path,
    .diff = kr_flac_info_diff
  },
  [KR_OPUS_INFO] = {
    .size = sizeof(kr_opus_info),
    .to_text = kr_opus_info_to_text,
    .pack_json = kr_opus_info_pack_json,
    .unpack_json = kr_opus_info_unpack_json,
    .pack_ebml = kr_opus_info_pack_ebml,
    .unpack_ebml = kr_opus_info_unpack_ebml,
    .init = kr_opus_info_init,
    .valid = kr_opus_info_valid,
    .random = kr_opus_info_random,
    .patch_path = kr_opus_info_patch_path,
    .diff = kr_opus_info_diff
  },
  [KR_VORBIS_INFO] = {
    .size = sizeof(kr_vorbis_info),
    .to_text = kr_vorbis_info_to_text,
    .pack_json = kr_vorbis_info_pack_json,
    .unpack_json = kr_vorbis_info_unpack_json,
    .pack_ebml = kr_vorbis_info_pack_ebml,
    .unpack_ebml = kr_vorbis_info_unpack_ebml,
    .init = kr_vorbis_info_init,
    .valid = kr_vorbis_info_valid,
    .random = kr_vorbis_info_random,
    .patch_path = kr_vorbis_info_patch_path,
    .diff = kr_vorbis_info_diff
  },
  [KR_MP3_INFO] = {
    .size = sizeof(kr_mp3_info),
    .to_text = kr_mp3_info_to_text,
    .pack_json = kr_mp3_info_pack_json,
    .unpack_json = kr_mp3_info_unpack_json,
    .pack_ebml = kr_mp3_info_pack_ebml,
    .unpack_ebml = kr_mp3_info_unpack_ebml,
    .init = kr_mp3_info_init,
    .valid = kr_mp3_info_valid,
    .random = kr_mp3_info_random,
    .patch_path = kr_mp3_info_patch_path,
    .diff = kr_mp3_info_diff
  },
  [KR_AAC_INFO] = {
    .size = sizeof(kr_aac_info),
    .to_text = kr_aac_info_to_text,
    .pack_json = kr_aac_info_pack_json,
    .unpack_json = kr_aac_info_unpack_json,
    .pack_ebml = kr_aac_info_pack_ebml,
    .unpack_ebml = kr_aac_info_unpack_ebml,
    .init = kr_aac_info_init,
    .valid = kr_aac_info_valid,
    .random = kr_aac_info_random,
    .patch_path = kr_aac_info_patch_path,
    .diff = kr_aac_info_diff
  },
  [KR_LAVC_ENCODER_INFO] = {
    .size = sizeof(kr_lavc_encoder_info),
    .to_text = kr_lavc_encoder_info_to_text,
    .pack_json = kr_lavc_encoder_info_pack_json,
    .unpack_json = kr_lavc_encoder_info_unpack_json,
    .pack_ebml = kr_lavc_encoder_info_pack_ebml,
    .unpack_ebml = kr_lavc_encoder_info_unpack_ebml,
    .init = kr_lavc_encoder_info_init,
    .valid = kr_lavc_encoder_info_valid,
    .random = kr_lavc_encoder_info_random,
    .patch_path = kr_lavc_encoder_info_patch_path,
    .diff = kr_lavc_encoder_info_diff
  },
  [KR_COMPOSITOR_OUTPUT_INFO] = {
    .size = sizeof(struct kr_compositor_output_info),
    .to_text = kr_compositor_output_info_to_text,
    .pack_json = kr_compositor_output_info_pack_json,
    .unpack_json = kr_compositor_output_info_unpack_json,
    .pack_ebml = kr_compositor_output_info_pack_ebml,
    .unpack_ebml = kr_compositor_output_info_unpack_ebml,
    .init = kr_compositor_output_info_init,
    .valid = kr_compositor_output_info_valid,
    .random = kr_compositor_output_info_random,
    .patch_path = kr_compositor_output_info_patch_path,
    .diff = kr_compositor_output_info_diff
  },
  [KR_COMPOSITOR_BUS_INFO] = {
    .size = sizeof(struct kr_compositor_bus_info),
    .to_text = kr_compositor_bus_info_to_text,
    .pack_json = kr_compositor_bus_info_pack_json,
    .unpack_json = kr_compositor_bus_info_unpack_json,
    .pack_ebml = kr_compositor_bus_info_pack_ebml,
    .unpack_ebml = kr_compositor_bus_info_unpack_ebml,
    .init = kr_compositor_bus_info_init,
    .valid = kr_compositor_bus_info_valid,
    .random = kr_compositor_bus_info_random,
    .patch_path = kr_compositor_bus_info_patch_path,
    .diff = kr_compositor_bus_info_diff
  },
  [KR_COMPOSITOR_INPUT_INFO] = {
    .size = sizeof(struct kr_compositor_input_info),
    .to_text = kr_compositor_input_info_to_text,
    .pack_json = kr_compositor_input_info_pack_json,
    .unpack_json = kr_compositor_input_info_unpack_json,
    .pack_ebml = kr_compositor_input_info_pack_ebml,
    .unpack_ebml = kr_compositor_input_info_unpack_ebml,
    .init = kr_compositor_input_info_init,
    .valid = kr_compositor_input_info_valid,
    .random = kr_compositor_input_info_random,
    .patch_path = kr_compositor_input_info_patch_path,
    .diff = kr_compositor_input_info_diff
  },
  [KR_COMPOSITOR_SOURCE_INFO] = {
    .size = sizeof(struct kr_compositor_source_info),
    .to_text = kr_compositor_source_info_to_text,
    .pack_json = kr_compositor_source_info_pack_json,
    .unpack_json = kr_compositor_source_info_unpack_json,
    .pack_ebml = kr_compositor_source_info_pack_ebml,
    .unpack_ebml = kr_compositor_source_info_unpack_ebml,
    .init = kr_compositor_source_info_init,
    .valid = kr_compositor_source_info_valid,
    .random = kr_compositor_source_info_random,
    .patch_path = kr_compositor_source_info_patch_path,
    .diff = kr_compositor_source_info_diff
  },
  [KR_COMPOSITOR_OVERLAY_INFO] = {
    .size = sizeof(struct kr_compositor_overlay_info),
    .to_text = kr_compositor_overlay_info_to_text,
    .pack_json = kr_compositor_overlay_info_pack_json,
    .unpack_json = kr_compositor_overlay_info_unpack_json,
    .pack_ebml = kr_compositor_overlay_info_pack_ebml,
    .unpack_ebml = kr_compositor_overlay_info_unpack_ebml,
    .init = kr_compositor_overlay_info_init,
    .valid = kr_compositor_overlay_info_valid,
    .random = kr_compositor_overlay_info_random,
    .patch_path = kr_compositor_overlay_info_patch_path,
    .diff = kr_compositor_overlay_info_diff
  },
  [KR_COMPOSITOR_PATH_INFO] = {
    .size = sizeof(struct kr_compositor_path_info),
    .to_text = kr_compositor_path_info_to_text,
    .pack_json = kr_compositor_path_info_pack_json,
    .unpack_json = kr_compositor_path_info_unpack_json,
    .pack_ebml = kr_compositor_path_info_pack_ebml,
    .unpack_ebml = kr_compositor_path_info_unpack_ebml,
    .init = kr_compositor_path_info_init,
    .valid = kr_compositor_path_info_valid,
    .random = kr_compositor_path_info_random,
    .patch_path = kr_compositor_path_info_patch_path,
    .diff = kr_compositor_path_info_diff
  },
  [KR_MTS_PACKET_INFO] = {
    .size = sizeof(kr_mts_packet_info),
    .to_text = kr_mts_packet_info_to_text,
    .pack_json = kr_mts_packet_info_pack_json,
    .unpack_json = kr_mts_packet_info_unpack_json,
    .pack_ebml = kr_mts_packet_info_pack_ebml,
    .unpack_ebml = kr_mts_packet_info_unpack_ebml,
    .init = kr_mts_packet_info_init,
    .valid = kr_mts_packet_info_valid,
    .random = kr_mts_packet_info_random,
    .patch_path = kr_mts_packet_info_patch_path,
    .diff = kr_mts_packet_info_diff
  },
  [KR_MTS_STREAM_INFO] = {
    .size = sizeof(kr_mts_stream_info),
    .to_text = kr_mts_stream_info_to_text,
    .pack_json = kr_mts_stream_info_pack_json,
    .unpack_json = kr_mts_stream_info_unpack_json,
    .pack_ebml = kr_mts_stream_info_pack_ebml,
    .unpack_ebml = kr_mts_stream_info_unpack_ebml,
    .init = kr_mts_stream_info_init,
    .valid = kr_mts_stream_info_valid,
    .random = kr_mts_stream_info_random,
    .patch_path = kr_mts_stream_info_patch_path,
    .diff = kr_mts_stream_info_diff
  },
  [KR_MTS_PROGRAM_INFO] = {
    .size = sizeof(kr_mts_program_info),
    .to_text = kr_mts_program_info_to_text,
    .pack_json = kr_mts_program_info_pack_json,
    .unpack_json = kr_mts_program_info_unpack_json,
    .pack_ebml = kr_mts_program_info_pack_ebml,
    .unpack_ebml = kr_mts_program_info_unpack_ebml,
    .init = kr_mts_program_info_init,
    .valid = kr_mts_program_info_valid,
    .random = kr_mts_program_info_random,
    .patch_path = kr_mts_program_info_patch_path,
    .diff = kr_mts_program_info_diff
  },
  [KR_MTS_INFO] = {
    .size = sizeof(kr_mts_info),
    .to_text = kr_mts_info_to_text,
    .pack_json = kr_mts_info_pack_json,
    .unpack_json = kr_mts_info_unpack_json,
    .pack_ebml = kr_mts_info_pack_ebml,
    .unpack_ebml = kr_mts_info_unpack_ebml,
    .init = kr_mts_info_init,
    .valid = kr_mts_info_valid,
    .random = kr_mts_info_random,
    .patch_path = kr_mts_info_patch_path,
    .diff = kr_mts_info_diff
  },
  [KR_TEXT_INFO] = {
    .size = sizeof(struct kr_text_info),
    .to_text = kr_text_info_to_text,
    .pack_json = kr_text_info_pack_json,
    .unpack_json = kr_text_info_unpack_json,
    .pack_ebml = kr_text_info_pack_ebml,
    .unpack_ebml = kr_text_info_unpack_ebml,
    .init = kr_text_info_init,
    .valid = kr_text_info_valid,
    .random = kr_text_info_random,
    .patch_path = kr_text_info_patch_path,
    .diff = kr_text_info_diff
  },
  [KR_VECTOR_INFO] = {
    .size = sizeof(struct kr_vector_info),
    .to_text = kr_vector_info_to_text,
    .pack_json = kr_vector_info_pack_json,
    .unpack_json = kr_vector_info_unpack_json,
    .pack_ebml = kr_vector_info_pack_ebml,
    .unpack_ebml = kr_vector_info_unpack_ebml,
    .init = kr_vector_info_init,
    .valid = kr_vector_info_valid,
    .random = kr_vector_info_random,
    .patch_path = kr_vector_info_patch_path,
    .diff = kr_vector_info_diff
  },
  [KR_FEATURE_ASPECT] = {
    .size = sizeof(kr_feature_aspect),
    .to_text = kr_feature_aspect_to_text,
    .pack_json = kr_feature_aspect_pack_json,
    .unpack_json = kr_feature_aspect_unpack_json,
    .pack_ebml = kr_feature_aspect_pack_ebml,
    .unpack_ebml = kr_feature_aspect_unpack_ebml,
    .init = kr_feature_aspect_init,
    .valid = kr_feature_aspect_valid,
    .random = kr_feature_aspect_random,
    .patch_path = kr_feature_aspect_patch_path,
    .diff = kr_feature_aspect_diff
  },
  [KR_FEATURE] = {
    .size = sizeof(struct kr_feature),
    .to_text = kr_feature_to_text,
    .pack_json = kr_feature_pack_json,
    .unpack_json = kr_feature_unpack_json,
    .pack_ebml = kr_feature_pack_ebml,
    .unpack_ebml = kr_feature_unpack_ebml,
    .init = kr_feature_init,
    .valid = kr_feature_valid,
    .random = kr_feature_random,
    .patch_path = kr_feature_patch_path,
    .diff = kr_feature_diff
  },
  [KR_IMAGE_STASH_ITEM_INFO] = {
    .size = sizeof(kr_image_stash_item_info),
    .to_text = kr_image_stash_item_info_to_text,
    .pack_json = kr_image_stash_item_info_pack_json,
    .unpack_json = kr_image_stash_item_info_unpack_json,
    .pack_ebml = kr_image_stash_item_info_pack_ebml,
    .unpack_ebml = kr_image_stash_item_info_unpack_ebml,
    .init = kr_image_stash_item_info_init,
    .valid = kr_image_stash_item_info_valid,
    .random = kr_image_stash_item_info_random,
    .patch_path = kr_image_stash_item_info_patch_path,
    .diff = kr_image_stash_item_info_diff
  },
  [KR_RECT] = {
    .size = sizeof(kr_rect),
    .to_text = kr_rect_to_text,
    .pack_json = kr_rect_pack_json,
    .unpack_json = kr_rect_unpack_json,
    .pack_ebml = kr_rect_pack_ebml,
    .unpack_ebml = kr_rect_unpack_ebml,
    .init = kr_rect_init,
    .valid = kr_rect_valid,
    .random = kr_rect_random,
    .patch_path = kr_rect_patch_path,
    .diff = kr_rect_diff
  },
  [KR_IMAGE_INFO] = {
    .size = sizeof(kr_image_info),
    .to_text = kr_image_info_to_text,
    .pack_json = kr_image_info_pack_json,
    .unpack_json = kr_image_info_unpack_json,
    .pack_ebml = kr_image_info_pack_ebml,
    .unpack_ebml = kr_image_info_unpack_ebml,
    .init = kr_image_info_init,
    .valid = kr_image_info_valid,
    .random = kr_image_info_random,
    .patch_path = kr_image_info_patch_path,
    .diff = kr_image_info_diff
  },
  [KR_CODED_IMAGE_INFO] = {
    .size = sizeof(kr_coded_image_info),
    .to_text = kr_coded_image_info_to_text,
    .pack_json = kr_coded_image_info_pack_json,
    .unpack_json = kr_coded_image_info_unpack_json,
    .pack_ebml = kr_coded_image_info_pack_ebml,
    .unpack_ebml = kr_coded_image_info_unpack_ebml,
    .init = kr_coded_image_info_init,
    .valid = kr_coded_image_info_valid,
    .random = kr_coded_image_info_random,
    .patch_path = kr_coded_image_info_patch_path,
    .diff = kr_coded_image_info_diff
  },
  [KR_FILE2_INFO] = {
    .size = sizeof(kr_file2_info),
    .to_text = kr_file2_info_to_text,
    .pack_json = kr_file2_info_pack_json,
    .unpack_json = kr_file2_info_unpack_json,
    .pack_ebml = kr_file2_info_pack_ebml,
    .unpack_ebml = kr_file2_info_unpack_ebml,
    .init = kr_file2_info_init,
    .valid = kr_file2_info_valid,
    .random = kr_file2_info_random,
    .patch_path = kr_file2_info_patch_path,
    .diff = kr_file2_info_diff
  },
  [KR_MIXER_PATH_INFO] = {
    .size = sizeof(struct kr_mixer_path_info),
    .to_text = kr_mixer_path_info_to_text,
    .pack_json = kr_mixer_path_info_pack_json,
    .unpack_json = kr_mixer_path_info_unpack_json,
    .pack_ebml = kr_mixer_path_info_pack_ebml,
    .unpack_ebml = kr_mixer_path_info_unpack_ebml,
    .init = kr_mixer_path_info_init,
    .valid = kr_mixer_path_info_valid,
    .random = kr_mixer_path_info_random,
    .patch_path = kr_mixer_path_info_patch_path,
    .diff = kr_mixer_path_info_diff
  },
  [KR_MEDIA_FILE_INFO] = {
    .size = sizeof(kr_media_file_info),
    .to_text = kr_media_file_info_to_text,
    .pack_json = kr_media_file_info_pack_json,
    .unpack_json = kr_media_file_info_unpack_json,
    .init = kr_media_file_info_init,
    .valid = kr_media_file_info_valid,
    .random = kr_media_file_info_random,
    .patch_path = kr_media_file_info_patch_path,
    .diff = kr_media_file_info_diff
  },
  [KR_MEDIA_METADATA_INFO] = {
    .size = sizeof(kr_media_metadata_info),
    .to_text = kr_media_metadata_info_to_text,
    .pack_json = kr_media_metadata_info_pack_json,
    .unpack_json = kr_media_metadata_info_unpack_json,
    .init = kr_media_metadata_info_init,
    .valid = kr_media_metadata_info_valid,
    .random = kr_media_metadata_info_random,
    .patch_path = kr_media_metadata_info_patch_path,
    .diff = kr_media_metadata_info_diff
  },
  [KR_MEDIA_URL_INFO] = {
    .size = sizeof(kr_media_url_info),
    .to_text = kr_media_url_info_to_text,
    .pack_json = kr_media_url_info_pack_json,
    .unpack_json = kr_media_url_info_unpack_json,
    .init = kr_media_url_info_init,
    .valid = kr_media_url_info_valid,
    .random = kr_media_url_info_random,
    .patch_path = kr_media_url_info_patch_path,
    .diff = kr_media_url_info_diff
  },
  [KR_MEDIA_ITEM_INFO] = {
    .size = sizeof(kr_media_item_info),
    .to_text = kr_media_item_info_to_text,
    .pack_json = kr_media_item_info_pack_json,
    .unpack_json = kr_media_item_info_unpack_json,
    .init = kr_media_item_info_init,
    .valid = kr_media_item_info_valid,
    .random = kr_media_item_info_random,
    .patch_path = kr_media_item_info_patch_path,
    .diff = kr_media_item_info_diff
  },
  [KR_MEDIA_INFO] = {
    .size = sizeof(kr_media_info),
    .to_text = kr_media_info_to_text,
    .pack_json = kr_media_info_pack_json,
    .unpack_json = kr_media_info_unpack_json,
    .init = kr_media_info_init,
    .valid = kr_media_info_valid,
    .random = kr_media_info_random,
    .patch_path = kr_media_info_patch_path,
    .diff = kr_media_info_diff
  },
  [KR_OPS_INFO] = {
    .size = sizeof(kr_ops_info),
    .to_text = kr_ops_info_to_text,
    .pack_json = kr_ops_info_pack_json,
    .unpack_json = kr_ops_info_unpack_json,
    .init = kr_ops_info_init,
    .valid = kr_ops_info_valid,
    .random = kr_ops_info_random,
    .patch_path = kr_ops_info_patch_path,
    .diff = kr_ops_info_diff
  },
  [KR_POS] = {
    .size = sizeof(struct kr_pos),
    .to_text = kr_pos_to_text,
    .pack_json = kr_pos_pack_json,
    .unpack_json = kr_pos_unpack_json,
    .pack_ebml = kr_pos_pack_ebml,
    .unpack_ebml = kr_pos_unpack_ebml,
    .init = kr_pos_init,
    .valid = kr_pos_valid,
    .random = kr_pos_random,
    .patch_path = kr_pos_patch_path,
    .diff = kr_pos_diff
  },
  [KR_PERSPECTIVE_VIEW] = {
    .size = sizeof(struct kr_perspective_view),
    .to_text = kr_perspective_view_to_text,
    .pack_json = kr_perspective_view_pack_json,
    .unpack_json = kr_perspective_view_unpack_json,
    .pack_ebml = kr_perspective_view_pack_ebml,
    .unpack_ebml = kr_perspective_view_unpack_ebml,
    .init = kr_perspective_view_init,
    .valid = kr_perspective_view_valid,
    .random = kr_perspective_view_random,
    .patch_path = kr_perspective_view_patch_path,
    .diff = kr_perspective_view_diff
  },
  [KR_M3U_TAG_PRIM_INFO] = {
    .size = sizeof(kr_m3u_tag_prim_info),
    .to_text = kr_m3u_tag_prim_info_to_text,
    .pack_json = kr_m3u_tag_prim_info_pack_json,
    .unpack_json = kr_m3u_tag_prim_info_unpack_json,
    .pack_ebml = kr_m3u_tag_prim_info_pack_ebml,
    .unpack_ebml = kr_m3u_tag_prim_info_unpack_ebml,
    .init = kr_m3u_tag_prim_info_init,
    .valid = kr_m3u_tag_prim_info_valid,
    .random = kr_m3u_tag_prim_info_random,
    .patch_path = kr_m3u_tag_prim_info_patch_path,
    .diff = kr_m3u_tag_prim_info_diff
  },
  [KR_M3U_TAG_ALIST_INFO] = {
    .size = sizeof(kr_m3u_tag_alist_info),
    .to_text = kr_m3u_tag_alist_info_to_text,
    .pack_json = kr_m3u_tag_alist_info_pack_json,
    .unpack_json = kr_m3u_tag_alist_info_unpack_json,
    .pack_ebml = kr_m3u_tag_alist_info_pack_ebml,
    .unpack_ebml = kr_m3u_tag_alist_info_unpack_ebml,
    .init = kr_m3u_tag_alist_info_init,
    .valid = kr_m3u_tag_alist_info_valid,
    .random = kr_m3u_tag_alist_info_random,
    .patch_path = kr_m3u_tag_alist_info_patch_path,
    .diff = kr_m3u_tag_alist_info_diff
  },
  [M3U_TAG_INFO] = {
    .size = sizeof(m3u_tag_info),
    .to_text = m3u_tag_info_to_text,
    .pack_json = m3u_tag_info_pack_json,
    .unpack_json = m3u_tag_info_unpack_json,
    .pack_ebml = m3u_tag_info_pack_ebml,
    .unpack_ebml = m3u_tag_info_unpack_ebml,
    .init = m3u_tag_info_init,
    .valid = m3u_tag_info_valid,
    .random = m3u_tag_info_random,
    .patch_path = m3u_tag_info_patch_path,
    .diff = m3u_tag_info_diff
  },
  [M3U_EL_INFO] = {
    .size = sizeof(m3u_el_info),
    .to_text = m3u_el_info_to_text,
    .pack_json = m3u_el_info_pack_json,
    .unpack_json = m3u_el_info_unpack_json,
    .pack_ebml = m3u_el_info_pack_ebml,
    .unpack_ebml = m3u_el_info_unpack_ebml,
    .init = m3u_el_info_init,
    .valid = m3u_el_info_valid,
    .random = m3u_el_info_random,
    .patch_path = m3u_el_info_patch_path,
    .diff = m3u_el_info_diff
  },
  [KR_M3U_INFO] = {
    .size = sizeof(kr_m3u_info),
    .to_text = kr_m3u_info_to_text,
    .pack_json = kr_m3u_info_pack_json,
    .unpack_json = kr_m3u_info_unpack_json,
    .pack_ebml = kr_m3u_info_pack_ebml,
    .unpack_ebml = kr_m3u_info_unpack_ebml,
    .init = kr_m3u_info_init,
    .valid = kr_m3u_info_valid,
    .random = kr_m3u_info_random,
    .patch_path = kr_m3u_info_patch_path,
    .diff = kr_m3u_info_diff
  },
  [KR_LIMIT_INFO] = {
    .size = sizeof(struct kr_limit_info),
    .to_text = kr_limit_info_to_text,
    .pack_json = kr_limit_info_pack_json,
    .unpack_json = kr_limit_info_unpack_json,
    .pack_ebml = kr_limit_info_pack_ebml,
    .unpack_ebml = kr_limit_info_unpack_ebml,
    .init = kr_limit_info_init,
    .valid = kr_limit_info_valid,
    .random = kr_limit_info_random,
    .patch_path = kr_limit_info_patch_path,
    .diff = kr_limit_info_diff
  },
  [KR_LIMITS_INFO] = {
    .size = sizeof(struct kr_limits_info),
    .to_text = kr_limits_info_to_text,
    .pack_json = kr_limits_info_pack_json,
    .unpack_json = kr_limits_info_unpack_json,
    .pack_ebml = kr_limits_info_pack_ebml,
    .unpack_ebml = kr_limits_info_unpack_ebml,
    .init = kr_limits_info_init,
    .valid = kr_limits_info_valid,
    .random = kr_limits_info_random,
    .patch_path = kr_limits_info_patch_path,
    .diff = kr_limits_info_diff
  },
  [KR_AVCONV_STATUS_INFO] = {
    .size = sizeof(kr_avconv_status_info),
    .to_text = kr_avconv_status_info_to_text,
    .pack_json = kr_avconv_status_info_pack_json,
    .unpack_json = kr_avconv_status_info_unpack_json,
    .init = kr_avconv_status_info_init,
    .valid = kr_avconv_status_info_valid,
    .random = kr_avconv_status_info_random,
    .patch_path = kr_avconv_status_info_patch_path,
    .diff = kr_avconv_status_info_diff
  },
  [KR_AVCONV_PARAMS_INFO] = {
    .size = sizeof(kr_avconv_params_info),
    .to_text = kr_avconv_params_info_to_text,
    .pack_json = kr_avconv_params_info_pack_json,
    .unpack_json = kr_avconv_params_info_unpack_json,
    .init = kr_avconv_params_info_init,
    .valid = kr_avconv_params_info_valid,
    .random = kr_avconv_params_info_random,
    .patch_path = kr_avconv_params_info_patch_path,
    .diff = kr_avconv_params_info_diff
  },
  [KR_AVCONV_INFO] = {
    .size = sizeof(kr_avconv_info),
    .to_text = kr_avconv_info_to_text,
    .pack_json = kr_avconv_info_pack_json,
    .unpack_json = kr_avconv_info_unpack_json,
    .init = kr_avconv_info_init,
    .valid = kr_avconv_info_valid,
    .random = kr_avconv_info_random,
    .patch_path = kr_avconv_info_patch_path,
    .diff = kr_avconv_info_diff
  },
  [KR_AVPROBE_FORMAT_INFO] = {
    .size = sizeof(kr_avprobe_format_info),
    .to_text = kr_avprobe_format_info_to_text,
    .pack_json = kr_avprobe_format_info_pack_json,
    .unpack_json = kr_avprobe_format_info_unpack_json,
    .init = kr_avprobe_format_info_init,
    .valid = kr_avprobe_format_info_valid,
    .random = kr_avprobe_format_info_random,
    .patch_path = kr_avprobe_format_info_patch_path,
    .diff = kr_avprobe_format_info_diff
  },
  [KR_AVPROBE_STREAM_INFO] = {
    .size = sizeof(kr_avprobe_stream_info),
    .to_text = kr_avprobe_stream_info_to_text,
    .pack_json = kr_avprobe_stream_info_pack_json,
    .unpack_json = kr_avprobe_stream_info_unpack_json,
    .init = kr_avprobe_stream_info_init,
    .valid = kr_avprobe_stream_info_valid,
    .random = kr_avprobe_stream_info_random,
    .patch_path = kr_avprobe_stream_info_patch_path,
    .diff = kr_avprobe_stream_info_diff
  },
  [KR_AVPROBE_MEDIA_INFO] = {
    .size = sizeof(kr_avprobe_media_info),
    .to_text = kr_avprobe_media_info_to_text,
    .pack_json = kr_avprobe_media_info_pack_json,
    .unpack_json = kr_avprobe_media_info_unpack_json,
    .init = kr_avprobe_media_info_init,
    .valid = kr_avprobe_media_info_valid,
    .random = kr_avprobe_media_info_random,
    .patch_path = kr_avprobe_media_info_patch_path,
    .diff = kr_avprobe_media_info_diff
  },
  [KR_AVPROBE_PARAMS_INFO] = {
    .size = sizeof(kr_avprobe_params_info),
    .to_text = kr_avprobe_params_info_to_text,
    .pack_json = kr_avprobe_params_info_pack_json,
    .unpack_json = kr_avprobe_params_info_unpack_json,
    .init = kr_avprobe_params_info_init,
    .valid = kr_avprobe_params_info_valid,
    .random = kr_avprobe_params_info_random,
    .patch_path = kr_avprobe_params_info_patch_path,
    .diff = kr_avprobe_params_info_diff
  },
  [KR_AVPROBE_STATUS_INFO] = {
    .size = sizeof(kr_avprobe_status_info),
    .to_text = kr_avprobe_status_info_to_text,
    .pack_json = kr_avprobe_status_info_pack_json,
    .unpack_json = kr_avprobe_status_info_unpack_json,
    .init = kr_avprobe_status_info_init,
    .valid = kr_avprobe_status_info_valid,
    .random = kr_avprobe_status_info_random,
    .patch_path = kr_avprobe_status_info_patch_path,
    .diff = kr_avprobe_status_info_diff
  },
  [KR_AVPROBE_INFO] = {
    .size = sizeof(kr_avprobe_info),
    .to_text = kr_avprobe_info_to_text,
    .pack_json = kr_avprobe_info_pack_json,
    .unpack_json = kr_avprobe_info_unpack_json,
    .init = kr_avprobe_info_init,
    .valid = kr_avprobe_info_valid,
    .random = kr_avprobe_info_random,
    .patch_path = kr_avprobe_info_patch_path,
    .diff = kr_avprobe_info_diff
  },
  [KR_CURL_STATUS_INFO] = {
    .size = sizeof(kr_curl_status_info),
    .to_text = kr_curl_status_info_to_text,
    .pack_json = kr_curl_status_info_pack_json,
    .unpack_json = kr_curl_status_info_unpack_json,
    .init = kr_curl_status_info_init,
    .valid = kr_curl_status_info_valid,
    .random = kr_curl_status_info_random,
    .patch_path = kr_curl_status_info_patch_path,
    .diff = kr_curl_status_info_diff
  },
  [KR_CURL_PARAMS_INFO] = {
    .size = sizeof(kr_curl_params_info),
    .to_text = kr_curl_params_info_to_text,
    .pack_json = kr_curl_params_info_pack_json,
    .unpack_json = kr_curl_params_info_unpack_json,
    .init = kr_curl_params_info_init,
    .valid = kr_curl_params_info_valid,
    .random = kr_curl_params_info_random,
    .patch_path = kr_curl_params_info_patch_path,
    .diff = kr_curl_params_info_diff
  },
  [KR_CURL_INFO] = {
    .size = sizeof(kr_curl_info),
    .to_text = kr_curl_info_to_text,
    .pack_json = kr_curl_info_pack_json,
    .unpack_json = kr_curl_info_unpack_json,
    .init = kr_curl_info_init,
    .valid = kr_curl_info_valid,
    .random = kr_curl_info_random,
    .patch_path = kr_curl_info_patch_path,
    .diff = kr_curl_info_diff
  },
  [KR_LIVESTREAMER_PARAMS_INFO] = {
    .size = sizeof(kr_livestreamer_params_info),
    .to_text = kr_livestreamer_params_info_to_text,
    .pack_json = kr_livestreamer_params_info_pack_json,
    .unpack_json = kr_livestreamer_params_info_unpack_json,
    .init = kr_livestreamer_params_info_init,
    .valid = kr_livestreamer_params_info_valid,
    .random = kr_livestreamer_params_info_random,
    .patch_path = kr_livestreamer_params_info_patch_path,
    .diff = kr_livestreamer_params_info_diff
  },
  [KR_LIVESTREAMER_STATUS_INFO] = {
    .size = sizeof(kr_livestreamer_status_info),
    .to_text = kr_livestreamer_status_info_to_text,
    .pack_json = kr_livestreamer_status_info_pack_json,
    .unpack_json = kr_livestreamer_status_info_unpack_json,
    .init = kr_livestreamer_status_info_init,
    .valid = kr_livestreamer_status_info_valid,
    .random = kr_livestreamer_status_info_random,
    .patch_path = kr_livestreamer_status_info_patch_path,
    .diff = kr_livestreamer_status_info_diff
  },
  [KR_LIVESTREAMER_INFO] = {
    .size = sizeof(kr_livestreamer_info),
    .to_text = kr_livestreamer_info_to_text,
    .pack_json = kr_livestreamer_info_pack_json,
    .unpack_json = kr_livestreamer_info_unpack_json,
    .init = kr_livestreamer_info_init,
    .valid = kr_livestreamer_info_valid,
    .random = kr_livestreamer_info_random,
    .patch_path = kr_livestreamer_info_patch_path,
    .diff = kr_livestreamer_info_diff
  },
  [KR_MEDIA_VERSION] = {
    .size = sizeof(kr_media_version),
    .to_text = kr_media_version_to_text,
    .pack_json = kr_media_version_pack_json,
    .unpack_json = kr_media_version_unpack_json,
    .init = kr_media_version_init,
    .valid = kr_media_version_valid,
    .random = kr_media_version_random,
    .patch_path = kr_media_version_patch_path,
    .diff = kr_media_version_diff
  },
  [KR_QUVI_MEDIA_INFO] = {
    .size = sizeof(kr_quvi_media_info),
    .to_text = kr_quvi_media_info_to_text,
    .pack_json = kr_quvi_media_info_pack_json,
    .unpack_json = kr_quvi_media_info_unpack_json,
    .init = kr_quvi_media_info_init,
    .valid = kr_quvi_media_info_valid,
    .random = kr_quvi_media_info_random,
    .patch_path = kr_quvi_media_info_patch_path,
    .diff = kr_quvi_media_info_diff
  },
  [KR_QUVI_STATUS_INFO] = {
    .size = sizeof(kr_quvi_status_info),
    .to_text = kr_quvi_status_info_to_text,
    .pack_json = kr_quvi_status_info_pack_json,
    .unpack_json = kr_quvi_status_info_unpack_json,
    .init = kr_quvi_status_info_init,
    .valid = kr_quvi_status_info_valid,
    .random = kr_quvi_status_info_random,
    .patch_path = kr_quvi_status_info_patch_path,
    .diff = kr_quvi_status_info_diff
  },
  [KR_QUVI_PARAMS_INFO] = {
    .size = sizeof(kr_quvi_params_info),
    .to_text = kr_quvi_params_info_to_text,
    .pack_json = kr_quvi_params_info_pack_json,
    .unpack_json = kr_quvi_params_info_unpack_json,
    .init = kr_quvi_params_info_init,
    .valid = kr_quvi_params_info_valid,
    .random = kr_quvi_params_info_random,
    .patch_path = kr_quvi_params_info_patch_path,
    .diff = kr_quvi_params_info_diff
  },
  [KR_QUVI_INFO] = {
    .size = sizeof(kr_quvi_info),
    .to_text = kr_quvi_info_to_text,
    .pack_json = kr_quvi_info_pack_json,
    .unpack_json = kr_quvi_info_unpack_json,
    .init = kr_quvi_info_init,
    .valid = kr_quvi_info_valid,
    .random = kr_quvi_info_random,
    .patch_path = kr_quvi_info_patch_path,
    .diff = kr_quvi_info_diff
  },
  [KR_TRANSMISSION_CLI_STATUS_INFO] = {
    .size = sizeof(kr_transmission_cli_status_info),
    .to_text = kr_transmission_cli_status_info_to_text,
    .pack_json = kr_transmission_cli_status_info_pack_json,
    .unpack_json = kr_transmission_cli_status_info_unpack_json,
    .init = kr_transmission_cli_status_info_init,
    .valid = kr_transmission_cli_status_info_valid,
    .random = kr_transmission_cli_status_info_random,
    .patch_path = kr_transmission_cli_status_info_patch_path,
    .diff = kr_transmission_cli_status_info_diff
  },
  [KR_TRANSMISSION_CLI_PARAMS_INFO] = {
    .size = sizeof(kr_transmission_cli_params_info),
    .to_text = kr_transmission_cli_params_info_to_text,
    .pack_json = kr_transmission_cli_params_info_pack_json,
    .unpack_json = kr_transmission_cli_params_info_unpack_json,
    .init = kr_transmission_cli_params_info_init,
    .valid = kr_transmission_cli_params_info_valid,
    .random = kr_transmission_cli_params_info_random,
    .patch_path = kr_transmission_cli_params_info_patch_path,
    .diff = kr_transmission_cli_params_info_diff
  },
  [KR_TRANSMISSION_CLI_INFO] = {
    .size = sizeof(kr_transmission_cli_info),
    .to_text = kr_transmission_cli_info_to_text,
    .pack_json = kr_transmission_cli_info_pack_json,
    .unpack_json = kr_transmission_cli_info_unpack_json,
    .init = kr_transmission_cli_info_init,
    .valid = kr_transmission_cli_info_valid,
    .random = kr_transmission_cli_info_random,
    .patch_path = kr_transmission_cli_info_patch_path,
    .diff = kr_transmission_cli_info_diff
  },
  [KR_FAIL_INFO] = {
    .size = sizeof(kr_fail_info),
    .to_text = kr_fail_info_to_text,
    .pack_json = kr_fail_info_pack_json,
    .unpack_json = kr_fail_info_unpack_json,
    .init = kr_fail_info_init,
    .valid = kr_fail_info_valid,
    .random = kr_fail_info_random,
    .patch_path = kr_fail_info_patch_path,
    .diff = kr_fail_info_diff
  },
  [KR_TASK_STATUS] = {
    .size = sizeof(kr_task_status),
    .to_text = kr_task_status_to_text,
    .pack_json = kr_task_status_pack_json,
    .unpack_json = kr_task_status_unpack_json,
    .init = kr_task_status_init,
    .valid = kr_task_status_valid,
    .random = kr_task_status_random,
    .patch_path = kr_task_status_patch_path,
    .diff = kr_task_status_diff
  },
  [KR_TASK_PARAMS_INFO] = {
    .size = sizeof(kr_task_params_info),
    .to_text = kr_task_params_info_to_text,
    .pack_json = kr_task_params_info_pack_json,
    .unpack_json = kr_task_params_info_unpack_json,
    .init = kr_task_params_info_init,
    .valid = kr_task_params_info_valid,
    .random = kr_task_params_info_random,
    .patch_path = kr_task_params_info_patch_path,
    .diff = kr_task_params_info_diff
  },
  [KR_TASK_INFO] = {
    .size = sizeof(kr_task_info),
    .to_text = kr_task_info_to_text,
    .pack_json = kr_task_info_pack_json,
    .unpack_json = kr_task_info_unpack_json,
    .init = kr_task_info_init,
    .valid = kr_task_info_valid,
    .random = kr_task_info_random,
    .patch_path = kr_task_info_patch_path,
    .diff = kr_task_info_diff
  },
  [KR_VALGRIND_STRESSOR_STATUS_INFO] = {
    .size = sizeof(kr_valgrind_stressor_status_info),
    .to_text = kr_valgrind_stressor_status_info_to_text,
    .pack_json = kr_valgrind_stressor_status_info_pack_json,
    .unpack_json = kr_valgrind_stressor_status_info_unpack_json,
    .init = kr_valgrind_stressor_status_info_init,
    .valid = kr_valgrind_stressor_status_info_valid,
    .random = kr_valgrind_stressor_status_info_random,
    .patch_path = kr_valgrind_stressor_status_info_patch_path,
    .diff = kr_valgrind_stressor_status_info_diff
  },
  [KR_VALGRIND_STRESSOR_PARAMS_INFO] = {
    .size = sizeof(kr_valgrind_stressor_params_info),
    .to_text = kr_valgrind_stressor_params_info_to_text,
    .pack_json = kr_valgrind_stressor_params_info_pack_json,
    .unpack_json = kr_valgrind_stressor_params_info_unpack_json,
    .init = kr_valgrind_stressor_params_info_init,
    .valid = kr_valgrind_stressor_params_info_valid,
    .random = kr_valgrind_stressor_params_info_random,
    .patch_path = kr_valgrind_stressor_params_info_patch_path,
    .diff = kr_valgrind_stressor_params_info_diff
  },
  [KR_VALGRIND_STRESSOR_INFO] = {
    .size = sizeof(kr_valgrind_stressor_info),
    .to_text = kr_valgrind_stressor_info_to_text,
    .pack_json = kr_valgrind_stressor_info_pack_json,
    .unpack_json = kr_valgrind_stressor_info_unpack_json,
    .init = kr_valgrind_stressor_info_init,
    .valid = kr_valgrind_stressor_info_valid,
    .random = kr_valgrind_stressor_info_random,
    .patch_path = kr_valgrind_stressor_info_patch_path,
    .diff = kr_valgrind_stressor_info_diff
  },
  [KR_XMMS_PARAMS_INFO] = {
    .size = sizeof(kr_xmms_params_info),
    .to_text = kr_xmms_params_info_to_text,
    .pack_json = kr_xmms_params_info_pack_json,
    .unpack_json = kr_xmms_params_info_unpack_json,
    .init = kr_xmms_params_info_init,
    .valid = kr_xmms_params_info_valid,
    .random = kr_xmms_params_info_random,
    .patch_path = kr_xmms_params_info_patch_path,
    .diff = kr_xmms_params_info_diff
  },
  [KR_XMMS_STATUS_INFO] = {
    .size = sizeof(kr_xmms_status_info),
    .to_text = kr_xmms_status_info_to_text,
    .pack_json = kr_xmms_status_info_pack_json,
    .unpack_json = kr_xmms_status_info_unpack_json,
    .init = kr_xmms_status_info_init,
    .valid = kr_xmms_status_info_valid,
    .random = kr_xmms_status_info_random,
    .patch_path = kr_xmms_status_info_patch_path,
    .diff = kr_xmms_status_info_diff
  },
  [KR_XMMS_INFO] = {
    .size = sizeof(kr_xmms_info),
    .to_text = kr_xmms_info_to_text,
    .pack_json = kr_xmms_info_pack_json,
    .unpack_json = kr_xmms_info_unpack_json,
    .init = kr_xmms_info_init,
    .valid = kr_xmms_info_valid,
    .random = kr_xmms_info_random,
    .patch_path = kr_xmms_info_patch_path,
    .diff = kr_xmms_info_diff
  },
  [KR_YDL_FORMAT_INFO] = {
    .size = sizeof(kr_ydl_format_info),
    .to_text = kr_ydl_format_info_to_text,
    .pack_json = kr_ydl_format_info_pack_json,
    .unpack_json = kr_ydl_format_info_unpack_json,
    .init = kr_ydl_format_info_init,
    .valid = kr_ydl_format_info_valid,
    .random = kr_ydl_format_info_random,
    .patch_path = kr_ydl_format_info_patch_path,
    .diff = kr_ydl_format_info_diff
  },
  [KR_YDL_VIDEO_INFO] = {
    .size = sizeof(kr_ydl_video_info),
    .to_text = kr_ydl_video_info_to_text,
    .pack_json = kr_ydl_video_info_pack_json,
    .unpack_json = kr_ydl_video_info_unpack_json,
    .init = kr_ydl_video_info_init,
    .valid = kr_ydl_video_info_valid,
    .random = kr_ydl_video_info_random,
    .patch_path = kr_ydl_video_info_patch_path,
    .diff = kr_ydl_video_info_diff
  },
  [KR_YDL_PARAMS_INFO] = {
    .size = sizeof(kr_ydl_params_info),
    .to_text = kr_ydl_params_info_to_text,
    .pack_json = kr_ydl_params_info_pack_json,
    .unpack_json = kr_ydl_params_info_unpack_json,
    .init = kr_ydl_params_info_init,
    .valid = kr_ydl_params_info_valid,
    .random = kr_ydl_params_info_random,
    .patch_path = kr_ydl_params_info_patch_path,
    .diff = kr_ydl_params_info_diff
  },
  [KR_YDL_STATUS_INFO] = {
    .size = sizeof(kr_ydl_status_info),
    .to_text = kr_ydl_status_info_to_text,
    .pack_json = kr_ydl_status_info_pack_json,
    .unpack_json = kr_ydl_status_info_unpack_json,
    .init = kr_ydl_status_info_init,
    .valid = kr_ydl_status_info_valid,
    .random = kr_ydl_status_info_random,
    .patch_path = kr_ydl_status_info_patch_path,
    .diff = kr_ydl_status_info_diff
  },
  [KR_YDL_INFO] = {
    .size = sizeof(kr_ydl_info),
    .to_text = kr_ydl_info_to_text,
    .pack_json = kr_ydl_info_pack_json,
    .unpack_json = kr_ydl_info_unpack_json,
    .init = kr_ydl_info_init,
    .valid = kr_ydl_info_valid,
    .random = kr_ydl_info_random,
    .patch_path = kr_ydl_info_patch_path,
    .diff = kr_ydl_info_diff
  },
  [KR_TIMESTAMP_INFO] = {
    .size = sizeof(kr_timestamp_info),
    .to_text = kr_timestamp_info_to_text,
    .pack_json = kr_timestamp_info_pack_json,
    .unpack_json = kr_timestamp_info_unpack_json,
    .pack_ebml = kr_timestamp_info_pack_ebml,
    .unpack_ebml = kr_timestamp_info_unpack_ebml,
    .init = kr_timestamp_info_init,
    .valid = kr_timestamp_info_valid,
    .random = kr_timestamp_info_random,
    .patch_path = kr_timestamp_info_patch_path,
    .diff = kr_timestamp_info_diff
  },
  [KR_MEDIA_SOURCE_INFO] = {
    .size = sizeof(kr_media_source_info),
    .to_text = kr_media_source_info_to_text,
    .pack_json = kr_media_source_info_pack_json,
    .unpack_json = kr_media_source_info_unpack_json,
    .pack_ebml = kr_media_source_info_pack_ebml,
    .unpack_ebml = kr_media_source_info_unpack_ebml,
    .init = kr_media_source_info_init,
    .valid = kr_media_source_info_valid,
    .random = kr_media_source_info_random,
    .patch_path = kr_media_source_info_patch_path,
    .diff = kr_media_source_info_diff
  },
  [KR_MEDIA_PACKET] = {
    .size = sizeof(kr_media_packet),
    .to_text = kr_media_packet_to_text,
    .pack_json = kr_media_packet_pack_json,
    .unpack_json = kr_media_packet_unpack_json,
    .pack_ebml = kr_media_packet_pack_ebml,
    .unpack_ebml = kr_media_packet_unpack_ebml,
    .init = kr_media_packet_init,
    .valid = kr_media_packet_valid,
    .random = kr_media_packet_random,
    .patch_path = kr_media_packet_patch_path,
    .diff = kr_media_packet_diff
  },
  [KR_TRACK_NAME] = {
    .size = sizeof(kr_track_name),
    .to_text = kr_track_name_to_text,
    .pack_json = kr_track_name_pack_json,
    .unpack_json = kr_track_name_unpack_json,
    .pack_ebml = kr_track_name_pack_ebml,
    .unpack_ebml = kr_track_name_unpack_ebml,
    .init = kr_track_name_init,
    .valid = kr_track_name_valid,
    .random = kr_track_name_random,
    .patch_path = kr_track_name_patch_path,
    .diff = kr_track_name_diff
  },
  [KR_MUX_NAME] = {
    .size = sizeof(kr_mux_name),
    .to_text = kr_mux_name_to_text,
    .pack_json = kr_mux_name_pack_json,
    .unpack_json = kr_mux_name_unpack_json,
    .pack_ebml = kr_mux_name_pack_ebml,
    .unpack_ebml = kr_mux_name_unpack_ebml,
    .init = kr_mux_name_init,
    .valid = kr_mux_name_valid,
    .random = kr_mux_name_random,
    .patch_path = kr_mux_name_patch_path,
    .diff = kr_mux_name_diff
  },
  [KR_DEMUX_INFO] = {
    .size = sizeof(kr_demux_info),
    .to_text = kr_demux_info_to_text,
    .pack_json = kr_demux_info_pack_json,
    .unpack_json = kr_demux_info_unpack_json,
    .pack_ebml = kr_demux_info_pack_ebml,
    .unpack_ebml = kr_demux_info_unpack_ebml,
    .init = kr_demux_info_init,
    .valid = kr_demux_info_valid,
    .random = kr_demux_info_random,
    .patch_path = kr_demux_info_patch_path,
    .diff = kr_demux_info_diff
  },
  [KR_MUX_INFO] = {
    .size = sizeof(kr_mux_info),
    .to_text = kr_mux_info_to_text,
    .pack_json = kr_mux_info_pack_json,
    .unpack_json = kr_mux_info_unpack_json,
    .pack_ebml = kr_mux_info_pack_ebml,
    .unpack_ebml = kr_mux_info_unpack_ebml,
    .init = kr_mux_info_init,
    .valid = kr_mux_info_valid,
    .random = kr_mux_info_random,
    .patch_path = kr_mux_info_patch_path,
    .diff = kr_mux_info_diff
  },
  [KR_VIDEO_ENCODE_INFO] = {
    .size = sizeof(kr_video_encode_info),
    .to_text = kr_video_encode_info_to_text,
    .pack_json = kr_video_encode_info_pack_json,
    .unpack_json = kr_video_encode_info_unpack_json,
    .pack_ebml = kr_video_encode_info_pack_ebml,
    .unpack_ebml = kr_video_encode_info_unpack_ebml,
    .init = kr_video_encode_info_init,
    .valid = kr_video_encode_info_valid,
    .random = kr_video_encode_info_random,
    .patch_path = kr_video_encode_info_patch_path,
    .diff = kr_video_encode_info_diff
  },
  [KR_AUDIO_ENCODE_INFO] = {
    .size = sizeof(kr_audio_encode_info),
    .to_text = kr_audio_encode_info_to_text,
    .pack_json = kr_audio_encode_info_pack_json,
    .unpack_json = kr_audio_encode_info_unpack_json,
    .pack_ebml = kr_audio_encode_info_pack_ebml,
    .unpack_ebml = kr_audio_encode_info_unpack_ebml,
    .init = kr_audio_encode_info_init,
    .valid = kr_audio_encode_info_valid,
    .random = kr_audio_encode_info_random,
    .patch_path = kr_audio_encode_info_patch_path,
    .diff = kr_audio_encode_info_diff
  },
  [KR_ENCODE_INFO] = {
    .size = sizeof(kr_encode_info),
    .to_text = kr_encode_info_to_text,
    .pack_json = kr_encode_info_pack_json,
    .unpack_json = kr_encode_info_unpack_json,
    .pack_ebml = kr_encode_info_pack_ebml,
    .unpack_ebml = kr_encode_info_unpack_ebml,
    .init = kr_encode_info_init,
    .valid = kr_encode_info_valid,
    .random = kr_encode_info_random,
    .patch_path = kr_encode_info_patch_path,
    .diff = kr_encode_info_diff
  },
  [KR_RECORD_INFO] = {
    .size = sizeof(kr_record_info),
    .to_text = kr_record_info_to_text,
    .pack_json = kr_record_info_pack_json,
    .unpack_json = kr_record_info_unpack_json,
    .pack_ebml = kr_record_info_pack_ebml,
    .unpack_ebml = kr_record_info_unpack_ebml,
    .init = kr_record_info_init,
    .valid = kr_record_info_valid,
    .random = kr_record_info_random,
    .patch_path = kr_record_info_patch_path,
    .diff = kr_record_info_diff
  },
  [KR_TRANSMISSION_INFO] = {
    .size = sizeof(kr_transmission_info),
    .to_text = kr_transmission_info_to_text,
    .pack_json = kr_transmission_info_pack_json,
    .unpack_json = kr_transmission_info_unpack_json,
    .pack_ebml = kr_transmission_info_pack_ebml,
    .unpack_ebml = kr_transmission_info_unpack_ebml,
    .init = kr_transmission_info_init,
    .valid = kr_transmission_info_valid,
    .random = kr_transmission_info_random,
    .patch_path = kr_transmission_info_patch_path,
    .diff = kr_transmission_info_diff
  },
  [KR_OUTPUT_INFO] = {
    .size = sizeof(kr_output_info),
    .to_text = kr_output_info_to_text,
    .pack_json = kr_output_info_pack_json,
    .unpack_json = kr_output_info_unpack_json,
    .pack_ebml = kr_output_info_pack_ebml,
    .unpack_ebml = kr_output_info_unpack_ebml,
    .init = kr_output_info_init,
    .valid = kr_output_info_valid,
    .random = kr_output_info_random,
    .patch_path = kr_output_info_patch_path,
    .diff = kr_output_info_diff
  },
  [KR_CAPTURE_AUDIO_INFO] = {
    .size = sizeof(kr_capture_audio_info),
    .to_text = kr_capture_audio_info_to_text,
    .pack_json = kr_capture_audio_info_pack_json,
    .unpack_json = kr_capture_audio_info_unpack_json,
    .pack_ebml = kr_capture_audio_info_pack_ebml,
    .unpack_ebml = kr_capture_audio_info_unpack_ebml,
    .init = kr_capture_audio_info_init,
    .valid = kr_capture_audio_info_valid,
    .random = kr_capture_audio_info_random,
    .patch_path = kr_capture_audio_info_patch_path,
    .diff = kr_capture_audio_info_diff
  },
  [KR_CAPTURE_VIDEO_INFO] = {
    .size = sizeof(kr_capture_video_info),
    .to_text = kr_capture_video_info_to_text,
    .pack_json = kr_capture_video_info_pack_json,
    .unpack_json = kr_capture_video_info_unpack_json,
    .pack_ebml = kr_capture_video_info_pack_ebml,
    .unpack_ebml = kr_capture_video_info_unpack_ebml,
    .init = kr_capture_video_info_init,
    .valid = kr_capture_video_info_valid,
    .random = kr_capture_video_info_random,
    .patch_path = kr_capture_video_info_patch_path,
    .diff = kr_capture_video_info_diff
  },
  [KR_CAPTURE_IMAGE_INFO] = {
    .size = sizeof(kr_capture_image_info),
    .to_text = kr_capture_image_info_to_text,
    .pack_json = kr_capture_image_info_pack_json,
    .unpack_json = kr_capture_image_info_unpack_json,
    .pack_ebml = kr_capture_image_info_pack_ebml,
    .unpack_ebml = kr_capture_image_info_unpack_ebml,
    .init = kr_capture_image_info_init,
    .valid = kr_capture_image_info_valid,
    .random = kr_capture_image_info_random,
    .patch_path = kr_capture_image_info_patch_path,
    .diff = kr_capture_image_info_diff
  },
  [KR_CAPTURE_INFO] = {
    .size = sizeof(kr_capture_info),
    .to_text = kr_capture_info_to_text,
    .pack_json = kr_capture_info_pack_json,
    .unpack_json = kr_capture_info_unpack_json,
    .pack_ebml = kr_capture_info_pack_ebml,
    .unpack_ebml = kr_capture_info_unpack_ebml,
    .init = kr_capture_info_init,
    .valid = kr_capture_info_valid,
    .random = kr_capture_info_random,
    .patch_path = kr_capture_info_patch_path,
    .diff = kr_capture_info_diff
  },
  [KR_SOURCE_FILE_INFO] = {
    .size = sizeof(kr_source_file_info),
    .to_text = kr_source_file_info_to_text,
    .pack_json = kr_source_file_info_pack_json,
    .unpack_json = kr_source_file_info_unpack_json,
    .pack_ebml = kr_source_file_info_pack_ebml,
    .unpack_ebml = kr_source_file_info_unpack_ebml,
    .init = kr_source_file_info_init,
    .valid = kr_source_file_info_valid,
    .random = kr_source_file_info_random,
    .patch_path = kr_source_file_info_patch_path,
    .diff = kr_source_file_info_diff
  },
  [KR_SOURCE_REMOTE_FILE_INFO] = {
    .size = sizeof(kr_source_remote_file_info),
    .to_text = kr_source_remote_file_info_to_text,
    .pack_json = kr_source_remote_file_info_pack_json,
    .unpack_json = kr_source_remote_file_info_unpack_json,
    .pack_ebml = kr_source_remote_file_info_pack_ebml,
    .unpack_ebml = kr_source_remote_file_info_unpack_ebml,
    .init = kr_source_remote_file_info_init,
    .valid = kr_source_remote_file_info_valid,
    .random = kr_source_remote_file_info_random,
    .patch_path = kr_source_remote_file_info_patch_path,
    .diff = kr_source_remote_file_info_diff
  },
  [KR_SOURCE_STREAM_INFO] = {
    .size = sizeof(kr_source_stream_info),
    .to_text = kr_source_stream_info_to_text,
    .pack_json = kr_source_stream_info_pack_json,
    .unpack_json = kr_source_stream_info_unpack_json,
    .pack_ebml = kr_source_stream_info_pack_ebml,
    .unpack_ebml = kr_source_stream_info_unpack_ebml,
    .init = kr_source_stream_info_init,
    .valid = kr_source_stream_info_valid,
    .random = kr_source_stream_info_random,
    .patch_path = kr_source_stream_info_patch_path,
    .diff = kr_source_stream_info_diff
  },
  [KR_STUDIO_SOURCE_INFO] = {
    .size = sizeof(kr_studio_source_info),
    .to_text = kr_studio_source_info_to_text,
    .pack_json = kr_studio_source_info_pack_json,
    .unpack_json = kr_studio_source_info_unpack_json,
    .pack_ebml = kr_studio_source_info_pack_ebml,
    .unpack_ebml = kr_studio_source_info_unpack_ebml,
    .init = kr_studio_source_info_init,
    .valid = kr_studio_source_info_valid,
    .random = kr_studio_source_info_random,
    .patch_path = kr_studio_source_info_patch_path,
    .diff = kr_studio_source_info_diff
  },
  [KR_DECODE_INFO] = {
    .size = sizeof(kr_decode_info),
    .to_text = kr_decode_info_to_text,
    .pack_json = kr_decode_info_pack_json,
    .unpack_json = kr_decode_info_unpack_json,
    .pack_ebml = kr_decode_info_pack_ebml,
    .unpack_ebml = kr_decode_info_unpack_ebml,
    .init = kr_decode_info_init,
    .valid = kr_decode_info_valid,
    .random = kr_decode_info_random,
    .patch_path = kr_decode_info_patch_path,
    .diff = kr_decode_info_diff
  },
  [KR_PLAY_INFO] = {
    .size = sizeof(kr_play_info),
    .to_text = kr_play_info_to_text,
    .pack_json = kr_play_info_pack_json,
    .unpack_json = kr_play_info_unpack_json,
    .pack_ebml = kr_play_info_pack_ebml,
    .unpack_ebml = kr_play_info_unpack_ebml,
    .init = kr_play_info_init,
    .valid = kr_play_info_valid,
    .random = kr_play_info_random,
    .patch_path = kr_play_info_patch_path,
    .diff = kr_play_info_diff
  },
  [KR_RENDER_IMAGE_INFO] = {
    .size = sizeof(kr_render_image_info),
    .to_text = kr_render_image_info_to_text,
    .pack_json = kr_render_image_info_pack_json,
    .unpack_json = kr_render_image_info_unpack_json,
    .pack_ebml = kr_render_image_info_pack_ebml,
    .unpack_ebml = kr_render_image_info_unpack_ebml,
    .init = kr_render_image_info_init,
    .valid = kr_render_image_info_valid,
    .random = kr_render_image_info_random,
    .patch_path = kr_render_image_info_patch_path,
    .diff = kr_render_image_info_diff
  },
  [KR_RENDER_VIDEO_INFO] = {
    .size = sizeof(kr_render_video_info),
    .to_text = kr_render_video_info_to_text,
    .pack_json = kr_render_video_info_pack_json,
    .unpack_json = kr_render_video_info_unpack_json,
    .pack_ebml = kr_render_video_info_pack_ebml,
    .unpack_ebml = kr_render_video_info_unpack_ebml,
    .init = kr_render_video_info_init,
    .valid = kr_render_video_info_valid,
    .random = kr_render_video_info_random,
    .patch_path = kr_render_video_info_patch_path,
    .diff = kr_render_video_info_diff
  },
  [KR_RENDER_AUDIO_INFO] = {
    .size = sizeof(kr_render_audio_info),
    .to_text = kr_render_audio_info_to_text,
    .pack_json = kr_render_audio_info_pack_json,
    .unpack_json = kr_render_audio_info_unpack_json,
    .pack_ebml = kr_render_audio_info_pack_ebml,
    .unpack_ebml = kr_render_audio_info_unpack_ebml,
    .init = kr_render_audio_info_init,
    .valid = kr_render_audio_info_valid,
    .random = kr_render_audio_info_random,
    .patch_path = kr_render_audio_info_patch_path,
    .diff = kr_render_audio_info_diff
  },
  [KR_RENDER_INFO] = {
    .size = sizeof(kr_render_info),
    .to_text = kr_render_info_to_text,
    .pack_json = kr_render_info_pack_json,
    .unpack_json = kr_render_info_unpack_json,
    .pack_ebml = kr_render_info_pack_ebml,
    .unpack_ebml = kr_render_info_unpack_ebml,
    .init = kr_render_info_init,
    .valid = kr_render_info_valid,
    .random = kr_render_info_random,
    .patch_path = kr_render_info_patch_path,
    .diff = kr_render_info_diff
  },
  [KR_GENERATE_AUDIO_INFO] = {
    .size = sizeof(kr_generate_audio_info),
    .to_text = kr_generate_audio_info_to_text,
    .pack_json = kr_generate_audio_info_pack_json,
    .unpack_json = kr_generate_audio_info_unpack_json,
    .pack_ebml = kr_generate_audio_info_pack_ebml,
    .unpack_ebml = kr_generate_audio_info_unpack_ebml,
    .init = kr_generate_audio_info_init,
    .valid = kr_generate_audio_info_valid,
    .random = kr_generate_audio_info_random,
    .patch_path = kr_generate_audio_info_patch_path,
    .diff = kr_generate_audio_info_diff
  },
  [KR_GENERATE_COLOR_INFO] = {
    .size = sizeof(kr_generate_color_info),
    .to_text = kr_generate_color_info_to_text,
    .pack_json = kr_generate_color_info_pack_json,
    .unpack_json = kr_generate_color_info_unpack_json,
    .pack_ebml = kr_generate_color_info_pack_ebml,
    .unpack_ebml = kr_generate_color_info_unpack_ebml,
    .init = kr_generate_color_info_init,
    .valid = kr_generate_color_info_valid,
    .random = kr_generate_color_info_random,
    .patch_path = kr_generate_color_info_patch_path,
    .diff = kr_generate_color_info_diff
  },
  [KR_GENERATE_VIDEO_INFO] = {
    .size = sizeof(kr_generate_video_info),
    .to_text = kr_generate_video_info_to_text,
    .pack_json = kr_generate_video_info_pack_json,
    .unpack_json = kr_generate_video_info_unpack_json,
    .pack_ebml = kr_generate_video_info_pack_ebml,
    .unpack_ebml = kr_generate_video_info_unpack_ebml,
    .init = kr_generate_video_info_init,
    .valid = kr_generate_video_info_valid,
    .random = kr_generate_video_info_random,
    .patch_path = kr_generate_video_info_patch_path,
    .diff = kr_generate_video_info_diff
  },
  [KR_GENERATE_INFO] = {
    .size = sizeof(kr_generate_info),
    .to_text = kr_generate_info_to_text,
    .pack_json = kr_generate_info_pack_json,
    .unpack_json = kr_generate_info_unpack_json,
    .pack_ebml = kr_generate_info_pack_ebml,
    .unpack_ebml = kr_generate_info_unpack_ebml,
    .init = kr_generate_info_init,
    .valid = kr_generate_info_valid,
    .random = kr_generate_info_random,
    .patch_path = kr_generate_info_patch_path,
    .diff = kr_generate_info_diff
  },
  [KR_STUDIO_PATH_INFO] = {
    .size = sizeof(kr_studio_path_info),
    .to_text = kr_studio_path_info_to_text,
    .pack_json = kr_studio_path_info_pack_json,
    .unpack_json = kr_studio_path_info_unpack_json,
    .pack_ebml = kr_studio_path_info_pack_ebml,
    .unpack_ebml = kr_studio_path_info_unpack_ebml,
    .init = kr_studio_path_info_init,
    .valid = kr_studio_path_info_valid,
    .random = kr_studio_path_info_random,
    .patch_path = kr_studio_path_info_patch_path,
    .diff = kr_studio_path_info_diff
  },
  [KR_TRACK_INFO] = {
    .size = sizeof(kr_track_info),
    .to_text = kr_track_info_to_text,
    .pack_json = kr_track_info_pack_json,
    .unpack_json = kr_track_info_unpack_json,
    .pack_ebml = kr_track_info_pack_ebml,
    .unpack_ebml = kr_track_info_unpack_ebml,
    .init = kr_track_info_init,
    .valid = kr_track_info_valid,
    .random = kr_track_info_random,
    .patch_path = kr_track_info_patch_path,
    .diff = kr_track_info_diff
  },
  [KR_TRACK_BLOCK_INFO] = {
    .size = sizeof(kr_track_block_info),
    .to_text = kr_track_block_info_to_text,
    .pack_json = kr_track_block_info_pack_json,
    .unpack_json = kr_track_block_info_unpack_json,
    .pack_ebml = kr_track_block_info_pack_ebml,
    .unpack_ebml = kr_track_block_info_unpack_ebml,
    .init = kr_track_block_info_init,
    .valid = kr_track_block_info_valid,
    .random = kr_track_block_info_random,
    .patch_path = kr_track_block_info_patch_path,
    .diff = kr_track_block_info_diff
  },
  [KR_HLS_VARIANT] = {
    .size = sizeof(kr_hls_variant),
    .to_text = kr_hls_variant_to_text,
    .pack_json = kr_hls_variant_pack_json,
    .unpack_json = kr_hls_variant_unpack_json,
    .pack_ebml = kr_hls_variant_pack_ebml,
    .unpack_ebml = kr_hls_variant_unpack_ebml,
    .init = kr_hls_variant_init,
    .valid = kr_hls_variant_valid,
    .random = kr_hls_variant_random,
    .patch_path = kr_hls_variant_patch_path,
    .diff = kr_hls_variant_diff
  },
  [KR_HLS_RENDITION] = {
    .size = sizeof(kr_hls_rendition),
    .to_text = kr_hls_rendition_to_text,
    .pack_json = kr_hls_rendition_pack_json,
    .unpack_json = kr_hls_rendition_unpack_json,
    .pack_ebml = kr_hls_rendition_pack_ebml,
    .unpack_ebml = kr_hls_rendition_unpack_ebml,
    .init = kr_hls_rendition_init,
    .valid = kr_hls_rendition_valid,
    .random = kr_hls_rendition_random,
    .patch_path = kr_hls_rendition_patch_path,
    .diff = kr_hls_rendition_diff
  },
  [KR_HLS_MASTER_PLAYLIST] = {
    .size = sizeof(kr_hls_master_playlist),
    .to_text = kr_hls_master_playlist_to_text,
    .pack_json = kr_hls_master_playlist_pack_json,
    .unpack_json = kr_hls_master_playlist_unpack_json,
    .pack_ebml = kr_hls_master_playlist_pack_ebml,
    .unpack_ebml = kr_hls_master_playlist_unpack_ebml,
    .init = kr_hls_master_playlist_init,
    .valid = kr_hls_master_playlist_valid,
    .random = kr_hls_master_playlist_random,
    .patch_path = kr_hls_master_playlist_patch_path,
    .diff = kr_hls_master_playlist_diff
  },
  [KR_HLS_MEDIA_SEGMENT] = {
    .size = sizeof(kr_hls_media_segment),
    .to_text = kr_hls_media_segment_to_text,
    .pack_json = kr_hls_media_segment_pack_json,
    .unpack_json = kr_hls_media_segment_unpack_json,
    .pack_ebml = kr_hls_media_segment_pack_ebml,
    .unpack_ebml = kr_hls_media_segment_unpack_ebml,
    .init = kr_hls_media_segment_init,
    .valid = kr_hls_media_segment_valid,
    .random = kr_hls_media_segment_random,
    .patch_path = kr_hls_media_segment_patch_path,
    .diff = kr_hls_media_segment_diff
  },
  [KR_HLS_MEDIA_PLAYLIST] = {
    .size = sizeof(kr_hls_media_playlist),
    .to_text = kr_hls_media_playlist_to_text,
    .pack_json = kr_hls_media_playlist_pack_json,
    .unpack_json = kr_hls_media_playlist_unpack_json,
    .pack_ebml = kr_hls_media_playlist_pack_ebml,
    .unpack_ebml = kr_hls_media_playlist_unpack_ebml,
    .init = kr_hls_media_playlist_init,
    .valid = kr_hls_media_playlist_valid,
    .random = kr_hls_media_playlist_random,
    .patch_path = kr_hls_media_playlist_patch_path,
    .diff = kr_hls_media_playlist_diff
  },
  [KR_HLS_PLAYLIST] = {
    .size = sizeof(kr_hls_playlist),
    .to_text = kr_hls_playlist_to_text,
    .pack_json = kr_hls_playlist_pack_json,
    .unpack_json = kr_hls_playlist_unpack_json,
    .pack_ebml = kr_hls_playlist_pack_ebml,
    .unpack_ebml = kr_hls_playlist_unpack_ebml,
    .init = kr_hls_playlist_init,
    .valid = kr_hls_playlist_valid,
    .random = kr_hls_playlist_random,
    .patch_path = kr_hls_playlist_patch_path,
    .diff = kr_hls_playlist_diff
  },
  [KR_HLS_TUNER_INFO] = {
    .size = sizeof(kr_hls_tuner_info),
    .to_text = kr_hls_tuner_info_to_text,
    .pack_json = kr_hls_tuner_info_pack_json,
    .unpack_json = kr_hls_tuner_info_unpack_json,
    .pack_ebml = kr_hls_tuner_info_pack_ebml,
    .unpack_ebml = kr_hls_tuner_info_unpack_ebml,
    .init = kr_hls_tuner_info_init,
    .valid = kr_hls_tuner_info_valid,
    .random = kr_hls_tuner_info_random,
    .patch_path = kr_hls_tuner_info_patch_path,
    .diff = kr_hls_tuner_info_diff
  },
  [KR_FPS_INFO] = {
    .size = sizeof(kr_fps_info),
    .to_text = kr_fps_info_to_text,
    .pack_json = kr_fps_info_pack_json,
    .unpack_json = kr_fps_info_unpack_json,
    .pack_ebml = kr_fps_info_pack_ebml,
    .unpack_ebml = kr_fps_info_unpack_ebml,
    .init = kr_fps_info_init,
    .valid = kr_fps_info_valid,
    .random = kr_fps_info_random,
    .patch_path = kr_fps_info_patch_path,
    .diff = kr_fps_info_diff
  },
  [KR_RADIO_INFO] = {
    .size = sizeof(kr_radio_info),
    .to_text = kr_radio_info_to_text,
    .pack_json = kr_radio_info_pack_json,
    .unpack_json = kr_radio_info_unpack_json,
    .pack_ebml = kr_radio_info_pack_ebml,
    .unpack_ebml = kr_radio_info_unpack_ebml,
    .init = kr_radio_info_init,
    .valid = kr_radio_info_valid,
    .random = kr_radio_info_random,
    .patch_path = kr_radio_info_patch_path,
    .diff = kr_radio_info_diff
  },
  [KR_WEB_SERVER_INFO] = {
    .size = sizeof(kr_web_server_info),
    .to_text = kr_web_server_info_to_text,
    .pack_json = kr_web_server_info_pack_json,
    .unpack_json = kr_web_server_info_unpack_json,
    .pack_ebml = kr_web_server_info_pack_ebml,
    .unpack_ebml = kr_web_server_info_unpack_ebml,
    .init = kr_web_server_info_init,
    .valid = kr_web_server_info_valid,
    .random = kr_web_server_info_random,
    .patch_path = kr_web_server_info_patch_path,
    .diff = kr_web_server_info_diff
  },
  [KR_WEB_SERVER_SETUP_INFO] = {
    .size = sizeof(kr_web_server_setup_info),
    .to_text = kr_web_server_setup_info_to_text,
    .pack_json = kr_web_server_setup_info_pack_json,
    .unpack_json = kr_web_server_setup_info_unpack_json,
    .pack_ebml = kr_web_server_setup_info_pack_ebml,
    .unpack_ebml = kr_web_server_setup_info_unpack_ebml,
    .init = kr_web_server_setup_info_init,
    .valid = kr_web_server_setup_info_valid,
    .random = kr_web_server_setup_info_random,
    .patch_path = kr_web_server_setup_info_patch_path,
    .diff = kr_web_server_setup_info_diff
  },
  [KR_WEB_REMOTE_INFO] = {
    .size = sizeof(kr_web_remote_info),
    .to_text = kr_web_remote_info_to_text,
    .pack_json = kr_web_remote_info_pack_json,
    .unpack_json = kr_web_remote_info_unpack_json,
    .pack_ebml = kr_web_remote_info_pack_ebml,
    .unpack_ebml = kr_web_remote_info_unpack_ebml,
    .init = kr_web_remote_info_init,
    .valid = kr_web_remote_info_valid,
    .random = kr_web_remote_info_random,
    .patch_path = kr_web_remote_info_patch_path,
    .diff = kr_web_remote_info_diff
  },
  [KR_WEB_SOCKET_IO_STATE] = {
    .size = sizeof(kr_web_socket_io_state),
    .to_text = kr_web_socket_io_state_to_text,
    .pack_json = kr_web_socket_io_state_pack_json,
    .unpack_json = kr_web_socket_io_state_unpack_json,
    .pack_ebml = kr_web_socket_io_state_pack_ebml,
    .unpack_ebml = kr_web_socket_io_state_unpack_ebml,
    .init = kr_web_socket_io_state_init,
    .valid = kr_web_socket_io_state_valid,
    .random = kr_web_socket_io_state_random,
    .patch_path = kr_web_socket_io_state_patch_path,
    .diff = kr_web_socket_io_state_diff
  },
  [KR_WEB_SOCKET_STATE] = {
    .size = sizeof(kr_web_socket_state),
    .to_text = kr_web_socket_state_to_text,
    .pack_json = kr_web_socket_state_pack_json,
    .unpack_json = kr_web_socket_state_unpack_json,
    .pack_ebml = kr_web_socket_state_pack_ebml,
    .unpack_ebml = kr_web_socket_state_unpack_ebml,
    .init = kr_web_socket_state_init,
    .valid = kr_web_socket_state_valid,
    .random = kr_web_socket_state_random,
    .patch_path = kr_web_socket_state_patch_path,
    .diff = kr_web_socket_state_diff
  },
  [KR_WEB_HTTP_IO_STATE] = {
    .size = sizeof(kr_web_http_io_state),
    .to_text = kr_web_http_io_state_to_text,
    .pack_json = kr_web_http_io_state_pack_json,
    .unpack_json = kr_web_http_io_state_unpack_json,
    .pack_ebml = kr_web_http_io_state_pack_ebml,
    .unpack_ebml = kr_web_http_io_state_unpack_ebml,
    .init = kr_web_http_io_state_init,
    .valid = kr_web_http_io_state_valid,
    .random = kr_web_http_io_state_random,
    .patch_path = kr_web_http_io_state_patch_path,
    .diff = kr_web_http_io_state_diff
  },
  [KR_WEB_HTTP_STATE] = {
    .size = sizeof(kr_web_http_state),
    .to_text = kr_web_http_state_to_text,
    .pack_json = kr_web_http_state_pack_json,
    .unpack_json = kr_web_http_state_unpack_json,
    .pack_ebml = kr_web_http_state_pack_ebml,
    .unpack_ebml = kr_web_http_state_unpack_ebml,
    .init = kr_web_http_state_init,
    .valid = kr_web_http_state_valid,
    .random = kr_web_http_state_random,
    .patch_path = kr_web_http_state_patch_path,
    .diff = kr_web_http_state_diff
  },
  [KR_WEB_CLIENT_INFO] = {
    .size = sizeof(kr_web_client_info),
    .to_text = kr_web_client_info_to_text,
    .pack_json = kr_web_client_info_pack_json,
    .unpack_json = kr_web_client_info_unpack_json,
    .pack_ebml = kr_web_client_info_pack_ebml,
    .unpack_ebml = kr_web_client_info_unpack_ebml,
    .init = kr_web_client_info_init,
    .valid = kr_web_client_info_valid,
    .random = kr_web_client_info_random,
    .patch_path = kr_web_client_info_patch_path,
    .diff = kr_web_client_info_diff
  },
  [KR_WEB_CLIENT_SETUP_INFO] = {
    .size = sizeof(kr_web_client_setup_info),
    .to_text = kr_web_client_setup_info_to_text,
    .pack_json = kr_web_client_setup_info_pack_json,
    .unpack_json = kr_web_client_setup_info_unpack_json,
    .pack_ebml = kr_web_client_setup_info_pack_ebml,
    .unpack_ebml = kr_web_client_setup_info_unpack_ebml,
    .init = kr_web_client_setup_info_init,
    .valid = kr_web_client_setup_info_valid,
    .random = kr_web_client_setup_info_random,
    .patch_path = kr_web_client_setup_info_patch_path,
    .diff = kr_web_client_setup_info_diff
  },
  [KR_WEB_PATH_INFO] = {
    .size = sizeof(kr_web_path_info),
    .to_text = kr_web_path_info_to_text,
    .pack_json = kr_web_path_info_pack_json,
    .unpack_json = kr_web_path_info_unpack_json,
    .pack_ebml = kr_web_path_info_pack_ebml,
    .unpack_ebml = kr_web_path_info_unpack_ebml,
    .init = kr_web_path_info_init,
    .valid = kr_web_path_info_valid,
    .random = kr_web_path_info_random,
    .patch_path = kr_web_path_info_patch_path,
    .diff = kr_web_path_info_diff
  },
  [KR_WEB_PATH_SETUP_INFO] = {
    .size = sizeof(kr_web_path_setup_info),
    .to_text = kr_web_path_setup_info_to_text,
    .pack_json = kr_web_path_setup_info_pack_json,
    .unpack_json = kr_web_path_setup_info_unpack_json,
    .pack_ebml = kr_web_path_setup_info_pack_ebml,
    .unpack_ebml = kr_web_path_setup_info_unpack_ebml,
    .init = kr_web_path_setup_info_init,
    .valid = kr_web_path_setup_info_valid,
    .random = kr_web_path_setup_info_random,
    .patch_path = kr_web_path_setup_info_patch_path,
    .diff = kr_web_path_setup_info_diff
  },
  [KR_PATH_INFO] = {
    .size = sizeof(kr_path_info),
    .to_text = kr_path_info_to_text,
    .pack_json = kr_path_info_pack_json,
    .unpack_json = kr_path_info_unpack_json,
    .pack_ebml = kr_path_info_pack_ebml,
    .unpack_ebml = kr_path_info_unpack_ebml,
    .init = kr_path_info_init,
    .valid = kr_path_info_valid,
    .random = kr_path_info_random,
    .patch_path = kr_path_info_patch_path,
    .diff = kr_path_info_diff
  },
  [KR_PATH_LIST] = {
    .size = sizeof(kr_path_list),
    .to_text = kr_path_list_to_text,
    .pack_json = kr_path_list_pack_json,
    .unpack_json = kr_path_list_unpack_json,
    .pack_ebml = kr_path_list_pack_ebml,
    .unpack_ebml = kr_path_list_unpack_ebml,
    .init = kr_path_list_init,
    .valid = kr_path_list_valid,
    .random = kr_path_list_random,
    .patch_path = kr_path_list_patch_path,
    .diff = kr_path_list_diff
  },
  [KR_UPLOAD_FILE_INFO] = {
    .size = sizeof(kr_upload_file_info),
    .to_text = kr_upload_file_info_to_text,
    .pack_json = kr_upload_file_info_pack_json,
    .unpack_json = kr_upload_file_info_unpack_json,
    .pack_ebml = kr_upload_file_info_pack_ebml,
    .unpack_ebml = kr_upload_file_info_unpack_ebml,
    .init = kr_upload_file_info_init,
    .valid = kr_upload_file_info_valid,
    .random = kr_upload_file_info_random,
    .patch_path = kr_upload_file_info_patch_path,
    .diff = kr_upload_file_info_diff
  },
  [KR_UPLOAD_STASH_INFO] = {
    .size = sizeof(kr_upload_stash_info),
    .to_text = kr_upload_stash_info_to_text,
    .pack_json = kr_upload_stash_info_pack_json,
    .unpack_json = kr_upload_stash_info_unpack_json,
    .pack_ebml = kr_upload_stash_info_pack_ebml,
    .unpack_ebml = kr_upload_stash_info_unpack_ebml,
    .init = kr_upload_stash_info_init,
    .valid = kr_upload_stash_info_valid,
    .random = kr_upload_stash_info_random,
    .patch_path = kr_upload_stash_info_patch_path,
    .diff = kr_upload_stash_info_diff
  },
  [KR_DOWNLOAD_FILE_INFO] = {
    .size = sizeof(kr_download_file_info),
    .to_text = kr_download_file_info_to_text,
    .pack_json = kr_download_file_info_pack_json,
    .unpack_json = kr_download_file_info_unpack_json,
    .pack_ebml = kr_download_file_info_pack_ebml,
    .unpack_ebml = kr_download_file_info_unpack_ebml,
    .init = kr_download_file_info_init,
    .valid = kr_download_file_info_valid,
    .random = kr_download_file_info_random,
    .patch_path = kr_download_file_info_patch_path,
    .diff = kr_download_file_info_diff
  },
  [KR_DOWNLOAD_STASH_INFO] = {
    .size = sizeof(kr_download_stash_info),
    .to_text = kr_download_stash_info_to_text,
    .pack_json = kr_download_stash_info_pack_json,
    .unpack_json = kr_download_stash_info_unpack_json,
    .pack_ebml = kr_download_stash_info_pack_ebml,
    .unpack_ebml = kr_download_stash_info_unpack_ebml,
    .init = kr_download_stash_info_init,
    .valid = kr_download_stash_info_valid,
    .random = kr_download_stash_info_random,
    .patch_path = kr_download_stash_info_patch_path,
    .diff = kr_download_stash_info_diff
  },
  [KR_UPLOAD_INFO] = {
    .size = sizeof(kr_upload_info),
    .to_text = kr_upload_info_to_text,
    .pack_json = kr_upload_info_pack_json,
    .unpack_json = kr_upload_info_unpack_json,
    .pack_ebml = kr_upload_info_pack_ebml,
    .unpack_ebml = kr_upload_info_unpack_ebml,
    .init = kr_upload_info_init,
    .valid = kr_upload_info_valid,
    .random = kr_upload_info_random,
    .patch_path = kr_upload_info_patch_path,
    .diff = kr_upload_info_diff
  },
  [KR_DOWNLOAD_INFO] = {
    .size = sizeof(kr_download_info),
    .to_text = kr_download_info_to_text,
    .pack_json = kr_download_info_pack_json,
    .unpack_json = kr_download_info_unpack_json,
    .pack_ebml = kr_download_info_pack_ebml,
    .unpack_ebml = kr_download_info_unpack_ebml,
    .init = kr_download_info_init,
    .valid = kr_download_info_valid,
    .random = kr_download_info_random,
    .patch_path = kr_download_info_patch_path,
    .diff = kr_download_info_diff
  },
  [KR_ACCEPT_INFO] = {
    .size = sizeof(kr_accept_info),
    .to_text = kr_accept_info_to_text,
    .pack_json = kr_accept_info_pack_json,
    .unpack_json = kr_accept_info_unpack_json,
    .pack_ebml = kr_accept_info_pack_ebml,
    .unpack_ebml = kr_accept_info_unpack_ebml,
    .init = kr_accept_info_init,
    .valid = kr_accept_info_valid,
    .random = kr_accept_info_random,
    .patch_path = kr_accept_info_patch_path,
    .diff = kr_accept_info_diff
  },
  [KR_SERVE_RANGE_INFO] = {
    .size = sizeof(kr_serve_range_info),
    .to_text = kr_serve_range_info_to_text,
    .pack_json = kr_serve_range_info_pack_json,
    .unpack_json = kr_serve_range_info_unpack_json,
    .pack_ebml = kr_serve_range_info_pack_ebml,
    .unpack_ebml = kr_serve_range_info_unpack_ebml,
    .init = kr_serve_range_info_init,
    .valid = kr_serve_range_info_valid,
    .random = kr_serve_range_info_random,
    .patch_path = kr_serve_range_info_patch_path,
    .diff = kr_serve_range_info_diff
  },
  [KR_SERVE_RANGES_INFO] = {
    .size = sizeof(kr_serve_ranges_info),
    .to_text = kr_serve_ranges_info_to_text,
    .pack_json = kr_serve_ranges_info_pack_json,
    .unpack_json = kr_serve_ranges_info_unpack_json,
    .pack_ebml = kr_serve_ranges_info_pack_ebml,
    .unpack_ebml = kr_serve_ranges_info_unpack_ebml,
    .init = kr_serve_ranges_info_init,
    .valid = kr_serve_ranges_info_valid,
    .random = kr_serve_ranges_info_random,
    .patch_path = kr_serve_ranges_info_patch_path,
    .diff = kr_serve_ranges_info_diff
  },
  [KR_SERVE_FULL_INFO] = {
    .size = sizeof(kr_serve_full_info),
    .to_text = kr_serve_full_info_to_text,
    .pack_json = kr_serve_full_info_pack_json,
    .unpack_json = kr_serve_full_info_unpack_json,
    .pack_ebml = kr_serve_full_info_pack_ebml,
    .unpack_ebml = kr_serve_full_info_unpack_ebml,
    .init = kr_serve_full_info_init,
    .valid = kr_serve_full_info_valid,
    .random = kr_serve_full_info_random,
    .patch_path = kr_serve_full_info_patch_path,
    .diff = kr_serve_full_info_diff
  },
  [KR_SERVE_INDEX_INFO] = {
    .size = sizeof(kr_serve_index_info),
    .to_text = kr_serve_index_info_to_text,
    .pack_json = kr_serve_index_info_pack_json,
    .unpack_json = kr_serve_index_info_unpack_json,
    .pack_ebml = kr_serve_index_info_pack_ebml,
    .unpack_ebml = kr_serve_index_info_unpack_ebml,
    .init = kr_serve_index_info_init,
    .valid = kr_serve_index_info_valid,
    .random = kr_serve_index_info_random,
    .patch_path = kr_serve_index_info_patch_path,
    .diff = kr_serve_index_info_diff
  },
  [KR_SERVE_REDIRECT_INFO] = {
    .size = sizeof(kr_serve_redirect_info),
    .to_text = kr_serve_redirect_info_to_text,
    .pack_json = kr_serve_redirect_info_pack_json,
    .unpack_json = kr_serve_redirect_info_unpack_json,
    .pack_ebml = kr_serve_redirect_info_pack_ebml,
    .unpack_ebml = kr_serve_redirect_info_unpack_ebml,
    .init = kr_serve_redirect_info_init,
    .valid = kr_serve_redirect_info_valid,
    .random = kr_serve_redirect_info_random,
    .patch_path = kr_serve_redirect_info_patch_path,
    .diff = kr_serve_redirect_info_diff
  },
  [KR_SERVE_INFO] = {
    .size = sizeof(kr_serve_info),
    .to_text = kr_serve_info_to_text,
    .pack_json = kr_serve_info_pack_json,
    .unpack_json = kr_serve_info_unpack_json,
    .pack_ebml = kr_serve_info_pack_ebml,
    .unpack_ebml = kr_serve_info_unpack_ebml,
    .init = kr_serve_info_init,
    .valid = kr_serve_info_valid,
    .random = kr_serve_info_random,
    .patch_path = kr_serve_info_patch_path,
    .diff = kr_serve_info_diff
  },
  [KR_DOWNLINK_INFO] = {
    .size = sizeof(kr_downlink_info),
    .to_text = kr_downlink_info_to_text,
    .pack_json = kr_downlink_info_pack_json,
    .unpack_json = kr_downlink_info_unpack_json,
    .pack_ebml = kr_downlink_info_pack_ebml,
    .unpack_ebml = kr_downlink_info_unpack_ebml,
    .init = kr_downlink_info_init,
    .valid = kr_downlink_info_valid,
    .random = kr_downlink_info_random,
    .patch_path = kr_downlink_info_patch_path,
    .diff = kr_downlink_info_diff
  },
  [KR_UPLINK_INFO] = {
    .size = sizeof(kr_uplink_info),
    .to_text = kr_uplink_info_to_text,
    .pack_json = kr_uplink_info_pack_json,
    .unpack_json = kr_uplink_info_unpack_json,
    .pack_ebml = kr_uplink_info_pack_ebml,
    .unpack_ebml = kr_uplink_info_unpack_ebml,
    .init = kr_uplink_info_init,
    .valid = kr_uplink_info_valid,
    .random = kr_uplink_info_random,
    .patch_path = kr_uplink_info_patch_path,
    .diff = kr_uplink_info_diff
  },
  [KR_SOURCE_INFO] = {
    .size = sizeof(kr_source_info),
    .to_text = kr_source_info_to_text,
    .pack_json = kr_source_info_pack_json,
    .unpack_json = kr_source_info_unpack_json,
    .pack_ebml = kr_source_info_pack_ebml,
    .unpack_ebml = kr_source_info_unpack_ebml,
    .init = kr_source_info_init,
    .valid = kr_source_info_valid,
    .random = kr_source_info_random,
    .patch_path = kr_source_info_patch_path,
    .diff = kr_source_info_diff
  },
  [KR_TRANSMIT_INFO] = {
    .size = sizeof(kr_transmit_info),
    .to_text = kr_transmit_info_to_text,
    .pack_json = kr_transmit_info_pack_json,
    .unpack_json = kr_transmit_info_unpack_json,
    .pack_ebml = kr_transmit_info_pack_ebml,
    .unpack_ebml = kr_transmit_info_unpack_ebml,
    .init = kr_transmit_info_init,
    .valid = kr_transmit_info_valid,
    .random = kr_transmit_info_random,
    .patch_path = kr_transmit_info_patch_path,
    .diff = kr_transmit_info_diff
  },
  [KR_XFER_RECONNECT_INFO] = {
    .size = sizeof(kr_xfer_reconnect_info),
    .to_text = kr_xfer_reconnect_info_to_text,
    .pack_json = kr_xfer_reconnect_info_pack_json,
    .unpack_json = kr_xfer_reconnect_info_unpack_json,
    .pack_ebml = kr_xfer_reconnect_info_pack_ebml,
    .unpack_ebml = kr_xfer_reconnect_info_unpack_ebml,
    .init = kr_xfer_reconnect_info_init,
    .valid = kr_xfer_reconnect_info_valid,
    .random = kr_xfer_reconnect_info_random,
    .patch_path = kr_xfer_reconnect_info_patch_path,
    .diff = kr_xfer_reconnect_info_diff
  },
  [KR_XFER_CLIENT_INFO] = {
    .size = sizeof(kr_xfer_client_info),
    .to_text = kr_xfer_client_info_to_text,
    .pack_json = kr_xfer_client_info_pack_json,
    .unpack_json = kr_xfer_client_info_unpack_json,
    .pack_ebml = kr_xfer_client_info_pack_ebml,
    .unpack_ebml = kr_xfer_client_info_unpack_ebml,
    .init = kr_xfer_client_info_init,
    .valid = kr_xfer_client_info_valid,
    .random = kr_xfer_client_info_random,
    .patch_path = kr_xfer_client_info_patch_path,
    .diff = kr_xfer_client_info_diff
  },
  [KR_XFER_ACCEPTOR_DIR_INFO] = {
    .size = sizeof(kr_xfer_acceptor_dir_info),
    .to_text = kr_xfer_acceptor_dir_info_to_text,
    .pack_json = kr_xfer_acceptor_dir_info_pack_json,
    .unpack_json = kr_xfer_acceptor_dir_info_unpack_json,
    .pack_ebml = kr_xfer_acceptor_dir_info_pack_ebml,
    .unpack_ebml = kr_xfer_acceptor_dir_info_unpack_ebml,
    .init = kr_xfer_acceptor_dir_info_init,
    .valid = kr_xfer_acceptor_dir_info_valid,
    .random = kr_xfer_acceptor_dir_info_random,
    .patch_path = kr_xfer_acceptor_dir_info_patch_path,
    .diff = kr_xfer_acceptor_dir_info_diff
  },
  [KR_XFER_ACCEPTOR_FILE_INFO] = {
    .size = sizeof(kr_xfer_acceptor_file_info),
    .to_text = kr_xfer_acceptor_file_info_to_text,
    .pack_json = kr_xfer_acceptor_file_info_pack_json,
    .unpack_json = kr_xfer_acceptor_file_info_unpack_json,
    .pack_ebml = kr_xfer_acceptor_file_info_pack_ebml,
    .unpack_ebml = kr_xfer_acceptor_file_info_unpack_ebml,
    .init = kr_xfer_acceptor_file_info_init,
    .valid = kr_xfer_acceptor_file_info_valid,
    .random = kr_xfer_acceptor_file_info_random,
    .patch_path = kr_xfer_acceptor_file_info_patch_path,
    .diff = kr_xfer_acceptor_file_info_diff
  },
  [KR_XFER_ACCEPTOR_BUF_INFO] = {
    .size = sizeof(kr_xfer_acceptor_buf_info),
    .to_text = kr_xfer_acceptor_buf_info_to_text,
    .pack_json = kr_xfer_acceptor_buf_info_pack_json,
    .unpack_json = kr_xfer_acceptor_buf_info_unpack_json,
    .pack_ebml = kr_xfer_acceptor_buf_info_pack_ebml,
    .unpack_ebml = kr_xfer_acceptor_buf_info_unpack_ebml,
    .init = kr_xfer_acceptor_buf_info_init,
    .valid = kr_xfer_acceptor_buf_info_valid,
    .random = kr_xfer_acceptor_buf_info_random,
    .patch_path = kr_xfer_acceptor_buf_info_patch_path,
    .diff = kr_xfer_acceptor_buf_info_diff
  },
  [KR_XFER_SHARE_DIR_INFO] = {
    .size = sizeof(kr_xfer_share_dir_info),
    .to_text = kr_xfer_share_dir_info_to_text,
    .pack_json = kr_xfer_share_dir_info_pack_json,
    .unpack_json = kr_xfer_share_dir_info_unpack_json,
    .pack_ebml = kr_xfer_share_dir_info_pack_ebml,
    .unpack_ebml = kr_xfer_share_dir_info_unpack_ebml,
    .init = kr_xfer_share_dir_info_init,
    .valid = kr_xfer_share_dir_info_valid,
    .random = kr_xfer_share_dir_info_random,
    .patch_path = kr_xfer_share_dir_info_patch_path,
    .diff = kr_xfer_share_dir_info_diff
  },
  [KR_XFER_SHARE_FILE_INFO] = {
    .size = sizeof(kr_xfer_share_file_info),
    .to_text = kr_xfer_share_file_info_to_text,
    .pack_json = kr_xfer_share_file_info_pack_json,
    .unpack_json = kr_xfer_share_file_info_unpack_json,
    .pack_ebml = kr_xfer_share_file_info_pack_ebml,
    .unpack_ebml = kr_xfer_share_file_info_unpack_ebml,
    .init = kr_xfer_share_file_info_init,
    .valid = kr_xfer_share_file_info_valid,
    .random = kr_xfer_share_file_info_random,
    .patch_path = kr_xfer_share_file_info_patch_path,
    .diff = kr_xfer_share_file_info_diff
  },
  [KR_XFER_SHARE_BUF_INFO] = {
    .size = sizeof(kr_xfer_share_buf_info),
    .to_text = kr_xfer_share_buf_info_to_text,
    .pack_json = kr_xfer_share_buf_info_pack_json,
    .unpack_json = kr_xfer_share_buf_info_unpack_json,
    .pack_ebml = kr_xfer_share_buf_info_pack_ebml,
    .unpack_ebml = kr_xfer_share_buf_info_unpack_ebml,
    .init = kr_xfer_share_buf_info_init,
    .valid = kr_xfer_share_buf_info_valid,
    .random = kr_xfer_share_buf_info_random,
    .patch_path = kr_xfer_share_buf_info_patch_path,
    .diff = kr_xfer_share_buf_info_diff
  },
  [KR_XFER_ACCEPTOR_INFO] = {
    .size = sizeof(kr_xfer_acceptor_info),
    .to_text = kr_xfer_acceptor_info_to_text,
    .pack_json = kr_xfer_acceptor_info_pack_json,
    .unpack_json = kr_xfer_acceptor_info_unpack_json,
    .pack_ebml = kr_xfer_acceptor_info_pack_ebml,
    .unpack_ebml = kr_xfer_acceptor_info_unpack_ebml,
    .init = kr_xfer_acceptor_info_init,
    .valid = kr_xfer_acceptor_info_valid,
    .random = kr_xfer_acceptor_info_random,
    .patch_path = kr_xfer_acceptor_info_patch_path,
    .diff = kr_xfer_acceptor_info_diff
  },
  [KR_XFER_RECEIVER_INFO] = {
    .size = sizeof(kr_xfer_receiver_info),
    .to_text = kr_xfer_receiver_info_to_text,
    .pack_json = kr_xfer_receiver_info_pack_json,
    .unpack_json = kr_xfer_receiver_info_unpack_json,
    .pack_ebml = kr_xfer_receiver_info_pack_ebml,
    .unpack_ebml = kr_xfer_receiver_info_unpack_ebml,
    .init = kr_xfer_receiver_info_init,
    .valid = kr_xfer_receiver_info_valid,
    .random = kr_xfer_receiver_info_random,
    .patch_path = kr_xfer_receiver_info_patch_path,
    .diff = kr_xfer_receiver_info_diff
  },
  [KR_XFER_TRANSMITTER_INFO] = {
    .size = sizeof(kr_xfer_transmitter_info),
    .to_text = kr_xfer_transmitter_info_to_text,
    .pack_json = kr_xfer_transmitter_info_pack_json,
    .unpack_json = kr_xfer_transmitter_info_unpack_json,
    .pack_ebml = kr_xfer_transmitter_info_pack_ebml,
    .unpack_ebml = kr_xfer_transmitter_info_unpack_ebml,
    .init = kr_xfer_transmitter_info_init,
    .valid = kr_xfer_transmitter_info_valid,
    .random = kr_xfer_transmitter_info_random,
    .patch_path = kr_xfer_transmitter_info_patch_path,
    .diff = kr_xfer_transmitter_info_diff
  },
  [KR_XFER_SHARE_INFO] = {
    .size = sizeof(kr_xfer_share_info),
    .to_text = kr_xfer_share_info_to_text,
    .pack_json = kr_xfer_share_info_pack_json,
    .unpack_json = kr_xfer_share_info_unpack_json,
    .pack_ebml = kr_xfer_share_info_pack_ebml,
    .unpack_ebml = kr_xfer_share_info_unpack_ebml,
    .init = kr_xfer_share_info_init,
    .valid = kr_xfer_share_info_valid,
    .random = kr_xfer_share_info_random,
    .patch_path = kr_xfer_share_info_patch_path,
    .diff = kr_xfer_share_info_diff
  },
  [KR_XFER_SERVER_INFO] = {
    .size = sizeof(kr_xfer_server_info),
    .to_text = kr_xfer_server_info_to_text,
    .pack_json = kr_xfer_server_info_pack_json,
    .unpack_json = kr_xfer_server_info_unpack_json,
    .pack_ebml = kr_xfer_server_info_pack_ebml,
    .unpack_ebml = kr_xfer_server_info_unpack_ebml,
    .init = kr_xfer_server_info_init,
    .valid = kr_xfer_server_info_valid,
    .random = kr_xfer_server_info_random,
    .patch_path = kr_xfer_server_info_patch_path,
    .diff = kr_xfer_server_info_diff
  },
  [KR_XFER_PATH_INFO] = {
    .size = sizeof(kr_xfer_path_info),
    .to_text = kr_xfer_path_info_to_text,
    .pack_json = kr_xfer_path_info_pack_json,
    .unpack_json = kr_xfer_path_info_unpack_json,
    .pack_ebml = kr_xfer_path_info_pack_ebml,
    .unpack_ebml = kr_xfer_path_info_unpack_ebml,
    .init = kr_xfer_path_info_init,
    .valid = kr_xfer_path_info_valid,
    .random = kr_xfer_path_info_random,
    .patch_path = kr_xfer_path_info_patch_path,
    .diff = kr_xfer_path_info_diff
  },
  [KR_XPDR_PATH_INFO] = {
    .size = sizeof(struct kr_xpdr_path_info),
    .to_text = kr_xpdr_path_info_to_text,
    .pack_json = kr_xpdr_path_info_pack_json,
    .unpack_json = kr_xpdr_path_info_unpack_json,
    .pack_ebml = kr_xpdr_path_info_pack_ebml,
    .unpack_ebml = kr_xpdr_path_info_unpack_ebml,
    .init = kr_xpdr_path_info_init,
    .valid = kr_xpdr_path_info_valid,
    .random = kr_xpdr_path_info_random,
    .patch_path = kr_xpdr_path_info_patch_path,
    .diff = kr_xpdr_path_info_diff
  }
};

#endif

