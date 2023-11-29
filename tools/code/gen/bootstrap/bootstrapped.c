#include "bootstrapped.h"
#include <string.h>

char *codegen_enum_to_string(cgen_enum val) {
  switch (val) {
    case CGEN_KR_ALSA_INFO:
      return "kr_alsa_info";
    case CGEN_KR_ALSA_PATH_INFO:
      return "kr_alsa_path_info";
    case CGEN_KR_DECKLINK_VIDEO_CONN_TYPE:
      return "kr_decklink_video_conn_type";
    case CGEN_KR_DECKLINK_AUDIO_CONN_TYPE:
      return "kr_decklink_audio_conn_type";
    case CGEN_KR_DECKLINK_INFO:
      return "kr_decklink_info";
    case CGEN_KR_DECKLINK_PATH_INFO:
      return "kr_decklink_path_info";
    case CGEN_KR_V4L2_STATE:
      return "kr_v4l2_state";
    case CGEN_KR_V4L2_PATH_INFO:
      return "kr_v4l2_path_info";
    case CGEN_KR_V4L2_INFO:
      return "kr_v4l2_info";
    case CGEN_KR_AUX_PORT_STATE:
      return "kr_aux_port_state";
    case CGEN_KR_AUX_INFO:
      return "kr_aux_info";
    case CGEN_KR_AUX_PATH_INFO:
      return "kr_aux_path_info";
    case CGEN_KR_JACK_DIRECTION:
      return "kr_jack_direction";
    case CGEN_KR_JACK_STATE:
      return "kr_jack_state";
    case CGEN_KR_JACK_PATH_STATE:
      return "kr_jack_path_state";
    case CGEN_KR_JACK_INFO:
      return "kr_jack_info";
    case CGEN_KR_JACK_PATH_INFO:
      return "kr_jack_path_info";
    case CGEN_KR_WAYLAND_STATE:
      return "kr_wayland_state";
    case CGEN_KR_WAYLAND_INFO:
      return "kr_wayland_info";
    case CGEN_KR_WAYLAND_PATH_INFO:
      return "kr_wayland_path_info";
    case CGEN_KR_X11_INFO:
      return "kr_x11_info";
    case CGEN_KR_X11_PATH_INFO:
      return "kr_x11_path_info";
    case CGEN_KR_AFX_EFFECT_TYPE:
      return "kr_afx_effect_type";
    case CGEN_KR_AFX_EFFECT_CONTROL:
      return "kr_afx_effect_control";
    case CGEN_KR_EQ_BAND_INFO:
      return "kr_eq_band_info";
    case CGEN_KR_EQ_INFO:
      return "kr_eq_info";
    case CGEN_KR_LOWPASS_INFO:
      return "kr_lowpass_info";
    case CGEN_KR_HIGHPASS_INFO:
      return "kr_highpass_info";
    case CGEN_KR_PASS_INFO:
      return "kr_pass_info";
    case CGEN_KR_VOLUME_INFO:
      return "kr_volume_info";
    case CGEN_KR_ANALOG_INFO:
      return "kr_analog_info";
    case CGEN_KR_METER_INFO:
      return "kr_meter_info";
    case CGEN_KR_AFX_INFO:
      return "kr_afx_info";
    case CGEN_KR_VIDEO_CODEC:
      return "kr_video_codec";
    case CGEN_KR_AUDIO_CODEC:
      return "kr_audio_codec";
    case CGEN_KR_CODEC_TYPE:
      return "kr_codec_type";
    case CGEN_KR_CODEC:
      return "kr_codec";
    case CGEN_KR_CODEC_SUB1:
      return "kr_codec_sub1";
    case CGEN_KR_AUDIO_INFO:
      return "kr_audio_info";
    case CGEN_KR_VIDEO_INFO:
      return "kr_video_info";
    case CGEN_KR_AV_INFO:
      return "kr_av_info";
    case CGEN_KR_AV_INFO_SUB1:
      return "kr_av_info_sub1";
    case CGEN_KR_DAALA_ENCODER_INFO:
      return "kr_daala_encoder_info";
    case CGEN_KR_VPX_ENCODER_INFO:
      return "kr_vpx_encoder_info";
    case CGEN_KR_H264_ENCODER_INFO:
      return "kr_h264_encoder_info";
    case CGEN_KR_THEORA_ENCODER_INFO:
      return "kr_theora_encoder_info";
    case CGEN_KR_Y4M_ENCODER_INFO:
      return "kr_y4m_encoder_info";
    case CGEN_KR_FLAC_INFO:
      return "kr_flac_info";
    case CGEN_KR_OPUS_APP:
      return "kr_opus_app";
    case CGEN_KR_OPUS_INFO:
      return "kr_opus_info";
    case CGEN_KR_VORBIS_INFO:
      return "kr_vorbis_info";
    case CGEN_KR_MP3_INFO:
      return "kr_mp3_info";
    case CGEN_KR_AAC_INFO:
      return "kr_aac_info";
    case CGEN_KR_LAVC_ENCODER_INFO:
      return "kr_lavc_encoder_info";
    case CGEN_KR_COMPOSITOR_PATH_TYPE:
      return "kr_compositor_path_type";
    case CGEN_KR_COMPOSITOR_OVERLAY_TYPE:
      return "kr_compositor_overlay_type";
    case CGEN_KR_COMPOSITOR_OUTPUT_INFO:
      return "kr_compositor_output_info";
    case CGEN_KR_COMPOSITOR_BUS_INFO:
      return "kr_compositor_bus_info";
    case CGEN_KR_COMPOSITOR_INPUT_INFO:
      return "kr_compositor_input_info";
    case CGEN_KR_COMPOSITOR_SOURCE_INFO:
      return "kr_compositor_source_info";
    case CGEN_KR_COMPOSITOR_OVERLAY_INFO:
      return "kr_compositor_overlay_info";
    case CGEN_KR_COMPOSITOR_OVERLAY_INFO_SUB1:
      return "kr_compositor_overlay_info_sub1";
    case CGEN_KR_COMPOSITOR_PATH_INFO:
      return "kr_compositor_path_info";
    case CGEN_KR_COMPOSITOR_PATH_INFO_SUB1:
      return "kr_compositor_path_info_sub1";
    case CGEN_KR_MTS_STREAM_TYPE:
      return "kr_mts_stream_type";
    case CGEN_KR_MTS_PACKET_INFO:
      return "kr_mts_packet_info";
    case CGEN_KR_MTS_STREAM_INFO:
      return "kr_mts_stream_info";
    case CGEN_KR_MTS_STREAM_INFO_SUB1:
      return "kr_mts_stream_info_sub1";
    case CGEN_KR_MTS_PROGRAM_INFO:
      return "kr_mts_program_info";
    case CGEN_KR_MTS_INFO:
      return "kr_mts_info";
    case CGEN_KR_CONTAINER_TYPE:
      return "kr_container_type";
    case CGEN_KR_TEXT_INFO:
      return "kr_text_info";
    case CGEN_KR_VECTOR_TYPE:
      return "kr_vector_type";
    case CGEN_KR_VECTOR_INFO:
      return "kr_vector_info";
    case CGEN_KR_FEATURE_ASPECT_TYPE:
      return "kr_feature_aspect_type";
    case CGEN_KR_FEATURE_ASPECT:
      return "kr_feature_aspect";
    case CGEN_KR_FEATURE:
      return "kr_feature";
    case CGEN_KR_IMAGE_STASH_ITEM_INFO:
      return "kr_image_stash_item_info";
    case CGEN_KR_IMAGE_STASH_ITEM_INFO_SUB1:
      return "kr_image_stash_item_info_sub1";
    case CGEN_KR_PIXEL_FMT:
      return "kr_pixel_fmt";
    case CGEN_KR_IMAGE_TYPE:
      return "kr_image_type";
    case CGEN_KR_RECT:
      return "kr_rect";
    case CGEN_KR_IMAGE_INFO:
      return "kr_image_info";
    case CGEN_KR_CODED_IMAGE_TYPE:
      return "kr_coded_image_type";
    case CGEN_KR_CODED_IMAGE_INFO:
      return "kr_coded_image_info";
    case CGEN_KR_FILE_ACCESS_MODE:
      return "kr_file_access_mode";
    case CGEN_KR_FILE2_INFO:
      return "kr_file2_info";
    case CGEN_KR_MIXER_CHANNELS:
      return "kr_mixer_channels";
    case CGEN_KR_MIXER_PATH_TYPE:
      return "kr_mixer_path_type";
    case CGEN_KR_MIXER_PATH_INFO:
      return "kr_mixer_path_info";
    case CGEN_KR_MEDIA_EVENT_TYPE:
      return "kr_media_event_type";
    case CGEN_KR_MEDIA_ITEM_TYPE:
      return "kr_media_item_type";
    case CGEN_KR_MEDIA_FILE_INFO:
      return "kr_media_file_info";
    case CGEN_KR_MEDIA_METADATA_INFO:
      return "kr_media_metadata_info";
    case CGEN_KR_MEDIA_URL_INFO:
      return "kr_media_url_info";
    case CGEN_KR_MEDIA_ITEM_INFO:
      return "kr_media_item_info";
    case CGEN_KR_MEDIA_ITEM_INFO_SUB1:
      return "kr_media_item_info_sub1";
    case CGEN_KR_MEDIA_INFO:
      return "kr_media_info";
    case CGEN_KR_OPS_INFO:
      return "kr_ops_info";
    case CGEN_KR_POS:
      return "kr_pos";
    case CGEN_KR_PERSPECTIVE_VIEW:
      return "kr_perspective_view";
    case CGEN_M3U_EL_TYPE:
      return "m3u_el_type";
    case CGEN_KR_M3U_TAG_NAME:
      return "kr_m3u_tag_name";
    case CGEN_KR_M3U_TAG_VAL_TYPE:
      return "kr_m3u_tag_val_type";
    case CGEN_KR_M3U_TAG_ATTR_NAME:
      return "kr_m3u_tag_attr_name";
    case CGEN_KR_M3U_TAG_PRIM_INFO:
      return "kr_m3u_tag_prim_info";
    case CGEN_KR_M3U_TAG_ALIST_INFO:
      return "kr_m3u_tag_alist_info";
    case CGEN_M3U_TAG_INFO:
      return "m3u_tag_info";
    case CGEN_M3U_TAG_INFO_SUB1:
      return "m3u_tag_info_sub1";
    case CGEN_M3U_EL_INFO:
      return "m3u_el_info";
    case CGEN_M3U_EL_INFO_SUB1:
      return "m3u_el_info_sub1";
    case CGEN_KR_M3U_INFO:
      return "kr_m3u_info";
    case CGEN_KR_LIMIT_INFO:
      return "kr_limit_info";
    case CGEN_KR_LIMITS_INFO:
      return "kr_limits_info";
    case CGEN_KR_PRIORITY:
      return "kr_priority";
    case CGEN_KR_AVCONV_STATUS_INFO:
      return "kr_avconv_status_info";
    case CGEN_KR_AVCONV_PARAMS_INFO:
      return "kr_avconv_params_info";
    case CGEN_KR_AVCONV_INFO:
      return "kr_avconv_info";
    case CGEN_KR_AVPROBE_CODEC_TYPE:
      return "kr_avprobe_codec_type";
    case CGEN_KR_AVPROBE_FORMAT_INFO:
      return "kr_avprobe_format_info";
    case CGEN_KR_AVPROBE_STREAM_INFO:
      return "kr_avprobe_stream_info";
    case CGEN_KR_AVPROBE_MEDIA_INFO:
      return "kr_avprobe_media_info";
    case CGEN_KR_AVPROBE_PARAMS_INFO:
      return "kr_avprobe_params_info";
    case CGEN_KR_AVPROBE_STATUS_INFO:
      return "kr_avprobe_status_info";
    case CGEN_KR_AVPROBE_INFO:
      return "kr_avprobe_info";
    case CGEN_KR_CURL_STATUS_INFO:
      return "kr_curl_status_info";
    case CGEN_KR_CURL_PARAMS_INFO:
      return "kr_curl_params_info";
    case CGEN_KR_CURL_INFO:
      return "kr_curl_info";
    case CGEN_KR_LIVESTREAMER_PARAMS_INFO:
      return "kr_livestreamer_params_info";
    case CGEN_KR_LIVESTREAMER_STATUS_INFO:
      return "kr_livestreamer_status_info";
    case CGEN_KR_LIVESTREAMER_INFO:
      return "kr_livestreamer_info";
    case CGEN_KR_MEDIA_VERSION:
      return "kr_media_version";
    case CGEN_KR_QUVI_MEDIA_INFO:
      return "kr_quvi_media_info";
    case CGEN_KR_QUVI_STATUS_INFO:
      return "kr_quvi_status_info";
    case CGEN_KR_QUVI_PARAMS_INFO:
      return "kr_quvi_params_info";
    case CGEN_KR_QUVI_INFO:
      return "kr_quvi_info";
    case CGEN_KR_TRANSMISSION_CLI_STATUS_INFO:
      return "kr_transmission_cli_status_info";
    case CGEN_KR_TRANSMISSION_CLI_PARAMS_INFO:
      return "kr_transmission_cli_params_info";
    case CGEN_KR_TRANSMISSION_CLI_INFO:
      return "kr_transmission_cli_info";
    case CGEN_KR_TASK_TYPE:
      return "kr_task_type";
    case CGEN_KR_TASK_STATE:
      return "kr_task_state";
    case CGEN_KR_TASK_EVENT_TYPE:
      return "kr_task_event_type";
    case CGEN_KR_FAIL_INFO:
      return "kr_fail_info";
    case CGEN_KR_TASK_STATUS:
      return "kr_task_status";
    case CGEN_KR_TASK_PARAMS_INFO:
      return "kr_task_params_info";
    case CGEN_KR_TASK_PARAMS_INFO_SUB1:
      return "kr_task_params_info_sub1";
    case CGEN_KR_TASK_INFO:
      return "kr_task_info";
    case CGEN_KR_TASK_INFO_SUB1:
      return "kr_task_info_sub1";
    case CGEN_KR_VALGRIND_STRESSOR_STATUS_INFO:
      return "kr_valgrind_stressor_status_info";
    case CGEN_KR_VALGRIND_STRESSOR_PARAMS_INFO:
      return "kr_valgrind_stressor_params_info";
    case CGEN_KR_VALGRIND_STRESSOR_INFO:
      return "kr_valgrind_stressor_info";
    case CGEN_KR_XMMS_PARAMS_INFO:
      return "kr_xmms_params_info";
    case CGEN_KR_XMMS_STATUS_INFO:
      return "kr_xmms_status_info";
    case CGEN_KR_XMMS_INFO:
      return "kr_xmms_info";
    case CGEN_KR_YDL_FORMAT_INFO:
      return "kr_ydl_format_info";
    case CGEN_KR_YDL_VIDEO_INFO:
      return "kr_ydl_video_info";
    case CGEN_KR_YDL_PARAMS_INFO:
      return "kr_ydl_params_info";
    case CGEN_KR_YDL_STATUS_INFO:
      return "kr_ydl_status_info";
    case CGEN_KR_YDL_INFO:
      return "kr_ydl_info";
    case CGEN_KR_TIMESTAMP_INFO:
      return "kr_timestamp_info";
    case CGEN_KR_EASING:
      return "kr_easing";
    case CGEN_KR_MEDIA_SOURCE_FORMAT:
      return "kr_media_source_format";
    case CGEN_KR_MEDIA_SOURCE_INFO:
      return "kr_media_source_info";
    case CGEN_KR_MEDIA_PACKET_TYPE:
      return "kr_media_packet_type";
    case CGEN_KR_MEDIA_PACKET:
      return "kr_media_packet";
    case CGEN_KR_TRACK_NAME:
      return "kr_track_name";
    case CGEN_KR_MUX_NAME:
      return "kr_mux_name";
    case CGEN_KR_DEMUX_INFO:
      return "kr_demux_info";
    case CGEN_KR_MUX_INFO:
      return "kr_mux_info";
    case CGEN_KR_VIDEO_ENCODE_INFO:
      return "kr_video_encode_info";
    case CGEN_KR_VIDEO_ENCODE_INFO_SUB1:
      return "kr_video_encode_info_sub1";
    case CGEN_KR_AUDIO_ENCODE_INFO:
      return "kr_audio_encode_info";
    case CGEN_KR_AUDIO_ENCODE_INFO_SUB1:
      return "kr_audio_encode_info_sub1";
    case CGEN_KR_ENCODE_TYPE:
      return "kr_encode_type";
    case CGEN_KR_ENCODE_INFO:
      return "kr_encode_info";
    case CGEN_KR_ENCODE_INFO_SUB1:
      return "kr_encode_info_sub1";
    case CGEN_KR_CAPTURE_TYPE:
      return "kr_capture_type";
    case CGEN_KR_RECORD_INFO:
      return "kr_record_info";
    case CGEN_KR_TRANSMISSION_INFO:
      return "kr_transmission_info";
    case CGEN_KR_OUTPUT_TYPE:
      return "kr_output_type";
    case CGEN_KR_OUTPUT_INFO:
      return "kr_output_info";
    case CGEN_KR_OUTPUT_INFO_SUB1:
      return "kr_output_info_sub1";
    case CGEN_KR_CAPTURE_AUDIO_INFO:
      return "kr_capture_audio_info";
    case CGEN_KR_CAPTURE_VIDEO_INFO:
      return "kr_capture_video_info";
    case CGEN_KR_CAPTURE_IMAGE_INFO:
      return "kr_capture_image_info";
    case CGEN_KR_CAPTURE_INFO:
      return "kr_capture_info";
    case CGEN_KR_CAPTURE_INFO_SUB1:
      return "kr_capture_info_sub1";
    case CGEN_KR_SOURCE_FILE_INFO:
      return "kr_source_file_info";
    case CGEN_KR_SOURCE_REMOTE_FILE_INFO:
      return "kr_source_remote_file_info";
    case CGEN_KR_STUDIO_SOURCE_STREAM_TYPE:
      return "kr_studio_source_stream_type";
    case CGEN_KR_SOURCE_STREAM_INFO:
      return "kr_source_stream_info";
    case CGEN_KR_STUDIO_SOURCE_TYPE:
      return "kr_studio_source_type";
    case CGEN_KR_STUDIO_SOURCE_INFO:
      return "kr_studio_source_info";
    case CGEN_KR_STUDIO_SOURCE_INFO_SUB1:
      return "kr_studio_source_info_sub1";
    case CGEN_KR_DECODE_INFO:
      return "kr_decode_info";
    case CGEN_KR_PLAY_INFO:
      return "kr_play_info";
    case CGEN_KR_RENDER_IMAGE_INFO:
      return "kr_render_image_info";
    case CGEN_KR_RENDER_VIDEO_INFO:
      return "kr_render_video_info";
    case CGEN_KR_RENDER_AUDIO_INFO:
      return "kr_render_audio_info";
    case CGEN_KR_RENDER_TYPE:
      return "kr_render_type";
    case CGEN_KR_RENDER_INFO:
      return "kr_render_info";
    case CGEN_KR_RENDER_INFO_SUB1:
      return "kr_render_info_sub1";
    case CGEN_KR_AUDIO_GENERATE_TYPE:
      return "kr_audio_generate_type";
    case CGEN_KR_GENERATE_AUDIO_INFO:
      return "kr_generate_audio_info";
    case CGEN_KR_VIDEO_GENERATE_TYPE:
      return "kr_video_generate_type";
    case CGEN_KR_GENERATE_COLOR_INFO:
      return "kr_generate_color_info";
    case CGEN_KR_GENERATE_VIDEO_INFO:
      return "kr_generate_video_info";
    case CGEN_KR_GENERATE_VIDEO_INFO_SUB1:
      return "kr_generate_video_info_sub1";
    case CGEN_KR_GENERATE_TYPE:
      return "kr_generate_type";
    case CGEN_KR_GENERATE_INFO:
      return "kr_generate_info";
    case CGEN_KR_GENERATE_INFO_SUB1:
      return "kr_generate_info_sub1";
    case CGEN_KR_STUDIO_PATH_TYPE:
      return "kr_studio_path_type";
    case CGEN_KR_STUDIO_PATH_INFO:
      return "kr_studio_path_info";
    case CGEN_KR_STUDIO_PATH_INFO_SUB1:
      return "kr_studio_path_info_sub1";
    case CGEN_KR_TRACK_BLOCK_TYPE:
      return "kr_track_block_type";
    case CGEN_KR_TRACK_MODE:
      return "kr_track_mode";
    case CGEN_KR_TRACK_EVENT_TYPE:
      return "kr_track_event_type";
    case CGEN_KR_TRACK_INFO:
      return "kr_track_info";
    case CGEN_KR_TRACK_INFO_SUB1:
      return "kr_track_info_sub1";
    case CGEN_KR_TRACK_BLOCK_INFO:
      return "kr_track_block_info";
    case CGEN_KR_HLS_PLAYLIST_TYPE:
      return "kr_hls_playlist_type";
    case CGEN_KR_HLS_RENDITION_TYPE:
      return "kr_hls_rendition_type";
    case CGEN_KR_HLS_MEDIA_PLAYLIST_TYPE:
      return "kr_hls_media_playlist_type";
    case CGEN_KR_HLS_VARIANT:
      return "kr_hls_variant";
    case CGEN_KR_HLS_RENDITION:
      return "kr_hls_rendition";
    case CGEN_KR_HLS_MASTER_PLAYLIST:
      return "kr_hls_master_playlist";
    case CGEN_KR_HLS_MEDIA_SEGMENT:
      return "kr_hls_media_segment";
    case CGEN_KR_HLS_MEDIA_PLAYLIST:
      return "kr_hls_media_playlist";
    case CGEN_KR_HLS_PLAYLIST:
      return "kr_hls_playlist";
    case CGEN_KR_HLS_PLAYLIST_SUB1:
      return "kr_hls_playlist_sub1";
    case CGEN_KR_HLS_TUNER_INFO:
      return "kr_hls_tuner_info";
    case CGEN_KR_FPS_INFO:
      return "kr_fps_info";
    case CGEN_KR_RADIO_MODE:
      return "kr_radio_mode";
    case CGEN_KR_RADIO_INFO:
      return "kr_radio_info";
    case CGEN_KR_WEB_CLIENT_PROTOCOL:
      return "kr_web_client_protocol";
    case CGEN_KR_WEB_CLIENT_TYPE:
      return "kr_web_client_type";
    case CGEN_KR_WEB_CLIENT_STATE:
      return "kr_web_client_state";
    case CGEN_KR_WEB_SERVER_TYPE:
      return "kr_web_server_type";
    case CGEN_KR_MULTIPART_STATE:
      return "kr_multipart_state";
    case CGEN_KR_WEB_SERVER_INFO:
      return "kr_web_server_info";
    case CGEN_KR_WEB_SERVER_SETUP_INFO:
      return "kr_web_server_setup_info";
    case CGEN_KR_WEB_REMOTE_INFO:
      return "kr_web_remote_info";
    case CGEN_KR_WEB_SOCKET_IO_STATE:
      return "kr_web_socket_io_state";
    case CGEN_KR_WEBSOCKET_FRAME_TYPE:
      return "kr_websocket_frame_type";
    case CGEN_KR_WEBSOCKET_PROTOCOL:
      return "kr_websocket_protocol";
    case CGEN_KR_WEB_SOCKET_STATE:
      return "kr_web_socket_state";
    case CGEN_KR_HTTP_MESSAGE_STATE:
      return "kr_http_message_state";
    case CGEN_KR_WEB_HTTP_IO_STATE:
      return "kr_web_http_io_state";
    case CGEN_KR_WEB_HTTP_STATE:
      return "kr_web_http_state";
    case CGEN_KR_WEB_CLIENT_INFO:
      return "kr_web_client_info";
    case CGEN_KR_WEB_CLIENT_INFO_SUB1:
      return "kr_web_client_info_sub1";
    case CGEN_KR_WEB_CLIENT_SETUP_INFO:
      return "kr_web_client_setup_info";
    case CGEN_KR_WEB_PATH_TYPE:
      return "kr_web_path_type";
    case CGEN_KR_WEB_PATH_INFO:
      return "kr_web_path_info";
    case CGEN_KR_WEB_PATH_INFO_SUB1:
      return "kr_web_path_info_sub1";
    case CGEN_KR_WEB_PATH_SETUP_INFO:
      return "kr_web_path_setup_info";
    case CGEN_KR_WEB_PATH_SETUP_INFO_SUB1:
      return "kr_web_path_setup_info_sub1";
    case CGEN_KR_PATH_INFO:
      return "kr_path_info";
    case CGEN_KR_PATH_LIST:
      return "kr_path_list";
    case CGEN_KR_UPLOAD_TYPE:
      return "kr_upload_type";
    case CGEN_KR_DOWNLOAD_TYPE:
      return "kr_download_type";
    case CGEN_KR_UPLOAD_FILE_INFO:
      return "kr_upload_file_info";
    case CGEN_KR_UPLOAD_STASH_INFO:
      return "kr_upload_stash_info";
    case CGEN_KR_DOWNLOAD_FILE_INFO:
      return "kr_download_file_info";
    case CGEN_KR_DOWNLOAD_STASH_INFO:
      return "kr_download_stash_info";
    case CGEN_KR_UPLOAD_INFO:
      return "kr_upload_info";
    case CGEN_KR_UPLOAD_INFO_SUB1:
      return "kr_upload_info_sub1";
    case CGEN_KR_DOWNLOAD_INFO:
      return "kr_download_info";
    case CGEN_KR_DOWNLOAD_INFO_SUB1:
      return "kr_download_info_sub1";
    case CGEN_KR_ACCEPT_INFO:
      return "kr_accept_info";
    case CGEN_KR_SERVE_RANGE_INFO:
      return "kr_serve_range_info";
    case CGEN_KR_SERVE_RANGES_INFO:
      return "kr_serve_ranges_info";
    case CGEN_KR_SERVE_FULL_INFO:
      return "kr_serve_full_info";
    case CGEN_KR_SERVE_INDEX_FORMAT:
      return "kr_serve_index_format";
    case CGEN_KR_SERVE_INDEX_INFO:
      return "kr_serve_index_info";
    case CGEN_KR_SERVE_REDIRECT_INFO:
      return "kr_serve_redirect_info";
    case CGEN_KR_SERVE_TYPE:
      return "kr_serve_type";
    case CGEN_KR_SERVE_INFO:
      return "kr_serve_info";
    case CGEN_KR_SERVE_INFO_SUB1:
      return "kr_serve_info_sub1";
    case CGEN_KR_DOWNLINK_INFO:
      return "kr_downlink_info";
    case CGEN_KR_UPLINK_INFO:
      return "kr_uplink_info";
    case CGEN_KR_SOURCE_INFO:
      return "kr_source_info";
    case CGEN_KR_TRANSMIT_INFO:
      return "kr_transmit_info";
    case CGEN_KR_XFER_CLIENT_TYPE:
      return "kr_xfer_client_type";
    case CGEN_KR_XFER_RECONNECT_INFO:
      return "kr_xfer_reconnect_info";
    case CGEN_KR_XFER_CLIENT_INFO:
      return "kr_xfer_client_info";
    case CGEN_KR_XFER_CLIENT_INFO_SUB1:
      return "kr_xfer_client_info_sub1";
    case CGEN_KR_XFER_ACCEPTOR_DIR_INFO:
      return "kr_xfer_acceptor_dir_info";
    case CGEN_KR_XFER_ACCEPTOR_FILE_INFO:
      return "kr_xfer_acceptor_file_info";
    case CGEN_KR_XFER_ACCEPTOR_BUF_INFO:
      return "kr_xfer_acceptor_buf_info";
    case CGEN_KR_XFER_SHARE_DIR_INFO:
      return "kr_xfer_share_dir_info";
    case CGEN_KR_XFER_SHARE_FILE_INFO:
      return "kr_xfer_share_file_info";
    case CGEN_KR_XFER_SHARE_BUF_INFO:
      return "kr_xfer_share_buf_info";
    case CGEN_KR_XFER_ACCEPTOR_TYPE:
      return "kr_xfer_acceptor_type";
    case CGEN_KR_XFER_ACCEPTOR_INFO:
      return "kr_xfer_acceptor_info";
    case CGEN_KR_XFER_ACCEPTOR_INFO_SUB1:
      return "kr_xfer_acceptor_info_sub1";
    case CGEN_KR_XFER_RECEIVER_INFO:
      return "kr_xfer_receiver_info";
    case CGEN_KR_XFER_TRANSMITTER_TYPE:
      return "kr_xfer_transmitter_type";
    case CGEN_KR_XFER_TRANSMITTER_INFO:
      return "kr_xfer_transmitter_info";
    case CGEN_KR_XFER_SHARE_TYPE:
      return "kr_xfer_share_type";
    case CGEN_KR_XFER_SHARE_INFO:
      return "kr_xfer_share_info";
    case CGEN_KR_XFER_SHARE_INFO_SUB1:
      return "kr_xfer_share_info_sub1";
    case CGEN_KR_XFER_SERVER_TYPE:
      return "kr_xfer_server_type";
    case CGEN_KR_XFER_SERVER_INFO:
      return "kr_xfer_server_info";
    case CGEN_KR_XFER_SERVER_INFO_SUB1:
      return "kr_xfer_server_info_sub1";
    case CGEN_KR_XFER_PATH_TYPE:
      return "kr_xfer_path_type";
    case CGEN_KR_XFER_PATH_INFO:
      return "kr_xfer_path_info";
    case CGEN_KR_XFER_PATH_INFO_SUB1:
      return "kr_xfer_path_info_sub1";
    case CGEN_KR_XPDR_TYPE:
      return "kr_xpdr_type";
    case CGEN_KR_XPDR_PATH_INFO:
      return "kr_xpdr_path_info";
    case CGEN_KR_XPDR_PATH_INFO_SUB1:
      return "kr_xpdr_path_info_sub1";
  }
  return "Unknown";
}

cgen_enum codegen_string_to_enum(char *string) {
  if (!strcmp(string,"kr_alsa_info")) {
    return CGEN_KR_ALSA_INFO;
  }
  if (!strcmp(string,"kr_alsa_path_info")) {
    return CGEN_KR_ALSA_PATH_INFO;
  }
  if (!strcmp(string,"kr_decklink_video_conn_type")) {
    return CGEN_KR_DECKLINK_VIDEO_CONN_TYPE;
  }
  if (!strcmp(string,"kr_decklink_audio_conn_type")) {
    return CGEN_KR_DECKLINK_AUDIO_CONN_TYPE;
  }
  if (!strcmp(string,"kr_decklink_info")) {
    return CGEN_KR_DECKLINK_INFO;
  }
  if (!strcmp(string,"kr_decklink_path_info")) {
    return CGEN_KR_DECKLINK_PATH_INFO;
  }
  if (!strcmp(string,"kr_v4l2_state")) {
    return CGEN_KR_V4L2_STATE;
  }
  if (!strcmp(string,"kr_v4l2_path_info")) {
    return CGEN_KR_V4L2_PATH_INFO;
  }
  if (!strcmp(string,"kr_v4l2_info")) {
    return CGEN_KR_V4L2_INFO;
  }
  if (!strcmp(string,"kr_aux_port_state")) {
    return CGEN_KR_AUX_PORT_STATE;
  }
  if (!strcmp(string,"kr_aux_info")) {
    return CGEN_KR_AUX_INFO;
  }
  if (!strcmp(string,"kr_aux_path_info")) {
    return CGEN_KR_AUX_PATH_INFO;
  }
  if (!strcmp(string,"kr_jack_direction")) {
    return CGEN_KR_JACK_DIRECTION;
  }
  if (!strcmp(string,"kr_jack_state")) {
    return CGEN_KR_JACK_STATE;
  }
  if (!strcmp(string,"kr_jack_path_state")) {
    return CGEN_KR_JACK_PATH_STATE;
  }
  if (!strcmp(string,"kr_jack_info")) {
    return CGEN_KR_JACK_INFO;
  }
  if (!strcmp(string,"kr_jack_path_info")) {
    return CGEN_KR_JACK_PATH_INFO;
  }
  if (!strcmp(string,"kr_wayland_state")) {
    return CGEN_KR_WAYLAND_STATE;
  }
  if (!strcmp(string,"kr_wayland_info")) {
    return CGEN_KR_WAYLAND_INFO;
  }
  if (!strcmp(string,"kr_wayland_path_info")) {
    return CGEN_KR_WAYLAND_PATH_INFO;
  }
  if (!strcmp(string,"kr_x11_info")) {
    return CGEN_KR_X11_INFO;
  }
  if (!strcmp(string,"kr_x11_path_info")) {
    return CGEN_KR_X11_PATH_INFO;
  }
  if (!strcmp(string,"kr_afx_effect_type")) {
    return CGEN_KR_AFX_EFFECT_TYPE;
  }
  if (!strcmp(string,"kr_afx_effect_control")) {
    return CGEN_KR_AFX_EFFECT_CONTROL;
  }
  if (!strcmp(string,"kr_eq_band_info")) {
    return CGEN_KR_EQ_BAND_INFO;
  }
  if (!strcmp(string,"kr_eq_info")) {
    return CGEN_KR_EQ_INFO;
  }
  if (!strcmp(string,"kr_lowpass_info")) {
    return CGEN_KR_LOWPASS_INFO;
  }
  if (!strcmp(string,"kr_highpass_info")) {
    return CGEN_KR_HIGHPASS_INFO;
  }
  if (!strcmp(string,"kr_pass_info")) {
    return CGEN_KR_PASS_INFO;
  }
  if (!strcmp(string,"kr_volume_info")) {
    return CGEN_KR_VOLUME_INFO;
  }
  if (!strcmp(string,"kr_analog_info")) {
    return CGEN_KR_ANALOG_INFO;
  }
  if (!strcmp(string,"kr_meter_info")) {
    return CGEN_KR_METER_INFO;
  }
  if (!strcmp(string,"kr_afx_info")) {
    return CGEN_KR_AFX_INFO;
  }
  if (!strcmp(string,"kr_video_codec")) {
    return CGEN_KR_VIDEO_CODEC;
  }
  if (!strcmp(string,"kr_audio_codec")) {
    return CGEN_KR_AUDIO_CODEC;
  }
  if (!strcmp(string,"kr_codec_type")) {
    return CGEN_KR_CODEC_TYPE;
  }
  if (!strcmp(string,"kr_codec")) {
    return CGEN_KR_CODEC;
  }
  if (!strcmp(string,"kr_codec_sub1")) {
    return CGEN_KR_CODEC_SUB1;
  }
  if (!strcmp(string,"kr_audio_info")) {
    return CGEN_KR_AUDIO_INFO;
  }
  if (!strcmp(string,"kr_video_info")) {
    return CGEN_KR_VIDEO_INFO;
  }
  if (!strcmp(string,"kr_av_info")) {
    return CGEN_KR_AV_INFO;
  }
  if (!strcmp(string,"kr_av_info_sub1")) {
    return CGEN_KR_AV_INFO_SUB1;
  }
  if (!strcmp(string,"kr_daala_encoder_info")) {
    return CGEN_KR_DAALA_ENCODER_INFO;
  }
  if (!strcmp(string,"kr_vpx_encoder_info")) {
    return CGEN_KR_VPX_ENCODER_INFO;
  }
  if (!strcmp(string,"kr_h264_encoder_info")) {
    return CGEN_KR_H264_ENCODER_INFO;
  }
  if (!strcmp(string,"kr_theora_encoder_info")) {
    return CGEN_KR_THEORA_ENCODER_INFO;
  }
  if (!strcmp(string,"kr_y4m_encoder_info")) {
    return CGEN_KR_Y4M_ENCODER_INFO;
  }
  if (!strcmp(string,"kr_flac_info")) {
    return CGEN_KR_FLAC_INFO;
  }
  if (!strcmp(string,"kr_opus_app")) {
    return CGEN_KR_OPUS_APP;
  }
  if (!strcmp(string,"kr_opus_info")) {
    return CGEN_KR_OPUS_INFO;
  }
  if (!strcmp(string,"kr_vorbis_info")) {
    return CGEN_KR_VORBIS_INFO;
  }
  if (!strcmp(string,"kr_mp3_info")) {
    return CGEN_KR_MP3_INFO;
  }
  if (!strcmp(string,"kr_aac_info")) {
    return CGEN_KR_AAC_INFO;
  }
  if (!strcmp(string,"kr_lavc_encoder_info")) {
    return CGEN_KR_LAVC_ENCODER_INFO;
  }
  if (!strcmp(string,"kr_compositor_path_type")) {
    return CGEN_KR_COMPOSITOR_PATH_TYPE;
  }
  if (!strcmp(string,"kr_compositor_overlay_type")) {
    return CGEN_KR_COMPOSITOR_OVERLAY_TYPE;
  }
  if (!strcmp(string,"kr_compositor_output_info")) {
    return CGEN_KR_COMPOSITOR_OUTPUT_INFO;
  }
  if (!strcmp(string,"kr_compositor_bus_info")) {
    return CGEN_KR_COMPOSITOR_BUS_INFO;
  }
  if (!strcmp(string,"kr_compositor_input_info")) {
    return CGEN_KR_COMPOSITOR_INPUT_INFO;
  }
  if (!strcmp(string,"kr_compositor_source_info")) {
    return CGEN_KR_COMPOSITOR_SOURCE_INFO;
  }
  if (!strcmp(string,"kr_compositor_overlay_info")) {
    return CGEN_KR_COMPOSITOR_OVERLAY_INFO;
  }
  if (!strcmp(string,"kr_compositor_overlay_info_sub1")) {
    return CGEN_KR_COMPOSITOR_OVERLAY_INFO_SUB1;
  }
  if (!strcmp(string,"kr_compositor_path_info")) {
    return CGEN_KR_COMPOSITOR_PATH_INFO;
  }
  if (!strcmp(string,"kr_compositor_path_info_sub1")) {
    return CGEN_KR_COMPOSITOR_PATH_INFO_SUB1;
  }
  if (!strcmp(string,"kr_mts_stream_type")) {
    return CGEN_KR_MTS_STREAM_TYPE;
  }
  if (!strcmp(string,"kr_mts_packet_info")) {
    return CGEN_KR_MTS_PACKET_INFO;
  }
  if (!strcmp(string,"kr_mts_stream_info")) {
    return CGEN_KR_MTS_STREAM_INFO;
  }
  if (!strcmp(string,"kr_mts_stream_info_sub1")) {
    return CGEN_KR_MTS_STREAM_INFO_SUB1;
  }
  if (!strcmp(string,"kr_mts_program_info")) {
    return CGEN_KR_MTS_PROGRAM_INFO;
  }
  if (!strcmp(string,"kr_mts_info")) {
    return CGEN_KR_MTS_INFO;
  }
  if (!strcmp(string,"kr_container_type")) {
    return CGEN_KR_CONTAINER_TYPE;
  }
  if (!strcmp(string,"kr_text_info")) {
    return CGEN_KR_TEXT_INFO;
  }
  if (!strcmp(string,"kr_vector_type")) {
    return CGEN_KR_VECTOR_TYPE;
  }
  if (!strcmp(string,"kr_vector_info")) {
    return CGEN_KR_VECTOR_INFO;
  }
  if (!strcmp(string,"kr_feature_aspect_type")) {
    return CGEN_KR_FEATURE_ASPECT_TYPE;
  }
  if (!strcmp(string,"kr_feature_aspect")) {
    return CGEN_KR_FEATURE_ASPECT;
  }
  if (!strcmp(string,"kr_feature")) {
    return CGEN_KR_FEATURE;
  }
  if (!strcmp(string,"kr_image_stash_item_info")) {
    return CGEN_KR_IMAGE_STASH_ITEM_INFO;
  }
  if (!strcmp(string,"kr_image_stash_item_info_sub1")) {
    return CGEN_KR_IMAGE_STASH_ITEM_INFO_SUB1;
  }
  if (!strcmp(string,"kr_pixel_fmt")) {
    return CGEN_KR_PIXEL_FMT;
  }
  if (!strcmp(string,"kr_image_type")) {
    return CGEN_KR_IMAGE_TYPE;
  }
  if (!strcmp(string,"kr_rect")) {
    return CGEN_KR_RECT;
  }
  if (!strcmp(string,"kr_image_info")) {
    return CGEN_KR_IMAGE_INFO;
  }
  if (!strcmp(string,"kr_coded_image_type")) {
    return CGEN_KR_CODED_IMAGE_TYPE;
  }
  if (!strcmp(string,"kr_coded_image_info")) {
    return CGEN_KR_CODED_IMAGE_INFO;
  }
  if (!strcmp(string,"kr_file_access_mode")) {
    return CGEN_KR_FILE_ACCESS_MODE;
  }
  if (!strcmp(string,"kr_file2_info")) {
    return CGEN_KR_FILE2_INFO;
  }
  if (!strcmp(string,"kr_mixer_channels")) {
    return CGEN_KR_MIXER_CHANNELS;
  }
  if (!strcmp(string,"kr_mixer_path_type")) {
    return CGEN_KR_MIXER_PATH_TYPE;
  }
  if (!strcmp(string,"kr_mixer_path_info")) {
    return CGEN_KR_MIXER_PATH_INFO;
  }
  if (!strcmp(string,"kr_media_event_type")) {
    return CGEN_KR_MEDIA_EVENT_TYPE;
  }
  if (!strcmp(string,"kr_media_item_type")) {
    return CGEN_KR_MEDIA_ITEM_TYPE;
  }
  if (!strcmp(string,"kr_media_file_info")) {
    return CGEN_KR_MEDIA_FILE_INFO;
  }
  if (!strcmp(string,"kr_media_metadata_info")) {
    return CGEN_KR_MEDIA_METADATA_INFO;
  }
  if (!strcmp(string,"kr_media_url_info")) {
    return CGEN_KR_MEDIA_URL_INFO;
  }
  if (!strcmp(string,"kr_media_item_info")) {
    return CGEN_KR_MEDIA_ITEM_INFO;
  }
  if (!strcmp(string,"kr_media_item_info_sub1")) {
    return CGEN_KR_MEDIA_ITEM_INFO_SUB1;
  }
  if (!strcmp(string,"kr_media_info")) {
    return CGEN_KR_MEDIA_INFO;
  }
  if (!strcmp(string,"kr_ops_info")) {
    return CGEN_KR_OPS_INFO;
  }
  if (!strcmp(string,"kr_pos")) {
    return CGEN_KR_POS;
  }
  if (!strcmp(string,"kr_perspective_view")) {
    return CGEN_KR_PERSPECTIVE_VIEW;
  }
  if (!strcmp(string,"m3u_el_type")) {
    return CGEN_M3U_EL_TYPE;
  }
  if (!strcmp(string,"kr_m3u_tag_name")) {
    return CGEN_KR_M3U_TAG_NAME;
  }
  if (!strcmp(string,"kr_m3u_tag_val_type")) {
    return CGEN_KR_M3U_TAG_VAL_TYPE;
  }
  if (!strcmp(string,"kr_m3u_tag_attr_name")) {
    return CGEN_KR_M3U_TAG_ATTR_NAME;
  }
  if (!strcmp(string,"kr_m3u_tag_prim_info")) {
    return CGEN_KR_M3U_TAG_PRIM_INFO;
  }
  if (!strcmp(string,"kr_m3u_tag_alist_info")) {
    return CGEN_KR_M3U_TAG_ALIST_INFO;
  }
  if (!strcmp(string,"m3u_tag_info")) {
    return CGEN_M3U_TAG_INFO;
  }
  if (!strcmp(string,"m3u_tag_info_sub1")) {
    return CGEN_M3U_TAG_INFO_SUB1;
  }
  if (!strcmp(string,"m3u_el_info")) {
    return CGEN_M3U_EL_INFO;
  }
  if (!strcmp(string,"m3u_el_info_sub1")) {
    return CGEN_M3U_EL_INFO_SUB1;
  }
  if (!strcmp(string,"kr_m3u_info")) {
    return CGEN_KR_M3U_INFO;
  }
  if (!strcmp(string,"kr_limit_info")) {
    return CGEN_KR_LIMIT_INFO;
  }
  if (!strcmp(string,"kr_limits_info")) {
    return CGEN_KR_LIMITS_INFO;
  }
  if (!strcmp(string,"kr_priority")) {
    return CGEN_KR_PRIORITY;
  }
  if (!strcmp(string,"kr_avconv_status_info")) {
    return CGEN_KR_AVCONV_STATUS_INFO;
  }
  if (!strcmp(string,"kr_avconv_params_info")) {
    return CGEN_KR_AVCONV_PARAMS_INFO;
  }
  if (!strcmp(string,"kr_avconv_info")) {
    return CGEN_KR_AVCONV_INFO;
  }
  if (!strcmp(string,"kr_avprobe_codec_type")) {
    return CGEN_KR_AVPROBE_CODEC_TYPE;
  }
  if (!strcmp(string,"kr_avprobe_format_info")) {
    return CGEN_KR_AVPROBE_FORMAT_INFO;
  }
  if (!strcmp(string,"kr_avprobe_stream_info")) {
    return CGEN_KR_AVPROBE_STREAM_INFO;
  }
  if (!strcmp(string,"kr_avprobe_media_info")) {
    return CGEN_KR_AVPROBE_MEDIA_INFO;
  }
  if (!strcmp(string,"kr_avprobe_params_info")) {
    return CGEN_KR_AVPROBE_PARAMS_INFO;
  }
  if (!strcmp(string,"kr_avprobe_status_info")) {
    return CGEN_KR_AVPROBE_STATUS_INFO;
  }
  if (!strcmp(string,"kr_avprobe_info")) {
    return CGEN_KR_AVPROBE_INFO;
  }
  if (!strcmp(string,"kr_curl_status_info")) {
    return CGEN_KR_CURL_STATUS_INFO;
  }
  if (!strcmp(string,"kr_curl_params_info")) {
    return CGEN_KR_CURL_PARAMS_INFO;
  }
  if (!strcmp(string,"kr_curl_info")) {
    return CGEN_KR_CURL_INFO;
  }
  if (!strcmp(string,"kr_livestreamer_params_info")) {
    return CGEN_KR_LIVESTREAMER_PARAMS_INFO;
  }
  if (!strcmp(string,"kr_livestreamer_status_info")) {
    return CGEN_KR_LIVESTREAMER_STATUS_INFO;
  }
  if (!strcmp(string,"kr_livestreamer_info")) {
    return CGEN_KR_LIVESTREAMER_INFO;
  }
  if (!strcmp(string,"kr_media_version")) {
    return CGEN_KR_MEDIA_VERSION;
  }
  if (!strcmp(string,"kr_quvi_media_info")) {
    return CGEN_KR_QUVI_MEDIA_INFO;
  }
  if (!strcmp(string,"kr_quvi_status_info")) {
    return CGEN_KR_QUVI_STATUS_INFO;
  }
  if (!strcmp(string,"kr_quvi_params_info")) {
    return CGEN_KR_QUVI_PARAMS_INFO;
  }
  if (!strcmp(string,"kr_quvi_info")) {
    return CGEN_KR_QUVI_INFO;
  }
  if (!strcmp(string,"kr_transmission_cli_status_info")) {
    return CGEN_KR_TRANSMISSION_CLI_STATUS_INFO;
  }
  if (!strcmp(string,"kr_transmission_cli_params_info")) {
    return CGEN_KR_TRANSMISSION_CLI_PARAMS_INFO;
  }
  if (!strcmp(string,"kr_transmission_cli_info")) {
    return CGEN_KR_TRANSMISSION_CLI_INFO;
  }
  if (!strcmp(string,"kr_task_type")) {
    return CGEN_KR_TASK_TYPE;
  }
  if (!strcmp(string,"kr_task_state")) {
    return CGEN_KR_TASK_STATE;
  }
  if (!strcmp(string,"kr_task_event_type")) {
    return CGEN_KR_TASK_EVENT_TYPE;
  }
  if (!strcmp(string,"kr_fail_info")) {
    return CGEN_KR_FAIL_INFO;
  }
  if (!strcmp(string,"kr_task_status")) {
    return CGEN_KR_TASK_STATUS;
  }
  if (!strcmp(string,"kr_task_params_info")) {
    return CGEN_KR_TASK_PARAMS_INFO;
  }
  if (!strcmp(string,"kr_task_params_info_sub1")) {
    return CGEN_KR_TASK_PARAMS_INFO_SUB1;
  }
  if (!strcmp(string,"kr_task_info")) {
    return CGEN_KR_TASK_INFO;
  }
  if (!strcmp(string,"kr_task_info_sub1")) {
    return CGEN_KR_TASK_INFO_SUB1;
  }
  if (!strcmp(string,"kr_valgrind_stressor_status_info")) {
    return CGEN_KR_VALGRIND_STRESSOR_STATUS_INFO;
  }
  if (!strcmp(string,"kr_valgrind_stressor_params_info")) {
    return CGEN_KR_VALGRIND_STRESSOR_PARAMS_INFO;
  }
  if (!strcmp(string,"kr_valgrind_stressor_info")) {
    return CGEN_KR_VALGRIND_STRESSOR_INFO;
  }
  if (!strcmp(string,"kr_xmms_params_info")) {
    return CGEN_KR_XMMS_PARAMS_INFO;
  }
  if (!strcmp(string,"kr_xmms_status_info")) {
    return CGEN_KR_XMMS_STATUS_INFO;
  }
  if (!strcmp(string,"kr_xmms_info")) {
    return CGEN_KR_XMMS_INFO;
  }
  if (!strcmp(string,"kr_ydl_format_info")) {
    return CGEN_KR_YDL_FORMAT_INFO;
  }
  if (!strcmp(string,"kr_ydl_video_info")) {
    return CGEN_KR_YDL_VIDEO_INFO;
  }
  if (!strcmp(string,"kr_ydl_params_info")) {
    return CGEN_KR_YDL_PARAMS_INFO;
  }
  if (!strcmp(string,"kr_ydl_status_info")) {
    return CGEN_KR_YDL_STATUS_INFO;
  }
  if (!strcmp(string,"kr_ydl_info")) {
    return CGEN_KR_YDL_INFO;
  }
  if (!strcmp(string,"kr_timestamp_info")) {
    return CGEN_KR_TIMESTAMP_INFO;
  }
  if (!strcmp(string,"kr_easing")) {
    return CGEN_KR_EASING;
  }
  if (!strcmp(string,"kr_media_source_format")) {
    return CGEN_KR_MEDIA_SOURCE_FORMAT;
  }
  if (!strcmp(string,"kr_media_source_info")) {
    return CGEN_KR_MEDIA_SOURCE_INFO;
  }
  if (!strcmp(string,"kr_media_packet_type")) {
    return CGEN_KR_MEDIA_PACKET_TYPE;
  }
  if (!strcmp(string,"kr_media_packet")) {
    return CGEN_KR_MEDIA_PACKET;
  }
  if (!strcmp(string,"kr_track_name")) {
    return CGEN_KR_TRACK_NAME;
  }
  if (!strcmp(string,"kr_mux_name")) {
    return CGEN_KR_MUX_NAME;
  }
  if (!strcmp(string,"kr_demux_info")) {
    return CGEN_KR_DEMUX_INFO;
  }
  if (!strcmp(string,"kr_mux_info")) {
    return CGEN_KR_MUX_INFO;
  }
  if (!strcmp(string,"kr_video_encode_info")) {
    return CGEN_KR_VIDEO_ENCODE_INFO;
  }
  if (!strcmp(string,"kr_video_encode_info_sub1")) {
    return CGEN_KR_VIDEO_ENCODE_INFO_SUB1;
  }
  if (!strcmp(string,"kr_audio_encode_info")) {
    return CGEN_KR_AUDIO_ENCODE_INFO;
  }
  if (!strcmp(string,"kr_audio_encode_info_sub1")) {
    return CGEN_KR_AUDIO_ENCODE_INFO_SUB1;
  }
  if (!strcmp(string,"kr_encode_type")) {
    return CGEN_KR_ENCODE_TYPE;
  }
  if (!strcmp(string,"kr_encode_info")) {
    return CGEN_KR_ENCODE_INFO;
  }
  if (!strcmp(string,"kr_encode_info_sub1")) {
    return CGEN_KR_ENCODE_INFO_SUB1;
  }
  if (!strcmp(string,"kr_capture_type")) {
    return CGEN_KR_CAPTURE_TYPE;
  }
  if (!strcmp(string,"kr_record_info")) {
    return CGEN_KR_RECORD_INFO;
  }
  if (!strcmp(string,"kr_transmission_info")) {
    return CGEN_KR_TRANSMISSION_INFO;
  }
  if (!strcmp(string,"kr_output_type")) {
    return CGEN_KR_OUTPUT_TYPE;
  }
  if (!strcmp(string,"kr_output_info")) {
    return CGEN_KR_OUTPUT_INFO;
  }
  if (!strcmp(string,"kr_output_info_sub1")) {
    return CGEN_KR_OUTPUT_INFO_SUB1;
  }
  if (!strcmp(string,"kr_capture_audio_info")) {
    return CGEN_KR_CAPTURE_AUDIO_INFO;
  }
  if (!strcmp(string,"kr_capture_video_info")) {
    return CGEN_KR_CAPTURE_VIDEO_INFO;
  }
  if (!strcmp(string,"kr_capture_image_info")) {
    return CGEN_KR_CAPTURE_IMAGE_INFO;
  }
  if (!strcmp(string,"kr_capture_info")) {
    return CGEN_KR_CAPTURE_INFO;
  }
  if (!strcmp(string,"kr_capture_info_sub1")) {
    return CGEN_KR_CAPTURE_INFO_SUB1;
  }
  if (!strcmp(string,"kr_source_file_info")) {
    return CGEN_KR_SOURCE_FILE_INFO;
  }
  if (!strcmp(string,"kr_source_remote_file_info")) {
    return CGEN_KR_SOURCE_REMOTE_FILE_INFO;
  }
  if (!strcmp(string,"kr_studio_source_stream_type")) {
    return CGEN_KR_STUDIO_SOURCE_STREAM_TYPE;
  }
  if (!strcmp(string,"kr_source_stream_info")) {
    return CGEN_KR_SOURCE_STREAM_INFO;
  }
  if (!strcmp(string,"kr_studio_source_type")) {
    return CGEN_KR_STUDIO_SOURCE_TYPE;
  }
  if (!strcmp(string,"kr_studio_source_info")) {
    return CGEN_KR_STUDIO_SOURCE_INFO;
  }
  if (!strcmp(string,"kr_studio_source_info_sub1")) {
    return CGEN_KR_STUDIO_SOURCE_INFO_SUB1;
  }
  if (!strcmp(string,"kr_decode_info")) {
    return CGEN_KR_DECODE_INFO;
  }
  if (!strcmp(string,"kr_play_info")) {
    return CGEN_KR_PLAY_INFO;
  }
  if (!strcmp(string,"kr_render_image_info")) {
    return CGEN_KR_RENDER_IMAGE_INFO;
  }
  if (!strcmp(string,"kr_render_video_info")) {
    return CGEN_KR_RENDER_VIDEO_INFO;
  }
  if (!strcmp(string,"kr_render_audio_info")) {
    return CGEN_KR_RENDER_AUDIO_INFO;
  }
  if (!strcmp(string,"kr_render_type")) {
    return CGEN_KR_RENDER_TYPE;
  }
  if (!strcmp(string,"kr_render_info")) {
    return CGEN_KR_RENDER_INFO;
  }
  if (!strcmp(string,"kr_render_info_sub1")) {
    return CGEN_KR_RENDER_INFO_SUB1;
  }
  if (!strcmp(string,"kr_audio_generate_type")) {
    return CGEN_KR_AUDIO_GENERATE_TYPE;
  }
  if (!strcmp(string,"kr_generate_audio_info")) {
    return CGEN_KR_GENERATE_AUDIO_INFO;
  }
  if (!strcmp(string,"kr_video_generate_type")) {
    return CGEN_KR_VIDEO_GENERATE_TYPE;
  }
  if (!strcmp(string,"kr_generate_color_info")) {
    return CGEN_KR_GENERATE_COLOR_INFO;
  }
  if (!strcmp(string,"kr_generate_video_info")) {
    return CGEN_KR_GENERATE_VIDEO_INFO;
  }
  if (!strcmp(string,"kr_generate_video_info_sub1")) {
    return CGEN_KR_GENERATE_VIDEO_INFO_SUB1;
  }
  if (!strcmp(string,"kr_generate_type")) {
    return CGEN_KR_GENERATE_TYPE;
  }
  if (!strcmp(string,"kr_generate_info")) {
    return CGEN_KR_GENERATE_INFO;
  }
  if (!strcmp(string,"kr_generate_info_sub1")) {
    return CGEN_KR_GENERATE_INFO_SUB1;
  }
  if (!strcmp(string,"kr_studio_path_type")) {
    return CGEN_KR_STUDIO_PATH_TYPE;
  }
  if (!strcmp(string,"kr_studio_path_info")) {
    return CGEN_KR_STUDIO_PATH_INFO;
  }
  if (!strcmp(string,"kr_studio_path_info_sub1")) {
    return CGEN_KR_STUDIO_PATH_INFO_SUB1;
  }
  if (!strcmp(string,"kr_track_block_type")) {
    return CGEN_KR_TRACK_BLOCK_TYPE;
  }
  if (!strcmp(string,"kr_track_mode")) {
    return CGEN_KR_TRACK_MODE;
  }
  if (!strcmp(string,"kr_track_event_type")) {
    return CGEN_KR_TRACK_EVENT_TYPE;
  }
  if (!strcmp(string,"kr_track_info")) {
    return CGEN_KR_TRACK_INFO;
  }
  if (!strcmp(string,"kr_track_info_sub1")) {
    return CGEN_KR_TRACK_INFO_SUB1;
  }
  if (!strcmp(string,"kr_track_block_info")) {
    return CGEN_KR_TRACK_BLOCK_INFO;
  }
  if (!strcmp(string,"kr_hls_playlist_type")) {
    return CGEN_KR_HLS_PLAYLIST_TYPE;
  }
  if (!strcmp(string,"kr_hls_rendition_type")) {
    return CGEN_KR_HLS_RENDITION_TYPE;
  }
  if (!strcmp(string,"kr_hls_media_playlist_type")) {
    return CGEN_KR_HLS_MEDIA_PLAYLIST_TYPE;
  }
  if (!strcmp(string,"kr_hls_variant")) {
    return CGEN_KR_HLS_VARIANT;
  }
  if (!strcmp(string,"kr_hls_rendition")) {
    return CGEN_KR_HLS_RENDITION;
  }
  if (!strcmp(string,"kr_hls_master_playlist")) {
    return CGEN_KR_HLS_MASTER_PLAYLIST;
  }
  if (!strcmp(string,"kr_hls_media_segment")) {
    return CGEN_KR_HLS_MEDIA_SEGMENT;
  }
  if (!strcmp(string,"kr_hls_media_playlist")) {
    return CGEN_KR_HLS_MEDIA_PLAYLIST;
  }
  if (!strcmp(string,"kr_hls_playlist")) {
    return CGEN_KR_HLS_PLAYLIST;
  }
  if (!strcmp(string,"kr_hls_playlist_sub1")) {
    return CGEN_KR_HLS_PLAYLIST_SUB1;
  }
  if (!strcmp(string,"kr_hls_tuner_info")) {
    return CGEN_KR_HLS_TUNER_INFO;
  }
  if (!strcmp(string,"kr_fps_info")) {
    return CGEN_KR_FPS_INFO;
  }
  if (!strcmp(string,"kr_radio_mode")) {
    return CGEN_KR_RADIO_MODE;
  }
  if (!strcmp(string,"kr_radio_info")) {
    return CGEN_KR_RADIO_INFO;
  }
  if (!strcmp(string,"kr_web_client_protocol")) {
    return CGEN_KR_WEB_CLIENT_PROTOCOL;
  }
  if (!strcmp(string,"kr_web_client_type")) {
    return CGEN_KR_WEB_CLIENT_TYPE;
  }
  if (!strcmp(string,"kr_web_client_state")) {
    return CGEN_KR_WEB_CLIENT_STATE;
  }
  if (!strcmp(string,"kr_web_server_type")) {
    return CGEN_KR_WEB_SERVER_TYPE;
  }
  if (!strcmp(string,"kr_multipart_state")) {
    return CGEN_KR_MULTIPART_STATE;
  }
  if (!strcmp(string,"kr_web_server_info")) {
    return CGEN_KR_WEB_SERVER_INFO;
  }
  if (!strcmp(string,"kr_web_server_setup_info")) {
    return CGEN_KR_WEB_SERVER_SETUP_INFO;
  }
  if (!strcmp(string,"kr_web_remote_info")) {
    return CGEN_KR_WEB_REMOTE_INFO;
  }
  if (!strcmp(string,"kr_web_socket_io_state")) {
    return CGEN_KR_WEB_SOCKET_IO_STATE;
  }
  if (!strcmp(string,"kr_websocket_frame_type")) {
    return CGEN_KR_WEBSOCKET_FRAME_TYPE;
  }
  if (!strcmp(string,"kr_websocket_protocol")) {
    return CGEN_KR_WEBSOCKET_PROTOCOL;
  }
  if (!strcmp(string,"kr_web_socket_state")) {
    return CGEN_KR_WEB_SOCKET_STATE;
  }
  if (!strcmp(string,"kr_http_message_state")) {
    return CGEN_KR_HTTP_MESSAGE_STATE;
  }
  if (!strcmp(string,"kr_web_http_io_state")) {
    return CGEN_KR_WEB_HTTP_IO_STATE;
  }
  if (!strcmp(string,"kr_web_http_state")) {
    return CGEN_KR_WEB_HTTP_STATE;
  }
  if (!strcmp(string,"kr_web_client_info")) {
    return CGEN_KR_WEB_CLIENT_INFO;
  }
  if (!strcmp(string,"kr_web_client_info_sub1")) {
    return CGEN_KR_WEB_CLIENT_INFO_SUB1;
  }
  if (!strcmp(string,"kr_web_client_setup_info")) {
    return CGEN_KR_WEB_CLIENT_SETUP_INFO;
  }
  if (!strcmp(string,"kr_web_path_type")) {
    return CGEN_KR_WEB_PATH_TYPE;
  }
  if (!strcmp(string,"kr_web_path_info")) {
    return CGEN_KR_WEB_PATH_INFO;
  }
  if (!strcmp(string,"kr_web_path_info_sub1")) {
    return CGEN_KR_WEB_PATH_INFO_SUB1;
  }
  if (!strcmp(string,"kr_web_path_setup_info")) {
    return CGEN_KR_WEB_PATH_SETUP_INFO;
  }
  if (!strcmp(string,"kr_web_path_setup_info_sub1")) {
    return CGEN_KR_WEB_PATH_SETUP_INFO_SUB1;
  }
  if (!strcmp(string,"kr_path_info")) {
    return CGEN_KR_PATH_INFO;
  }
  if (!strcmp(string,"kr_path_list")) {
    return CGEN_KR_PATH_LIST;
  }
  if (!strcmp(string,"kr_upload_type")) {
    return CGEN_KR_UPLOAD_TYPE;
  }
  if (!strcmp(string,"kr_download_type")) {
    return CGEN_KR_DOWNLOAD_TYPE;
  }
  if (!strcmp(string,"kr_upload_file_info")) {
    return CGEN_KR_UPLOAD_FILE_INFO;
  }
  if (!strcmp(string,"kr_upload_stash_info")) {
    return CGEN_KR_UPLOAD_STASH_INFO;
  }
  if (!strcmp(string,"kr_download_file_info")) {
    return CGEN_KR_DOWNLOAD_FILE_INFO;
  }
  if (!strcmp(string,"kr_download_stash_info")) {
    return CGEN_KR_DOWNLOAD_STASH_INFO;
  }
  if (!strcmp(string,"kr_upload_info")) {
    return CGEN_KR_UPLOAD_INFO;
  }
  if (!strcmp(string,"kr_upload_info_sub1")) {
    return CGEN_KR_UPLOAD_INFO_SUB1;
  }
  if (!strcmp(string,"kr_download_info")) {
    return CGEN_KR_DOWNLOAD_INFO;
  }
  if (!strcmp(string,"kr_download_info_sub1")) {
    return CGEN_KR_DOWNLOAD_INFO_SUB1;
  }
  if (!strcmp(string,"kr_accept_info")) {
    return CGEN_KR_ACCEPT_INFO;
  }
  if (!strcmp(string,"kr_serve_range_info")) {
    return CGEN_KR_SERVE_RANGE_INFO;
  }
  if (!strcmp(string,"kr_serve_ranges_info")) {
    return CGEN_KR_SERVE_RANGES_INFO;
  }
  if (!strcmp(string,"kr_serve_full_info")) {
    return CGEN_KR_SERVE_FULL_INFO;
  }
  if (!strcmp(string,"kr_serve_index_format")) {
    return CGEN_KR_SERVE_INDEX_FORMAT;
  }
  if (!strcmp(string,"kr_serve_index_info")) {
    return CGEN_KR_SERVE_INDEX_INFO;
  }
  if (!strcmp(string,"kr_serve_redirect_info")) {
    return CGEN_KR_SERVE_REDIRECT_INFO;
  }
  if (!strcmp(string,"kr_serve_type")) {
    return CGEN_KR_SERVE_TYPE;
  }
  if (!strcmp(string,"kr_serve_info")) {
    return CGEN_KR_SERVE_INFO;
  }
  if (!strcmp(string,"kr_serve_info_sub1")) {
    return CGEN_KR_SERVE_INFO_SUB1;
  }
  if (!strcmp(string,"kr_downlink_info")) {
    return CGEN_KR_DOWNLINK_INFO;
  }
  if (!strcmp(string,"kr_uplink_info")) {
    return CGEN_KR_UPLINK_INFO;
  }
  if (!strcmp(string,"kr_source_info")) {
    return CGEN_KR_SOURCE_INFO;
  }
  if (!strcmp(string,"kr_transmit_info")) {
    return CGEN_KR_TRANSMIT_INFO;
  }
  if (!strcmp(string,"kr_xfer_client_type")) {
    return CGEN_KR_XFER_CLIENT_TYPE;
  }
  if (!strcmp(string,"kr_xfer_reconnect_info")) {
    return CGEN_KR_XFER_RECONNECT_INFO;
  }
  if (!strcmp(string,"kr_xfer_client_info")) {
    return CGEN_KR_XFER_CLIENT_INFO;
  }
  if (!strcmp(string,"kr_xfer_client_info_sub1")) {
    return CGEN_KR_XFER_CLIENT_INFO_SUB1;
  }
  if (!strcmp(string,"kr_xfer_acceptor_dir_info")) {
    return CGEN_KR_XFER_ACCEPTOR_DIR_INFO;
  }
  if (!strcmp(string,"kr_xfer_acceptor_file_info")) {
    return CGEN_KR_XFER_ACCEPTOR_FILE_INFO;
  }
  if (!strcmp(string,"kr_xfer_acceptor_buf_info")) {
    return CGEN_KR_XFER_ACCEPTOR_BUF_INFO;
  }
  if (!strcmp(string,"kr_xfer_share_dir_info")) {
    return CGEN_KR_XFER_SHARE_DIR_INFO;
  }
  if (!strcmp(string,"kr_xfer_share_file_info")) {
    return CGEN_KR_XFER_SHARE_FILE_INFO;
  }
  if (!strcmp(string,"kr_xfer_share_buf_info")) {
    return CGEN_KR_XFER_SHARE_BUF_INFO;
  }
  if (!strcmp(string,"kr_xfer_acceptor_type")) {
    return CGEN_KR_XFER_ACCEPTOR_TYPE;
  }
  if (!strcmp(string,"kr_xfer_acceptor_info")) {
    return CGEN_KR_XFER_ACCEPTOR_INFO;
  }
  if (!strcmp(string,"kr_xfer_acceptor_info_sub1")) {
    return CGEN_KR_XFER_ACCEPTOR_INFO_SUB1;
  }
  if (!strcmp(string,"kr_xfer_receiver_info")) {
    return CGEN_KR_XFER_RECEIVER_INFO;
  }
  if (!strcmp(string,"kr_xfer_transmitter_type")) {
    return CGEN_KR_XFER_TRANSMITTER_TYPE;
  }
  if (!strcmp(string,"kr_xfer_transmitter_info")) {
    return CGEN_KR_XFER_TRANSMITTER_INFO;
  }
  if (!strcmp(string,"kr_xfer_share_type")) {
    return CGEN_KR_XFER_SHARE_TYPE;
  }
  if (!strcmp(string,"kr_xfer_share_info")) {
    return CGEN_KR_XFER_SHARE_INFO;
  }
  if (!strcmp(string,"kr_xfer_share_info_sub1")) {
    return CGEN_KR_XFER_SHARE_INFO_SUB1;
  }
  if (!strcmp(string,"kr_xfer_server_type")) {
    return CGEN_KR_XFER_SERVER_TYPE;
  }
  if (!strcmp(string,"kr_xfer_server_info")) {
    return CGEN_KR_XFER_SERVER_INFO;
  }
  if (!strcmp(string,"kr_xfer_server_info_sub1")) {
    return CGEN_KR_XFER_SERVER_INFO_SUB1;
  }
  if (!strcmp(string,"kr_xfer_path_type")) {
    return CGEN_KR_XFER_PATH_TYPE;
  }
  if (!strcmp(string,"kr_xfer_path_info")) {
    return CGEN_KR_XFER_PATH_INFO;
  }
  if (!strcmp(string,"kr_xfer_path_info_sub1")) {
    return CGEN_KR_XFER_PATH_INFO_SUB1;
  }
  if (!strcmp(string,"kr_xpdr_type")) {
    return CGEN_KR_XPDR_TYPE;
  }
  if (!strcmp(string,"kr_xpdr_path_info")) {
    return CGEN_KR_XPDR_PATH_INFO;
  }
  if (!strcmp(string,"kr_xpdr_path_info_sub1")) {
    return CGEN_KR_XPDR_PATH_INFO_SUB1;
  }
  return 0;
}

int codegen_is_union(char *type) {
  return 0;
}

int codegen_is_enum(char *type) {
  if (!strcmp(type,"kr_decklink_video_conn_type")) {
    return CGEN_KR_DECKLINK_VIDEO_CONN_TYPE;
  }
  if (!strcmp(type,"kr_decklink_audio_conn_type")) {
    return CGEN_KR_DECKLINK_AUDIO_CONN_TYPE;
  }
  if (!strcmp(type,"kr_v4l2_state")) {
    return CGEN_KR_V4L2_STATE;
  }
  if (!strcmp(type,"kr_aux_port_state")) {
    return CGEN_KR_AUX_PORT_STATE;
  }
  if (!strcmp(type,"kr_jack_direction")) {
    return CGEN_KR_JACK_DIRECTION;
  }
  if (!strcmp(type,"kr_jack_state")) {
    return CGEN_KR_JACK_STATE;
  }
  if (!strcmp(type,"kr_jack_path_state")) {
    return CGEN_KR_JACK_PATH_STATE;
  }
  if (!strcmp(type,"kr_wayland_state")) {
    return CGEN_KR_WAYLAND_STATE;
  }
  if (!strcmp(type,"kr_afx_effect_type")) {
    return CGEN_KR_AFX_EFFECT_TYPE;
  }
  if (!strcmp(type,"kr_afx_effect_control")) {
    return CGEN_KR_AFX_EFFECT_CONTROL;
  }
  if (!strcmp(type,"kr_video_codec")) {
    return CGEN_KR_VIDEO_CODEC;
  }
  if (!strcmp(type,"kr_audio_codec")) {
    return CGEN_KR_AUDIO_CODEC;
  }
  if (!strcmp(type,"kr_codec_type")) {
    return CGEN_KR_CODEC_TYPE;
  }
  if (!strcmp(type,"kr_opus_app")) {
    return CGEN_KR_OPUS_APP;
  }
  if (!strcmp(type,"kr_compositor_path_type")) {
    return CGEN_KR_COMPOSITOR_PATH_TYPE;
  }
  if (!strcmp(type,"kr_compositor_overlay_type")) {
    return CGEN_KR_COMPOSITOR_OVERLAY_TYPE;
  }
  if (!strcmp(type,"kr_mts_stream_type")) {
    return CGEN_KR_MTS_STREAM_TYPE;
  }
  if (!strcmp(type,"kr_container_type")) {
    return CGEN_KR_CONTAINER_TYPE;
  }
  if (!strcmp(type,"kr_vector_type")) {
    return CGEN_KR_VECTOR_TYPE;
  }
  if (!strcmp(type,"kr_feature_aspect_type")) {
    return CGEN_KR_FEATURE_ASPECT_TYPE;
  }
  if (!strcmp(type,"kr_pixel_fmt")) {
    return CGEN_KR_PIXEL_FMT;
  }
  if (!strcmp(type,"kr_image_type")) {
    return CGEN_KR_IMAGE_TYPE;
  }
  if (!strcmp(type,"kr_coded_image_type")) {
    return CGEN_KR_CODED_IMAGE_TYPE;
  }
  if (!strcmp(type,"kr_file_access_mode")) {
    return CGEN_KR_FILE_ACCESS_MODE;
  }
  if (!strcmp(type,"kr_mixer_channels")) {
    return CGEN_KR_MIXER_CHANNELS;
  }
  if (!strcmp(type,"kr_mixer_path_type")) {
    return CGEN_KR_MIXER_PATH_TYPE;
  }
  if (!strcmp(type,"kr_media_event_type")) {
    return CGEN_KR_MEDIA_EVENT_TYPE;
  }
  if (!strcmp(type,"kr_media_item_type")) {
    return CGEN_KR_MEDIA_ITEM_TYPE;
  }
  if (!strcmp(type,"m3u_el_type")) {
    return CGEN_M3U_EL_TYPE;
  }
  if (!strcmp(type,"kr_m3u_tag_name")) {
    return CGEN_KR_M3U_TAG_NAME;
  }
  if (!strcmp(type,"kr_m3u_tag_val_type")) {
    return CGEN_KR_M3U_TAG_VAL_TYPE;
  }
  if (!strcmp(type,"kr_m3u_tag_attr_name")) {
    return CGEN_KR_M3U_TAG_ATTR_NAME;
  }
  if (!strcmp(type,"kr_priority")) {
    return CGEN_KR_PRIORITY;
  }
  if (!strcmp(type,"kr_avprobe_codec_type")) {
    return CGEN_KR_AVPROBE_CODEC_TYPE;
  }
  if (!strcmp(type,"kr_task_type")) {
    return CGEN_KR_TASK_TYPE;
  }
  if (!strcmp(type,"kr_task_state")) {
    return CGEN_KR_TASK_STATE;
  }
  if (!strcmp(type,"kr_task_event_type")) {
    return CGEN_KR_TASK_EVENT_TYPE;
  }
  if (!strcmp(type,"kr_easing")) {
    return CGEN_KR_EASING;
  }
  if (!strcmp(type,"kr_media_source_format")) {
    return CGEN_KR_MEDIA_SOURCE_FORMAT;
  }
  if (!strcmp(type,"kr_media_packet_type")) {
    return CGEN_KR_MEDIA_PACKET_TYPE;
  }
  if (!strcmp(type,"kr_encode_type")) {
    return CGEN_KR_ENCODE_TYPE;
  }
  if (!strcmp(type,"kr_capture_type")) {
    return CGEN_KR_CAPTURE_TYPE;
  }
  if (!strcmp(type,"kr_output_type")) {
    return CGEN_KR_OUTPUT_TYPE;
  }
  if (!strcmp(type,"kr_studio_source_stream_type")) {
    return CGEN_KR_STUDIO_SOURCE_STREAM_TYPE;
  }
  if (!strcmp(type,"kr_studio_source_type")) {
    return CGEN_KR_STUDIO_SOURCE_TYPE;
  }
  if (!strcmp(type,"kr_render_type")) {
    return CGEN_KR_RENDER_TYPE;
  }
  if (!strcmp(type,"kr_audio_generate_type")) {
    return CGEN_KR_AUDIO_GENERATE_TYPE;
  }
  if (!strcmp(type,"kr_video_generate_type")) {
    return CGEN_KR_VIDEO_GENERATE_TYPE;
  }
  if (!strcmp(type,"kr_generate_type")) {
    return CGEN_KR_GENERATE_TYPE;
  }
  if (!strcmp(type,"kr_studio_path_type")) {
    return CGEN_KR_STUDIO_PATH_TYPE;
  }
  if (!strcmp(type,"kr_track_block_type")) {
    return CGEN_KR_TRACK_BLOCK_TYPE;
  }
  if (!strcmp(type,"kr_track_mode")) {
    return CGEN_KR_TRACK_MODE;
  }
  if (!strcmp(type,"kr_track_event_type")) {
    return CGEN_KR_TRACK_EVENT_TYPE;
  }
  if (!strcmp(type,"kr_hls_playlist_type")) {
    return CGEN_KR_HLS_PLAYLIST_TYPE;
  }
  if (!strcmp(type,"kr_hls_rendition_type")) {
    return CGEN_KR_HLS_RENDITION_TYPE;
  }
  if (!strcmp(type,"kr_hls_media_playlist_type")) {
    return CGEN_KR_HLS_MEDIA_PLAYLIST_TYPE;
  }
  if (!strcmp(type,"kr_radio_mode")) {
    return CGEN_KR_RADIO_MODE;
  }
  if (!strcmp(type,"kr_web_client_protocol")) {
    return CGEN_KR_WEB_CLIENT_PROTOCOL;
  }
  if (!strcmp(type,"kr_web_client_type")) {
    return CGEN_KR_WEB_CLIENT_TYPE;
  }
  if (!strcmp(type,"kr_web_client_state")) {
    return CGEN_KR_WEB_CLIENT_STATE;
  }
  if (!strcmp(type,"kr_web_server_type")) {
    return CGEN_KR_WEB_SERVER_TYPE;
  }
  if (!strcmp(type,"kr_multipart_state")) {
    return CGEN_KR_MULTIPART_STATE;
  }
  if (!strcmp(type,"kr_websocket_frame_type")) {
    return CGEN_KR_WEBSOCKET_FRAME_TYPE;
  }
  if (!strcmp(type,"kr_websocket_protocol")) {
    return CGEN_KR_WEBSOCKET_PROTOCOL;
  }
  if (!strcmp(type,"kr_http_message_state")) {
    return CGEN_KR_HTTP_MESSAGE_STATE;
  }
  if (!strcmp(type,"kr_web_path_type")) {
    return CGEN_KR_WEB_PATH_TYPE;
  }
  if (!strcmp(type,"kr_upload_type")) {
    return CGEN_KR_UPLOAD_TYPE;
  }
  if (!strcmp(type,"kr_download_type")) {
    return CGEN_KR_DOWNLOAD_TYPE;
  }
  if (!strcmp(type,"kr_serve_index_format")) {
    return CGEN_KR_SERVE_INDEX_FORMAT;
  }
  if (!strcmp(type,"kr_serve_type")) {
    return CGEN_KR_SERVE_TYPE;
  }
  if (!strcmp(type,"kr_xfer_client_type")) {
    return CGEN_KR_XFER_CLIENT_TYPE;
  }
  if (!strcmp(type,"kr_xfer_acceptor_type")) {
    return CGEN_KR_XFER_ACCEPTOR_TYPE;
  }
  if (!strcmp(type,"kr_xfer_transmitter_type")) {
    return CGEN_KR_XFER_TRANSMITTER_TYPE;
  }
  if (!strcmp(type,"kr_xfer_share_type")) {
    return CGEN_KR_XFER_SHARE_TYPE;
  }
  if (!strcmp(type,"kr_xfer_server_type")) {
    return CGEN_KR_XFER_SERVER_TYPE;
  }
  if (!strcmp(type,"kr_xfer_path_type")) {
    return CGEN_KR_XFER_PATH_TYPE;
  }
  if (!strcmp(type,"kr_xpdr_type")) {
    return CGEN_KR_XPDR_TYPE;
  }
  return 0;
}

