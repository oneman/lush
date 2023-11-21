#if !defined(_gen_meta_info_H)
# define _gen_meta_info_H (1)

#include <stddef.h>
#include <strings.h>
#include "struct_info.h"
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
enum kr_meta_info_type {
  KR_KR_ALSA_INFO_META_INFO = 1,
  KR_KR_ALSA_PATH_INFO_META_INFO,
  KR_KR_DECKLINK_VIDEO_CONN_TYPE_META_INFO,
  KR_KR_DECKLINK_AUDIO_CONN_TYPE_META_INFO,
  KR_KR_DECKLINK_INFO_META_INFO,
  KR_KR_DECKLINK_PATH_INFO_META_INFO,
  KR_KR_V4L2_STATE_META_INFO,
  KR_KR_V4L2_PATH_INFO_META_INFO,
  KR_KR_V4L2_INFO_META_INFO,
  KR_KR_AUX_PORT_STATE_META_INFO,
  KR_KR_AUX_INFO_META_INFO,
  KR_KR_AUX_PATH_INFO_META_INFO,
  KR_KR_JACK_DIRECTION_META_INFO,
  KR_KR_JACK_STATE_META_INFO,
  KR_KR_JACK_PATH_STATE_META_INFO,
  KR_KR_JACK_INFO_META_INFO,
  KR_KR_JACK_PATH_INFO_META_INFO,
  KR_KR_WAYLAND_STATE_META_INFO,
  KR_KR_WAYLAND_INFO_META_INFO,
  KR_KR_WAYLAND_PATH_INFO_META_INFO,
  KR_KR_X11_INFO_META_INFO,
  KR_KR_X11_PATH_INFO_META_INFO,
  KR_KR_AFX_EFFECT_TYPE_META_INFO,
  KR_KR_AFX_EFFECT_CONTROL_META_INFO,
  KR_KR_EQ_BAND_INFO_META_INFO,
  KR_KR_EQ_INFO_META_INFO,
  KR_KR_LOWPASS_INFO_META_INFO,
  KR_KR_HIGHPASS_INFO_META_INFO,
  KR_KR_PASS_INFO_META_INFO,
  KR_KR_VOLUME_INFO_META_INFO,
  KR_KR_ANALOG_INFO_META_INFO,
  KR_KR_METER_INFO_META_INFO,
  KR_KR_AFX_INFO_META_INFO,
  KR_KR_VIDEO_CODEC_META_INFO,
  KR_KR_AUDIO_CODEC_META_INFO,
  KR_KR_CODEC_TYPE_META_INFO,
  KR_KR_CODEC_META_INFO,
  KR_KR_CODEC_SUB1_META_INFO,
  KR_KR_AUDIO_INFO_META_INFO,
  KR_KR_VIDEO_INFO_META_INFO,
  KR_KR_AV_INFO_META_INFO,
  KR_KR_AV_INFO_SUB1_META_INFO,
  KR_KR_DAALA_ENCODER_INFO_META_INFO,
  KR_KR_VPX_ENCODER_INFO_META_INFO,
  KR_KR_H264_ENCODER_INFO_META_INFO,
  KR_KR_THEORA_ENCODER_INFO_META_INFO,
  KR_KR_Y4M_ENCODER_INFO_META_INFO,
  KR_KR_FLAC_INFO_META_INFO,
  KR_KR_OPUS_APP_META_INFO,
  KR_KR_OPUS_INFO_META_INFO,
  KR_KR_VORBIS_INFO_META_INFO,
  KR_KR_MP3_INFO_META_INFO,
  KR_KR_AAC_INFO_META_INFO,
  KR_KR_LAVC_ENCODER_INFO_META_INFO,
  KR_KR_COMPOSITOR_PATH_TYPE_META_INFO,
  KR_KR_COMPOSITOR_OVERLAY_TYPE_META_INFO,
  KR_KR_COMPOSITOR_OUTPUT_INFO_META_INFO,
  KR_KR_COMPOSITOR_BUS_INFO_META_INFO,
  KR_KR_COMPOSITOR_INPUT_INFO_META_INFO,
  KR_KR_COMPOSITOR_SOURCE_INFO_META_INFO,
  KR_KR_COMPOSITOR_OVERLAY_INFO_META_INFO,
  KR_KR_COMPOSITOR_OVERLAY_INFO_SUB1_META_INFO,
  KR_KR_COMPOSITOR_PATH_INFO_META_INFO,
  KR_KR_COMPOSITOR_PATH_INFO_SUB1_META_INFO,
  KR_KR_MTS_STREAM_TYPE_META_INFO,
  KR_KR_MTS_PACKET_INFO_META_INFO,
  KR_KR_MTS_STREAM_INFO_META_INFO,
  KR_KR_MTS_STREAM_INFO_SUB1_META_INFO,
  KR_KR_MTS_PROGRAM_INFO_META_INFO,
  KR_KR_MTS_INFO_META_INFO,
  KR_KR_CONTAINER_TYPE_META_INFO,
  KR_KR_TEXT_INFO_META_INFO,
  KR_KR_VECTOR_TYPE_META_INFO,
  KR_KR_VECTOR_INFO_META_INFO,
  KR_KR_FEATURE_ASPECT_TYPE_META_INFO,
  KR_KR_FEATURE_ASPECT_META_INFO,
  KR_KR_FEATURE_META_INFO,
  KR_KR_IMAGE_STASH_ITEM_INFO_META_INFO,
  KR_KR_IMAGE_STASH_ITEM_INFO_SUB1_META_INFO,
  KR_KR_PIXEL_FMT_META_INFO,
  KR_KR_IMAGE_TYPE_META_INFO,
  KR_KR_RECT_META_INFO,
  KR_KR_IMAGE_INFO_META_INFO,
  KR_KR_CODED_IMAGE_TYPE_META_INFO,
  KR_KR_CODED_IMAGE_INFO_META_INFO,
  KR_KR_FILE_MODE_META_INFO,
  KR_KR_FILE2_INFO_META_INFO,
  KR_KR_MIXER_CHANNELS_META_INFO,
  KR_KR_MIXER_PATH_TYPE_META_INFO,
  KR_KR_MIXER_PATH_INFO_META_INFO,
  KR_KR_MEDIA_EVENT_TYPE_META_INFO,
  KR_KR_MEDIA_ITEM_TYPE_META_INFO,
  KR_KR_MEDIA_FILE_INFO_META_INFO,
  KR_KR_MEDIA_METADATA_INFO_META_INFO,
  KR_KR_MEDIA_URL_INFO_META_INFO,
  KR_KR_MEDIA_ITEM_INFO_META_INFO,
  KR_KR_MEDIA_ITEM_INFO_SUB1_META_INFO,
  KR_KR_MEDIA_INFO_META_INFO,
  KR_KR_OPS_INFO_META_INFO,
  KR_KR_POS_META_INFO,
  KR_KR_PERSPECTIVE_VIEW_META_INFO,
  KR_M3U_EL_TYPE_META_INFO,
  KR_KR_M3U_TAG_NAME_META_INFO,
  KR_KR_M3U_TAG_VAL_TYPE_META_INFO,
  KR_KR_M3U_TAG_ATTR_NAME_META_INFO,
  KR_KR_M3U_TAG_PRIM_INFO_META_INFO,
  KR_KR_M3U_TAG_ALIST_INFO_META_INFO,
  KR_M3U_TAG_INFO_META_INFO,
  KR_M3U_TAG_INFO_SUB1_META_INFO,
  KR_M3U_EL_INFO_META_INFO,
  KR_M3U_EL_INFO_SUB1_META_INFO,
  KR_KR_M3U_INFO_META_INFO,
  KR_KR_LIMIT_INFO_META_INFO,
  KR_KR_LIMITS_INFO_META_INFO,
  KR_KR_PRIORITY_META_INFO,
  KR_KR_AVCONV_STATUS_INFO_META_INFO,
  KR_KR_AVCONV_PARAMS_INFO_META_INFO,
  KR_KR_AVCONV_INFO_META_INFO,
  KR_KR_AVPROBE_CODEC_TYPE_META_INFO,
  KR_KR_AVPROBE_FORMAT_INFO_META_INFO,
  KR_KR_AVPROBE_STREAM_INFO_META_INFO,
  KR_KR_AVPROBE_MEDIA_INFO_META_INFO,
  KR_KR_AVPROBE_PARAMS_INFO_META_INFO,
  KR_KR_AVPROBE_STATUS_INFO_META_INFO,
  KR_KR_AVPROBE_INFO_META_INFO,
  KR_KR_CURL_STATUS_INFO_META_INFO,
  KR_KR_CURL_PARAMS_INFO_META_INFO,
  KR_KR_CURL_INFO_META_INFO,
  KR_KR_LIVESTREAMER_PARAMS_INFO_META_INFO,
  KR_KR_LIVESTREAMER_STATUS_INFO_META_INFO,
  KR_KR_LIVESTREAMER_INFO_META_INFO,
  KR_KR_MEDIA_VERSION_META_INFO,
  KR_KR_QUVI_MEDIA_INFO_META_INFO,
  KR_KR_QUVI_STATUS_INFO_META_INFO,
  KR_KR_QUVI_PARAMS_INFO_META_INFO,
  KR_KR_QUVI_INFO_META_INFO,
  KR_KR_TRANSMISSION_CLI_STATUS_INFO_META_INFO,
  KR_KR_TRANSMISSION_CLI_PARAMS_INFO_META_INFO,
  KR_KR_TRANSMISSION_CLI_INFO_META_INFO,
  KR_KR_TASK_TYPE_META_INFO,
  KR_KR_TASK_STATE_META_INFO,
  KR_KR_TASK_EVENT_TYPE_META_INFO,
  KR_KR_FAIL_INFO_META_INFO,
  KR_KR_TASK_STATUS_META_INFO,
  KR_KR_TASK_PARAMS_INFO_META_INFO,
  KR_KR_TASK_PARAMS_INFO_SUB1_META_INFO,
  KR_KR_TASK_INFO_META_INFO,
  KR_KR_TASK_INFO_SUB1_META_INFO,
  KR_KR_VALGRIND_STRESSOR_STATUS_INFO_META_INFO,
  KR_KR_VALGRIND_STRESSOR_PARAMS_INFO_META_INFO,
  KR_KR_VALGRIND_STRESSOR_INFO_META_INFO,
  KR_KR_XMMS_PARAMS_INFO_META_INFO,
  KR_KR_XMMS_STATUS_INFO_META_INFO,
  KR_KR_XMMS_INFO_META_INFO,
  KR_KR_YDL_FORMAT_INFO_META_INFO,
  KR_KR_YDL_VIDEO_INFO_META_INFO,
  KR_KR_YDL_PARAMS_INFO_META_INFO,
  KR_KR_YDL_STATUS_INFO_META_INFO,
  KR_KR_YDL_INFO_META_INFO,
  KR_KR_TIMESTAMP_INFO_META_INFO,
  KR_KR_EASING_META_INFO,
  KR_KR_MEDIA_SOURCE_FORMAT_META_INFO,
  KR_KR_MEDIA_SOURCE_INFO_META_INFO,
  KR_KR_MEDIA_PACKET_TYPE_META_INFO,
  KR_KR_MEDIA_PACKET_META_INFO,
  KR_KR_TRACK_NAME_META_INFO,
  KR_KR_MUX_NAME_META_INFO,
  KR_KR_DEMUX_INFO_META_INFO,
  KR_KR_MUX_INFO_META_INFO,
  KR_KR_VIDEO_ENCODE_INFO_META_INFO,
  KR_KR_VIDEO_ENCODE_INFO_SUB1_META_INFO,
  KR_KR_AUDIO_ENCODE_INFO_META_INFO,
  KR_KR_AUDIO_ENCODE_INFO_SUB1_META_INFO,
  KR_KR_ENCODE_TYPE_META_INFO,
  KR_KR_ENCODE_INFO_META_INFO,
  KR_KR_ENCODE_INFO_SUB1_META_INFO,
  KR_KR_CAPTURE_TYPE_META_INFO,
  KR_KR_RECORD_INFO_META_INFO,
  KR_KR_TRANSMISSION_INFO_META_INFO,
  KR_KR_OUTPUT_TYPE_META_INFO,
  KR_KR_OUTPUT_INFO_META_INFO,
  KR_KR_OUTPUT_INFO_SUB1_META_INFO,
  KR_KR_CAPTURE_AUDIO_INFO_META_INFO,
  KR_KR_CAPTURE_VIDEO_INFO_META_INFO,
  KR_KR_CAPTURE_IMAGE_INFO_META_INFO,
  KR_KR_CAPTURE_INFO_META_INFO,
  KR_KR_CAPTURE_INFO_SUB1_META_INFO,
  KR_KR_SOURCE_FILE_INFO_META_INFO,
  KR_KR_SOURCE_REMOTE_FILE_INFO_META_INFO,
  KR_KR_STUDIO_SOURCE_STREAM_TYPE_META_INFO,
  KR_KR_SOURCE_STREAM_INFO_META_INFO,
  KR_KR_STUDIO_SOURCE_TYPE_META_INFO,
  KR_KR_STUDIO_SOURCE_INFO_META_INFO,
  KR_KR_STUDIO_SOURCE_INFO_SUB1_META_INFO,
  KR_KR_DECODE_INFO_META_INFO,
  KR_KR_PLAY_INFO_META_INFO,
  KR_KR_RENDER_IMAGE_INFO_META_INFO,
  KR_KR_RENDER_VIDEO_INFO_META_INFO,
  KR_KR_RENDER_AUDIO_INFO_META_INFO,
  KR_KR_RENDER_TYPE_META_INFO,
  KR_KR_RENDER_INFO_META_INFO,
  KR_KR_RENDER_INFO_SUB1_META_INFO,
  KR_KR_AUDIO_GENERATE_TYPE_META_INFO,
  KR_KR_GENERATE_AUDIO_INFO_META_INFO,
  KR_KR_VIDEO_GENERATE_TYPE_META_INFO,
  KR_KR_GENERATE_COLOR_INFO_META_INFO,
  KR_KR_GENERATE_VIDEO_INFO_META_INFO,
  KR_KR_GENERATE_VIDEO_INFO_SUB1_META_INFO,
  KR_KR_GENERATE_TYPE_META_INFO,
  KR_KR_GENERATE_INFO_META_INFO,
  KR_KR_GENERATE_INFO_SUB1_META_INFO,
  KR_KR_STUDIO_PATH_TYPE_META_INFO,
  KR_KR_STUDIO_PATH_INFO_META_INFO,
  KR_KR_STUDIO_PATH_INFO_SUB1_META_INFO,
  KR_KR_TRACK_BLOCK_TYPE_META_INFO,
  KR_KR_TRACK_MODE_META_INFO,
  KR_KR_TRACK_EVENT_TYPE_META_INFO,
  KR_KR_TRACK_INFO_META_INFO,
  KR_KR_TRACK_INFO_SUB1_META_INFO,
  KR_KR_TRACK_BLOCK_INFO_META_INFO,
  KR_KR_HLS_PLAYLIST_TYPE_META_INFO,
  KR_KR_HLS_RENDITION_TYPE_META_INFO,
  KR_KR_HLS_MEDIA_PLAYLIST_TYPE_META_INFO,
  KR_KR_HLS_VARIANT_META_INFO,
  KR_KR_HLS_RENDITION_META_INFO,
  KR_KR_HLS_MASTER_PLAYLIST_META_INFO,
  KR_KR_HLS_MEDIA_SEGMENT_META_INFO,
  KR_KR_HLS_MEDIA_PLAYLIST_META_INFO,
  KR_KR_HLS_PLAYLIST_META_INFO,
  KR_KR_HLS_PLAYLIST_SUB1_META_INFO,
  KR_KR_HLS_TUNER_INFO_META_INFO,
  KR_KR_FPS_INFO_META_INFO,
  KR_KR_RADIO_MODE_META_INFO,
  KR_KR_RADIO_INFO_META_INFO,
  KR_KR_WEB_CLIENT_PROTOCOL_META_INFO,
  KR_KR_WEB_CLIENT_TYPE_META_INFO,
  KR_KR_WEB_CLIENT_STATE_META_INFO,
  KR_KR_WEB_SERVER_TYPE_META_INFO,
  KR_KR_MULTIPART_STATE_META_INFO,
  KR_KR_WEB_SERVER_INFO_META_INFO,
  KR_KR_WEB_SERVER_SETUP_INFO_META_INFO,
  KR_KR_WEB_REMOTE_INFO_META_INFO,
  KR_KR_WEB_SOCKET_IO_STATE_META_INFO,
  KR_KR_WEBSOCKET_FRAME_TYPE_META_INFO,
  KR_KR_WEBSOCKET_PROTOCOL_META_INFO,
  KR_KR_WEB_SOCKET_STATE_META_INFO,
  KR_KR_HTTP_MESSAGE_STATE_META_INFO,
  KR_KR_WEB_HTTP_IO_STATE_META_INFO,
  KR_KR_WEB_HTTP_STATE_META_INFO,
  KR_KR_WEB_CLIENT_INFO_META_INFO,
  KR_KR_WEB_CLIENT_INFO_SUB1_META_INFO,
  KR_KR_WEB_CLIENT_SETUP_INFO_META_INFO,
  KR_KR_WEB_PATH_TYPE_META_INFO,
  KR_KR_WEB_PATH_INFO_META_INFO,
  KR_KR_WEB_PATH_INFO_SUB1_META_INFO,
  KR_KR_WEB_PATH_SETUP_INFO_META_INFO,
  KR_KR_WEB_PATH_SETUP_INFO_SUB1_META_INFO,
  KR_KR_PATH_INFO_META_INFO,
  KR_KR_PATH_LIST_META_INFO,
  KR_KR_UPLOAD_TYPE_META_INFO,
  KR_KR_DOWNLOAD_TYPE_META_INFO,
  KR_KR_UPLOAD_FILE_INFO_META_INFO,
  KR_KR_UPLOAD_STASH_INFO_META_INFO,
  KR_KR_DOWNLOAD_FILE_INFO_META_INFO,
  KR_KR_DOWNLOAD_STASH_INFO_META_INFO,
  KR_KR_UPLOAD_INFO_META_INFO,
  KR_KR_UPLOAD_INFO_SUB1_META_INFO,
  KR_KR_DOWNLOAD_INFO_META_INFO,
  KR_KR_DOWNLOAD_INFO_SUB1_META_INFO,
  KR_KR_ACCEPT_INFO_META_INFO,
  KR_KR_SERVE_RANGE_INFO_META_INFO,
  KR_KR_SERVE_RANGES_INFO_META_INFO,
  KR_KR_SERVE_FULL_INFO_META_INFO,
  KR_KR_SERVE_INDEX_FORMAT_META_INFO,
  KR_KR_SERVE_INDEX_INFO_META_INFO,
  KR_KR_SERVE_REDIRECT_INFO_META_INFO,
  KR_KR_SERVE_TYPE_META_INFO,
  KR_KR_SERVE_INFO_META_INFO,
  KR_KR_SERVE_INFO_SUB1_META_INFO,
  KR_KR_DOWNLINK_INFO_META_INFO,
  KR_KR_UPLINK_INFO_META_INFO,
  KR_KR_SOURCE_INFO_META_INFO,
  KR_KR_TRANSMIT_INFO_META_INFO,
  KR_KR_XFER_CLIENT_TYPE_META_INFO,
  KR_KR_XFER_RECONNECT_INFO_META_INFO,
  KR_KR_XFER_CLIENT_INFO_META_INFO,
  KR_KR_XFER_CLIENT_INFO_SUB1_META_INFO,
  KR_KR_XFER_ACCEPTOR_DIR_INFO_META_INFO,
  KR_KR_XFER_ACCEPTOR_FILE_INFO_META_INFO,
  KR_KR_XFER_ACCEPTOR_BUF_INFO_META_INFO,
  KR_KR_XFER_SHARE_DIR_INFO_META_INFO,
  KR_KR_XFER_SHARE_FILE_INFO_META_INFO,
  KR_KR_XFER_SHARE_BUF_INFO_META_INFO,
  KR_KR_XFER_ACCEPTOR_TYPE_META_INFO,
  KR_KR_XFER_ACCEPTOR_INFO_META_INFO,
  KR_KR_XFER_ACCEPTOR_INFO_SUB1_META_INFO,
  KR_KR_XFER_RECEIVER_INFO_META_INFO,
  KR_KR_XFER_TRANSMITTER_TYPE_META_INFO,
  KR_KR_XFER_TRANSMITTER_INFO_META_INFO,
  KR_KR_XFER_SHARE_TYPE_META_INFO,
  KR_KR_XFER_SHARE_INFO_META_INFO,
  KR_KR_XFER_SHARE_INFO_SUB1_META_INFO,
  KR_KR_XFER_SERVER_TYPE_META_INFO,
  KR_KR_XFER_SERVER_INFO_META_INFO,
  KR_KR_XFER_SERVER_INFO_SUB1_META_INFO,
  KR_KR_XFER_PATH_TYPE_META_INFO,
  KR_KR_XFER_PATH_INFO_META_INFO,
  KR_KR_XFER_PATH_INFO_SUB1_META_INFO,
  KR_KR_XPDR_TYPE_META_INFO,
  KR_KR_XPDR_PATH_INFO_META_INFO,
  KR_KR_XPDR_PATH_INFO_SUB1_META_INFO,
};

enum kr_meta_info_last {
  KR_META_INFO_LAST = 311
};

extern const struct_info meta_info[];

char *enum_to_str(int idx, int value);
int str_to_enum(int idx, char *str);
int enum_to_index(int idx, int value);

#endif
