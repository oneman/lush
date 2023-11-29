#include "metainfo.h"

char *enum_to_str(int idx, int value) {
  switch(idx) {
    case KR_KR_DECKLINK_VIDEO_CONN_TYPE_META_INFO: {
      switch(value) {
        case KR_DL_VIDEO_DEFAULT: return "video_default";
        case KR_DL_SDI: return "sdi";
        case KR_DL_OPTICAL_SDI: return "optical_sdi";
        case KR_DL_HDMI: return "hdmi";
        case KR_DL_COMPONENT: return "component";
        case KR_DL_COMPOSITE: return "composite";
        case KR_DL_SVIDEO: return "svideo";
      }
      break;
    }
    case KR_KR_DECKLINK_AUDIO_CONN_TYPE_META_INFO: {
      switch(value) {
        case KR_DL_AUDIO_DEFAULT: return "audio_default";
        case KR_DL_EMBEDDED: return "embedded";
        case KR_DL_ANALOG: return "analog";
        case KR_DL_AESEBU: return "aesebu";
      }
      break;
    }
    case KR_KR_V4L2_STATE_META_INFO: {
      switch(value) {
        case KR_V4L2_VOID: return "void";
        case KR_V4L2_OPEN: return "open";
        case KR_V4L2_CAPTURE: return "capture";
      }
      break;
    }
    case KR_KR_AUX_PORT_STATE_META_INFO: {
      switch(value) {
        case KR_AUX_PORT_DISCONNECTED: return "disconnected";
        case KR_AUX_PORT_CONNECTED: return "connected";
      }
      break;
    }
    case KR_KR_JACK_DIRECTION_META_INFO: {
      switch(value) {
        case KR_JACK_INPUT: return "input";
        case KR_JACK_OUTPUT: return "output";
      }
      break;
    }
    case KR_KR_JACK_STATE_META_INFO: {
      switch(value) {
        case KR_JACK_OFFLINE: return "offline";
        case KR_JACK_ONLINE: return "online";
      }
      break;
    }
    case KR_KR_JACK_PATH_STATE_META_INFO: {
      switch(value) {
        case KR_JACK_PATH_FAIL: return "fail";
        case KR_JACK_PATH_ACTIVE: return "active";
      }
      break;
    }
    case KR_KR_WAYLAND_STATE_META_INFO: {
      switch(value) {
        case KR_WL_DISCONNECTED: return "disconnected";
        case KR_WL_CONNECTED: return "connected";
      }
      break;
    }
    case KR_KR_AFX_EFFECT_TYPE_META_INFO: {
      switch(value) {
        case KR_AFX_NONE: return "none";
        case KR_AFX_EQ: return "eq";
        case KR_AFX_LOWPASS: return "lowpass";
        case KR_AFX_HIGHPASS: return "highpass";
        case KR_AFX_ANALOG: return "analog";
        case KR_AFX_VOLUME: return "volume";
      }
      break;
    }
    case KR_KR_AFX_EFFECT_CONTROL_META_INFO: {
      switch(value) {
        case KR_AFX_DB: return "db";
        case KR_AFX_BW: return "bw";
        case KR_AFX_HZ: return "hz";
        case KR_AFX_DRIVE: return "drive";
        case KR_AFX_BLEND: return "blend";
      }
      break;
    }
    case KR_KR_VIDEO_CODEC_META_INFO: {
      switch(value) {
        case KR_VIDEO_CODEC_DAALA: return "daala";
        case KR_VIDEO_CODEC_VPX: return "vpx";
        case KR_VIDEO_CODEC_H264: return "h264";
        case KR_VIDEO_CODEC_THEORA: return "theora";
        case KR_VIDEO_CODEC_Y4M: return "y4m";
      }
      break;
    }
    case KR_KR_AUDIO_CODEC_META_INFO: {
      switch(value) {
        case KR_AUDIO_CODEC_FLAC: return "flac";
        case KR_AUDIO_CODEC_OPUS: return "opus";
        case KR_AUDIO_CODEC_VORBIS: return "vorbis";
        case KR_AUDIO_CODEC_MP3: return "mp3";
        case KR_AUDIO_CODEC_AAC: return "aac";
      }
      break;
    }
    case KR_KR_CODEC_TYPE_META_INFO: {
      switch(value) {
        case KR_CODEC_AUDIO: return "audio";
        case KR_CODEC_VIDEO: return "video";
      }
      break;
    }
    case KR_KR_OPUS_APP_META_INFO: {
      switch(value) {
        case KR_OPUS_APP_AUDIO: return "audio";
        case KR_OPUS_APP_LOWDELAY: return "lowdelay";
        case KR_OPUS_APP_VOIP: return "voip";
      }
      break;
    }
    case KR_KR_COMPOSITOR_PATH_TYPE_META_INFO: {
      switch(value) {
        case KR_COM_SOURCE: return "source";
        case KR_COM_OVERLAY: return "overlay";
        case KR_COM_INPUT: return "input";
        case KR_COM_BUS: return "bus";
        case KR_COM_OUTPUT: return "output";
      }
      break;
    }
    case KR_KR_COMPOSITOR_OVERLAY_TYPE_META_INFO: {
      switch(value) {
        case KR_TEXT: return "text";
        case KR_VECTOR: return "vector";
      }
      break;
    }
    case KR_KR_MTS_STREAM_TYPE_META_INFO: {
      switch(value) {
        case MTS_VIDEO: return "video";
        case MTS_AUDIO: return "audio";
        case MTS_OTHER: return "other";
      }
      break;
    }
    case KR_KR_CONTAINER_TYPE_META_INFO: {
      switch(value) {
        case KR_CONTAINER_RAW: return "raw";
        case KR_CONTAINER_MKV: return "mkv";
        case KR_CONTAINER_WEBM: return "webm";
        case KR_CONTAINER_OGG: return "ogg";
        case KR_CONTAINER_FLAC: return "flac";
        case KR_CONTAINER_Y4M: return "y4m";
        case KR_CONTAINER_TXT: return "txt";
        case KR_CONTAINER_TS: return "ts";
        case KR_CONTAINER_OCC: return "occ";
      }
      break;
    }
    case KR_KR_VECTOR_TYPE_META_INFO: {
      switch(value) {
        case NOTHING: return "nothing";
        case HEX: return "hex";
        case CIRCLE: return "circle";
        case RECT: return "rect";
        case TRIANGLE: return "triangle";
        case VIPER: return "viper";
        case METER: return "meter";
        case GRID: return "grid";
        case CURVE: return "curve";
        case ARROW: return "arrow";
        case CLOCK: return "clock";
        case SHADOW: return "shadow";
      }
      break;
    }
    case KR_KR_FEATURE_ASPECT_TYPE_META_INFO: {
      switch(value) {
        case KR_FEATURE_COMMENTS: return "comments";
        case KR_FEATURE_TAG: return "tag";
        case KR_FEATURE_HEADER: return "header";
        case KR_FEATURE_FUNCTION: return "function";
        case KR_FEATURE_STORY: return "story";
      }
      break;
    }
    case KR_KR_PIXEL_FMT_META_INFO: {
      switch(value) {
        case KR_PIXELS_ARGB: return "argb";
        case KR_PIXELS_YUV422P: return "yuv422p";
        case KR_PIXELS_YVU422P: return "yvu422p";
        case KR_PIXELS_YUV444: return "yuv444";
        case KR_PIXELS_YUV420: return "yuv420";
        case KR_PIXELS_YUV422: return "yuv422";
      }
      break;
    }
    case KR_KR_IMAGE_TYPE_META_INFO: {
      switch(value) {
        case KR_IMAGE_ENC: return "enc";
        case KR_IMAGE_RAW: return "raw";
      }
      break;
    }
    case KR_KR_CODED_IMAGE_TYPE_META_INFO: {
      switch(value) {
        case KR_CODED_IMAGE_PNG: return "png";
        case KR_CODED_IMAGE_JPG: return "jpg";
        case KR_CODED_IMAGE_GIF: return "gif";
        case KR_CODED_IMAGE_VP8: return "vp8";
        case KR_CODED_IMAGE_VP9: return "vp9";
        case KR_CODED_IMAGE_DAALA: return "daala";
        case KR_CODED_IMAGE_THEORA: return "theora";
        case KR_CODED_IMAGE_Y4M: return "y4m";
        case KR_CODED_IMAGE_H264: return "h264";
      }
      break;
    }
    case KR_KR_FILE_ACCESS_MODE_META_INFO: {
      switch(value) {
        case KR_FILE_ACCESS_ONLY: return "only";
        case KR_FILE_ACCESS_APPEND: return "append";
        case KR_FILE_ACCESS_MODIFY: return "modify";
      }
      break;
    }
    case KR_KR_MIXER_CHANNELS_META_INFO: {
      switch(value) {
        case KR_MONO: return "mono";
        case KR_STEREO: return "stereo";
        case KR_THREE: return "three";
        case KR_QUAD: return "quad";
        case KR_FIVE: return "five";
        case KR_SIX: return "six";
        case KR_SEVEN: return "seven";
        case KR_EIGHT: return "eight";
      }
      break;
    }
    case KR_KR_MIXER_PATH_TYPE_META_INFO: {
      switch(value) {
        case KR_MXR_SOURCE: return "source";
        case KR_MXR_INPUT: return "input";
        case KR_MXR_BUS: return "bus";
        case KR_MXR_OUTPUT: return "output";
      }
      break;
    }
    case KR_KR_MEDIA_EVENT_TYPE_META_INFO: {
      switch(value) {
        case KR_MEDIA_CREATE: return "create";
        case KR_MEDIA_ITEM_ADD: return "item_add";
        case KR_MEDIA_FILE_CREATE: return "file_create";
        case KR_MEDIA_DESTROY: return "destroy";
      }
      break;
    }
    case KR_KR_MEDIA_ITEM_TYPE_META_INFO: {
      switch(value) {
        case KR_MEDIA_URL: return "url";
        case KR_MEDIA_METADATA: return "metadata";
        case KR_MEDIA_FILE: return "file";
      }
      break;
    }
    case KR_M3U_EL_TYPE_META_INFO: {
      switch(value) {
        case M3U_URI: return "uri";
        case M3U_TAG: return "tag";
      }
      break;
    }
    case KR_KR_M3U_TAG_NAME_META_INFO: {
      switch(value) {
        case EXTM3U: return "extm3u";
        case EXTINF: return "extinf";
        case EXT_X_BYTERANGE: return "ext_x_byterange";
        case EXT_X_TARGETDURATION: return "ext_x_targetduration";
        case EXT_X_MEDIA_SEQUENCE: return "ext_x_media_sequence";
        case EXT_X_KEY: return "ext_x_key";
        case EXT_X_PROGRAM_DATE_TIME: return "ext_x_program_date_time";
        case EXT_X_ALLOW_CACHE: return "ext_x_allow_cache";
        case EXT_X_PLAYLIST_TYPE: return "ext_x_playlist_type";
        case EXT_X_STREAM_INF: return "ext_x_stream_inf";
        case EXT_X_I_FRAME_STREAM_INF: return "ext_x_i_frame_stream_inf";
        case EXT_X_I_FRAMES_ONLY: return "ext_x_i_frames_only";
        case EXT_X_MAP: return "ext_x_map";
        case EXT_X_MEDIA: return "ext_x_media";
        case EXT_X_ENDLIST: return "ext_x_endlist";
        case EXT_X_DISCONTINUITY: return "ext_x_discontinuity";
        case EXT_X_DISCONTINUITY_SEQUENCE: return "ext_x_discontinuity_sequence";
        case EXT_X_INDEPENDENT_SEGMENTS: return "ext_x_independent_segments";
        case EXT_X_START: return "ext_x_start";
        case EXT_X_VERSION: return "ext_x_version";
      }
      break;
    }
    case KR_KR_M3U_TAG_VAL_TYPE_META_INFO: {
      switch(value) {
        case M3U_TAG_VAL_PRIMITIVE: return "primitive";
        case M3U_TAG_VAL_ATTR_LIST: return "attr_list";
      }
      break;
    }
    case KR_KR_M3U_TAG_ATTR_NAME_META_INFO: {
      switch(value) {
        case M3U_ATTR_METHOD: return "method";
        case M3U_ATTR_URI: return "uri";
        case M3U_ATTR_IV: return "iv";
        case M3U_ATTR_KEYFORMAT: return "keyformat";
        case M3U_ATTR_KEYFORMATVERSIONS: return "keyformatversions";
        case M3U_ATTR_TYPE: return "type";
        case M3U_ATTR_GROUP_ID: return "group_id";
        case M3U_ATTR_PROGRAM_ID: return "program_id";
        case M3U_ATTR_LANGUAGE: return "language";
        case M3U_ATTR_ASSOC_LANGUAGE: return "assoc_language";
        case M3U_ATTR_NAME: return "name";
        case M3U_ATTR_DEFAULT: return "default";
        case M3U_ATTR_AUTOSELECT: return "autoselect";
        case M3U_ATTR_FORCED: return "forced";
        case M3U_ATTR_INSTREAM_ID: return "instream_id";
        case M3U_ATTR_CHARACTERISTICS: return "characteristics";
        case M3U_ATTR_BANDWIDTH: return "bandwidth";
        case M3U_ATTR_CODECS: return "codecs";
        case M3U_ATTR_RESOLUTION: return "resolution";
        case M3U_ATTR_AUDIO: return "audio";
        case M3U_ATTR_VIDEO: return "video";
        case M3U_ATTR_SUBTITLES: return "subtitles";
        case M3U_ATTR_CLOSED_CAPTIONS: return "closed_captions";
        case M3U_ATTR_BYTERANGE: return "byterange";
        case M3U_ATTR_TIME_OFFEST: return "time_offest";
        case M3U_ATTR_PRECISE: return "precise";
      }
      break;
    }
    case KR_KR_PRIORITY_META_INFO: {
      switch(value) {
        case KR_PRIORITY_UNKNOWN: return "unknown";
        case KR_PRIORITY_NORMAL: return "normal";
        case KR_PRIORITY_NONINTERACTIVE: return "noninteractive";
        case KR_PRIORITY_TRANSCODER: return "transcoder";
        case KR_PRIORITY_REALTIME: return "realtime";
        case KR_PRIORITY_REALTIME_IO: return "realtime_io";
      }
      break;
    }
    case KR_KR_AVPROBE_CODEC_TYPE_META_INFO: {
      switch(value) {
        case KR_AUDIO: return "audio";
        case KR_VIDEO: return "video";
      }
      break;
    }
    case KR_KR_TASK_TYPE_META_INFO: {
      switch(value) {
        case KR_VALGRIND_STRESSOR: return "valgrind_stressor";
        case KR_QUVI: return "quvi";
        case KR_AVCONV: return "avconv";
        case KR_XMMS: return "xmms";
        case KR_CURL: return "curl";
        case KR_TRANSMISSION_CLI: return "transmission_cli";
        case KR_LIVESTREAMER: return "livestreamer";
        case KR_AVPROBE: return "avprobe";
        case KR_YDL: return "ydl";
      }
      break;
    }
    case KR_KR_TASK_STATE_META_INFO: {
      switch(value) {
        case KR_TASK_READY: return "ready";
        case KR_TASK_RUNNING: return "running";
        case KR_TASK_FAILED: return "failed";
        case KR_TASK_TIMEDOUT: return "timedout";
        case KR_TASK_SUCCEEDED: return "succeeded";
        case KR_TASK_CANCELLED: return "cancelled";
        case KR_TASK_SAME_STATE: return "same_state";
      }
      break;
    }
    case KR_KR_TASK_EVENT_TYPE_META_INFO: {
      switch(value) {
        case KR_TASK_CREATE: return "create";
        case KR_TASK_START: return "start";
        case KR_TASK_PATCH: return "patch";
        case KR_TASK_PROGRESS: return "progress";
        case KR_TASK_FAILURE: return "failure";
        case KR_TASK_SUCCESS: return "success";
        case KR_TASK_CANCEL: return "cancel";
        case KR_TASK_TIMEOUT: return "timeout";
        case KR_TASK_DESTROY: return "destroy";
      }
      break;
    }
    case KR_KR_EASING_META_INFO: {
      switch(value) {
        case LINEAR: return "linear";
        case EASEINSINE: return "easeinsine";
        case EASEOUTSINE: return "easeoutsine";
        case EASEINOUTSINE: return "easeinoutsine";
        case EASEINCUBIC: return "easeincubic";
        case EASEOUTCUBIC: return "easeoutcubic";
        case EASEINOUTCUBIC: return "easeinoutcubic";
        case EASEINOUTELASTIC: return "easeinoutelastic";
      }
      break;
    }
    case KR_KR_MEDIA_SOURCE_FORMAT_META_INFO: {
      switch(value) {
        case MEDIA_SOURCE_MTS: return "media_source_mts";
      }
      break;
    }
    case KR_KR_MEDIA_PACKET_TYPE_META_INFO: {
      switch(value) {
        case KR_PKT_HEADER: return "header";
        case KR_PKT_KEY: return "key";
        case KR_PKT_NORMAL: return "normal";
      }
      break;
    }
    case KR_KR_ENCODE_TYPE_META_INFO: {
      switch(value) {
        case KR_ENCODE_AUDIO: return "audio";
        case KR_ENCODE_VIDEO: return "video";
      }
      break;
    }
    case KR_KR_CAPTURE_TYPE_META_INFO: {
      switch(value) {
        case KR_CAPTURE_AUDIO: return "audio";
        case KR_CAPTURE_VIDEO: return "video";
        case KR_CAPTURE_IMAGE: return "image";
      }
      break;
    }
    case KR_KR_OUTPUT_TYPE_META_INFO: {
      switch(value) {
        case KR_STUDIO_RECORD: return "record";
        case KR_STUDIO_UPLINK: return "uplink";
        case KR_STUDIO_TRANSMISSION: return "transmission";
      }
      break;
    }
    case KR_KR_STUDIO_SOURCE_STREAM_TYPE_META_INFO: {
      switch(value) {
        case KR_SOURCE_STREAM_ACCEPT: return "accept";
        case KR_SOURCE_STREAM_DOWNLINK: return "downlink";
      }
      break;
    }
    case KR_KR_STUDIO_SOURCE_TYPE_META_INFO: {
      switch(value) {
        case KR_SOURCE_LOCAL_FILE: return "local_file";
        case KR_SOURCE_REMOTE_FILE: return "remote_file";
        case KR_SOURCE_STREAM: return "stream";
      }
      break;
    }
    case KR_KR_RENDER_TYPE_META_INFO: {
      switch(value) {
        case KR_RENDER_IMAGE: return "image";
        case KR_RENDER_VIDEO: return "video";
        case KR_RENDER_AUDIO: return "audio";
      }
      break;
    }
    case KR_KR_AUDIO_GENERATE_TYPE_META_INFO: {
      switch(value) {
        case KR_GENERATE_NOISE: return "noise";
        case KR_GENERATE_TONE: return "tone";
        case KR_GENERATE_SILENCE: return "silence";
      }
      break;
    }
    case KR_KR_VIDEO_GENERATE_TYPE_META_INFO: {
      switch(value) {
        case KR_GENERATE_COLOR: return "color";
        case KR_GENERATE_NO_SIGNAL: return "no_signal";
      }
      break;
    }
    case KR_KR_GENERATE_TYPE_META_INFO: {
      switch(value) {
        case KR_GENERATE_AUDIO: return "audio";
        case KR_GENERATE_VIDEO: return "video";
      }
      break;
    }
    case KR_KR_STUDIO_PATH_TYPE_META_INFO: {
      switch(value) {
        case KR_STUDIO_SOURCE: return "source";
        case KR_STUDIO_GENERATE: return "generate";
        case KR_STUDIO_CAPTURE: return "capture";
        case KR_STUDIO_RENDER: return "render";
        case KR_STUDIO_PLAY: return "play";
        case KR_STUDIO_ENCODE: return "encode";
        case KR_STUDIO_DECODE: return "decode";
        case KR_STUDIO_DEMUX: return "demux";
        case KR_STUDIO_MUX: return "mux";
        case KR_STUDIO_OUTPUT: return "output";
      }
      break;
    }
    case KR_KR_TRACK_BLOCK_TYPE_META_INFO: {
      switch(value) {
        case KR_TRACK_CODED_AUDIO: return "coded_audio";
        case KR_TRACK_CODED_VIDEO: return "coded_video";
        case KR_TRACK_MUX: return "mux";
        case KR_TRACK_AUDIO: return "audio";
        case KR_TRACK_VIDEO: return "video";
        case KR_TRACK_SUBTITLE: return "subtitle";
        case KR_TRACK_METADATA: return "metadata";
        case KR_TRACK_CONTROL: return "control";
      }
      break;
    }
    case KR_KR_TRACK_MODE_META_INFO: {
      switch(value) {
        case KR_TRACK_FINITE: return "finite";
        case KR_TRACK_INFINITE: return "infinite";
      }
      break;
    }
    case KR_KR_TRACK_EVENT_TYPE_META_INFO: {
      switch(value) {
        case KR_TRACK_ADD: return "add";
        case KR_TRACK_REMOVE: return "remove";
        case KR_TRACK_READ: return "read";
        case KR_TRACK_WRITE: return "write";
      }
      break;
    }
    case KR_KR_HLS_PLAYLIST_TYPE_META_INFO: {
      switch(value) {
        case KR_HLS_PLAYLIST_MASTER: return "master";
        case KR_HLS_PLAYLIST_MEDIA: return "media";
      }
      break;
    }
    case KR_KR_HLS_RENDITION_TYPE_META_INFO: {
      switch(value) {
        case HLS_AUDIO: return "audio";
        case HLS_VIDEO: return "video";
        case HLS_SUBTITLES: return "subtitles";
        case HLS_CLOSED_CAPTIONS: return "closed_captions";
      }
      break;
    }
    case KR_KR_HLS_MEDIA_PLAYLIST_TYPE_META_INFO: {
      switch(value) {
        case HLS_VOD: return "vod";
        case HLS_LIVE: return "live";
      }
      break;
    }
    case KR_KR_RADIO_MODE_META_INFO: {
      switch(value) {
        case KR_RADIO_MODE_AUDIO: return "audio";
        case KR_RADIO_MODE_AUDIOVIDEO: return "audiovideo";
      }
      break;
    }
    case KR_KR_WEB_CLIENT_PROTOCOL_META_INFO: {
      switch(value) {
        case KR_PROTOCOL_HTTP: return "http";
        case KR_PROTOCOL_WEBSOCKET: return "websocket";
        case KR_PROTOCOL_TCP: return "tcp";
      }
      break;
    }
    case KR_KR_WEB_CLIENT_TYPE_META_INFO: {
      switch(value) {
        case KR_WEB_CLIENT_ACCEPT: return "accept";
        case KR_WEB_CLIENT_CONNECT: return "connect";
      }
      break;
    }
    case KR_KR_WEB_CLIENT_STATE_META_INFO: {
      switch(value) {
        case KR_WEB_RESOLVING: return "resolving";
        case KR_WEB_CONNECTING: return "connecting";
        case KR_WEB_ACTIVE: return "active";
        case KR_WEB_FLUSHING: return "flushing";
      }
      break;
    }
    case KR_KR_WEB_SERVER_TYPE_META_INFO: {
      switch(value) {
        case KR_WEB_SERVER_HTTP: return "http";
        case KR_WEB_SERVER_WEBSOCKET: return "websocket";
        case KR_WEB_SERVER_HTTP_WEBSOCKET: return "http_websocket";
      }
      break;
    }
    case KR_KR_MULTIPART_STATE_META_INFO: {
      switch(value) {
        case KR_MULTIPART_DONE: return "done";
        case KR_MULTIPART_INIT: return "init";
        case KR_MULTIPART_INFO: return "info";
        case KR_MULTIPART_DATA: return "data";
      }
      break;
    }
    case KR_KR_WEBSOCKET_FRAME_TYPE_META_INFO: {
      switch(value) {
        case KR_WEBSOCKET_BINARY: return "binary";
        case KR_WEBSOCKET_TEXT: return "text";
      }
      break;
    }
    case KR_KR_WEBSOCKET_PROTOCOL_META_INFO: {
      switch(value) {
        case KR_WS_PROTOCOL_UNKNOWN: return "unknown";
        case KR_WS_PROTOCOL_KRAD: return "krad";
        case KR_WS_PROTOCOL_STREAM: return "stream";
        case KR_WS_PROTOCOL_XMMS: return "xmms";
      }
      break;
    }
    case KR_KR_HTTP_MESSAGE_STATE_META_INFO: {
      switch(value) {
        case KR_HTTP_IN_HEADER: return "header";
        case KR_HTTP_IN_CONTENT: return "content";
      }
      break;
    }
    case KR_KR_WEB_PATH_TYPE_META_INFO: {
      switch(value) {
        case KR_WEB_SERVER: return "server";
        case KR_WEB_CLIENT: return "client";
        case KR_WEB_PROXY: return "proxy";
      }
      break;
    }
    case KR_KR_UPLOAD_TYPE_META_INFO: {
      switch(value) {
        case KR_UPLOAD_FILE: return "file";
        case KR_UPLOAD_STASH: return "stash";
      }
      break;
    }
    case KR_KR_DOWNLOAD_TYPE_META_INFO: {
      switch(value) {
        case KR_DOWNLOAD_FILE: return "file";
        case KR_DOWNLOAD_STASH: return "stash";
      }
      break;
    }
    case KR_KR_SERVE_INDEX_FORMAT_META_INFO: {
      switch(value) {
        case KR_SERVE_INDEX_JSON: return "json";
        case KR_SERVE_INDEX_HTML: return "html";
      }
      break;
    }
    case KR_KR_SERVE_TYPE_META_INFO: {
      switch(value) {
        case KR_SERVE_FULL: return "full";
        case KR_SERVE_RANGES: return "ranges";
        case KR_SERVE_INDEX: return "index";
        case KR_SERVE_REDIRECT: return "redirect";
      }
      break;
    }
    case KR_KR_XFER_CLIENT_TYPE_META_INFO: {
      switch(value) {
        case KR_DOWNLOAD: return "download";
        case KR_UPLOAD: return "upload";
        case KR_ACCEPT: return "accept";
        case KR_SERVE: return "serve";
        case KR_DOWNLINK: return "downlink";
        case KR_UPLINK: return "uplink";
        case KR_SOURCE: return "source";
        case KR_TRANSMIT: return "transmit";
      }
      break;
    }
    case KR_KR_XFER_ACCEPTOR_TYPE_META_INFO: {
      switch(value) {
        case KR_XFER_ACCEPT_DIR: return "dir";
        case KR_XFER_ACCEPT_FILE: return "file";
        case KR_XFER_ACCEPT_BUF: return "buf";
      }
      break;
    }
    case KR_KR_XFER_TRANSMITTER_TYPE_META_INFO: {
      switch(value) {
        case KR_XFER_TRANSMITTER_HTTP: return "http";
        case KR_XFER_TRANSMITTER_WS: return "ws";
      }
      break;
    }
    case KR_KR_XFER_SHARE_TYPE_META_INFO: {
      switch(value) {
        case KR_XFER_SHARE_DIR: return "dir";
        case KR_XFER_SHARE_FILE: return "file";
        case KR_XFER_SHARE_BUF: return "buf";
      }
      break;
    }
    case KR_KR_XFER_SERVER_TYPE_META_INFO: {
      switch(value) {
        case KR_XFER_SHARING: return "sharing";
        case KR_XFER_ACCEPTING: return "accepting";
        case KR_XFER_TRANSMITTER: return "transmitter";
        case KR_XFER_RECEIVER: return "receiver";
      }
      break;
    }
    case KR_KR_XFER_PATH_TYPE_META_INFO: {
      switch(value) {
        case KR_XFER_SERVER: return "server";
        case KR_XFER_CLIENT: return "client";
      }
      break;
    }
    case KR_KR_XPDR_TYPE_META_INFO: {
      switch(value) {
        case KR_JACK: return "jack";
        case KR_WAYLAND: return "wayland";
        case KR_V4L2: return "v4l2";
        case KR_DECKLINK: return "decklink";
        case KR_X11: return "x11";
        case KR_ALSA: return "alsa";
        case KR_AUX: return "aux";
        case KR_JACK_IN: return "jack_in";
        case KR_JACK_OUT: return "jack_out";
        case KR_WAYLAND_OUT: return "wayland_out";
        case KR_V4L2_IN: return "v4l2_in";
        case KR_DECKLINK_VIDEO_IN: return "decklink_video_in";
        case KR_DECKLINK_AUDIO_IN: return "decklink_audio_in";
        case KR_X11_IN: return "x11_in";
        case KR_ALSA_IN: return "alsa_in";
        case KR_ALSA_OUT: return "alsa_out";
        case KR_AUX_VIDEO_IN: return "aux_video_in";
        case KR_AUX_VIDEO_OUT: return "aux_video_out";
        case KR_AUX_AUDIO_IN: return "aux_audio_in";
        case KR_AUX_AUDIO_OUT: return "aux_audio_out";
      }
      break;
    }
  }
  return "unknown";
}

int str_to_enum(int idx, char *str) {
  const char *name;
  name = strstr(str, "KR_");
  if (name) name = str + 3;
  else name = str;
  switch(idx) {
    case KR_KR_DECKLINK_VIDEO_CONN_TYPE_META_INFO: {
      if (!strcasecmp(name, "video_default")) {
        return KR_DL_VIDEO_DEFAULT;
      }
      if (!strcasecmp(name, "sdi")) {
        return KR_DL_SDI;
      }
      if (!strcasecmp(name, "optical_sdi")) {
        return KR_DL_OPTICAL_SDI;
      }
      if (!strcasecmp(name, "hdmi")) {
        return KR_DL_HDMI;
      }
      if (!strcasecmp(name, "component")) {
        return KR_DL_COMPONENT;
      }
      if (!strcasecmp(name, "composite")) {
        return KR_DL_COMPOSITE;
      }
      if (!strcasecmp(name, "svideo")) {
        return KR_DL_SVIDEO;
      }
      break;
    }
    case KR_KR_DECKLINK_AUDIO_CONN_TYPE_META_INFO: {
      if (!strcasecmp(name, "audio_default")) {
        return KR_DL_AUDIO_DEFAULT;
      }
      if (!strcasecmp(name, "embedded")) {
        return KR_DL_EMBEDDED;
      }
      if (!strcasecmp(name, "analog")) {
        return KR_DL_ANALOG;
      }
      if (!strcasecmp(name, "aesebu")) {
        return KR_DL_AESEBU;
      }
      break;
    }
    case KR_KR_V4L2_STATE_META_INFO: {
      if (!strcasecmp(name, "void")) {
        return KR_V4L2_VOID;
      }
      if (!strcasecmp(name, "open")) {
        return KR_V4L2_OPEN;
      }
      if (!strcasecmp(name, "capture")) {
        return KR_V4L2_CAPTURE;
      }
      break;
    }
    case KR_KR_AUX_PORT_STATE_META_INFO: {
      if (!strcasecmp(name, "disconnected")) {
        return KR_AUX_PORT_DISCONNECTED;
      }
      if (!strcasecmp(name, "connected")) {
        return KR_AUX_PORT_CONNECTED;
      }
      break;
    }
    case KR_KR_JACK_DIRECTION_META_INFO: {
      if (!strcasecmp(name, "input")) {
        return KR_JACK_INPUT;
      }
      if (!strcasecmp(name, "output")) {
        return KR_JACK_OUTPUT;
      }
      break;
    }
    case KR_KR_JACK_STATE_META_INFO: {
      if (!strcasecmp(name, "offline")) {
        return KR_JACK_OFFLINE;
      }
      if (!strcasecmp(name, "online")) {
        return KR_JACK_ONLINE;
      }
      break;
    }
    case KR_KR_JACK_PATH_STATE_META_INFO: {
      if (!strcasecmp(name, "fail")) {
        return KR_JACK_PATH_FAIL;
      }
      if (!strcasecmp(name, "active")) {
        return KR_JACK_PATH_ACTIVE;
      }
      break;
    }
    case KR_KR_WAYLAND_STATE_META_INFO: {
      if (!strcasecmp(name, "disconnected")) {
        return KR_WL_DISCONNECTED;
      }
      if (!strcasecmp(name, "connected")) {
        return KR_WL_CONNECTED;
      }
      break;
    }
    case KR_KR_AFX_EFFECT_TYPE_META_INFO: {
      if (!strcasecmp(name, "none")) {
        return KR_AFX_NONE;
      }
      if (!strcasecmp(name, "eq")) {
        return KR_AFX_EQ;
      }
      if (!strcasecmp(name, "lowpass")) {
        return KR_AFX_LOWPASS;
      }
      if (!strcasecmp(name, "highpass")) {
        return KR_AFX_HIGHPASS;
      }
      if (!strcasecmp(name, "analog")) {
        return KR_AFX_ANALOG;
      }
      if (!strcasecmp(name, "volume")) {
        return KR_AFX_VOLUME;
      }
      break;
    }
    case KR_KR_AFX_EFFECT_CONTROL_META_INFO: {
      if (!strcasecmp(name, "db")) {
        return KR_AFX_DB;
      }
      if (!strcasecmp(name, "bw")) {
        return KR_AFX_BW;
      }
      if (!strcasecmp(name, "hz")) {
        return KR_AFX_HZ;
      }
      if (!strcasecmp(name, "drive")) {
        return KR_AFX_DRIVE;
      }
      if (!strcasecmp(name, "blend")) {
        return KR_AFX_BLEND;
      }
      break;
    }
    case KR_KR_VIDEO_CODEC_META_INFO: {
      if (!strcasecmp(name, "daala")) {
        return KR_VIDEO_CODEC_DAALA;
      }
      if (!strcasecmp(name, "vpx")) {
        return KR_VIDEO_CODEC_VPX;
      }
      if (!strcasecmp(name, "h264")) {
        return KR_VIDEO_CODEC_H264;
      }
      if (!strcasecmp(name, "theora")) {
        return KR_VIDEO_CODEC_THEORA;
      }
      if (!strcasecmp(name, "y4m")) {
        return KR_VIDEO_CODEC_Y4M;
      }
      break;
    }
    case KR_KR_AUDIO_CODEC_META_INFO: {
      if (!strcasecmp(name, "flac")) {
        return KR_AUDIO_CODEC_FLAC;
      }
      if (!strcasecmp(name, "opus")) {
        return KR_AUDIO_CODEC_OPUS;
      }
      if (!strcasecmp(name, "vorbis")) {
        return KR_AUDIO_CODEC_VORBIS;
      }
      if (!strcasecmp(name, "mp3")) {
        return KR_AUDIO_CODEC_MP3;
      }
      if (!strcasecmp(name, "aac")) {
        return KR_AUDIO_CODEC_AAC;
      }
      break;
    }
    case KR_KR_CODEC_TYPE_META_INFO: {
      if (!strcasecmp(name, "audio")) {
        return KR_CODEC_AUDIO;
      }
      if (!strcasecmp(name, "video")) {
        return KR_CODEC_VIDEO;
      }
      break;
    }
    case KR_KR_OPUS_APP_META_INFO: {
      if (!strcasecmp(name, "audio")) {
        return KR_OPUS_APP_AUDIO;
      }
      if (!strcasecmp(name, "lowdelay")) {
        return KR_OPUS_APP_LOWDELAY;
      }
      if (!strcasecmp(name, "voip")) {
        return KR_OPUS_APP_VOIP;
      }
      break;
    }
    case KR_KR_COMPOSITOR_PATH_TYPE_META_INFO: {
      if (!strcasecmp(name, "source")) {
        return KR_COM_SOURCE;
      }
      if (!strcasecmp(name, "overlay")) {
        return KR_COM_OVERLAY;
      }
      if (!strcasecmp(name, "input")) {
        return KR_COM_INPUT;
      }
      if (!strcasecmp(name, "bus")) {
        return KR_COM_BUS;
      }
      if (!strcasecmp(name, "output")) {
        return KR_COM_OUTPUT;
      }
      break;
    }
    case KR_KR_COMPOSITOR_OVERLAY_TYPE_META_INFO: {
      if (!strcasecmp(name, "text")) {
        return KR_TEXT;
      }
      if (!strcasecmp(name, "vector")) {
        return KR_VECTOR;
      }
      break;
    }
    case KR_KR_MTS_STREAM_TYPE_META_INFO: {
      if (!strcasecmp(name, "video")) {
        return MTS_VIDEO;
      }
      if (!strcasecmp(name, "audio")) {
        return MTS_AUDIO;
      }
      if (!strcasecmp(name, "other")) {
        return MTS_OTHER;
      }
      break;
    }
    case KR_KR_CONTAINER_TYPE_META_INFO: {
      if (!strcasecmp(name, "raw")) {
        return KR_CONTAINER_RAW;
      }
      if (!strcasecmp(name, "mkv")) {
        return KR_CONTAINER_MKV;
      }
      if (!strcasecmp(name, "webm")) {
        return KR_CONTAINER_WEBM;
      }
      if (!strcasecmp(name, "ogg")) {
        return KR_CONTAINER_OGG;
      }
      if (!strcasecmp(name, "flac")) {
        return KR_CONTAINER_FLAC;
      }
      if (!strcasecmp(name, "y4m")) {
        return KR_CONTAINER_Y4M;
      }
      if (!strcasecmp(name, "txt")) {
        return KR_CONTAINER_TXT;
      }
      if (!strcasecmp(name, "ts")) {
        return KR_CONTAINER_TS;
      }
      if (!strcasecmp(name, "occ")) {
        return KR_CONTAINER_OCC;
      }
      break;
    }
    case KR_KR_VECTOR_TYPE_META_INFO: {
      if (!strcasecmp(name, "nothing")) {
        return NOTHING;
      }
      if (!strcasecmp(name, "hex")) {
        return HEX;
      }
      if (!strcasecmp(name, "circle")) {
        return CIRCLE;
      }
      if (!strcasecmp(name, "rect")) {
        return RECT;
      }
      if (!strcasecmp(name, "triangle")) {
        return TRIANGLE;
      }
      if (!strcasecmp(name, "viper")) {
        return VIPER;
      }
      if (!strcasecmp(name, "meter")) {
        return METER;
      }
      if (!strcasecmp(name, "grid")) {
        return GRID;
      }
      if (!strcasecmp(name, "curve")) {
        return CURVE;
      }
      if (!strcasecmp(name, "arrow")) {
        return ARROW;
      }
      if (!strcasecmp(name, "clock")) {
        return CLOCK;
      }
      if (!strcasecmp(name, "shadow")) {
        return SHADOW;
      }
      break;
    }
    case KR_KR_FEATURE_ASPECT_TYPE_META_INFO: {
      if (!strcasecmp(name, "comments")) {
        return KR_FEATURE_COMMENTS;
      }
      if (!strcasecmp(name, "tag")) {
        return KR_FEATURE_TAG;
      }
      if (!strcasecmp(name, "header")) {
        return KR_FEATURE_HEADER;
      }
      if (!strcasecmp(name, "function")) {
        return KR_FEATURE_FUNCTION;
      }
      if (!strcasecmp(name, "story")) {
        return KR_FEATURE_STORY;
      }
      break;
    }
    case KR_KR_PIXEL_FMT_META_INFO: {
      if (!strcasecmp(name, "argb")) {
        return KR_PIXELS_ARGB;
      }
      if (!strcasecmp(name, "yuv422p")) {
        return KR_PIXELS_YUV422P;
      }
      if (!strcasecmp(name, "yvu422p")) {
        return KR_PIXELS_YVU422P;
      }
      if (!strcasecmp(name, "yuv444")) {
        return KR_PIXELS_YUV444;
      }
      if (!strcasecmp(name, "yuv420")) {
        return KR_PIXELS_YUV420;
      }
      if (!strcasecmp(name, "yuv422")) {
        return KR_PIXELS_YUV422;
      }
      break;
    }
    case KR_KR_IMAGE_TYPE_META_INFO: {
      if (!strcasecmp(name, "enc")) {
        return KR_IMAGE_ENC;
      }
      if (!strcasecmp(name, "raw")) {
        return KR_IMAGE_RAW;
      }
      break;
    }
    case KR_KR_CODED_IMAGE_TYPE_META_INFO: {
      if (!strcasecmp(name, "png")) {
        return KR_CODED_IMAGE_PNG;
      }
      if (!strcasecmp(name, "jpg")) {
        return KR_CODED_IMAGE_JPG;
      }
      if (!strcasecmp(name, "gif")) {
        return KR_CODED_IMAGE_GIF;
      }
      if (!strcasecmp(name, "vp8")) {
        return KR_CODED_IMAGE_VP8;
      }
      if (!strcasecmp(name, "vp9")) {
        return KR_CODED_IMAGE_VP9;
      }
      if (!strcasecmp(name, "daala")) {
        return KR_CODED_IMAGE_DAALA;
      }
      if (!strcasecmp(name, "theora")) {
        return KR_CODED_IMAGE_THEORA;
      }
      if (!strcasecmp(name, "y4m")) {
        return KR_CODED_IMAGE_Y4M;
      }
      if (!strcasecmp(name, "h264")) {
        return KR_CODED_IMAGE_H264;
      }
      break;
    }
    case KR_KR_FILE_ACCESS_MODE_META_INFO: {
      if (!strcasecmp(name, "only")) {
        return KR_FILE_ACCESS_ONLY;
      }
      if (!strcasecmp(name, "append")) {
        return KR_FILE_ACCESS_APPEND;
      }
      if (!strcasecmp(name, "modify")) {
        return KR_FILE_ACCESS_MODIFY;
      }
      break;
    }
    case KR_KR_MIXER_CHANNELS_META_INFO: {
      if (!strcasecmp(name, "mono")) {
        return KR_MONO;
      }
      if (!strcasecmp(name, "stereo")) {
        return KR_STEREO;
      }
      if (!strcasecmp(name, "three")) {
        return KR_THREE;
      }
      if (!strcasecmp(name, "quad")) {
        return KR_QUAD;
      }
      if (!strcasecmp(name, "five")) {
        return KR_FIVE;
      }
      if (!strcasecmp(name, "six")) {
        return KR_SIX;
      }
      if (!strcasecmp(name, "seven")) {
        return KR_SEVEN;
      }
      if (!strcasecmp(name, "eight")) {
        return KR_EIGHT;
      }
      break;
    }
    case KR_KR_MIXER_PATH_TYPE_META_INFO: {
      if (!strcasecmp(name, "source")) {
        return KR_MXR_SOURCE;
      }
      if (!strcasecmp(name, "input")) {
        return KR_MXR_INPUT;
      }
      if (!strcasecmp(name, "bus")) {
        return KR_MXR_BUS;
      }
      if (!strcasecmp(name, "output")) {
        return KR_MXR_OUTPUT;
      }
      break;
    }
    case KR_KR_MEDIA_EVENT_TYPE_META_INFO: {
      if (!strcasecmp(name, "create")) {
        return KR_MEDIA_CREATE;
      }
      if (!strcasecmp(name, "item_add")) {
        return KR_MEDIA_ITEM_ADD;
      }
      if (!strcasecmp(name, "file_create")) {
        return KR_MEDIA_FILE_CREATE;
      }
      if (!strcasecmp(name, "destroy")) {
        return KR_MEDIA_DESTROY;
      }
      break;
    }
    case KR_KR_MEDIA_ITEM_TYPE_META_INFO: {
      if (!strcasecmp(name, "url")) {
        return KR_MEDIA_URL;
      }
      if (!strcasecmp(name, "metadata")) {
        return KR_MEDIA_METADATA;
      }
      if (!strcasecmp(name, "file")) {
        return KR_MEDIA_FILE;
      }
      break;
    }
    case KR_M3U_EL_TYPE_META_INFO: {
      if (!strcasecmp(name, "uri")) {
        return M3U_URI;
      }
      if (!strcasecmp(name, "tag")) {
        return M3U_TAG;
      }
      break;
    }
    case KR_KR_M3U_TAG_NAME_META_INFO: {
      if (!strcasecmp(name, "extm3u")) {
        return EXTM3U;
      }
      if (!strcasecmp(name, "extinf")) {
        return EXTINF;
      }
      if (!strcasecmp(name, "ext_x_byterange")) {
        return EXT_X_BYTERANGE;
      }
      if (!strcasecmp(name, "ext_x_targetduration")) {
        return EXT_X_TARGETDURATION;
      }
      if (!strcasecmp(name, "ext_x_media_sequence")) {
        return EXT_X_MEDIA_SEQUENCE;
      }
      if (!strcasecmp(name, "ext_x_key")) {
        return EXT_X_KEY;
      }
      if (!strcasecmp(name, "ext_x_program_date_time")) {
        return EXT_X_PROGRAM_DATE_TIME;
      }
      if (!strcasecmp(name, "ext_x_allow_cache")) {
        return EXT_X_ALLOW_CACHE;
      }
      if (!strcasecmp(name, "ext_x_playlist_type")) {
        return EXT_X_PLAYLIST_TYPE;
      }
      if (!strcasecmp(name, "ext_x_stream_inf")) {
        return EXT_X_STREAM_INF;
      }
      if (!strcasecmp(name, "ext_x_i_frame_stream_inf")) {
        return EXT_X_I_FRAME_STREAM_INF;
      }
      if (!strcasecmp(name, "ext_x_i_frames_only")) {
        return EXT_X_I_FRAMES_ONLY;
      }
      if (!strcasecmp(name, "ext_x_map")) {
        return EXT_X_MAP;
      }
      if (!strcasecmp(name, "ext_x_media")) {
        return EXT_X_MEDIA;
      }
      if (!strcasecmp(name, "ext_x_endlist")) {
        return EXT_X_ENDLIST;
      }
      if (!strcasecmp(name, "ext_x_discontinuity")) {
        return EXT_X_DISCONTINUITY;
      }
      if (!strcasecmp(name, "ext_x_discontinuity_sequence")) {
        return EXT_X_DISCONTINUITY_SEQUENCE;
      }
      if (!strcasecmp(name, "ext_x_independent_segments")) {
        return EXT_X_INDEPENDENT_SEGMENTS;
      }
      if (!strcasecmp(name, "ext_x_start")) {
        return EXT_X_START;
      }
      if (!strcasecmp(name, "ext_x_version")) {
        return EXT_X_VERSION;
      }
      break;
    }
    case KR_KR_M3U_TAG_VAL_TYPE_META_INFO: {
      if (!strcasecmp(name, "primitive")) {
        return M3U_TAG_VAL_PRIMITIVE;
      }
      if (!strcasecmp(name, "attr_list")) {
        return M3U_TAG_VAL_ATTR_LIST;
      }
      break;
    }
    case KR_KR_M3U_TAG_ATTR_NAME_META_INFO: {
      if (!strcasecmp(name, "method")) {
        return M3U_ATTR_METHOD;
      }
      if (!strcasecmp(name, "uri")) {
        return M3U_ATTR_URI;
      }
      if (!strcasecmp(name, "iv")) {
        return M3U_ATTR_IV;
      }
      if (!strcasecmp(name, "keyformat")) {
        return M3U_ATTR_KEYFORMAT;
      }
      if (!strcasecmp(name, "keyformatversions")) {
        return M3U_ATTR_KEYFORMATVERSIONS;
      }
      if (!strcasecmp(name, "type")) {
        return M3U_ATTR_TYPE;
      }
      if (!strcasecmp(name, "group_id")) {
        return M3U_ATTR_GROUP_ID;
      }
      if (!strcasecmp(name, "program_id")) {
        return M3U_ATTR_PROGRAM_ID;
      }
      if (!strcasecmp(name, "language")) {
        return M3U_ATTR_LANGUAGE;
      }
      if (!strcasecmp(name, "assoc_language")) {
        return M3U_ATTR_ASSOC_LANGUAGE;
      }
      if (!strcasecmp(name, "name")) {
        return M3U_ATTR_NAME;
      }
      if (!strcasecmp(name, "default")) {
        return M3U_ATTR_DEFAULT;
      }
      if (!strcasecmp(name, "autoselect")) {
        return M3U_ATTR_AUTOSELECT;
      }
      if (!strcasecmp(name, "forced")) {
        return M3U_ATTR_FORCED;
      }
      if (!strcasecmp(name, "instream_id")) {
        return M3U_ATTR_INSTREAM_ID;
      }
      if (!strcasecmp(name, "characteristics")) {
        return M3U_ATTR_CHARACTERISTICS;
      }
      if (!strcasecmp(name, "bandwidth")) {
        return M3U_ATTR_BANDWIDTH;
      }
      if (!strcasecmp(name, "codecs")) {
        return M3U_ATTR_CODECS;
      }
      if (!strcasecmp(name, "resolution")) {
        return M3U_ATTR_RESOLUTION;
      }
      if (!strcasecmp(name, "audio")) {
        return M3U_ATTR_AUDIO;
      }
      if (!strcasecmp(name, "video")) {
        return M3U_ATTR_VIDEO;
      }
      if (!strcasecmp(name, "subtitles")) {
        return M3U_ATTR_SUBTITLES;
      }
      if (!strcasecmp(name, "closed_captions")) {
        return M3U_ATTR_CLOSED_CAPTIONS;
      }
      if (!strcasecmp(name, "byterange")) {
        return M3U_ATTR_BYTERANGE;
      }
      if (!strcasecmp(name, "time_offest")) {
        return M3U_ATTR_TIME_OFFEST;
      }
      if (!strcasecmp(name, "precise")) {
        return M3U_ATTR_PRECISE;
      }
      break;
    }
    case KR_KR_PRIORITY_META_INFO: {
      if (!strcasecmp(name, "unknown")) {
        return KR_PRIORITY_UNKNOWN;
      }
      if (!strcasecmp(name, "normal")) {
        return KR_PRIORITY_NORMAL;
      }
      if (!strcasecmp(name, "noninteractive")) {
        return KR_PRIORITY_NONINTERACTIVE;
      }
      if (!strcasecmp(name, "transcoder")) {
        return KR_PRIORITY_TRANSCODER;
      }
      if (!strcasecmp(name, "realtime")) {
        return KR_PRIORITY_REALTIME;
      }
      if (!strcasecmp(name, "realtime_io")) {
        return KR_PRIORITY_REALTIME_IO;
      }
      break;
    }
    case KR_KR_AVPROBE_CODEC_TYPE_META_INFO: {
      if (!strcasecmp(name, "audio")) {
        return KR_AUDIO;
      }
      if (!strcasecmp(name, "video")) {
        return KR_VIDEO;
      }
      break;
    }
    case KR_KR_TASK_TYPE_META_INFO: {
      if (!strcasecmp(name, "valgrind_stressor")) {
        return KR_VALGRIND_STRESSOR;
      }
      if (!strcasecmp(name, "quvi")) {
        return KR_QUVI;
      }
      if (!strcasecmp(name, "avconv")) {
        return KR_AVCONV;
      }
      if (!strcasecmp(name, "xmms")) {
        return KR_XMMS;
      }
      if (!strcasecmp(name, "curl")) {
        return KR_CURL;
      }
      if (!strcasecmp(name, "transmission_cli")) {
        return KR_TRANSMISSION_CLI;
      }
      if (!strcasecmp(name, "livestreamer")) {
        return KR_LIVESTREAMER;
      }
      if (!strcasecmp(name, "avprobe")) {
        return KR_AVPROBE;
      }
      if (!strcasecmp(name, "ydl")) {
        return KR_YDL;
      }
      break;
    }
    case KR_KR_TASK_STATE_META_INFO: {
      if (!strcasecmp(name, "ready")) {
        return KR_TASK_READY;
      }
      if (!strcasecmp(name, "running")) {
        return KR_TASK_RUNNING;
      }
      if (!strcasecmp(name, "failed")) {
        return KR_TASK_FAILED;
      }
      if (!strcasecmp(name, "timedout")) {
        return KR_TASK_TIMEDOUT;
      }
      if (!strcasecmp(name, "succeeded")) {
        return KR_TASK_SUCCEEDED;
      }
      if (!strcasecmp(name, "cancelled")) {
        return KR_TASK_CANCELLED;
      }
      if (!strcasecmp(name, "same_state")) {
        return KR_TASK_SAME_STATE;
      }
      break;
    }
    case KR_KR_TASK_EVENT_TYPE_META_INFO: {
      if (!strcasecmp(name, "create")) {
        return KR_TASK_CREATE;
      }
      if (!strcasecmp(name, "start")) {
        return KR_TASK_START;
      }
      if (!strcasecmp(name, "patch")) {
        return KR_TASK_PATCH;
      }
      if (!strcasecmp(name, "progress")) {
        return KR_TASK_PROGRESS;
      }
      if (!strcasecmp(name, "failure")) {
        return KR_TASK_FAILURE;
      }
      if (!strcasecmp(name, "success")) {
        return KR_TASK_SUCCESS;
      }
      if (!strcasecmp(name, "cancel")) {
        return KR_TASK_CANCEL;
      }
      if (!strcasecmp(name, "timeout")) {
        return KR_TASK_TIMEOUT;
      }
      if (!strcasecmp(name, "destroy")) {
        return KR_TASK_DESTROY;
      }
      break;
    }
    case KR_KR_EASING_META_INFO: {
      if (!strcasecmp(name, "linear")) {
        return LINEAR;
      }
      if (!strcasecmp(name, "easeinsine")) {
        return EASEINSINE;
      }
      if (!strcasecmp(name, "easeoutsine")) {
        return EASEOUTSINE;
      }
      if (!strcasecmp(name, "easeinoutsine")) {
        return EASEINOUTSINE;
      }
      if (!strcasecmp(name, "easeincubic")) {
        return EASEINCUBIC;
      }
      if (!strcasecmp(name, "easeoutcubic")) {
        return EASEOUTCUBIC;
      }
      if (!strcasecmp(name, "easeinoutcubic")) {
        return EASEINOUTCUBIC;
      }
      if (!strcasecmp(name, "easeinoutelastic")) {
        return EASEINOUTELASTIC;
      }
      break;
    }
    case KR_KR_MEDIA_SOURCE_FORMAT_META_INFO: {
      if (!strcasecmp(name, "media_source_mts")) {
        return MEDIA_SOURCE_MTS;
      }
      break;
    }
    case KR_KR_MEDIA_PACKET_TYPE_META_INFO: {
      if (!strcasecmp(name, "header")) {
        return KR_PKT_HEADER;
      }
      if (!strcasecmp(name, "key")) {
        return KR_PKT_KEY;
      }
      if (!strcasecmp(name, "normal")) {
        return KR_PKT_NORMAL;
      }
      break;
    }
    case KR_KR_ENCODE_TYPE_META_INFO: {
      if (!strcasecmp(name, "audio")) {
        return KR_ENCODE_AUDIO;
      }
      if (!strcasecmp(name, "video")) {
        return KR_ENCODE_VIDEO;
      }
      break;
    }
    case KR_KR_CAPTURE_TYPE_META_INFO: {
      if (!strcasecmp(name, "audio")) {
        return KR_CAPTURE_AUDIO;
      }
      if (!strcasecmp(name, "video")) {
        return KR_CAPTURE_VIDEO;
      }
      if (!strcasecmp(name, "image")) {
        return KR_CAPTURE_IMAGE;
      }
      break;
    }
    case KR_KR_OUTPUT_TYPE_META_INFO: {
      if (!strcasecmp(name, "record")) {
        return KR_STUDIO_RECORD;
      }
      if (!strcasecmp(name, "uplink")) {
        return KR_STUDIO_UPLINK;
      }
      if (!strcasecmp(name, "transmission")) {
        return KR_STUDIO_TRANSMISSION;
      }
      break;
    }
    case KR_KR_STUDIO_SOURCE_STREAM_TYPE_META_INFO: {
      if (!strcasecmp(name, "accept")) {
        return KR_SOURCE_STREAM_ACCEPT;
      }
      if (!strcasecmp(name, "downlink")) {
        return KR_SOURCE_STREAM_DOWNLINK;
      }
      break;
    }
    case KR_KR_STUDIO_SOURCE_TYPE_META_INFO: {
      if (!strcasecmp(name, "local_file")) {
        return KR_SOURCE_LOCAL_FILE;
      }
      if (!strcasecmp(name, "remote_file")) {
        return KR_SOURCE_REMOTE_FILE;
      }
      if (!strcasecmp(name, "stream")) {
        return KR_SOURCE_STREAM;
      }
      break;
    }
    case KR_KR_RENDER_TYPE_META_INFO: {
      if (!strcasecmp(name, "image")) {
        return KR_RENDER_IMAGE;
      }
      if (!strcasecmp(name, "video")) {
        return KR_RENDER_VIDEO;
      }
      if (!strcasecmp(name, "audio")) {
        return KR_RENDER_AUDIO;
      }
      break;
    }
    case KR_KR_AUDIO_GENERATE_TYPE_META_INFO: {
      if (!strcasecmp(name, "noise")) {
        return KR_GENERATE_NOISE;
      }
      if (!strcasecmp(name, "tone")) {
        return KR_GENERATE_TONE;
      }
      if (!strcasecmp(name, "silence")) {
        return KR_GENERATE_SILENCE;
      }
      break;
    }
    case KR_KR_VIDEO_GENERATE_TYPE_META_INFO: {
      if (!strcasecmp(name, "color")) {
        return KR_GENERATE_COLOR;
      }
      if (!strcasecmp(name, "no_signal")) {
        return KR_GENERATE_NO_SIGNAL;
      }
      break;
    }
    case KR_KR_GENERATE_TYPE_META_INFO: {
      if (!strcasecmp(name, "audio")) {
        return KR_GENERATE_AUDIO;
      }
      if (!strcasecmp(name, "video")) {
        return KR_GENERATE_VIDEO;
      }
      break;
    }
    case KR_KR_STUDIO_PATH_TYPE_META_INFO: {
      if (!strcasecmp(name, "source")) {
        return KR_STUDIO_SOURCE;
      }
      if (!strcasecmp(name, "generate")) {
        return KR_STUDIO_GENERATE;
      }
      if (!strcasecmp(name, "capture")) {
        return KR_STUDIO_CAPTURE;
      }
      if (!strcasecmp(name, "render")) {
        return KR_STUDIO_RENDER;
      }
      if (!strcasecmp(name, "play")) {
        return KR_STUDIO_PLAY;
      }
      if (!strcasecmp(name, "encode")) {
        return KR_STUDIO_ENCODE;
      }
      if (!strcasecmp(name, "decode")) {
        return KR_STUDIO_DECODE;
      }
      if (!strcasecmp(name, "demux")) {
        return KR_STUDIO_DEMUX;
      }
      if (!strcasecmp(name, "mux")) {
        return KR_STUDIO_MUX;
      }
      if (!strcasecmp(name, "output")) {
        return KR_STUDIO_OUTPUT;
      }
      break;
    }
    case KR_KR_TRACK_BLOCK_TYPE_META_INFO: {
      if (!strcasecmp(name, "coded_audio")) {
        return KR_TRACK_CODED_AUDIO;
      }
      if (!strcasecmp(name, "coded_video")) {
        return KR_TRACK_CODED_VIDEO;
      }
      if (!strcasecmp(name, "mux")) {
        return KR_TRACK_MUX;
      }
      if (!strcasecmp(name, "audio")) {
        return KR_TRACK_AUDIO;
      }
      if (!strcasecmp(name, "video")) {
        return KR_TRACK_VIDEO;
      }
      if (!strcasecmp(name, "subtitle")) {
        return KR_TRACK_SUBTITLE;
      }
      if (!strcasecmp(name, "metadata")) {
        return KR_TRACK_METADATA;
      }
      if (!strcasecmp(name, "control")) {
        return KR_TRACK_CONTROL;
      }
      break;
    }
    case KR_KR_TRACK_MODE_META_INFO: {
      if (!strcasecmp(name, "finite")) {
        return KR_TRACK_FINITE;
      }
      if (!strcasecmp(name, "infinite")) {
        return KR_TRACK_INFINITE;
      }
      break;
    }
    case KR_KR_TRACK_EVENT_TYPE_META_INFO: {
      if (!strcasecmp(name, "add")) {
        return KR_TRACK_ADD;
      }
      if (!strcasecmp(name, "remove")) {
        return KR_TRACK_REMOVE;
      }
      if (!strcasecmp(name, "read")) {
        return KR_TRACK_READ;
      }
      if (!strcasecmp(name, "write")) {
        return KR_TRACK_WRITE;
      }
      break;
    }
    case KR_KR_HLS_PLAYLIST_TYPE_META_INFO: {
      if (!strcasecmp(name, "master")) {
        return KR_HLS_PLAYLIST_MASTER;
      }
      if (!strcasecmp(name, "media")) {
        return KR_HLS_PLAYLIST_MEDIA;
      }
      break;
    }
    case KR_KR_HLS_RENDITION_TYPE_META_INFO: {
      if (!strcasecmp(name, "audio")) {
        return HLS_AUDIO;
      }
      if (!strcasecmp(name, "video")) {
        return HLS_VIDEO;
      }
      if (!strcasecmp(name, "subtitles")) {
        return HLS_SUBTITLES;
      }
      if (!strcasecmp(name, "closed_captions")) {
        return HLS_CLOSED_CAPTIONS;
      }
      break;
    }
    case KR_KR_HLS_MEDIA_PLAYLIST_TYPE_META_INFO: {
      if (!strcasecmp(name, "vod")) {
        return HLS_VOD;
      }
      if (!strcasecmp(name, "live")) {
        return HLS_LIVE;
      }
      break;
    }
    case KR_KR_RADIO_MODE_META_INFO: {
      if (!strcasecmp(name, "audio")) {
        return KR_RADIO_MODE_AUDIO;
      }
      if (!strcasecmp(name, "audiovideo")) {
        return KR_RADIO_MODE_AUDIOVIDEO;
      }
      break;
    }
    case KR_KR_WEB_CLIENT_PROTOCOL_META_INFO: {
      if (!strcasecmp(name, "http")) {
        return KR_PROTOCOL_HTTP;
      }
      if (!strcasecmp(name, "websocket")) {
        return KR_PROTOCOL_WEBSOCKET;
      }
      if (!strcasecmp(name, "tcp")) {
        return KR_PROTOCOL_TCP;
      }
      break;
    }
    case KR_KR_WEB_CLIENT_TYPE_META_INFO: {
      if (!strcasecmp(name, "accept")) {
        return KR_WEB_CLIENT_ACCEPT;
      }
      if (!strcasecmp(name, "connect")) {
        return KR_WEB_CLIENT_CONNECT;
      }
      break;
    }
    case KR_KR_WEB_CLIENT_STATE_META_INFO: {
      if (!strcasecmp(name, "resolving")) {
        return KR_WEB_RESOLVING;
      }
      if (!strcasecmp(name, "connecting")) {
        return KR_WEB_CONNECTING;
      }
      if (!strcasecmp(name, "active")) {
        return KR_WEB_ACTIVE;
      }
      if (!strcasecmp(name, "flushing")) {
        return KR_WEB_FLUSHING;
      }
      break;
    }
    case KR_KR_WEB_SERVER_TYPE_META_INFO: {
      if (!strcasecmp(name, "http")) {
        return KR_WEB_SERVER_HTTP;
      }
      if (!strcasecmp(name, "websocket")) {
        return KR_WEB_SERVER_WEBSOCKET;
      }
      if (!strcasecmp(name, "http_websocket")) {
        return KR_WEB_SERVER_HTTP_WEBSOCKET;
      }
      break;
    }
    case KR_KR_MULTIPART_STATE_META_INFO: {
      if (!strcasecmp(name, "done")) {
        return KR_MULTIPART_DONE;
      }
      if (!strcasecmp(name, "init")) {
        return KR_MULTIPART_INIT;
      }
      if (!strcasecmp(name, "info")) {
        return KR_MULTIPART_INFO;
      }
      if (!strcasecmp(name, "data")) {
        return KR_MULTIPART_DATA;
      }
      break;
    }
    case KR_KR_WEBSOCKET_FRAME_TYPE_META_INFO: {
      if (!strcasecmp(name, "binary")) {
        return KR_WEBSOCKET_BINARY;
      }
      if (!strcasecmp(name, "text")) {
        return KR_WEBSOCKET_TEXT;
      }
      break;
    }
    case KR_KR_WEBSOCKET_PROTOCOL_META_INFO: {
      if (!strcasecmp(name, "unknown")) {
        return KR_WS_PROTOCOL_UNKNOWN;
      }
      if (!strcasecmp(name, "krad")) {
        return KR_WS_PROTOCOL_KRAD;
      }
      if (!strcasecmp(name, "stream")) {
        return KR_WS_PROTOCOL_STREAM;
      }
      if (!strcasecmp(name, "xmms")) {
        return KR_WS_PROTOCOL_XMMS;
      }
      break;
    }
    case KR_KR_HTTP_MESSAGE_STATE_META_INFO: {
      if (!strcasecmp(name, "header")) {
        return KR_HTTP_IN_HEADER;
      }
      if (!strcasecmp(name, "content")) {
        return KR_HTTP_IN_CONTENT;
      }
      break;
    }
    case KR_KR_WEB_PATH_TYPE_META_INFO: {
      if (!strcasecmp(name, "server")) {
        return KR_WEB_SERVER;
      }
      if (!strcasecmp(name, "client")) {
        return KR_WEB_CLIENT;
      }
      if (!strcasecmp(name, "proxy")) {
        return KR_WEB_PROXY;
      }
      break;
    }
    case KR_KR_UPLOAD_TYPE_META_INFO: {
      if (!strcasecmp(name, "file")) {
        return KR_UPLOAD_FILE;
      }
      if (!strcasecmp(name, "stash")) {
        return KR_UPLOAD_STASH;
      }
      break;
    }
    case KR_KR_DOWNLOAD_TYPE_META_INFO: {
      if (!strcasecmp(name, "file")) {
        return KR_DOWNLOAD_FILE;
      }
      if (!strcasecmp(name, "stash")) {
        return KR_DOWNLOAD_STASH;
      }
      break;
    }
    case KR_KR_SERVE_INDEX_FORMAT_META_INFO: {
      if (!strcasecmp(name, "json")) {
        return KR_SERVE_INDEX_JSON;
      }
      if (!strcasecmp(name, "html")) {
        return KR_SERVE_INDEX_HTML;
      }
      break;
    }
    case KR_KR_SERVE_TYPE_META_INFO: {
      if (!strcasecmp(name, "full")) {
        return KR_SERVE_FULL;
      }
      if (!strcasecmp(name, "ranges")) {
        return KR_SERVE_RANGES;
      }
      if (!strcasecmp(name, "index")) {
        return KR_SERVE_INDEX;
      }
      if (!strcasecmp(name, "redirect")) {
        return KR_SERVE_REDIRECT;
      }
      break;
    }
    case KR_KR_XFER_CLIENT_TYPE_META_INFO: {
      if (!strcasecmp(name, "download")) {
        return KR_DOWNLOAD;
      }
      if (!strcasecmp(name, "upload")) {
        return KR_UPLOAD;
      }
      if (!strcasecmp(name, "accept")) {
        return KR_ACCEPT;
      }
      if (!strcasecmp(name, "serve")) {
        return KR_SERVE;
      }
      if (!strcasecmp(name, "downlink")) {
        return KR_DOWNLINK;
      }
      if (!strcasecmp(name, "uplink")) {
        return KR_UPLINK;
      }
      if (!strcasecmp(name, "source")) {
        return KR_SOURCE;
      }
      if (!strcasecmp(name, "transmit")) {
        return KR_TRANSMIT;
      }
      break;
    }
    case KR_KR_XFER_ACCEPTOR_TYPE_META_INFO: {
      if (!strcasecmp(name, "dir")) {
        return KR_XFER_ACCEPT_DIR;
      }
      if (!strcasecmp(name, "file")) {
        return KR_XFER_ACCEPT_FILE;
      }
      if (!strcasecmp(name, "buf")) {
        return KR_XFER_ACCEPT_BUF;
      }
      break;
    }
    case KR_KR_XFER_TRANSMITTER_TYPE_META_INFO: {
      if (!strcasecmp(name, "http")) {
        return KR_XFER_TRANSMITTER_HTTP;
      }
      if (!strcasecmp(name, "ws")) {
        return KR_XFER_TRANSMITTER_WS;
      }
      break;
    }
    case KR_KR_XFER_SHARE_TYPE_META_INFO: {
      if (!strcasecmp(name, "dir")) {
        return KR_XFER_SHARE_DIR;
      }
      if (!strcasecmp(name, "file")) {
        return KR_XFER_SHARE_FILE;
      }
      if (!strcasecmp(name, "buf")) {
        return KR_XFER_SHARE_BUF;
      }
      break;
    }
    case KR_KR_XFER_SERVER_TYPE_META_INFO: {
      if (!strcasecmp(name, "sharing")) {
        return KR_XFER_SHARING;
      }
      if (!strcasecmp(name, "accepting")) {
        return KR_XFER_ACCEPTING;
      }
      if (!strcasecmp(name, "transmitter")) {
        return KR_XFER_TRANSMITTER;
      }
      if (!strcasecmp(name, "receiver")) {
        return KR_XFER_RECEIVER;
      }
      break;
    }
    case KR_KR_XFER_PATH_TYPE_META_INFO: {
      if (!strcasecmp(name, "server")) {
        return KR_XFER_SERVER;
      }
      if (!strcasecmp(name, "client")) {
        return KR_XFER_CLIENT;
      }
      break;
    }
    case KR_KR_XPDR_TYPE_META_INFO: {
      if (!strcasecmp(name, "jack")) {
        return KR_JACK;
      }
      if (!strcasecmp(name, "wayland")) {
        return KR_WAYLAND;
      }
      if (!strcasecmp(name, "v4l2")) {
        return KR_V4L2;
      }
      if (!strcasecmp(name, "decklink")) {
        return KR_DECKLINK;
      }
      if (!strcasecmp(name, "x11")) {
        return KR_X11;
      }
      if (!strcasecmp(name, "alsa")) {
        return KR_ALSA;
      }
      if (!strcasecmp(name, "aux")) {
        return KR_AUX;
      }
      if (!strcasecmp(name, "jack_in")) {
        return KR_JACK_IN;
      }
      if (!strcasecmp(name, "jack_out")) {
        return KR_JACK_OUT;
      }
      if (!strcasecmp(name, "wayland_out")) {
        return KR_WAYLAND_OUT;
      }
      if (!strcasecmp(name, "v4l2_in")) {
        return KR_V4L2_IN;
      }
      if (!strcasecmp(name, "decklink_video_in")) {
        return KR_DECKLINK_VIDEO_IN;
      }
      if (!strcasecmp(name, "decklink_audio_in")) {
        return KR_DECKLINK_AUDIO_IN;
      }
      if (!strcasecmp(name, "x11_in")) {
        return KR_X11_IN;
      }
      if (!strcasecmp(name, "alsa_in")) {
        return KR_ALSA_IN;
      }
      if (!strcasecmp(name, "alsa_out")) {
        return KR_ALSA_OUT;
      }
      if (!strcasecmp(name, "aux_video_in")) {
        return KR_AUX_VIDEO_IN;
      }
      if (!strcasecmp(name, "aux_video_out")) {
        return KR_AUX_VIDEO_OUT;
      }
      if (!strcasecmp(name, "aux_audio_in")) {
        return KR_AUX_AUDIO_IN;
      }
      if (!strcasecmp(name, "aux_audio_out")) {
        return KR_AUX_AUDIO_OUT;
      }
      break;
    }
  }
  return -1;
}

int enum_to_index(int idx, int value) {
  switch(idx) {
    case KR_KR_DECKLINK_VIDEO_CONN_TYPE_META_INFO: {
      switch(value) {
        case KR_DL_VIDEO_DEFAULT: return 0;
        case KR_DL_SDI: return 1;
        case KR_DL_OPTICAL_SDI: return 2;
        case KR_DL_HDMI: return 3;
        case KR_DL_COMPONENT: return 4;
        case KR_DL_COMPOSITE: return 5;
        case KR_DL_SVIDEO: return 6;
      }
      break;
    }
    case KR_KR_DECKLINK_AUDIO_CONN_TYPE_META_INFO: {
      switch(value) {
        case KR_DL_AUDIO_DEFAULT: return 0;
        case KR_DL_EMBEDDED: return 1;
        case KR_DL_ANALOG: return 2;
        case KR_DL_AESEBU: return 3;
      }
      break;
    }
    case KR_KR_V4L2_STATE_META_INFO: {
      switch(value) {
        case KR_V4L2_VOID: return 0;
        case KR_V4L2_OPEN: return 1;
        case KR_V4L2_CAPTURE: return 2;
      }
      break;
    }
    case KR_KR_AUX_PORT_STATE_META_INFO: {
      switch(value) {
        case KR_AUX_PORT_DISCONNECTED: return 0;
        case KR_AUX_PORT_CONNECTED: return 1;
      }
      break;
    }
    case KR_KR_JACK_DIRECTION_META_INFO: {
      switch(value) {
        case KR_JACK_INPUT: return 0;
        case KR_JACK_OUTPUT: return 1;
      }
      break;
    }
    case KR_KR_JACK_STATE_META_INFO: {
      switch(value) {
        case KR_JACK_OFFLINE: return 0;
        case KR_JACK_ONLINE: return 1;
      }
      break;
    }
    case KR_KR_JACK_PATH_STATE_META_INFO: {
      switch(value) {
        case KR_JACK_PATH_FAIL: return 0;
        case KR_JACK_PATH_ACTIVE: return 1;
      }
      break;
    }
    case KR_KR_WAYLAND_STATE_META_INFO: {
      switch(value) {
        case KR_WL_DISCONNECTED: return 0;
        case KR_WL_CONNECTED: return 1;
      }
      break;
    }
    case KR_KR_AFX_EFFECT_TYPE_META_INFO: {
      switch(value) {
        case KR_AFX_NONE: return 0;
        case KR_AFX_EQ: return 1;
        case KR_AFX_LOWPASS: return 2;
        case KR_AFX_HIGHPASS: return 3;
        case KR_AFX_ANALOG: return 4;
        case KR_AFX_VOLUME: return 5;
      }
      break;
    }
    case KR_KR_AFX_EFFECT_CONTROL_META_INFO: {
      switch(value) {
        case KR_AFX_DB: return 0;
        case KR_AFX_BW: return 1;
        case KR_AFX_HZ: return 2;
        case KR_AFX_DRIVE: return 3;
        case KR_AFX_BLEND: return 4;
      }
      break;
    }
    case KR_KR_VIDEO_CODEC_META_INFO: {
      switch(value) {
        case KR_VIDEO_CODEC_DAALA: return 0;
        case KR_VIDEO_CODEC_VPX: return 1;
        case KR_VIDEO_CODEC_H264: return 2;
        case KR_VIDEO_CODEC_THEORA: return 3;
        case KR_VIDEO_CODEC_Y4M: return 4;
      }
      break;
    }
    case KR_KR_AUDIO_CODEC_META_INFO: {
      switch(value) {
        case KR_AUDIO_CODEC_FLAC: return 0;
        case KR_AUDIO_CODEC_OPUS: return 1;
        case KR_AUDIO_CODEC_VORBIS: return 2;
        case KR_AUDIO_CODEC_MP3: return 3;
        case KR_AUDIO_CODEC_AAC: return 4;
      }
      break;
    }
    case KR_KR_CODEC_TYPE_META_INFO: {
      switch(value) {
        case KR_CODEC_AUDIO: return 0;
        case KR_CODEC_VIDEO: return 1;
      }
      break;
    }
    case KR_KR_OPUS_APP_META_INFO: {
      switch(value) {
        case KR_OPUS_APP_AUDIO: return 0;
        case KR_OPUS_APP_LOWDELAY: return 1;
        case KR_OPUS_APP_VOIP: return 2;
      }
      break;
    }
    case KR_KR_COMPOSITOR_PATH_TYPE_META_INFO: {
      switch(value) {
        case KR_COM_SOURCE: return 0;
        case KR_COM_OVERLAY: return 1;
        case KR_COM_INPUT: return 2;
        case KR_COM_BUS: return 3;
        case KR_COM_OUTPUT: return 4;
      }
      break;
    }
    case KR_KR_COMPOSITOR_OVERLAY_TYPE_META_INFO: {
      switch(value) {
        case KR_TEXT: return 0;
        case KR_VECTOR: return 1;
      }
      break;
    }
    case KR_KR_MTS_STREAM_TYPE_META_INFO: {
      switch(value) {
        case MTS_VIDEO: return 0;
        case MTS_AUDIO: return 1;
        case MTS_OTHER: return 2;
      }
      break;
    }
    case KR_KR_CONTAINER_TYPE_META_INFO: {
      switch(value) {
        case KR_CONTAINER_RAW: return 0;
        case KR_CONTAINER_MKV: return 1;
        case KR_CONTAINER_WEBM: return 2;
        case KR_CONTAINER_OGG: return 3;
        case KR_CONTAINER_FLAC: return 4;
        case KR_CONTAINER_Y4M: return 5;
        case KR_CONTAINER_TXT: return 6;
        case KR_CONTAINER_TS: return 7;
        case KR_CONTAINER_OCC: return 8;
      }
      break;
    }
    case KR_KR_VECTOR_TYPE_META_INFO: {
      switch(value) {
        case NOTHING: return 0;
        case HEX: return 1;
        case CIRCLE: return 2;
        case RECT: return 3;
        case TRIANGLE: return 4;
        case VIPER: return 5;
        case METER: return 6;
        case GRID: return 7;
        case CURVE: return 8;
        case ARROW: return 9;
        case CLOCK: return 10;
        case SHADOW: return 11;
      }
      break;
    }
    case KR_KR_FEATURE_ASPECT_TYPE_META_INFO: {
      switch(value) {
        case KR_FEATURE_COMMENTS: return 0;
        case KR_FEATURE_TAG: return 1;
        case KR_FEATURE_HEADER: return 2;
        case KR_FEATURE_FUNCTION: return 3;
        case KR_FEATURE_STORY: return 4;
      }
      break;
    }
    case KR_KR_PIXEL_FMT_META_INFO: {
      switch(value) {
        case KR_PIXELS_ARGB: return 0;
        case KR_PIXELS_YUV422P: return 1;
        case KR_PIXELS_YVU422P: return 2;
        case KR_PIXELS_YUV444: return 3;
        case KR_PIXELS_YUV420: return 4;
        case KR_PIXELS_YUV422: return 5;
      }
      break;
    }
    case KR_KR_IMAGE_TYPE_META_INFO: {
      switch(value) {
        case KR_IMAGE_ENC: return 0;
        case KR_IMAGE_RAW: return 1;
      }
      break;
    }
    case KR_KR_CODED_IMAGE_TYPE_META_INFO: {
      switch(value) {
        case KR_CODED_IMAGE_PNG: return 0;
        case KR_CODED_IMAGE_JPG: return 1;
        case KR_CODED_IMAGE_GIF: return 2;
        case KR_CODED_IMAGE_VP8: return 3;
        case KR_CODED_IMAGE_VP9: return 4;
        case KR_CODED_IMAGE_DAALA: return 5;
        case KR_CODED_IMAGE_THEORA: return 6;
        case KR_CODED_IMAGE_Y4M: return 7;
        case KR_CODED_IMAGE_H264: return 8;
      }
      break;
    }
    case KR_KR_FILE_ACCESS_MODE_META_INFO: {
      switch(value) {
        case KR_FILE_ACCESS_ONLY: return 0;
        case KR_FILE_ACCESS_APPEND: return 1;
        case KR_FILE_ACCESS_MODIFY: return 2;
      }
      break;
    }
    case KR_KR_MIXER_CHANNELS_META_INFO: {
      switch(value) {
        case KR_MONO: return 0;
        case KR_STEREO: return 1;
        case KR_THREE: return 2;
        case KR_QUAD: return 3;
        case KR_FIVE: return 4;
        case KR_SIX: return 5;
        case KR_SEVEN: return 6;
        case KR_EIGHT: return 7;
      }
      break;
    }
    case KR_KR_MIXER_PATH_TYPE_META_INFO: {
      switch(value) {
        case KR_MXR_SOURCE: return 0;
        case KR_MXR_INPUT: return 1;
        case KR_MXR_BUS: return 2;
        case KR_MXR_OUTPUT: return 3;
      }
      break;
    }
    case KR_KR_MEDIA_EVENT_TYPE_META_INFO: {
      switch(value) {
        case KR_MEDIA_CREATE: return 0;
        case KR_MEDIA_ITEM_ADD: return 1;
        case KR_MEDIA_FILE_CREATE: return 2;
        case KR_MEDIA_DESTROY: return 3;
      }
      break;
    }
    case KR_KR_MEDIA_ITEM_TYPE_META_INFO: {
      switch(value) {
        case KR_MEDIA_URL: return 0;
        case KR_MEDIA_METADATA: return 1;
        case KR_MEDIA_FILE: return 2;
      }
      break;
    }
    case KR_M3U_EL_TYPE_META_INFO: {
      switch(value) {
        case M3U_URI: return 0;
        case M3U_TAG: return 1;
      }
      break;
    }
    case KR_KR_M3U_TAG_NAME_META_INFO: {
      switch(value) {
        case EXTM3U: return 0;
        case EXTINF: return 1;
        case EXT_X_BYTERANGE: return 2;
        case EXT_X_TARGETDURATION: return 3;
        case EXT_X_MEDIA_SEQUENCE: return 4;
        case EXT_X_KEY: return 5;
        case EXT_X_PROGRAM_DATE_TIME: return 6;
        case EXT_X_ALLOW_CACHE: return 7;
        case EXT_X_PLAYLIST_TYPE: return 8;
        case EXT_X_STREAM_INF: return 9;
        case EXT_X_I_FRAME_STREAM_INF: return 10;
        case EXT_X_I_FRAMES_ONLY: return 11;
        case EXT_X_MAP: return 12;
        case EXT_X_MEDIA: return 13;
        case EXT_X_ENDLIST: return 14;
        case EXT_X_DISCONTINUITY: return 15;
        case EXT_X_DISCONTINUITY_SEQUENCE: return 16;
        case EXT_X_INDEPENDENT_SEGMENTS: return 17;
        case EXT_X_START: return 18;
        case EXT_X_VERSION: return 19;
      }
      break;
    }
    case KR_KR_M3U_TAG_VAL_TYPE_META_INFO: {
      switch(value) {
        case M3U_TAG_VAL_PRIMITIVE: return 0;
        case M3U_TAG_VAL_ATTR_LIST: return 1;
      }
      break;
    }
    case KR_KR_M3U_TAG_ATTR_NAME_META_INFO: {
      switch(value) {
        case M3U_ATTR_METHOD: return 0;
        case M3U_ATTR_URI: return 1;
        case M3U_ATTR_IV: return 2;
        case M3U_ATTR_KEYFORMAT: return 3;
        case M3U_ATTR_KEYFORMATVERSIONS: return 4;
        case M3U_ATTR_TYPE: return 5;
        case M3U_ATTR_GROUP_ID: return 6;
        case M3U_ATTR_PROGRAM_ID: return 7;
        case M3U_ATTR_LANGUAGE: return 8;
        case M3U_ATTR_ASSOC_LANGUAGE: return 9;
        case M3U_ATTR_NAME: return 10;
        case M3U_ATTR_DEFAULT: return 11;
        case M3U_ATTR_AUTOSELECT: return 12;
        case M3U_ATTR_FORCED: return 13;
        case M3U_ATTR_INSTREAM_ID: return 14;
        case M3U_ATTR_CHARACTERISTICS: return 15;
        case M3U_ATTR_BANDWIDTH: return 16;
        case M3U_ATTR_CODECS: return 17;
        case M3U_ATTR_RESOLUTION: return 18;
        case M3U_ATTR_AUDIO: return 19;
        case M3U_ATTR_VIDEO: return 20;
        case M3U_ATTR_SUBTITLES: return 21;
        case M3U_ATTR_CLOSED_CAPTIONS: return 22;
        case M3U_ATTR_BYTERANGE: return 23;
        case M3U_ATTR_TIME_OFFEST: return 24;
        case M3U_ATTR_PRECISE: return 25;
      }
      break;
    }
    case KR_KR_PRIORITY_META_INFO: {
      switch(value) {
        case KR_PRIORITY_UNKNOWN: return 0;
        case KR_PRIORITY_NORMAL: return 1;
        case KR_PRIORITY_NONINTERACTIVE: return 2;
        case KR_PRIORITY_TRANSCODER: return 3;
        case KR_PRIORITY_REALTIME: return 4;
        case KR_PRIORITY_REALTIME_IO: return 5;
      }
      break;
    }
    case KR_KR_AVPROBE_CODEC_TYPE_META_INFO: {
      switch(value) {
        case KR_AUDIO: return 0;
        case KR_VIDEO: return 1;
      }
      break;
    }
    case KR_KR_TASK_TYPE_META_INFO: {
      switch(value) {
        case KR_VALGRIND_STRESSOR: return 0;
        case KR_QUVI: return 1;
        case KR_AVCONV: return 2;
        case KR_XMMS: return 3;
        case KR_CURL: return 4;
        case KR_TRANSMISSION_CLI: return 5;
        case KR_LIVESTREAMER: return 6;
        case KR_AVPROBE: return 7;
        case KR_YDL: return 8;
      }
      break;
    }
    case KR_KR_TASK_STATE_META_INFO: {
      switch(value) {
        case KR_TASK_READY: return 0;
        case KR_TASK_RUNNING: return 1;
        case KR_TASK_FAILED: return 2;
        case KR_TASK_TIMEDOUT: return 3;
        case KR_TASK_SUCCEEDED: return 4;
        case KR_TASK_CANCELLED: return 5;
        case KR_TASK_SAME_STATE: return 6;
      }
      break;
    }
    case KR_KR_TASK_EVENT_TYPE_META_INFO: {
      switch(value) {
        case KR_TASK_CREATE: return 0;
        case KR_TASK_START: return 1;
        case KR_TASK_PATCH: return 2;
        case KR_TASK_PROGRESS: return 3;
        case KR_TASK_FAILURE: return 4;
        case KR_TASK_SUCCESS: return 5;
        case KR_TASK_CANCEL: return 6;
        case KR_TASK_TIMEOUT: return 7;
        case KR_TASK_DESTROY: return 8;
      }
      break;
    }
    case KR_KR_EASING_META_INFO: {
      switch(value) {
        case LINEAR: return 0;
        case EASEINSINE: return 1;
        case EASEOUTSINE: return 2;
        case EASEINOUTSINE: return 3;
        case EASEINCUBIC: return 4;
        case EASEOUTCUBIC: return 5;
        case EASEINOUTCUBIC: return 6;
        case EASEINOUTELASTIC: return 7;
      }
      break;
    }
    case KR_KR_MEDIA_SOURCE_FORMAT_META_INFO: {
      switch(value) {
        case MEDIA_SOURCE_MTS: return 0;
      }
      break;
    }
    case KR_KR_MEDIA_PACKET_TYPE_META_INFO: {
      switch(value) {
        case KR_PKT_HEADER: return 0;
        case KR_PKT_KEY: return 1;
        case KR_PKT_NORMAL: return 2;
      }
      break;
    }
    case KR_KR_ENCODE_TYPE_META_INFO: {
      switch(value) {
        case KR_ENCODE_AUDIO: return 0;
        case KR_ENCODE_VIDEO: return 1;
      }
      break;
    }
    case KR_KR_CAPTURE_TYPE_META_INFO: {
      switch(value) {
        case KR_CAPTURE_AUDIO: return 0;
        case KR_CAPTURE_VIDEO: return 1;
        case KR_CAPTURE_IMAGE: return 2;
      }
      break;
    }
    case KR_KR_OUTPUT_TYPE_META_INFO: {
      switch(value) {
        case KR_STUDIO_RECORD: return 0;
        case KR_STUDIO_UPLINK: return 1;
        case KR_STUDIO_TRANSMISSION: return 2;
      }
      break;
    }
    case KR_KR_STUDIO_SOURCE_STREAM_TYPE_META_INFO: {
      switch(value) {
        case KR_SOURCE_STREAM_ACCEPT: return 0;
        case KR_SOURCE_STREAM_DOWNLINK: return 1;
      }
      break;
    }
    case KR_KR_STUDIO_SOURCE_TYPE_META_INFO: {
      switch(value) {
        case KR_SOURCE_LOCAL_FILE: return 0;
        case KR_SOURCE_REMOTE_FILE: return 1;
        case KR_SOURCE_STREAM: return 2;
      }
      break;
    }
    case KR_KR_RENDER_TYPE_META_INFO: {
      switch(value) {
        case KR_RENDER_IMAGE: return 0;
        case KR_RENDER_VIDEO: return 1;
        case KR_RENDER_AUDIO: return 2;
      }
      break;
    }
    case KR_KR_AUDIO_GENERATE_TYPE_META_INFO: {
      switch(value) {
        case KR_GENERATE_NOISE: return 0;
        case KR_GENERATE_TONE: return 1;
        case KR_GENERATE_SILENCE: return 2;
      }
      break;
    }
    case KR_KR_VIDEO_GENERATE_TYPE_META_INFO: {
      switch(value) {
        case KR_GENERATE_COLOR: return 0;
        case KR_GENERATE_NO_SIGNAL: return 1;
      }
      break;
    }
    case KR_KR_GENERATE_TYPE_META_INFO: {
      switch(value) {
        case KR_GENERATE_AUDIO: return 0;
        case KR_GENERATE_VIDEO: return 1;
      }
      break;
    }
    case KR_KR_STUDIO_PATH_TYPE_META_INFO: {
      switch(value) {
        case KR_STUDIO_SOURCE: return 0;
        case KR_STUDIO_GENERATE: return 1;
        case KR_STUDIO_CAPTURE: return 2;
        case KR_STUDIO_RENDER: return 3;
        case KR_STUDIO_PLAY: return 4;
        case KR_STUDIO_ENCODE: return 5;
        case KR_STUDIO_DECODE: return 6;
        case KR_STUDIO_DEMUX: return 7;
        case KR_STUDIO_MUX: return 8;
        case KR_STUDIO_OUTPUT: return 9;
      }
      break;
    }
    case KR_KR_TRACK_BLOCK_TYPE_META_INFO: {
      switch(value) {
        case KR_TRACK_CODED_AUDIO: return 0;
        case KR_TRACK_CODED_VIDEO: return 1;
        case KR_TRACK_MUX: return 2;
        case KR_TRACK_AUDIO: return 3;
        case KR_TRACK_VIDEO: return 4;
        case KR_TRACK_SUBTITLE: return 5;
        case KR_TRACK_METADATA: return 6;
        case KR_TRACK_CONTROL: return 7;
      }
      break;
    }
    case KR_KR_TRACK_MODE_META_INFO: {
      switch(value) {
        case KR_TRACK_FINITE: return 0;
        case KR_TRACK_INFINITE: return 1;
      }
      break;
    }
    case KR_KR_TRACK_EVENT_TYPE_META_INFO: {
      switch(value) {
        case KR_TRACK_ADD: return 0;
        case KR_TRACK_REMOVE: return 1;
        case KR_TRACK_READ: return 2;
        case KR_TRACK_WRITE: return 3;
      }
      break;
    }
    case KR_KR_HLS_PLAYLIST_TYPE_META_INFO: {
      switch(value) {
        case KR_HLS_PLAYLIST_MASTER: return 0;
        case KR_HLS_PLAYLIST_MEDIA: return 1;
      }
      break;
    }
    case KR_KR_HLS_RENDITION_TYPE_META_INFO: {
      switch(value) {
        case HLS_AUDIO: return 0;
        case HLS_VIDEO: return 1;
        case HLS_SUBTITLES: return 2;
        case HLS_CLOSED_CAPTIONS: return 3;
      }
      break;
    }
    case KR_KR_HLS_MEDIA_PLAYLIST_TYPE_META_INFO: {
      switch(value) {
        case HLS_VOD: return 0;
        case HLS_LIVE: return 1;
      }
      break;
    }
    case KR_KR_RADIO_MODE_META_INFO: {
      switch(value) {
        case KR_RADIO_MODE_AUDIO: return 0;
        case KR_RADIO_MODE_AUDIOVIDEO: return 1;
      }
      break;
    }
    case KR_KR_WEB_CLIENT_PROTOCOL_META_INFO: {
      switch(value) {
        case KR_PROTOCOL_HTTP: return 0;
        case KR_PROTOCOL_WEBSOCKET: return 1;
        case KR_PROTOCOL_TCP: return 2;
      }
      break;
    }
    case KR_KR_WEB_CLIENT_TYPE_META_INFO: {
      switch(value) {
        case KR_WEB_CLIENT_ACCEPT: return 0;
        case KR_WEB_CLIENT_CONNECT: return 1;
      }
      break;
    }
    case KR_KR_WEB_CLIENT_STATE_META_INFO: {
      switch(value) {
        case KR_WEB_RESOLVING: return 0;
        case KR_WEB_CONNECTING: return 1;
        case KR_WEB_ACTIVE: return 2;
        case KR_WEB_FLUSHING: return 3;
      }
      break;
    }
    case KR_KR_WEB_SERVER_TYPE_META_INFO: {
      switch(value) {
        case KR_WEB_SERVER_HTTP: return 0;
        case KR_WEB_SERVER_WEBSOCKET: return 1;
        case KR_WEB_SERVER_HTTP_WEBSOCKET: return 2;
      }
      break;
    }
    case KR_KR_MULTIPART_STATE_META_INFO: {
      switch(value) {
        case KR_MULTIPART_DONE: return 0;
        case KR_MULTIPART_INIT: return 1;
        case KR_MULTIPART_INFO: return 2;
        case KR_MULTIPART_DATA: return 3;
      }
      break;
    }
    case KR_KR_WEBSOCKET_FRAME_TYPE_META_INFO: {
      switch(value) {
        case KR_WEBSOCKET_BINARY: return 0;
        case KR_WEBSOCKET_TEXT: return 1;
      }
      break;
    }
    case KR_KR_WEBSOCKET_PROTOCOL_META_INFO: {
      switch(value) {
        case KR_WS_PROTOCOL_UNKNOWN: return 0;
        case KR_WS_PROTOCOL_KRAD: return 1;
        case KR_WS_PROTOCOL_STREAM: return 2;
        case KR_WS_PROTOCOL_XMMS: return 3;
      }
      break;
    }
    case KR_KR_HTTP_MESSAGE_STATE_META_INFO: {
      switch(value) {
        case KR_HTTP_IN_HEADER: return 0;
        case KR_HTTP_IN_CONTENT: return 1;
      }
      break;
    }
    case KR_KR_WEB_PATH_TYPE_META_INFO: {
      switch(value) {
        case KR_WEB_SERVER: return 0;
        case KR_WEB_CLIENT: return 1;
        case KR_WEB_PROXY: return 2;
      }
      break;
    }
    case KR_KR_UPLOAD_TYPE_META_INFO: {
      switch(value) {
        case KR_UPLOAD_FILE: return 0;
        case KR_UPLOAD_STASH: return 1;
      }
      break;
    }
    case KR_KR_DOWNLOAD_TYPE_META_INFO: {
      switch(value) {
        case KR_DOWNLOAD_FILE: return 0;
        case KR_DOWNLOAD_STASH: return 1;
      }
      break;
    }
    case KR_KR_SERVE_INDEX_FORMAT_META_INFO: {
      switch(value) {
        case KR_SERVE_INDEX_JSON: return 0;
        case KR_SERVE_INDEX_HTML: return 1;
      }
      break;
    }
    case KR_KR_SERVE_TYPE_META_INFO: {
      switch(value) {
        case KR_SERVE_FULL: return 0;
        case KR_SERVE_RANGES: return 1;
        case KR_SERVE_INDEX: return 2;
        case KR_SERVE_REDIRECT: return 3;
      }
      break;
    }
    case KR_KR_XFER_CLIENT_TYPE_META_INFO: {
      switch(value) {
        case KR_DOWNLOAD: return 0;
        case KR_UPLOAD: return 1;
        case KR_ACCEPT: return 2;
        case KR_SERVE: return 3;
        case KR_DOWNLINK: return 4;
        case KR_UPLINK: return 5;
        case KR_SOURCE: return 6;
        case KR_TRANSMIT: return 7;
      }
      break;
    }
    case KR_KR_XFER_ACCEPTOR_TYPE_META_INFO: {
      switch(value) {
        case KR_XFER_ACCEPT_DIR: return 0;
        case KR_XFER_ACCEPT_FILE: return 1;
        case KR_XFER_ACCEPT_BUF: return 2;
      }
      break;
    }
    case KR_KR_XFER_TRANSMITTER_TYPE_META_INFO: {
      switch(value) {
        case KR_XFER_TRANSMITTER_HTTP: return 0;
        case KR_XFER_TRANSMITTER_WS: return 1;
      }
      break;
    }
    case KR_KR_XFER_SHARE_TYPE_META_INFO: {
      switch(value) {
        case KR_XFER_SHARE_DIR: return 0;
        case KR_XFER_SHARE_FILE: return 1;
        case KR_XFER_SHARE_BUF: return 2;
      }
      break;
    }
    case KR_KR_XFER_SERVER_TYPE_META_INFO: {
      switch(value) {
        case KR_XFER_SHARING: return 0;
        case KR_XFER_ACCEPTING: return 1;
        case KR_XFER_TRANSMITTER: return 2;
        case KR_XFER_RECEIVER: return 3;
      }
      break;
    }
    case KR_KR_XFER_PATH_TYPE_META_INFO: {
      switch(value) {
        case KR_XFER_SERVER: return 0;
        case KR_XFER_CLIENT: return 1;
      }
      break;
    }
    case KR_KR_XPDR_TYPE_META_INFO: {
      switch(value) {
        case KR_JACK: return 0;
        case KR_WAYLAND: return 1;
        case KR_V4L2: return 2;
        case KR_DECKLINK: return 3;
        case KR_X11: return 4;
        case KR_ALSA: return 5;
        case KR_AUX: return 6;
        case KR_JACK_IN: return 7;
        case KR_JACK_OUT: return 8;
        case KR_WAYLAND_OUT: return 9;
        case KR_V4L2_IN: return 10;
        case KR_DECKLINK_VIDEO_IN: return 11;
        case KR_DECKLINK_AUDIO_IN: return 12;
        case KR_X11_IN: return 13;
        case KR_ALSA_IN: return 14;
        case KR_ALSA_OUT: return 15;
        case KR_AUX_VIDEO_IN: return 16;
        case KR_AUX_VIDEO_OUT: return 17;
        case KR_AUX_AUDIO_IN: return 18;
        case KR_AUX_AUDIO_OUT: return 19;
      }
      break;
    }
  }
  return -1;
}

const struct_info meta_info[] = {
  [KR_KR_ALSA_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_alsa_info",
    .member_count = 2,
    .members = {
      {
        .name = "card",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_alsa_info, card),
        .sz = sizeof(((kr_alsa_info*)0)->card),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "name",
        .arr = 40,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_alsa_info, name),
        .sz = sizeof(((kr_alsa_info*)0)->name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 39,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_alsa_info),
    .is_typedef = 1
  },
  [KR_KR_ALSA_PATH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_alsa_path_info",
    .member_count = 1,
    .members = {
      {
        .name = "card_num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_alsa_path_info, card_num),
        .sz = sizeof(((kr_alsa_path_info*)0)->card_num),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_alsa_path_info),
    .is_typedef = 1
  },
  [KR_KR_DECKLINK_VIDEO_CONN_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_decklink_video_conn_type",
    .member_count = 7,
    .members = {
      {
        .name = "KR_DL_VIDEO_DEFAULT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_DL_SDI",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_DL_OPTICAL_SDI",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_DL_HDMI",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_DL_COMPONENT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_DL_COMPOSITE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_DL_SVIDEO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_decklink_video_conn_type),
    .is_typedef = 1
  },
  [KR_KR_DECKLINK_AUDIO_CONN_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_decklink_audio_conn_type",
    .member_count = 4,
    .members = {
      {
        .name = "KR_DL_AUDIO_DEFAULT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_DL_EMBEDDED",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_DL_ANALOG",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_DL_AESEBU",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_decklink_audio_conn_type),
    .is_typedef = 1
  },
  [KR_KR_DECKLINK_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_decklink_info",
    .member_count = 2,
    .members = {
      {
        .name = "name",
        .arr = 64,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_decklink_info, name),
        .sz = sizeof(((kr_decklink_info*)0)->name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 63,
            .notnull = 0
          }
        }
      },
      {
        .name = "num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_decklink_info, num),
        .sz = sizeof(((kr_decklink_info*)0)->num),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_decklink_info),
    .is_typedef = 1
  },
  [KR_KR_DECKLINK_PATH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_decklink_path_info",
    .member_count = 6,
    .members = {
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_decklink_path_info, width),
        .sz = sizeof(((kr_decklink_path_info*)0)->width),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_decklink_path_info, height),
        .sz = sizeof(((kr_decklink_path_info*)0)->height),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_decklink_path_info, num),
        .sz = sizeof(((kr_decklink_path_info*)0)->num),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "den",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_decklink_path_info, den),
        .sz = sizeof(((kr_decklink_path_info*)0)->den),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "video_conn",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_decklink_path_info, video_conn),
        .sz = sizeof(((kr_decklink_path_info*)0)->video_conn),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_decklink_video_conn_type",
            .enum_init = "",
            .idx = KR_KR_DECKLINK_VIDEO_CONN_TYPE_META_INFO
          }
        }
      },
      {
        .name = "audio_conn",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_decklink_path_info, audio_conn),
        .sz = sizeof(((kr_decklink_path_info*)0)->audio_conn),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_decklink_audio_conn_type",
            .enum_init = "",
            .idx = KR_KR_DECKLINK_AUDIO_CONN_TYPE_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_decklink_path_info),
    .is_typedef = 1
  },
  [KR_KR_V4L2_STATE_META_INFO] = {
    .type = 1,
    .name = "kr_v4l2_state",
    .member_count = 3,
    .members = {
      {
        .name = "KR_V4L2_VOID",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_V4L2_OPEN",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_V4L2_CAPTURE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_v4l2_state),
    .is_typedef = 1
  },
  [KR_KR_V4L2_PATH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_v4l2_path_info",
    .member_count = 5,
    .members = {
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_v4l2_path_info, width),
        .sz = sizeof(((kr_v4l2_path_info*)0)->width),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_v4l2_path_info, height),
        .sz = sizeof(((kr_v4l2_path_info*)0)->height),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_v4l2_path_info, num),
        .sz = sizeof(((kr_v4l2_path_info*)0)->num),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "den",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_v4l2_path_info, den),
        .sz = sizeof(((kr_v4l2_path_info*)0)->den),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "format",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_v4l2_path_info, format),
        .sz = sizeof(((kr_v4l2_path_info*)0)->format),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_v4l2_path_info),
    .is_typedef = 1
  },
  [KR_KR_V4L2_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_v4l2_info",
    .member_count = 3,
    .members = {
      {
        .name = "dev",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_v4l2_info, dev),
        .sz = sizeof(((kr_v4l2_info*)0)->dev),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "priority",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_v4l2_info, priority),
        .sz = sizeof(((kr_v4l2_info*)0)->priority),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "state",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_v4l2_info, state),
        .sz = sizeof(((kr_v4l2_info*)0)->state),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_v4l2_state",
            .enum_init = "",
            .idx = KR_KR_V4L2_STATE_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_v4l2_info),
    .is_typedef = 1
  },
  [KR_KR_AUX_PORT_STATE_META_INFO] = {
    .type = 1,
    .name = "kr_aux_port_state",
    .member_count = 2,
    .members = {
      {
        .name = "KR_AUX_PORT_DISCONNECTED",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AUX_PORT_CONNECTED",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_aux_port_state),
    .is_typedef = 1
  },
  [KR_KR_AUX_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_aux_info",
    .member_count = 1,
    .members = {
      {
        .name = "state",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_aux_info, state),
        .sz = sizeof(((kr_aux_info*)0)->state),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_aux_info),
    .is_typedef = 1
  },
  [KR_KR_AUX_PATH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_aux_path_info",
    .member_count = 1,
    .members = {
      {
        .name = "state",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_aux_path_info, state),
        .sz = sizeof(((kr_aux_path_info*)0)->state),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_aux_port_state",
            .enum_init = "",
            .idx = KR_KR_AUX_PORT_STATE_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_aux_path_info),
    .is_typedef = 1
  },
  [KR_KR_JACK_DIRECTION_META_INFO] = {
    .type = 1,
    .name = "kr_jack_direction",
    .member_count = 2,
    .members = {
      {
        .name = "KR_JACK_INPUT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_JACK_OUTPUT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_jack_direction),
    .is_typedef = 1
  },
  [KR_KR_JACK_STATE_META_INFO] = {
    .type = 1,
    .name = "kr_jack_state",
    .member_count = 2,
    .members = {
      {
        .name = "KR_JACK_OFFLINE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_JACK_ONLINE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_jack_state),
    .is_typedef = 1
  },
  [KR_KR_JACK_PATH_STATE_META_INFO] = {
    .type = 1,
    .name = "kr_jack_path_state",
    .member_count = 2,
    .members = {
      {
        .name = "KR_JACK_PATH_FAIL",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_JACK_PATH_ACTIVE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_jack_path_state),
    .is_typedef = 1
  },
  [KR_KR_JACK_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_jack_info",
    .member_count = 9,
    .members = {
      {
        .name = "client_name",
        .arr = 64,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_jack_info, client_name),
        .sz = sizeof(((kr_jack_info*)0)->client_name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 63,
            .notnull = 0
          }
        }
      },
      {
        .name = "server_name",
        .arr = 64,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_jack_info, server_name),
        .sz = sizeof(((kr_jack_info*)0)->server_name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 63,
            .notnull = 0
          }
        }
      },
      {
        .name = "state",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_jack_info, state),
        .sz = sizeof(((kr_jack_info*)0)->state),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_jack_state",
            .enum_init = "",
            .idx = KR_KR_JACK_STATE_META_INFO
          }
        }
      },
      {
        .name = "inputs",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_jack_info, inputs),
        .sz = sizeof(((kr_jack_info*)0)->inputs),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "outputs",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_jack_info, outputs),
        .sz = sizeof(((kr_jack_info*)0)->outputs),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "sample_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_jack_info, sample_rate),
        .sz = sizeof(((kr_jack_info*)0)->sample_rate),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "period_size",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_jack_info, period_size),
        .sz = sizeof(((kr_jack_info*)0)->period_size),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "xruns",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_jack_info, xruns),
        .sz = sizeof(((kr_jack_info*)0)->xruns),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "frames",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_jack_info, frames),
        .sz = sizeof(((kr_jack_info*)0)->frames),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_jack_info),
    .is_typedef = 1
  },
  [KR_KR_JACK_PATH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_jack_path_info",
    .member_count = 3,
    .members = {
      {
        .name = "name",
        .arr = 64,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_jack_path_info, name),
        .sz = sizeof(((kr_jack_path_info*)0)->name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 63,
            .notnull = 0
          }
        }
      },
      {
        .name = "channels",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_jack_path_info, channels),
        .sz = sizeof(((kr_jack_path_info*)0)->channels),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "direction",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_jack_path_info, direction),
        .sz = sizeof(((kr_jack_path_info*)0)->direction),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_jack_direction",
            .enum_init = "",
            .idx = KR_KR_JACK_DIRECTION_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_jack_path_info),
    .is_typedef = 1
  },
  [KR_KR_WAYLAND_STATE_META_INFO] = {
    .type = 1,
    .name = "kr_wayland_state",
    .member_count = 2,
    .members = {
      {
        .name = "KR_WL_DISCONNECTED",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WL_CONNECTED",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_wayland_state),
    .is_typedef = 1
  },
  [KR_KR_WAYLAND_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_wayland_info",
    .member_count = 2,
    .members = {
      {
        .name = "display_name",
        .arr = 128,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_wayland_info, display_name),
        .sz = sizeof(((kr_wayland_info*)0)->display_name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 127,
            .notnull = 0
          }
        }
      },
      {
        .name = "state",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_wayland_info, state),
        .sz = sizeof(((kr_wayland_info*)0)->state),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_wayland_state",
            .enum_init = "",
            .idx = KR_KR_WAYLAND_STATE_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_wayland_info),
    .is_typedef = 1
  },
  [KR_KR_WAYLAND_PATH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_wayland_path_info",
    .member_count = 3,
    .members = {
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_wayland_path_info, width),
        .sz = sizeof(((kr_wayland_path_info*)0)->width),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_wayland_path_info, height),
        .sz = sizeof(((kr_wayland_path_info*)0)->height),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "fullscreen",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_wayland_path_info, fullscreen),
        .sz = sizeof(((kr_wayland_path_info*)0)->fullscreen),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_wayland_path_info),
    .is_typedef = 1
  },
  [KR_KR_X11_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_x11_info",
    .member_count = 1,
    .members = {
      {
        .name = "display",
        .arr = 64,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_x11_info, display),
        .sz = sizeof(((kr_x11_info*)0)->display),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 63,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_x11_info),
    .is_typedef = 1
  },
  [KR_KR_X11_PATH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_x11_path_info",
    .member_count = 7,
    .members = {
      {
        .name = "display",
        .arr = 64,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_x11_path_info, display),
        .sz = sizeof(((kr_x11_path_info*)0)->display),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 63,
            .notnull = 0
          }
        }
      },
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_x11_path_info, width),
        .sz = sizeof(((kr_x11_path_info*)0)->width),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_x11_path_info, height),
        .sz = sizeof(((kr_x11_path_info*)0)->height),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_x11_path_info, num),
        .sz = sizeof(((kr_x11_path_info*)0)->num),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "den",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_x11_path_info, den),
        .sz = sizeof(((kr_x11_path_info*)0)->den),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "x",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_x11_path_info, x),
        .sz = sizeof(((kr_x11_path_info*)0)->x),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "y",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_x11_path_info, y),
        .sz = sizeof(((kr_x11_path_info*)0)->y),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_x11_path_info),
    .is_typedef = 1
  },
  [KR_KR_AFX_EFFECT_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_afx_effect_type",
    .member_count = 6,
    .members = {
      {
        .name = "KR_AFX_NONE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AFX_EQ",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AFX_LOWPASS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AFX_HIGHPASS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AFX_ANALOG",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AFX_VOLUME",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_afx_effect_type),
    .is_typedef = 1
  },
  [KR_KR_AFX_EFFECT_CONTROL_META_INFO] = {
    .type = 1,
    .name = "kr_afx_effect_control",
    .member_count = 5,
    .members = {
      {
        .name = "KR_AFX_DB",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AFX_BW",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AFX_HZ",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AFX_DRIVE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AFX_BLEND",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_afx_effect_control),
    .is_typedef = 1
  },
  [KR_KR_EQ_BAND_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_eq_band_info",
    .member_count = 3,
    .members = {
      {
        .name = "db",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_eq_band_info, db),
        .sz = sizeof(((struct kr_eq_band_info*)0)->db),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = -50.00,
            .max = 20.00
          }
        }
      },
      {
        .name = "bw",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_eq_band_info, bw),
        .sz = sizeof(((struct kr_eq_band_info*)0)->bw),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 1.00,
            .min = 0.50,
            .max = 4.00
          }
        }
      },
      {
        .name = "hz",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_eq_band_info, hz),
        .sz = sizeof(((struct kr_eq_band_info*)0)->hz),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = 20.00,
            .max = 20000.00
          }
        }
      }
    },
    .sz = sizeof(struct kr_eq_band_info),
    .is_typedef = 0
  },
  [KR_KR_EQ_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_eq_info",
    .member_count = 1,
    .members = {
      {
        .name = "band",
        .arr = KR_EQ_MAX_BANDS,
        .arr_notnull = 0,
        .len_def = "KR_EQ_MAX_BANDS",
        .ptr = 0,
        .off = offsetof(struct kr_eq_info, band),
        .sz = sizeof(((struct kr_eq_info*)0)->band),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_eq_band_info",
            .idx = KR_KR_EQ_BAND_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(struct kr_eq_info),
    .is_typedef = 0
  },
  [KR_KR_LOWPASS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_lowpass_info",
    .member_count = 2,
    .members = {
      {
        .name = "bw",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_lowpass_info, bw),
        .sz = sizeof(((struct kr_lowpass_info*)0)->bw),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.66,
            .min = 0.66,
            .max = 4.00
          }
        }
      },
      {
        .name = "hz",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_lowpass_info, hz),
        .sz = sizeof(((struct kr_lowpass_info*)0)->hz),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 20000.00,
            .min = 20.00,
            .max = 20000.00
          }
        }
      }
    },
    .sz = sizeof(struct kr_lowpass_info),
    .is_typedef = 0
  },
  [KR_KR_HIGHPASS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_highpass_info",
    .member_count = 2,
    .members = {
      {
        .name = "bw",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_highpass_info, bw),
        .sz = sizeof(((struct kr_highpass_info*)0)->bw),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.66,
            .min = 0.66,
            .max = 4.00
          }
        }
      },
      {
        .name = "hz",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_highpass_info, hz),
        .sz = sizeof(((struct kr_highpass_info*)0)->hz),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 20.00,
            .min = 20.00,
            .max = 20000.00
          }
        }
      }
    },
    .sz = sizeof(struct kr_highpass_info),
    .is_typedef = 0
  },
  [KR_KR_PASS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_pass_info",
    .member_count = 2,
    .members = {
      {
        .name = "low",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_pass_info, low),
        .sz = sizeof(((struct kr_pass_info*)0)->low),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_lowpass_info",
            .idx = KR_KR_LOWPASS_INFO_META_INFO
          }
        }
      },
      {
        .name = "high",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_pass_info, high),
        .sz = sizeof(((struct kr_pass_info*)0)->high),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_highpass_info",
            .idx = KR_KR_HIGHPASS_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(struct kr_pass_info),
    .is_typedef = 0
  },
  [KR_KR_VOLUME_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_volume_info",
    .member_count = 2,
    .members = {
      {
        .name = "gain",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_volume_info, gain),
        .sz = sizeof(((struct kr_volume_info*)0)->gain),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = -120.00,
            .max = 120.00
          }
        }
      },
      {
        .name = "fader",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_volume_info, fader),
        .sz = sizeof(((struct kr_volume_info*)0)->fader),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = -120.00,
            .max = 120.00
          }
        }
      }
    },
    .sz = sizeof(struct kr_volume_info),
    .is_typedef = 0
  },
  [KR_KR_ANALOG_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_analog_info",
    .member_count = 2,
    .members = {
      {
        .name = "drive",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_analog_info, drive),
        .sz = sizeof(((struct kr_analog_info*)0)->drive),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 10.00
          }
        }
      },
      {
        .name = "blend",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_analog_info, blend),
        .sz = sizeof(((struct kr_analog_info*)0)->blend),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = -10.00,
            .max = 10.00
          }
        }
      }
    },
    .sz = sizeof(struct kr_analog_info),
    .is_typedef = 0
  },
  [KR_KR_METER_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_meter_info",
    .member_count = 2,
    .members = {
      {
        .name = "avg",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_meter_info, avg),
        .sz = sizeof(((struct kr_meter_info*)0)->avg),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = -120.00,
            .min = -120.00,
            .max = 120.00
          }
        }
      },
      {
        .name = "peak",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_meter_info, peak),
        .sz = sizeof(((struct kr_meter_info*)0)->peak),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = -120.00,
            .min = -120.00,
            .max = 120.00
          }
        }
      }
    },
    .sz = sizeof(struct kr_meter_info),
    .is_typedef = 0
  },
  [KR_KR_AFX_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_afx_info",
    .member_count = 5,
    .members = {
      {
        .name = "pass",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_afx_info, pass),
        .sz = sizeof(((struct kr_afx_info*)0)->pass),
        .opt = 1,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_pass_info",
            .idx = KR_KR_PASS_INFO_META_INFO
          }
        }
      },
      {
        .name = "analog",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_afx_info, analog),
        .sz = sizeof(((struct kr_afx_info*)0)->analog),
        .opt = 1,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_analog_info",
            .idx = KR_KR_ANALOG_INFO_META_INFO
          }
        }
      },
      {
        .name = "eq",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_afx_info, eq),
        .sz = sizeof(((struct kr_afx_info*)0)->eq),
        .opt = 1,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_eq_info",
            .idx = KR_KR_EQ_INFO_META_INFO
          }
        }
      },
      {
        .name = "volume",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_afx_info, volume),
        .sz = sizeof(((struct kr_afx_info*)0)->volume),
        .opt = 1,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_volume_info",
            .idx = KR_KR_VOLUME_INFO_META_INFO
          }
        }
      },
      {
        .name = "meter",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_afx_info, meter),
        .sz = sizeof(((struct kr_afx_info*)0)->meter),
        .opt = 1,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_meter_info",
            .idx = KR_KR_METER_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(struct kr_afx_info),
    .is_typedef = 0
  },
  [KR_KR_VIDEO_CODEC_META_INFO] = {
    .type = 1,
    .name = "kr_video_codec",
    .member_count = 5,
    .members = {
      {
        .name = "KR_VIDEO_CODEC_DAALA",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_VIDEO_CODEC_VPX",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_VIDEO_CODEC_H264",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_VIDEO_CODEC_THEORA",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_VIDEO_CODEC_Y4M",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_video_codec),
    .is_typedef = 1
  },
  [KR_KR_AUDIO_CODEC_META_INFO] = {
    .type = 1,
    .name = "kr_audio_codec",
    .member_count = 5,
    .members = {
      {
        .name = "KR_AUDIO_CODEC_FLAC",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AUDIO_CODEC_OPUS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AUDIO_CODEC_VORBIS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AUDIO_CODEC_MP3",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AUDIO_CODEC_AAC",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_audio_codec),
    .is_typedef = 1
  },
  [KR_KR_CODEC_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_codec_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_CODEC_AUDIO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CODEC_VIDEO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_codec_type),
    .is_typedef = 1
  },
  [KR_KR_CODEC_META_INFO] = {
    .type = 2,
    .name = "kr_codec",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_codec, type),
        .sz = sizeof(((kr_codec*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_codec_type",
            .enum_init = "",
            .idx = KR_KR_CODEC_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 38
          }
        }
      }
    },
    .sz = sizeof(kr_codec),
    .is_typedef = 1
  },
  [KR_KR_CODEC_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_codec_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "audio",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_codec, audio),
        .sz = sizeof(((kr_codec*)0)->audio),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_audio_codec",
            .enum_init = "",
            .idx = KR_KR_AUDIO_CODEC_META_INFO
          }
        }
      },
      {
        .name = "video",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_codec, video),
        .sz = sizeof(((kr_codec*)0)->video),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_video_codec",
            .enum_init = "",
            .idx = KR_KR_VIDEO_CODEC_META_INFO
          }
        }
      }
    },
    .root_idx = 37,
    .is_typedef = 0
  },
  [KR_KR_AUDIO_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_audio_info",
    .member_count = 4,
    .members = {
      {
        .name = "channels",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_audio_info, channels),
        .sz = sizeof(((kr_audio_info*)0)->channels),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "bit_depth",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_audio_info, bit_depth),
        .sz = sizeof(((kr_audio_info*)0)->bit_depth),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "sample_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_audio_info, sample_rate),
        .sz = sizeof(((kr_audio_info*)0)->sample_rate),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "samples",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_audio_info, samples),
        .sz = sizeof(((kr_audio_info*)0)->samples),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_audio_info),
    .is_typedef = 1
  },
  [KR_KR_VIDEO_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_video_info",
    .member_count = 5,
    .members = {
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_video_info, width),
        .sz = sizeof(((kr_video_info*)0)->width),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_video_info, height),
        .sz = sizeof(((kr_video_info*)0)->height),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "fps_num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_video_info, fps_num),
        .sz = sizeof(((kr_video_info*)0)->fps_num),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "fps_den",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_video_info, fps_den),
        .sz = sizeof(((kr_video_info*)0)->fps_den),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "pixel_fmt",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_video_info, pixel_fmt),
        .sz = sizeof(((kr_video_info*)0)->pixel_fmt),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_pixel_fmt",
            .enum_init = "",
            .idx = KR_KR_PIXEL_FMT_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_video_info),
    .is_typedef = 1
  },
  [KR_KR_AV_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_av_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_av_info, type),
        .sz = sizeof(((kr_av_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_codec_type",
            .enum_init = "",
            .idx = KR_KR_CODEC_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 42
          }
        }
      }
    },
    .sz = sizeof(kr_av_info),
    .is_typedef = 1
  },
  [KR_KR_AV_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_av_info_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "audio",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_av_info, audio),
        .sz = sizeof(((kr_av_info*)0)->audio),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_audio_info",
            .idx = KR_KR_AUDIO_INFO_META_INFO
          }
        }
      },
      {
        .name = "video",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_av_info, video),
        .sz = sizeof(((kr_av_info*)0)->video),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_video_info",
            .idx = KR_KR_VIDEO_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 41,
    .is_typedef = 0
  },
  [KR_KR_DAALA_ENCODER_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_daala_encoder_info",
    .member_count = 7,
    .members = {
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_daala_encoder_info, width),
        .sz = sizeof(((kr_daala_encoder_info*)0)->width),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_daala_encoder_info, height),
        .sz = sizeof(((kr_daala_encoder_info*)0)->height),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "pixel_fmt",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_daala_encoder_info, pixel_fmt),
        .sz = sizeof(((kr_daala_encoder_info*)0)->pixel_fmt),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_pixel_fmt",
            .enum_init = "",
            .idx = KR_KR_PIXEL_FMT_META_INFO
          }
        }
      },
      {
        .name = "fps_num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_daala_encoder_info, fps_num),
        .sz = sizeof(((kr_daala_encoder_info*)0)->fps_num),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "fps_den",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_daala_encoder_info, fps_den),
        .sz = sizeof(((kr_daala_encoder_info*)0)->fps_den),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "bitrate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_daala_encoder_info, bitrate),
        .sz = sizeof(((kr_daala_encoder_info*)0)->bitrate),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "key_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_daala_encoder_info, key_rate),
        .sz = sizeof(((kr_daala_encoder_info*)0)->key_rate),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_daala_encoder_info),
    .is_typedef = 1
  },
  [KR_KR_VPX_ENCODER_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_vpx_encoder_info",
    .member_count = 7,
    .members = {
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_vpx_encoder_info, width),
        .sz = sizeof(((kr_vpx_encoder_info*)0)->width),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_vpx_encoder_info, height),
        .sz = sizeof(((kr_vpx_encoder_info*)0)->height),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "pixel_fmt",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_vpx_encoder_info, pixel_fmt),
        .sz = sizeof(((kr_vpx_encoder_info*)0)->pixel_fmt),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_pixel_fmt",
            .enum_init = "",
            .idx = KR_KR_PIXEL_FMT_META_INFO
          }
        }
      },
      {
        .name = "fps_num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_vpx_encoder_info, fps_num),
        .sz = sizeof(((kr_vpx_encoder_info*)0)->fps_num),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "fps_den",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_vpx_encoder_info, fps_den),
        .sz = sizeof(((kr_vpx_encoder_info*)0)->fps_den),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "bit_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_vpx_encoder_info, bit_rate),
        .sz = sizeof(((kr_vpx_encoder_info*)0)->bit_rate),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "key_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_vpx_encoder_info, key_rate),
        .sz = sizeof(((kr_vpx_encoder_info*)0)->key_rate),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_vpx_encoder_info),
    .is_typedef = 1
  },
  [KR_KR_H264_ENCODER_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_h264_encoder_info",
    .member_count = 7,
    .members = {
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_h264_encoder_info, width),
        .sz = sizeof(((kr_h264_encoder_info*)0)->width),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_h264_encoder_info, height),
        .sz = sizeof(((kr_h264_encoder_info*)0)->height),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "pixel_fmt",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_h264_encoder_info, pixel_fmt),
        .sz = sizeof(((kr_h264_encoder_info*)0)->pixel_fmt),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_pixel_fmt",
            .enum_init = "",
            .idx = KR_KR_PIXEL_FMT_META_INFO
          }
        }
      },
      {
        .name = "fps_num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_h264_encoder_info, fps_num),
        .sz = sizeof(((kr_h264_encoder_info*)0)->fps_num),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "fps_den",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_h264_encoder_info, fps_den),
        .sz = sizeof(((kr_h264_encoder_info*)0)->fps_den),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "bit_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_h264_encoder_info, bit_rate),
        .sz = sizeof(((kr_h264_encoder_info*)0)->bit_rate),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "key_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_h264_encoder_info, key_rate),
        .sz = sizeof(((kr_h264_encoder_info*)0)->key_rate),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_h264_encoder_info),
    .is_typedef = 1
  },
  [KR_KR_THEORA_ENCODER_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_theora_encoder_info",
    .member_count = 8,
    .members = {
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_theora_encoder_info, width),
        .sz = sizeof(((kr_theora_encoder_info*)0)->width),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_theora_encoder_info, height),
        .sz = sizeof(((kr_theora_encoder_info*)0)->height),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "pixel_fmt",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_theora_encoder_info, pixel_fmt),
        .sz = sizeof(((kr_theora_encoder_info*)0)->pixel_fmt),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_pixel_fmt",
            .enum_init = "",
            .idx = KR_KR_PIXEL_FMT_META_INFO
          }
        }
      },
      {
        .name = "fps_num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_theora_encoder_info, fps_num),
        .sz = sizeof(((kr_theora_encoder_info*)0)->fps_num),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "fps_den",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_theora_encoder_info, fps_den),
        .sz = sizeof(((kr_theora_encoder_info*)0)->fps_den),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "key_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_theora_encoder_info, key_rate),
        .sz = sizeof(((kr_theora_encoder_info*)0)->key_rate),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "quality",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_theora_encoder_info, quality),
        .sz = sizeof(((kr_theora_encoder_info*)0)->quality),
        .opt = 0,
        .type = 6,
        .type_info = {

        }
      },
      {
        .name = "eos",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_theora_encoder_info, eos),
        .sz = sizeof(((kr_theora_encoder_info*)0)->eos),
        .opt = 0,
        .type = 6,
        .type_info = {

        }
      }
    },
    .sz = sizeof(kr_theora_encoder_info),
    .is_typedef = 1
  },
  [KR_KR_Y4M_ENCODER_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_y4m_encoder_info",
    .member_count = 5,
    .members = {
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_y4m_encoder_info, width),
        .sz = sizeof(((kr_y4m_encoder_info*)0)->width),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_y4m_encoder_info, height),
        .sz = sizeof(((kr_y4m_encoder_info*)0)->height),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "pixel_fmt",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_y4m_encoder_info, pixel_fmt),
        .sz = sizeof(((kr_y4m_encoder_info*)0)->pixel_fmt),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_pixel_fmt",
            .enum_init = "",
            .idx = KR_KR_PIXEL_FMT_META_INFO
          }
        }
      },
      {
        .name = "fps_num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_y4m_encoder_info, fps_num),
        .sz = sizeof(((kr_y4m_encoder_info*)0)->fps_num),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "fps_den",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_y4m_encoder_info, fps_den),
        .sz = sizeof(((kr_y4m_encoder_info*)0)->fps_den),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_y4m_encoder_info),
    .is_typedef = 1
  },
  [KR_KR_FLAC_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_flac_info",
    .member_count = 7,
    .members = {
      {
        .name = "channels",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_flac_info, channels),
        .sz = sizeof(((kr_flac_info*)0)->channels),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "bit_depth",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_flac_info, bit_depth),
        .sz = sizeof(((kr_flac_info*)0)->bit_depth),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "sample_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_flac_info, sample_rate),
        .sz = sizeof(((kr_flac_info*)0)->sample_rate),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "frame_size",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_flac_info, frame_size),
        .sz = sizeof(((kr_flac_info*)0)->frame_size),
        .opt = 1,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "total_frames",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_flac_info, total_frames),
        .sz = sizeof(((kr_flac_info*)0)->total_frames),
        .opt = 1,
        .type = 5,
        .type_info = {
          .int64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "total_bytes",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_flac_info, total_bytes),
        .sz = sizeof(((kr_flac_info*)0)->total_bytes),
        .opt = 1,
        .type = 5,
        .type_info = {
          .int64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "total_samples",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_flac_info, total_samples),
        .sz = sizeof(((kr_flac_info*)0)->total_samples),
        .opt = 1,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_flac_info),
    .is_typedef = 1
  },
  [KR_KR_OPUS_APP_META_INFO] = {
    .type = 1,
    .name = "kr_opus_app",
    .member_count = 3,
    .members = {
      {
        .name = "KR_OPUS_APP_AUDIO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_OPUS_APP_LOWDELAY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_OPUS_APP_VOIP",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_opus_app),
    .is_typedef = 1
  },
  [KR_KR_OPUS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_opus_info",
    .member_count = 7,
    .members = {
      {
        .name = "channels",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_opus_info, channels),
        .sz = sizeof(((kr_opus_info*)0)->channels),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "sample_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_opus_info, sample_rate),
        .sz = sizeof(((kr_opus_info*)0)->sample_rate),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "bit_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_opus_info, bit_rate),
        .sz = sizeof(((kr_opus_info*)0)->bit_rate),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "frame_size",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_opus_info, frame_size),
        .sz = sizeof(((kr_opus_info*)0)->frame_size),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "app",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_opus_info, app),
        .sz = sizeof(((kr_opus_info*)0)->app),
        .opt = 1,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_opus_app",
            .enum_init = "opt",
            .idx = KR_KR_OPUS_APP_META_INFO
          }
        }
      },
      {
        .name = "total_frames",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_opus_info, total_frames),
        .sz = sizeof(((kr_opus_info*)0)->total_frames),
        .opt = 1,
        .type = 5,
        .type_info = {
          .int64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "total_bytes",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_opus_info, total_bytes),
        .sz = sizeof(((kr_opus_info*)0)->total_bytes),
        .opt = 1,
        .type = 5,
        .type_info = {
          .int64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_opus_info),
    .is_typedef = 1
  },
  [KR_KR_VORBIS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_vorbis_info",
    .member_count = 7,
    .members = {
      {
        .name = "channels",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_vorbis_info, channels),
        .sz = sizeof(((kr_vorbis_info*)0)->channels),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "sample_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_vorbis_info, sample_rate),
        .sz = sizeof(((kr_vorbis_info*)0)->sample_rate),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "quality",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_vorbis_info, quality),
        .sz = sizeof(((kr_vorbis_info*)0)->quality),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      },
      {
        .name = "small_block_sz",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_vorbis_info, small_block_sz),
        .sz = sizeof(((kr_vorbis_info*)0)->small_block_sz),
        .opt = 1,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "large_block_sz",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_vorbis_info, large_block_sz),
        .sz = sizeof(((kr_vorbis_info*)0)->large_block_sz),
        .opt = 1,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "total_frames",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_vorbis_info, total_frames),
        .sz = sizeof(((kr_vorbis_info*)0)->total_frames),
        .opt = 1,
        .type = 5,
        .type_info = {
          .int64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "total_bytes",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_vorbis_info, total_bytes),
        .sz = sizeof(((kr_vorbis_info*)0)->total_bytes),
        .opt = 1,
        .type = 5,
        .type_info = {
          .int64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_vorbis_info),
    .is_typedef = 1
  },
  [KR_KR_MP3_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_mp3_info",
    .member_count = 6,
    .members = {
      {
        .name = "channels",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mp3_info, channels),
        .sz = sizeof(((kr_mp3_info*)0)->channels),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "sample_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mp3_info, sample_rate),
        .sz = sizeof(((kr_mp3_info*)0)->sample_rate),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "bit_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mp3_info, bit_rate),
        .sz = sizeof(((kr_mp3_info*)0)->bit_rate),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "total_frames",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mp3_info, total_frames),
        .sz = sizeof(((kr_mp3_info*)0)->total_frames),
        .opt = 1,
        .type = 5,
        .type_info = {
          .int64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "total_bytes",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mp3_info, total_bytes),
        .sz = sizeof(((kr_mp3_info*)0)->total_bytes),
        .opt = 1,
        .type = 5,
        .type_info = {
          .int64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "total_samples",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mp3_info, total_samples),
        .sz = sizeof(((kr_mp3_info*)0)->total_samples),
        .opt = 1,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_mp3_info),
    .is_typedef = 1
  },
  [KR_KR_AAC_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_aac_info",
    .member_count = 5,
    .members = {
      {
        .name = "channels",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_aac_info, channels),
        .sz = sizeof(((kr_aac_info*)0)->channels),
        .opt = 0,
        .type = 6,
        .type_info = {

        }
      },
      {
        .name = "sample_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_aac_info, sample_rate),
        .sz = sizeof(((kr_aac_info*)0)->sample_rate),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "bit_rate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_aac_info, bit_rate),
        .sz = sizeof(((kr_aac_info*)0)->bit_rate),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 576000,
            .min = 96000,
            .max = 40000
          }
        }
      },
      {
        .name = "total_samples",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_aac_info, total_samples),
        .sz = sizeof(((kr_aac_info*)0)->total_samples),
        .opt = 1,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "total_bytes",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_aac_info, total_bytes),
        .sz = sizeof(((kr_aac_info*)0)->total_bytes),
        .opt = 1,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_aac_info),
    .is_typedef = 1
  },
  [KR_KR_LAVC_ENCODER_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_lavc_encoder_info",
    .member_count = 1,
    .members = {
      {
        .name = "bongo",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_lavc_encoder_info, bongo),
        .sz = sizeof(((kr_lavc_encoder_info*)0)->bongo),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_lavc_encoder_info),
    .is_typedef = 1
  },
  [KR_KR_COMPOSITOR_PATH_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_compositor_path_type",
    .member_count = 5,
    .members = {
      {
        .name = "KR_COM_SOURCE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_COM_OVERLAY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_COM_INPUT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_COM_BUS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_COM_OUTPUT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_compositor_path_type),
    .is_typedef = 1
  },
  [KR_KR_COMPOSITOR_OVERLAY_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_compositor_overlay_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_TEXT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_VECTOR",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_compositor_overlay_type),
    .is_typedef = 1
  },
  [KR_KR_COMPOSITOR_OUTPUT_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_compositor_output_info",
    .member_count = 1,
    .members = {
      {
        .name = "opacity",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_output_info, opacity),
        .sz = sizeof(((struct kr_compositor_output_info*)0)->opacity),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 1.00,
            .min = 0.00,
            .max = 1.00
          }
        }
      }
    },
    .sz = sizeof(struct kr_compositor_output_info),
    .is_typedef = 0
  },
  [KR_KR_COMPOSITOR_BUS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_compositor_bus_info",
    .member_count = 1,
    .members = {
      {
        .name = "opacity",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_bus_info, opacity),
        .sz = sizeof(((struct kr_compositor_bus_info*)0)->opacity),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 1.00,
            .min = 0.00,
            .max = 1.00
          }
        }
      }
    },
    .sz = sizeof(struct kr_compositor_bus_info),
    .is_typedef = 0
  },
  [KR_KR_COMPOSITOR_INPUT_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_compositor_input_info",
    .member_count = 6,
    .members = {
      {
        .name = "opacity",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_input_info, opacity),
        .sz = sizeof(((struct kr_compositor_input_info*)0)->opacity),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 1.00,
            .min = 0.00,
            .max = 1.00
          }
        }
      },
      {
        .name = "pos",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_input_info, pos),
        .sz = sizeof(((struct kr_compositor_input_info*)0)->pos),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_rect",
            .idx = KR_KR_RECT_META_INFO
          }
        }
      },
      {
        .name = "crop",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_input_info, crop),
        .sz = sizeof(((struct kr_compositor_input_info*)0)->crop),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_rect",
            .idx = KR_KR_RECT_META_INFO
          }
        }
      },
      {
        .name = "view",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_input_info, view),
        .sz = sizeof(((struct kr_compositor_input_info*)0)->view),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_perspective_view",
            .idx = KR_KR_PERSPECTIVE_VIEW_META_INFO
          }
        }
      },
      {
        .name = "z",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_input_info, z),
        .sz = sizeof(((struct kr_compositor_input_info*)0)->z),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "rotation",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_input_info, rotation),
        .sz = sizeof(((struct kr_compositor_input_info*)0)->rotation),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = -360000.00,
            .max = 360000.00
          }
        }
      }
    },
    .sz = sizeof(struct kr_compositor_input_info),
    .is_typedef = 0
  },
  [KR_KR_COMPOSITOR_SOURCE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_compositor_source_info",
    .member_count = 3,
    .members = {
      {
        .name = "opacity",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_source_info, opacity),
        .sz = sizeof(((struct kr_compositor_source_info*)0)->opacity),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 1.00,
            .min = 0.00,
            .max = 1.00
          }
        }
      },
      {
        .name = "crop",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_source_info, crop),
        .sz = sizeof(((struct kr_compositor_source_info*)0)->crop),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_rect",
            .idx = KR_KR_RECT_META_INFO
          }
        }
      },
      {
        .name = "view",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_source_info, view),
        .sz = sizeof(((struct kr_compositor_source_info*)0)->view),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_perspective_view",
            .idx = KR_KR_PERSPECTIVE_VIEW_META_INFO
          }
        }
      }
    },
    .sz = sizeof(struct kr_compositor_source_info),
    .is_typedef = 0
  },
  [KR_KR_COMPOSITOR_OVERLAY_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_compositor_overlay_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_overlay_info, type),
        .sz = sizeof(((struct kr_compositor_overlay_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_compositor_overlay_type",
            .enum_init = "",
            .idx = KR_KR_COMPOSITOR_OVERLAY_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 62
          }
        }
      }
    },
    .sz = sizeof(struct kr_compositor_overlay_info),
    .is_typedef = 0
  },
  [KR_KR_COMPOSITOR_OVERLAY_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_compositor_overlay_info_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "text",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_overlay_info, text),
        .sz = sizeof(((struct kr_compositor_overlay_info*)0)->text),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_text_info",
            .idx = KR_KR_TEXT_INFO_META_INFO
          }
        }
      },
      {
        .name = "vector",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_overlay_info, vector),
        .sz = sizeof(((struct kr_compositor_overlay_info*)0)->vector),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_vector_info",
            .idx = KR_KR_VECTOR_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 61,
    .is_typedef = 0
  },
  [KR_KR_COMPOSITOR_PATH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_compositor_path_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_path_info, type),
        .sz = sizeof(((struct kr_compositor_path_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_compositor_path_type",
            .enum_init = "",
            .idx = KR_KR_COMPOSITOR_PATH_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 64
          }
        }
      }
    },
    .sz = sizeof(struct kr_compositor_path_info),
    .is_typedef = 0
  },
  [KR_KR_COMPOSITOR_PATH_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_compositor_path_info_sub1",
    .member_count = 5,
    .members = {
      {
        .name = "source",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_path_info, source),
        .sz = sizeof(((struct kr_compositor_path_info*)0)->source),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_compositor_source_info",
            .idx = KR_KR_COMPOSITOR_SOURCE_INFO_META_INFO
          }
        }
      },
      {
        .name = "overlay",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_path_info, overlay),
        .sz = sizeof(((struct kr_compositor_path_info*)0)->overlay),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_compositor_overlay_info",
            .idx = KR_KR_COMPOSITOR_OVERLAY_INFO_META_INFO
          }
        }
      },
      {
        .name = "input",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_path_info, input),
        .sz = sizeof(((struct kr_compositor_path_info*)0)->input),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_compositor_input_info",
            .idx = KR_KR_COMPOSITOR_INPUT_INFO_META_INFO
          }
        }
      },
      {
        .name = "bus",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_path_info, bus),
        .sz = sizeof(((struct kr_compositor_path_info*)0)->bus),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_compositor_bus_info",
            .idx = KR_KR_COMPOSITOR_BUS_INFO_META_INFO
          }
        }
      },
      {
        .name = "output",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_compositor_path_info, output),
        .sz = sizeof(((struct kr_compositor_path_info*)0)->output),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_compositor_output_info",
            .idx = KR_KR_COMPOSITOR_OUTPUT_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 63,
    .is_typedef = 0
  },
  [KR_KR_MTS_STREAM_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_mts_stream_type",
    .member_count = 3,
    .members = {
      {
        .name = "MTS_VIDEO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "MTS_AUDIO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "MTS_OTHER",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_mts_stream_type),
    .is_typedef = 1
  },
  [KR_KR_MTS_PACKET_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_mts_packet_info",
    .member_count = 7,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_packet_info, type),
        .sz = sizeof(((kr_mts_packet_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_mts_stream_type",
            .enum_init = "",
            .idx = KR_KR_MTS_STREAM_TYPE_META_INFO
          }
        }
      },
      {
        .name = "id",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_packet_info, id),
        .sz = sizeof(((kr_mts_packet_info*)0)->id),
        .opt = 0,
        .type = 3,
        .type_info = {

        }
      },
      {
        .name = "track",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_packet_info, track),
        .sz = sizeof(((kr_mts_packet_info*)0)->track),
        .opt = 0,
        .type = 2,
        .type_info = {

        }
      },
      {
        .name = "pts",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_packet_info, pts),
        .sz = sizeof(((kr_mts_packet_info*)0)->pts),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "dts",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_packet_info, dts),
        .sz = sizeof(((kr_mts_packet_info*)0)->dts),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "sz",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_packet_info, sz),
        .sz = sizeof(((kr_mts_packet_info*)0)->sz),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "pos",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_packet_info, pos),
        .sz = sizeof(((kr_mts_packet_info*)0)->pos),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_mts_packet_info),
    .is_typedef = 1
  },
  [KR_KR_MTS_STREAM_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_mts_stream_info",
    .member_count = 5,
    .members = {
      {
        .name = "id",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_stream_info, id),
        .sz = sizeof(((kr_mts_stream_info*)0)->id),
        .opt = 0,
        .type = 7,
        .type_info = {

        }
      },
      {
        .name = "type_str",
        .arr = 64,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_stream_info, type_str),
        .sz = sizeof(((kr_mts_stream_info*)0)->type_str),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 63,
            .notnull = 0
          }
        }
      },
      {
        .name = "codec",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_stream_info, codec),
        .sz = sizeof(((kr_mts_stream_info*)0)->codec),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_codec",
            .idx = KR_KR_CODEC_META_INFO
          }
        }
      },
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_stream_info, type),
        .sz = sizeof(((kr_mts_stream_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_mts_stream_type",
            .enum_init = "",
            .idx = KR_KR_MTS_STREAM_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 68
          }
        }
      }
    },
    .sz = sizeof(kr_mts_stream_info),
    .is_typedef = 1
  },
  [KR_KR_MTS_STREAM_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_mts_stream_info_sub1",
    .member_count = 3,
    .members = {
      {
        .name = "audio",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_stream_info, audio),
        .sz = sizeof(((kr_mts_stream_info*)0)->audio),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_audio_info",
            .idx = KR_KR_AUDIO_INFO_META_INFO
          }
        }
      },
      {
        .name = "video",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_stream_info, video),
        .sz = sizeof(((kr_mts_stream_info*)0)->video),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_video_info",
            .idx = KR_KR_VIDEO_INFO_META_INFO
          }
        }
      },
      {
        .name = "coconut",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_stream_info, coconut),
        .sz = sizeof(((kr_mts_stream_info*)0)->coconut),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .root_idx = 67,
    .is_typedef = 0
  },
  [KR_KR_MTS_PROGRAM_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_mts_program_info",
    .member_count = 3,
    .members = {
      {
        .name = "id",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_program_info, id),
        .sz = sizeof(((kr_mts_program_info*)0)->id),
        .opt = 0,
        .type = 7,
        .type_info = {

        }
      },
      {
        .name = "streams",
        .arr = MTS_NSTREAMS,
        .arr_notnull = 0,
        .len_def = "MTS_NSTREAMS",
        .ptr = 0,
        .off = offsetof(kr_mts_program_info, streams),
        .sz = sizeof(((kr_mts_program_info*)0)->streams),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_mts_stream_info",
            .idx = KR_KR_MTS_STREAM_INFO_META_INFO
          }
        }
      },
      {
        .name = "nstreams",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_program_info, nstreams),
        .sz = sizeof(((kr_mts_program_info*)0)->nstreams),
        .opt = 0,
        .type = 6,
        .type_info = {

        }
      }
    },
    .sz = sizeof(kr_mts_program_info),
    .is_typedef = 1
  },
  [KR_KR_MTS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_mts_info",
    .member_count = 3,
    .members = {
      {
        .name = "npackets",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_info, npackets),
        .sz = sizeof(((kr_mts_info*)0)->npackets),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "programs",
        .arr = MTS_NPROGRAMS,
        .arr_notnull = 0,
        .len_def = "MTS_NPROGRAMS",
        .ptr = 0,
        .off = offsetof(kr_mts_info, programs),
        .sz = sizeof(((kr_mts_info*)0)->programs),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_mts_program_info",
            .idx = KR_KR_MTS_PROGRAM_INFO_META_INFO
          }
        }
      },
      {
        .name = "nprograms",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mts_info, nprograms),
        .sz = sizeof(((kr_mts_info*)0)->nprograms),
        .opt = 0,
        .type = 6,
        .type_info = {

        }
      }
    },
    .sz = sizeof(kr_mts_info),
    .is_typedef = 1
  },
  [KR_KR_CONTAINER_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_container_type",
    .member_count = 9,
    .members = {
      {
        .name = "KR_CONTAINER_RAW",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CONTAINER_MKV",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CONTAINER_WEBM",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CONTAINER_OGG",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CONTAINER_FLAC",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CONTAINER_Y4M",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CONTAINER_TXT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CONTAINER_TS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CONTAINER_OCC",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_container_type),
    .is_typedef = 1
  },
  [KR_KR_TEXT_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_text_info",
    .member_count = 5,
    .members = {
      {
        .name = "string",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_text_info, string),
        .sz = sizeof(((struct kr_text_info*)0)->string),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      },
      {
        .name = "font",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_text_info, font),
        .sz = sizeof(((struct kr_text_info*)0)->font),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "red",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_text_info, red),
        .sz = sizeof(((struct kr_text_info*)0)->red),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      },
      {
        .name = "green",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_text_info, green),
        .sz = sizeof(((struct kr_text_info*)0)->green),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      },
      {
        .name = "blue",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_text_info, blue),
        .sz = sizeof(((struct kr_text_info*)0)->blue),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      }
    },
    .sz = sizeof(struct kr_text_info),
    .is_typedef = 0
  },
  [KR_KR_VECTOR_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_vector_type",
    .member_count = 12,
    .members = {
      {
        .name = "NOTHING",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "HEX",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "CIRCLE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "RECT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "TRIANGLE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "VIPER",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "METER",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "GRID",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "CURVE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "ARROW",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "CLOCK",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "SHADOW",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_vector_type),
    .is_typedef = 1
  },
  [KR_KR_VECTOR_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_vector_info",
    .member_count = 4,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_vector_info, type),
        .sz = sizeof(((struct kr_vector_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_vector_type",
            .enum_init = "",
            .idx = KR_KR_VECTOR_TYPE_META_INFO
          }
        }
      },
      {
        .name = "red",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_vector_info, red),
        .sz = sizeof(((struct kr_vector_info*)0)->red),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      },
      {
        .name = "green",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_vector_info, green),
        .sz = sizeof(((struct kr_vector_info*)0)->green),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      },
      {
        .name = "blue",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_vector_info, blue),
        .sz = sizeof(((struct kr_vector_info*)0)->blue),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      }
    },
    .sz = sizeof(struct kr_vector_info),
    .is_typedef = 0
  },
  [KR_KR_FEATURE_ASPECT_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_feature_aspect_type",
    .member_count = 5,
    .members = {
      {
        .name = "KR_FEATURE_COMMENTS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_FEATURE_TAG",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_FEATURE_HEADER",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_FEATURE_FUNCTION",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_FEATURE_STORY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_feature_aspect_type),
    .is_typedef = 1
  },
  [KR_KR_FEATURE_ASPECT_META_INFO] = {
    .type = 2,
    .name = "kr_feature_aspect",
    .member_count = 3,
    .members = {
      {
        .name = "id",
        .arr = 42,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_feature_aspect, id),
        .sz = sizeof(((kr_feature_aspect*)0)->id),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 41,
            .notnull = 0
          }
        }
      },
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_feature_aspect, type),
        .sz = sizeof(((kr_feature_aspect*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_feature_aspect_type",
            .enum_init = "",
            .idx = KR_KR_FEATURE_ASPECT_TYPE_META_INFO
          }
        }
      },
      {
        .name = "value",
        .arr = 2048,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_feature_aspect, value),
        .sz = sizeof(((kr_feature_aspect*)0)->value),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 2047,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_feature_aspect),
    .is_typedef = 1
  },
  [KR_KR_FEATURE_META_INFO] = {
    .type = 2,
    .name = "kr_feature",
    .member_count = 3,
    .members = {
      {
        .name = "id",
        .arr = 42,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_feature, id),
        .sz = sizeof(((struct kr_feature*)0)->id),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 41,
            .notnull = 0
          }
        }
      },
      {
        .name = "name",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_feature, name),
        .sz = sizeof(((struct kr_feature*)0)->name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "aspects",
        .arr = KR_FEATURE_NASPECTS,
        .arr_notnull = 0,
        .len_def = "KR_FEATURE_NASPECTS",
        .ptr = 0,
        .off = offsetof(struct kr_feature, aspects),
        .sz = sizeof(((struct kr_feature*)0)->aspects),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_feature_aspect",
            .idx = KR_KR_FEATURE_ASPECT_META_INFO
          }
        }
      }
    },
    .sz = sizeof(struct kr_feature),
    .is_typedef = 0
  },
  [KR_KR_IMAGE_STASH_ITEM_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_image_stash_item_info",
    .member_count = 6,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_image_stash_item_info, type),
        .sz = sizeof(((kr_image_stash_item_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_image_type",
            .enum_init = "",
            .idx = KR_KR_IMAGE_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 79
          }
        }
      },
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_image_stash_item_info, width),
        .sz = sizeof(((kr_image_stash_item_info*)0)->width),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_image_stash_item_info, height),
        .sz = sizeof(((kr_image_stash_item_info*)0)->height),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "sz",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_image_stash_item_info, sz),
        .sz = sizeof(((kr_image_stash_item_info*)0)->sz),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "created_on",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_image_stash_item_info, created_on),
        .sz = sizeof(((kr_image_stash_item_info*)0)->created_on),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_image_stash_item_info),
    .is_typedef = 1
  },
  [KR_KR_IMAGE_STASH_ITEM_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_image_stash_item_info_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "enc_fmt",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_image_stash_item_info, enc_fmt),
        .sz = sizeof(((kr_image_stash_item_info*)0)->enc_fmt),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_coded_image_type",
            .enum_init = "",
            .idx = KR_KR_CODED_IMAGE_TYPE_META_INFO
          }
        }
      },
      {
        .name = "raw_fmt",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_image_stash_item_info, raw_fmt),
        .sz = sizeof(((kr_image_stash_item_info*)0)->raw_fmt),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_pixel_fmt",
            .enum_init = "",
            .idx = KR_KR_PIXEL_FMT_META_INFO
          }
        }
      }
    },
    .root_idx = 78,
    .is_typedef = 0
  },
  [KR_KR_PIXEL_FMT_META_INFO] = {
    .type = 1,
    .name = "kr_pixel_fmt",
    .member_count = 6,
    .members = {
      {
        .name = "KR_PIXELS_ARGB",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_PIXELS_YUV422P",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_PIXELS_YVU422P",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_PIXELS_YUV444",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_PIXELS_YUV420",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_PIXELS_YUV422",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_pixel_fmt),
    .is_typedef = 1
  },
  [KR_KR_IMAGE_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_image_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_IMAGE_ENC",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_IMAGE_RAW",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_image_type),
    .is_typedef = 1
  },
  [KR_KR_RECT_META_INFO] = {
    .type = 2,
    .name = "kr_rect",
    .member_count = 4,
    .members = {
      {
        .name = "x",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_rect, x),
        .sz = sizeof(((kr_rect*)0)->x),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = -128000,
            .max = 128000
          }
        }
      },
      {
        .name = "y",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_rect, y),
        .sz = sizeof(((kr_rect*)0)->y),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = -128000,
            .max = 128000
          }
        }
      },
      {
        .name = "w",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_rect, w),
        .sz = sizeof(((kr_rect*)0)->w),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 7680,
            .min = 1,
            .max = 256000
          }
        }
      },
      {
        .name = "h",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_rect, h),
        .sz = sizeof(((kr_rect*)0)->h),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 4320,
            .min = 1,
            .max = 256000
          }
        }
      }
    },
    .sz = sizeof(kr_rect),
    .is_typedef = 1
  },
  [KR_KR_IMAGE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_image_info",
    .member_count = 3,
    .members = {
      {
        .name = "w",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_image_info, w),
        .sz = sizeof(((kr_image_info*)0)->w),
        .opt = 0,
        .type = 7,
        .type_info = {

        }
      },
      {
        .name = "h",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_image_info, h),
        .sz = sizeof(((kr_image_info*)0)->h),
        .opt = 0,
        .type = 7,
        .type_info = {

        }
      },
      {
        .name = "fmt",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_image_info, fmt),
        .sz = sizeof(((kr_image_info*)0)->fmt),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_pixel_fmt",
            .enum_init = "",
            .idx = KR_KR_PIXEL_FMT_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_image_info),
    .is_typedef = 1
  },
  [KR_KR_CODED_IMAGE_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_coded_image_type",
    .member_count = 9,
    .members = {
      {
        .name = "KR_CODED_IMAGE_PNG",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CODED_IMAGE_JPG",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CODED_IMAGE_GIF",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CODED_IMAGE_VP8",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CODED_IMAGE_VP9",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CODED_IMAGE_DAALA",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CODED_IMAGE_THEORA",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CODED_IMAGE_Y4M",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CODED_IMAGE_H264",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_coded_image_type),
    .is_typedef = 1
  },
  [KR_KR_CODED_IMAGE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_coded_image_info",
    .member_count = 3,
    .members = {
      {
        .name = "fmt",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_coded_image_info, fmt),
        .sz = sizeof(((kr_coded_image_info*)0)->fmt),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_coded_image_type",
            .enum_init = "",
            .idx = KR_KR_CODED_IMAGE_TYPE_META_INFO
          }
        }
      },
      {
        .name = "sz",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_coded_image_info, sz),
        .sz = sizeof(((kr_coded_image_info*)0)->sz),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "image",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_coded_image_info, image),
        .sz = sizeof(((kr_coded_image_info*)0)->image),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_image_info",
            .idx = KR_KR_IMAGE_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_coded_image_info),
    .is_typedef = 1
  },
  [KR_KR_FILE_ACCESS_MODE_META_INFO] = {
    .type = 1,
    .name = "kr_file_access_mode",
    .member_count = 3,
    .members = {
      {
        .name = "KR_FILE_ACCESS_ONLY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_FILE_ACCESS_APPEND",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_FILE_ACCESS_MODIFY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_file_access_mode),
    .is_typedef = 1
  },
  [KR_KR_FILE2_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_file2_info",
    .member_count = 8,
    .members = {
      {
        .name = "access_mode",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_file2_info, access_mode),
        .sz = sizeof(((kr_file2_info*)0)->access_mode),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_file_access_mode",
            .enum_init = "",
            .idx = KR_KR_FILE_ACCESS_MODE_META_INFO
          }
        }
      },
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_file2_info, len),
        .sz = sizeof(((kr_file2_info*)0)->len),
        .opt = 0,
        .type = 7,
        .type_info = {

        }
      },
      {
        .name = "path",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_file2_info, path),
        .sz = sizeof(((kr_file2_info*)0)->path),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      },
      {
        .name = "sz",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_file2_info, sz),
        .sz = sizeof(((kr_file2_info*)0)->sz),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "wrote",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_file2_info, wrote),
        .sz = sizeof(((kr_file2_info*)0)->wrote),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "remote_fs",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_file2_info, remote_fs),
        .sz = sizeof(((kr_file2_info*)0)->remote_fs),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "lastmod",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_file2_info, lastmod),
        .sz = sizeof(((kr_file2_info*)0)->lastmod),
        .opt = 0,
        .type = 5,
        .type_info = {
          .int64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "type",
        .arr = 26,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_file2_info, type),
        .sz = sizeof(((kr_file2_info*)0)->type),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 25,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_file2_info),
    .is_typedef = 1
  },
  [KR_KR_MIXER_CHANNELS_META_INFO] = {
    .type = 1,
    .name = "kr_mixer_channels",
    .member_count = 8,
    .members = {
      {
        .name = "KR_MONO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_STEREO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_THREE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_QUAD",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_FIVE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_SIX",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_SEVEN",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_EIGHT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_mixer_channels),
    .is_typedef = 1
  },
  [KR_KR_MIXER_PATH_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_mixer_path_type",
    .member_count = 4,
    .members = {
      {
        .name = "KR_MXR_SOURCE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_MXR_INPUT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_MXR_BUS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_MXR_OUTPUT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_mixer_path_type),
    .is_typedef = 1
  },
  [KR_KR_MIXER_PATH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_mixer_path_info",
    .member_count = 3,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_mixer_path_info, type),
        .sz = sizeof(((struct kr_mixer_path_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_mixer_path_type",
            .enum_init = "mxr_source",
            .idx = KR_KR_MIXER_PATH_TYPE_META_INFO
          }
        }
      },
      {
        .name = "channels",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_mixer_path_info, channels),
        .sz = sizeof(((struct kr_mixer_path_info*)0)->channels),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_mixer_channels",
            .enum_init = "stereo",
            .idx = KR_KR_MIXER_CHANNELS_META_INFO
          }
        }
      },
      {
        .name = "afx",
        .arr = KR_MXR_MAX_CHANNELS,
        .arr_notnull = 0,
        .len_def = "KR_MXR_MAX_CHANNELS",
        .ptr = 0,
        .off = offsetof(struct kr_mixer_path_info, afx),
        .sz = sizeof(((struct kr_mixer_path_info*)0)->afx),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_afx_info",
            .idx = KR_KR_AFX_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(struct kr_mixer_path_info),
    .is_typedef = 0
  },
  [KR_KR_MEDIA_EVENT_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_media_event_type",
    .member_count = 4,
    .members = {
      {
        .name = "KR_MEDIA_CREATE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_MEDIA_ITEM_ADD",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_MEDIA_FILE_CREATE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_MEDIA_DESTROY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_media_event_type),
    .is_typedef = 1
  },
  [KR_KR_MEDIA_ITEM_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_media_item_type",
    .member_count = 3,
    .members = {
      {
        .name = "KR_MEDIA_URL",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_MEDIA_METADATA",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_MEDIA_FILE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_media_item_type),
    .is_typedef = 1
  },
  [KR_KR_MEDIA_FILE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_media_file_info",
    .member_count = 1,
    .members = {
      {
        .name = "name",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_file_info, name),
        .sz = sizeof(((kr_media_file_info*)0)->name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_media_file_info),
    .is_typedef = 1
  },
  [KR_KR_MEDIA_METADATA_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_media_metadata_info",
    .member_count = 4,
    .members = {
      {
        .name = "name",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_metadata_info, name),
        .sz = sizeof(((kr_media_metadata_info*)0)->name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      },
      {
        .name = "duration",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_metadata_info, duration),
        .sz = sizeof(((kr_media_metadata_info*)0)->duration),
        .opt = 0,
        .type = 11,
        .type_info = {
          .double_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      },
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_metadata_info, width),
        .sz = sizeof(((kr_media_metadata_info*)0)->width),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_metadata_info, height),
        .sz = sizeof(((kr_media_metadata_info*)0)->height),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_media_metadata_info),
    .is_typedef = 1
  },
  [KR_KR_MEDIA_URL_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_media_url_info",
    .member_count = 1,
    .members = {
      {
        .name = "url",
        .arr = 1024,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_url_info, url),
        .sz = sizeof(((kr_media_url_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 1023,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_media_url_info),
    .is_typedef = 1
  },
  [KR_KR_MEDIA_ITEM_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_media_item_info",
    .member_count = 4,
    .members = {
      {
        .name = "create_time",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_item_info, create_time),
        .sz = sizeof(((kr_media_item_info*)0)->create_time),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_timestamp_info",
            .idx = KR_KR_TIMESTAMP_INFO_META_INFO
          }
        }
      },
      {
        .name = "task",
        .arr = 40,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_item_info, task),
        .sz = sizeof(((kr_media_item_info*)0)->task),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 39,
            .notnull = 0
          }
        }
      },
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_item_info, type),
        .sz = sizeof(((kr_media_item_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_media_item_type",
            .enum_init = "",
            .idx = KR_KR_MEDIA_ITEM_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 97
          }
        }
      }
    },
    .sz = sizeof(kr_media_item_info),
    .is_typedef = 1
  },
  [KR_KR_MEDIA_ITEM_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_media_item_info_sub1",
    .member_count = 3,
    .members = {
      {
        .name = "url",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_item_info, url),
        .sz = sizeof(((kr_media_item_info*)0)->url),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_media_url_info",
            .idx = KR_KR_MEDIA_URL_INFO_META_INFO
          }
        }
      },
      {
        .name = "metadata",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_item_info, metadata),
        .sz = sizeof(((kr_media_item_info*)0)->metadata),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_media_metadata_info",
            .idx = KR_KR_MEDIA_METADATA_INFO_META_INFO
          }
        }
      },
      {
        .name = "file",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_item_info, file),
        .sz = sizeof(((kr_media_item_info*)0)->file),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_media_file_info",
            .idx = KR_KR_MEDIA_FILE_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 96,
    .is_typedef = 0
  },
  [KR_KR_MEDIA_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_media_info",
    .member_count = 5,
    .members = {
      {
        .name = "id",
        .arr = 40,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_info, id),
        .sz = sizeof(((kr_media_info*)0)->id),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 39,
            .notnull = 0
          }
        }
      },
      {
        .name = "nitems",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_info, nitems),
        .sz = sizeof(((kr_media_info*)0)->nitems),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "create_time",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_info, create_time),
        .sz = sizeof(((kr_media_info*)0)->create_time),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_timestamp_info",
            .idx = KR_KR_TIMESTAMP_INFO_META_INFO
          }
        }
      },
      {
        .name = "item",
        .arr = KR_MEDIA_INFO_NITEMS,
        .arr_notnull = 0,
        .len_def = "KR_MEDIA_INFO_NITEMS",
        .ptr = 0,
        .off = offsetof(kr_media_info, item),
        .sz = sizeof(((kr_media_info*)0)->item),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_media_item_info",
            .idx = KR_KR_MEDIA_ITEM_INFO_META_INFO
          }
        }
      },
      {
        .name = "media_dir",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_info, media_dir),
        .sz = sizeof(((kr_media_info*)0)->media_dir),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_media_info),
    .is_typedef = 1
  },
  [KR_KR_OPS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_ops_info",
    .member_count = 1,
    .members = {
      {
        .name = "sysname",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ops_info, sysname),
        .sz = sizeof(((kr_ops_info*)0)->sysname),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 15,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_ops_info),
    .is_typedef = 1
  },
  [KR_KR_POS_META_INFO] = {
    .type = 2,
    .name = "kr_pos",
    .member_count = 2,
    .members = {
      {
        .name = "x",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_pos, x),
        .sz = sizeof(((struct kr_pos*)0)->x),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "y",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_pos, y),
        .sz = sizeof(((struct kr_pos*)0)->y),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(struct kr_pos),
    .is_typedef = 0
  },
  [KR_KR_PERSPECTIVE_VIEW_META_INFO] = {
    .type = 2,
    .name = "kr_perspective_view",
    .member_count = 4,
    .members = {
      {
        .name = "top_left",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_perspective_view, top_left),
        .sz = sizeof(((struct kr_perspective_view*)0)->top_left),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_pos",
            .idx = KR_KR_POS_META_INFO
          }
        }
      },
      {
        .name = "top_right",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_perspective_view, top_right),
        .sz = sizeof(((struct kr_perspective_view*)0)->top_right),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_pos",
            .idx = KR_KR_POS_META_INFO
          }
        }
      },
      {
        .name = "bottom_left",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_perspective_view, bottom_left),
        .sz = sizeof(((struct kr_perspective_view*)0)->bottom_left),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_pos",
            .idx = KR_KR_POS_META_INFO
          }
        }
      },
      {
        .name = "bottom_right",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_perspective_view, bottom_right),
        .sz = sizeof(((struct kr_perspective_view*)0)->bottom_right),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_pos",
            .idx = KR_KR_POS_META_INFO
          }
        }
      }
    },
    .sz = sizeof(struct kr_perspective_view),
    .is_typedef = 0
  },
  [KR_M3U_EL_TYPE_META_INFO] = {
    .type = 1,
    .name = "m3u_el_type",
    .member_count = 2,
    .members = {
      {
        .name = "M3U_URI",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_TAG",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(m3u_el_type),
    .is_typedef = 1
  },
  [KR_KR_M3U_TAG_NAME_META_INFO] = {
    .type = 1,
    .name = "kr_m3u_tag_name",
    .member_count = 20,
    .members = {
      {
        .name = "EXTM3U",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXTINF",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_BYTERANGE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_TARGETDURATION",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_MEDIA_SEQUENCE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_KEY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_PROGRAM_DATE_TIME",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_ALLOW_CACHE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_PLAYLIST_TYPE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_STREAM_INF",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_I_FRAME_STREAM_INF",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_I_FRAMES_ONLY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_MAP",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_MEDIA",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_ENDLIST",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_DISCONTINUITY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_DISCONTINUITY_SEQUENCE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_INDEPENDENT_SEGMENTS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_START",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EXT_X_VERSION",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_m3u_tag_name),
    .is_typedef = 1
  },
  [KR_KR_M3U_TAG_VAL_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_m3u_tag_val_type",
    .member_count = 2,
    .members = {
      {
        .name = "M3U_TAG_VAL_PRIMITIVE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_TAG_VAL_ATTR_LIST",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_m3u_tag_val_type),
    .is_typedef = 1
  },
  [KR_KR_M3U_TAG_ATTR_NAME_META_INFO] = {
    .type = 1,
    .name = "kr_m3u_tag_attr_name",
    .member_count = 26,
    .members = {
      {
        .name = "M3U_ATTR_METHOD",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_URI",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_IV",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_KEYFORMAT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_KEYFORMATVERSIONS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_TYPE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_GROUP_ID",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_PROGRAM_ID",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_LANGUAGE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_ASSOC_LANGUAGE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_NAME",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_DEFAULT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_AUTOSELECT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_FORCED",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_INSTREAM_ID",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_CHARACTERISTICS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_BANDWIDTH",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_CODECS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_RESOLUTION",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_AUDIO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_VIDEO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_SUBTITLES",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_CLOSED_CAPTIONS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_BYTERANGE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_TIME_OFFEST",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "M3U_ATTR_PRECISE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_m3u_tag_attr_name),
    .is_typedef = 1
  },
  [KR_KR_M3U_TAG_PRIM_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_m3u_tag_prim_info",
    .member_count = 1,
    .members = {
      {
        .name = "value",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_m3u_tag_prim_info, value),
        .sz = sizeof(((kr_m3u_tag_prim_info*)0)->value),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_m3u_tag_prim_info),
    .is_typedef = 1
  },
  [KR_KR_M3U_TAG_ALIST_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_m3u_tag_alist_info",
    .member_count = 2,
    .members = {
      {
        .name = "name",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_m3u_tag_alist_info, name),
        .sz = sizeof(((kr_m3u_tag_alist_info*)0)->name),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_m3u_tag_attr_name",
            .enum_init = "",
            .idx = KR_KR_M3U_TAG_ATTR_NAME_META_INFO
          }
        }
      },
      {
        .name = "value",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_m3u_tag_alist_info, value),
        .sz = sizeof(((kr_m3u_tag_alist_info*)0)->value),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_m3u_tag_alist_info),
    .is_typedef = 1
  },
  [KR_M3U_TAG_INFO_META_INFO] = {
    .type = 2,
    .name = "m3u_tag_info",
    .member_count = 4,
    .members = {
      {
        .name = "name",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(m3u_tag_info, name),
        .sz = sizeof(((m3u_tag_info*)0)->name),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_m3u_tag_name",
            .enum_init = "",
            .idx = KR_KR_M3U_TAG_NAME_META_INFO
          }
        }
      },
      {
        .name = "vtype",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(m3u_tag_info, vtype),
        .sz = sizeof(((m3u_tag_info*)0)->vtype),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_m3u_tag_val_type",
            .enum_init = "",
            .idx = KR_KR_M3U_TAG_VAL_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 109
          }
        }
      },
      {
        .name = "count",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(m3u_tag_info, count),
        .sz = sizeof(((m3u_tag_info*)0)->count),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(m3u_tag_info),
    .is_typedef = 1
  },
  [KR_M3U_TAG_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "m3u_tag_info_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "prim",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(m3u_tag_info, prim),
        .sz = sizeof(((m3u_tag_info*)0)->prim),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_m3u_tag_prim_info",
            .idx = KR_KR_M3U_TAG_PRIM_INFO_META_INFO
          }
        }
      },
      {
        .name = "alist",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(m3u_tag_info, alist),
        .sz = sizeof(((m3u_tag_info*)0)->alist),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_m3u_tag_alist_info",
            .idx = KR_KR_M3U_TAG_ALIST_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 108,
    .is_typedef = 0
  },
  [KR_M3U_EL_INFO_META_INFO] = {
    .type = 2,
    .name = "m3u_el_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(m3u_el_info, type),
        .sz = sizeof(((m3u_el_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "m3u_el_type",
            .enum_init = "",
            .idx = KR_M3U_EL_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 111
          }
        }
      }
    },
    .sz = sizeof(m3u_el_info),
    .is_typedef = 1
  },
  [KR_M3U_EL_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "m3u_el_info_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "uri",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(m3u_el_info, uri),
        .sz = sizeof(((m3u_el_info*)0)->uri),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      },
      {
        .name = "tag",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(m3u_el_info, tag),
        .sz = sizeof(((m3u_el_info*)0)->tag),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "m3u_tag_info",
            .idx = KR_M3U_TAG_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 110,
    .is_typedef = 0
  },
  [KR_KR_M3U_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_m3u_info",
    .member_count = 2,
    .members = {
      {
        .name = "elements",
        .arr = M3U_MAX_EL,
        .arr_notnull = 0,
        .len_def = "M3U_MAX_EL",
        .ptr = 0,
        .off = offsetof(kr_m3u_info, elements),
        .sz = sizeof(((kr_m3u_info*)0)->elements),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "m3u_el_info",
            .idx = KR_M3U_EL_INFO_META_INFO
          }
        }
      },
      {
        .name = "el_count",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_m3u_info, el_count),
        .sz = sizeof(((kr_m3u_info*)0)->el_count),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_m3u_info),
    .is_typedef = 1
  },
  [KR_KR_LIMIT_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_limit_info",
    .member_count = 2,
    .members = {
      {
        .name = "label",
        .arr = 32,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_limit_info, label),
        .sz = sizeof(((struct kr_limit_info*)0)->label),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 31,
            .notnull = 0
          }
        }
      },
      {
        .name = "value",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_limit_info, value),
        .sz = sizeof(((struct kr_limit_info*)0)->value),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(struct kr_limit_info),
    .is_typedef = 0
  },
  [KR_KR_LIMITS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_limits_info",
    .member_count = 2,
    .members = {
      {
        .name = "limits",
        .arr = 256,
        .arr_notnull = 1,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_limits_info, limits),
        .sz = sizeof(((struct kr_limits_info*)0)->limits),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_limit_info",
            .idx = KR_KR_LIMIT_INFO_META_INFO
          }
        }
      },
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_limits_info, len),
        .sz = sizeof(((struct kr_limits_info*)0)->len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(struct kr_limits_info),
    .is_typedef = 0
  },
  [KR_KR_PRIORITY_META_INFO] = {
    .type = 1,
    .name = "kr_priority",
    .member_count = 6,
    .members = {
      {
        .name = "KR_PRIORITY_UNKNOWN",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_PRIORITY_NORMAL",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_PRIORITY_NONINTERACTIVE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_PRIORITY_TRANSCODER",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_PRIORITY_REALTIME",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_PRIORITY_REALTIME_IO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_priority),
    .is_typedef = 1
  },
  [KR_KR_AVCONV_STATUS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_avconv_status_info",
    .member_count = 2,
    .members = {
      {
        .name = "input",
        .arr = 1024,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avconv_status_info, input),
        .sz = sizeof(((kr_avconv_status_info*)0)->input),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 1023,
            .notnull = 0
          }
        }
      },
      {
        .name = "output",
        .arr = 1024,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avconv_status_info, output),
        .sz = sizeof(((kr_avconv_status_info*)0)->output),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 1023,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_avconv_status_info),
    .is_typedef = 1
  },
  [KR_KR_AVCONV_PARAMS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_avconv_params_info",
    .member_count = 2,
    .members = {
      {
        .name = "input",
        .arr = 1024,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avconv_params_info, input),
        .sz = sizeof(((kr_avconv_params_info*)0)->input),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 1023,
            .notnull = 0
          }
        }
      },
      {
        .name = "output",
        .arr = 1024,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avconv_params_info, output),
        .sz = sizeof(((kr_avconv_params_info*)0)->output),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 1023,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_avconv_params_info),
    .is_typedef = 1
  },
  [KR_KR_AVCONV_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_avconv_info",
    .member_count = 2,
    .members = {
      {
        .name = "input",
        .arr = 1024,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avconv_info, input),
        .sz = sizeof(((kr_avconv_info*)0)->input),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 1023,
            .notnull = 0
          }
        }
      },
      {
        .name = "output",
        .arr = 1024,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avconv_info, output),
        .sz = sizeof(((kr_avconv_info*)0)->output),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 1023,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_avconv_info),
    .is_typedef = 1
  },
  [KR_KR_AVPROBE_CODEC_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_avprobe_codec_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_AUDIO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_VIDEO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_avprobe_codec_type),
    .is_typedef = 1
  },
  [KR_KR_AVPROBE_FORMAT_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_avprobe_format_info",
    .member_count = 8,
    .members = {
      {
        .name = "filename",
        .arr = KR_AVP_MAX_SIZE,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_SIZE",
        .ptr = 0,
        .off = offsetof(kr_avprobe_format_info, filename),
        .sz = sizeof(((kr_avprobe_format_info*)0)->filename),
        .opt = 0,
        .type = 1,
        .type_info = {

        }
      },
      {
        .name = "nb_streams",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avprobe_format_info, nb_streams),
        .sz = sizeof(((kr_avprobe_format_info*)0)->nb_streams),
        .opt = 1,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "format_name",
        .arr = KR_AVP_MAX_SIZE,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_SIZE",
        .ptr = 0,
        .off = offsetof(kr_avprobe_format_info, format_name),
        .sz = sizeof(((kr_avprobe_format_info*)0)->format_name),
        .opt = 1,
        .type = 1,
        .type_info = {

        }
      },
      {
        .name = "format_long_name",
        .arr = KR_AVP_MAX_SIZE,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_SIZE",
        .ptr = 0,
        .off = offsetof(kr_avprobe_format_info, format_long_name),
        .sz = sizeof(((kr_avprobe_format_info*)0)->format_long_name),
        .opt = 1,
        .type = 1,
        .type_info = {

        }
      },
      {
        .name = "start_time",
        .arr = KR_AVP_MAX_SIZE,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_SIZE",
        .ptr = 0,
        .off = offsetof(kr_avprobe_format_info, start_time),
        .sz = sizeof(((kr_avprobe_format_info*)0)->start_time),
        .opt = 1,
        .type = 1,
        .type_info = {

        }
      },
      {
        .name = "duration",
        .arr = KR_AVP_MAX_SIZE,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_SIZE",
        .ptr = 0,
        .off = offsetof(kr_avprobe_format_info, duration),
        .sz = sizeof(((kr_avprobe_format_info*)0)->duration),
        .opt = 1,
        .type = 1,
        .type_info = {

        }
      },
      {
        .name = "size",
        .arr = KR_AVP_MAX_SIZE,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_SIZE",
        .ptr = 0,
        .off = offsetof(kr_avprobe_format_info, size),
        .sz = sizeof(((kr_avprobe_format_info*)0)->size),
        .opt = 1,
        .type = 1,
        .type_info = {

        }
      },
      {
        .name = "bit_rate",
        .arr = KR_AVP_MAX_SIZE,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_SIZE",
        .ptr = 0,
        .off = offsetof(kr_avprobe_format_info, bit_rate),
        .sz = sizeof(((kr_avprobe_format_info*)0)->bit_rate),
        .opt = 1,
        .type = 1,
        .type_info = {

        }
      }
    },
    .sz = sizeof(kr_avprobe_format_info),
    .is_typedef = 1
  },
  [KR_KR_AVPROBE_STREAM_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_avprobe_stream_info",
    .member_count = 11,
    .members = {
      {
        .name = "index",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avprobe_stream_info, index),
        .sz = sizeof(((kr_avprobe_stream_info*)0)->index),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "codec_name",
        .arr = KR_AVP_MAX_SIZE,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_SIZE",
        .ptr = 0,
        .off = offsetof(kr_avprobe_stream_info, codec_name),
        .sz = sizeof(((kr_avprobe_stream_info*)0)->codec_name),
        .opt = 0,
        .type = 1,
        .type_info = {

        }
      },
      {
        .name = "codec_type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avprobe_stream_info, codec_type),
        .sz = sizeof(((kr_avprobe_stream_info*)0)->codec_type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_avprobe_codec_type",
            .enum_init = "",
            .idx = KR_KR_AVPROBE_CODEC_TYPE_META_INFO
          }
        }
      },
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avprobe_stream_info, width),
        .sz = sizeof(((kr_avprobe_stream_info*)0)->width),
        .opt = 1,
        .type = 7,
        .type_info = {

        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avprobe_stream_info, height),
        .sz = sizeof(((kr_avprobe_stream_info*)0)->height),
        .opt = 1,
        .type = 7,
        .type_info = {

        }
      },
      {
        .name = "start_time",
        .arr = KR_AVP_MAX_SIZE,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_SIZE",
        .ptr = 0,
        .off = offsetof(kr_avprobe_stream_info, start_time),
        .sz = sizeof(((kr_avprobe_stream_info*)0)->start_time),
        .opt = 1,
        .type = 1,
        .type_info = {

        }
      },
      {
        .name = "duration",
        .arr = KR_AVP_MAX_SIZE,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_SIZE",
        .ptr = 0,
        .off = offsetof(kr_avprobe_stream_info, duration),
        .sz = sizeof(((kr_avprobe_stream_info*)0)->duration),
        .opt = 1,
        .type = 1,
        .type_info = {

        }
      },
      {
        .name = "channels",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avprobe_stream_info, channels),
        .sz = sizeof(((kr_avprobe_stream_info*)0)->channels),
        .opt = 1,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "sample_rate",
        .arr = KR_AVP_MAX_SIZE,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_SIZE",
        .ptr = 0,
        .off = offsetof(kr_avprobe_stream_info, sample_rate),
        .sz = sizeof(((kr_avprobe_stream_info*)0)->sample_rate),
        .opt = 1,
        .type = 1,
        .type_info = {

        }
      },
      {
        .name = "display_aspect_ratio",
        .arr = KR_AVP_MAX_SIZE,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_SIZE",
        .ptr = 0,
        .off = offsetof(kr_avprobe_stream_info, display_aspect_ratio),
        .sz = sizeof(((kr_avprobe_stream_info*)0)->display_aspect_ratio),
        .opt = 1,
        .type = 1,
        .type_info = {

        }
      },
      {
        .name = "pix_fmt",
        .arr = KR_AVP_MAX_SIZE,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_SIZE",
        .ptr = 0,
        .off = offsetof(kr_avprobe_stream_info, pix_fmt),
        .sz = sizeof(((kr_avprobe_stream_info*)0)->pix_fmt),
        .opt = 1,
        .type = 1,
        .type_info = {

        }
      }
    },
    .sz = sizeof(kr_avprobe_stream_info),
    .is_typedef = 1
  },
  [KR_KR_AVPROBE_MEDIA_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_avprobe_media_info",
    .member_count = 2,
    .members = {
      {
        .name = "format",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avprobe_media_info, format),
        .sz = sizeof(((kr_avprobe_media_info*)0)->format),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_avprobe_format_info",
            .idx = KR_KR_AVPROBE_FORMAT_INFO_META_INFO
          }
        }
      },
      {
        .name = "streams",
        .arr = KR_AVP_MAX_STREAMS,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_STREAMS",
        .ptr = 0,
        .off = offsetof(kr_avprobe_media_info, streams),
        .sz = sizeof(((kr_avprobe_media_info*)0)->streams),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_avprobe_stream_info",
            .idx = KR_KR_AVPROBE_STREAM_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_avprobe_media_info),
    .is_typedef = 1
  },
  [KR_KR_AVPROBE_PARAMS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_avprobe_params_info",
    .member_count = 1,
    .members = {
      {
        .name = "filename",
        .arr = KR_AVP_MAX_SIZE,
        .arr_notnull = 0,
        .len_def = "KR_AVP_MAX_SIZE",
        .ptr = 0,
        .off = offsetof(kr_avprobe_params_info, filename),
        .sz = sizeof(((kr_avprobe_params_info*)0)->filename),
        .opt = 0,
        .type = 1,
        .type_info = {

        }
      }
    },
    .sz = sizeof(kr_avprobe_params_info),
    .is_typedef = 1
  },
  [KR_KR_AVPROBE_STATUS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_avprobe_status_info",
    .member_count = 1,
    .members = {
      {
        .name = "media",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avprobe_status_info, media),
        .sz = sizeof(((kr_avprobe_status_info*)0)->media),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_avprobe_media_info",
            .idx = KR_KR_AVPROBE_MEDIA_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_avprobe_status_info),
    .is_typedef = 1
  },
  [KR_KR_AVPROBE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_avprobe_info",
    .member_count = 2,
    .members = {
      {
        .name = "params",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avprobe_info, params),
        .sz = sizeof(((kr_avprobe_info*)0)->params),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_avprobe_params_info",
            .idx = KR_KR_AVPROBE_PARAMS_INFO_META_INFO
          }
        }
      },
      {
        .name = "status",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_avprobe_info, status),
        .sz = sizeof(((kr_avprobe_info*)0)->status),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_avprobe_status_info",
            .idx = KR_KR_AVPROBE_STATUS_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_avprobe_info),
    .is_typedef = 1
  },
  [KR_KR_CURL_STATUS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_curl_status_info",
    .member_count = 3,
    .members = {
      {
        .name = "url",
        .arr = 2048,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_curl_status_info, url),
        .sz = sizeof(((kr_curl_status_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 2047,
            .notnull = 0
          }
        }
      },
      {
        .name = "dest",
        .arr = 2048,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_curl_status_info, dest),
        .sz = sizeof(((kr_curl_status_info*)0)->dest),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 2047,
            .notnull = 0
          }
        }
      },
      {
        .name = "percent_complete",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_curl_status_info, percent_complete),
        .sz = sizeof(((kr_curl_status_info*)0)->percent_complete),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_curl_status_info),
    .is_typedef = 1
  },
  [KR_KR_CURL_PARAMS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_curl_params_info",
    .member_count = 3,
    .members = {
      {
        .name = "url",
        .arr = 2048,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_curl_params_info, url),
        .sz = sizeof(((kr_curl_params_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 2047,
            .notnull = 0
          }
        }
      },
      {
        .name = "dest",
        .arr = 2048,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_curl_params_info, dest),
        .sz = sizeof(((kr_curl_params_info*)0)->dest),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 2047,
            .notnull = 0
          }
        }
      },
      {
        .name = "percent_complete",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_curl_params_info, percent_complete),
        .sz = sizeof(((kr_curl_params_info*)0)->percent_complete),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_curl_params_info),
    .is_typedef = 1
  },
  [KR_KR_CURL_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_curl_info",
    .member_count = 3,
    .members = {
      {
        .name = "url",
        .arr = 2048,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_curl_info, url),
        .sz = sizeof(((kr_curl_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 2047,
            .notnull = 0
          }
        }
      },
      {
        .name = "dest",
        .arr = 2048,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_curl_info, dest),
        .sz = sizeof(((kr_curl_info*)0)->dest),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 2047,
            .notnull = 0
          }
        }
      },
      {
        .name = "percent_complete",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_curl_info, percent_complete),
        .sz = sizeof(((kr_curl_info*)0)->percent_complete),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_curl_info),
    .is_typedef = 1
  },
  [KR_KR_LIVESTREAMER_PARAMS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_livestreamer_params_info",
    .member_count = 2,
    .members = {
      {
        .name = "save_file",
        .arr = 1024,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_livestreamer_params_info, save_file),
        .sz = sizeof(((kr_livestreamer_params_info*)0)->save_file),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 1023,
            .notnull = 0
          }
        }
      },
      {
        .name = "url",
        .arr = 2048,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_livestreamer_params_info, url),
        .sz = sizeof(((kr_livestreamer_params_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 2047,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_livestreamer_params_info),
    .is_typedef = 1
  },
  [KR_KR_LIVESTREAMER_STATUS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_livestreamer_status_info",
    .member_count = 2,
    .members = {
      {
        .name = "save_file",
        .arr = 1024,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_livestreamer_status_info, save_file),
        .sz = sizeof(((kr_livestreamer_status_info*)0)->save_file),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 1023,
            .notnull = 0
          }
        }
      },
      {
        .name = "url",
        .arr = 2048,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_livestreamer_status_info, url),
        .sz = sizeof(((kr_livestreamer_status_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 2047,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_livestreamer_status_info),
    .is_typedef = 1
  },
  [KR_KR_LIVESTREAMER_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_livestreamer_info",
    .member_count = 2,
    .members = {
      {
        .name = "save_file",
        .arr = 1024,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_livestreamer_info, save_file),
        .sz = sizeof(((kr_livestreamer_info*)0)->save_file),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 1023,
            .notnull = 0
          }
        }
      },
      {
        .name = "url",
        .arr = 2048,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_livestreamer_info, url),
        .sz = sizeof(((kr_livestreamer_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 2047,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_livestreamer_info),
    .is_typedef = 1
  },
  [KR_KR_MEDIA_VERSION_META_INFO] = {
    .type = 2,
    .name = "kr_media_version",
    .member_count = 9,
    .members = {
      {
        .name = "url",
        .arr = 2048,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_version, url),
        .sz = sizeof(((kr_media_version*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 2047,
            .notnull = 0
          }
        }
      },
      {
        .name = "id",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_version, id),
        .sz = sizeof(((kr_media_version*)0)->id),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "container",
        .arr = 32,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_version, container),
        .sz = sizeof(((kr_media_version*)0)->container),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 31,
            .notnull = 0
          }
        }
      },
      {
        .name = "video_encoding",
        .arr = 32,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_version, video_encoding),
        .sz = sizeof(((kr_media_version*)0)->video_encoding),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 31,
            .notnull = 0
          }
        }
      },
      {
        .name = "audio_encoding",
        .arr = 32,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_version, audio_encoding),
        .sz = sizeof(((kr_media_version*)0)->audio_encoding),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 31,
            .notnull = 0
          }
        }
      },
      {
        .name = "video_bitrate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_version, video_bitrate),
        .sz = sizeof(((kr_media_version*)0)->video_bitrate),
        .opt = 0,
        .type = 11,
        .type_info = {
          .double_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      },
      {
        .name = "audio_bitrate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_version, audio_bitrate),
        .sz = sizeof(((kr_media_version*)0)->audio_bitrate),
        .opt = 0,
        .type = 11,
        .type_info = {
          .double_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      },
      {
        .name = "video_height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_version, video_height),
        .sz = sizeof(((kr_media_version*)0)->video_height),
        .opt = 0,
        .type = 11,
        .type_info = {
          .double_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      },
      {
        .name = "video_width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_version, video_width),
        .sz = sizeof(((kr_media_version*)0)->video_width),
        .opt = 0,
        .type = 11,
        .type_info = {
          .double_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      }
    },
    .sz = sizeof(kr_media_version),
    .is_typedef = 1
  },
  [KR_KR_QUVI_MEDIA_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_quvi_media_info",
    .member_count = 7,
    .members = {
      {
        .name = "id",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_quvi_media_info, id),
        .sz = sizeof(((kr_quvi_media_info*)0)->id),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "title",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_quvi_media_info, title),
        .sz = sizeof(((kr_quvi_media_info*)0)->title),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "thumbnail_url",
        .arr = 2048,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_quvi_media_info, thumbnail_url),
        .sz = sizeof(((kr_quvi_media_info*)0)->thumbnail_url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 2047,
            .notnull = 0
          }
        }
      },
      {
        .name = "start_time_ms",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_quvi_media_info, start_time_ms),
        .sz = sizeof(((kr_quvi_media_info*)0)->start_time_ms),
        .opt = 0,
        .type = 11,
        .type_info = {
          .double_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      },
      {
        .name = "duration_ms",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_quvi_media_info, duration_ms),
        .sz = sizeof(((kr_quvi_media_info*)0)->duration_ms),
        .opt = 0,
        .type = 11,
        .type_info = {
          .double_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      },
      {
        .name = "versions",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_quvi_media_info, versions),
        .sz = sizeof(((kr_quvi_media_info*)0)->versions),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "version",
        .arr = KR_QUVI_NVERSIONS,
        .arr_notnull = 0,
        .len_def = "KR_QUVI_NVERSIONS",
        .ptr = 0,
        .off = offsetof(kr_quvi_media_info, version),
        .sz = sizeof(((kr_quvi_media_info*)0)->version),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_media_version",
            .idx = KR_KR_MEDIA_VERSION_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_quvi_media_info),
    .is_typedef = 1
  },
  [KR_KR_QUVI_STATUS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_quvi_status_info",
    .member_count = 1,
    .members = {
      {
        .name = "media_info",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_quvi_status_info, media_info),
        .sz = sizeof(((kr_quvi_status_info*)0)->media_info),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_quvi_media_info",
            .idx = KR_KR_QUVI_MEDIA_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_quvi_status_info),
    .is_typedef = 1
  },
  [KR_KR_QUVI_PARAMS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_quvi_params_info",
    .member_count = 1,
    .members = {
      {
        .name = "url",
        .arr = 2048,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_quvi_params_info, url),
        .sz = sizeof(((kr_quvi_params_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 2047,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_quvi_params_info),
    .is_typedef = 1
  },
  [KR_KR_QUVI_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_quvi_info",
    .member_count = 2,
    .members = {
      {
        .name = "params",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_quvi_info, params),
        .sz = sizeof(((kr_quvi_info*)0)->params),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_quvi_params_info",
            .idx = KR_KR_QUVI_PARAMS_INFO_META_INFO
          }
        }
      },
      {
        .name = "status",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_quvi_info, status),
        .sz = sizeof(((kr_quvi_info*)0)->status),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_quvi_status_info",
            .idx = KR_KR_QUVI_STATUS_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_quvi_info),
    .is_typedef = 1
  },
  [KR_KR_TRANSMISSION_CLI_STATUS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_transmission_cli_status_info",
    .member_count = 1,
    .members = {
      {
        .name = "file",
        .arr = 1024,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_transmission_cli_status_info, file),
        .sz = sizeof(((kr_transmission_cli_status_info*)0)->file),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 1023,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_transmission_cli_status_info),
    .is_typedef = 1
  },
  [KR_KR_TRANSMISSION_CLI_PARAMS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_transmission_cli_params_info",
    .member_count = 1,
    .members = {
      {
        .name = "file",
        .arr = 1024,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_transmission_cli_params_info, file),
        .sz = sizeof(((kr_transmission_cli_params_info*)0)->file),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 1023,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_transmission_cli_params_info),
    .is_typedef = 1
  },
  [KR_KR_TRANSMISSION_CLI_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_transmission_cli_info",
    .member_count = 1,
    .members = {
      {
        .name = "file",
        .arr = 1024,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_transmission_cli_info, file),
        .sz = sizeof(((kr_transmission_cli_info*)0)->file),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 1023,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_transmission_cli_info),
    .is_typedef = 1
  },
  [KR_KR_TASK_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_task_type",
    .member_count = 9,
    .members = {
      {
        .name = "KR_VALGRIND_STRESSOR",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_QUVI",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AVCONV",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_XMMS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CURL",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TRANSMISSION_CLI",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_LIVESTREAMER",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AVPROBE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_YDL",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_task_type),
    .is_typedef = 1
  },
  [KR_KR_TASK_STATE_META_INFO] = {
    .type = 1,
    .name = "kr_task_state",
    .member_count = 7,
    .members = {
      {
        .name = "KR_TASK_READY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TASK_RUNNING",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TASK_FAILED",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TASK_TIMEDOUT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TASK_SUCCEEDED",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TASK_CANCELLED",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TASK_SAME_STATE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_task_state),
    .is_typedef = 1
  },
  [KR_KR_TASK_EVENT_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_task_event_type",
    .member_count = 9,
    .members = {
      {
        .name = "KR_TASK_CREATE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TASK_START",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TASK_PATCH",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TASK_PROGRESS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TASK_FAILURE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TASK_SUCCESS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TASK_CANCEL",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TASK_TIMEOUT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TASK_DESTROY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_task_event_type),
    .is_typedef = 1
  },
  [KR_KR_FAIL_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_fail_info",
    .member_count = 5,
    .members = {
      {
        .name = "reason",
        .arr = 32,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_fail_info, reason),
        .sz = sizeof(((kr_fail_info*)0)->reason),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 31,
            .notnull = 0
          }
        }
      },
      {
        .name = "details",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_fail_info, details),
        .sz = sizeof(((kr_fail_info*)0)->details),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "subfault",
        .arr = 32,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_fail_info, subfault),
        .sz = sizeof(((kr_fail_info*)0)->subfault),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 31,
            .notnull = 0
          }
        }
      },
      {
        .name = "should_retry",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_fail_info, should_retry),
        .sz = sizeof(((kr_fail_info*)0)->should_retry),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "attempt",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_fail_info, attempt),
        .sz = sizeof(((kr_fail_info*)0)->attempt),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_fail_info),
    .is_typedef = 1
  },
  [KR_KR_TASK_STATUS_META_INFO] = {
    .type = 2,
    .name = "kr_task_status",
    .member_count = 5,
    .members = {
      {
        .name = "state",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_status, state),
        .sz = sizeof(((kr_task_status*)0)->state),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_task_state",
            .enum_init = "",
            .idx = KR_KR_TASK_STATE_META_INFO
          }
        }
      },
      {
        .name = "create_time",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_status, create_time),
        .sz = sizeof(((kr_task_status*)0)->create_time),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_timestamp_info",
            .idx = KR_KR_TIMESTAMP_INFO_META_INFO
          }
        }
      },
      {
        .name = "start_time",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_status, start_time),
        .sz = sizeof(((kr_task_status*)0)->start_time),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_timestamp_info",
            .idx = KR_KR_TIMESTAMP_INFO_META_INFO
          }
        }
      },
      {
        .name = "stop_time",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_status, stop_time),
        .sz = sizeof(((kr_task_status*)0)->stop_time),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_timestamp_info",
            .idx = KR_KR_TIMESTAMP_INFO_META_INFO
          }
        }
      },
      {
        .name = "fail",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_status, fail),
        .sz = sizeof(((kr_task_status*)0)->fail),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_fail_info",
            .idx = KR_KR_FAIL_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_task_status),
    .is_typedef = 1
  },
  [KR_KR_TASK_PARAMS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_task_params_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_params_info, type),
        .sz = sizeof(((kr_task_params_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_task_type",
            .enum_init = "ydl",
            .idx = KR_KR_TASK_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 146
          }
        }
      }
    },
    .sz = sizeof(kr_task_params_info),
    .is_typedef = 1
  },
  [KR_KR_TASK_PARAMS_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_task_params_info_sub1",
    .member_count = 9,
    .members = {
      {
        .name = "valgrind_stressor",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_params_info, valgrind_stressor),
        .sz = sizeof(((kr_task_params_info*)0)->valgrind_stressor),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_valgrind_stressor_params_info",
            .idx = KR_KR_VALGRIND_STRESSOR_PARAMS_INFO_META_INFO
          }
        }
      },
      {
        .name = "quvi",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_params_info, quvi),
        .sz = sizeof(((kr_task_params_info*)0)->quvi),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_quvi_params_info",
            .idx = KR_KR_QUVI_PARAMS_INFO_META_INFO
          }
        }
      },
      {
        .name = "avconv",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_params_info, avconv),
        .sz = sizeof(((kr_task_params_info*)0)->avconv),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_avconv_params_info",
            .idx = KR_KR_AVCONV_PARAMS_INFO_META_INFO
          }
        }
      },
      {
        .name = "xmms",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_params_info, xmms),
        .sz = sizeof(((kr_task_params_info*)0)->xmms),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xmms_params_info",
            .idx = KR_KR_XMMS_PARAMS_INFO_META_INFO
          }
        }
      },
      {
        .name = "curl",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_params_info, curl),
        .sz = sizeof(((kr_task_params_info*)0)->curl),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_curl_params_info",
            .idx = KR_KR_CURL_PARAMS_INFO_META_INFO
          }
        }
      },
      {
        .name = "transmission_cli",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_params_info, transmission_cli),
        .sz = sizeof(((kr_task_params_info*)0)->transmission_cli),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_transmission_cli_params_info",
            .idx = KR_KR_TRANSMISSION_CLI_PARAMS_INFO_META_INFO
          }
        }
      },
      {
        .name = "livestreamer",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_params_info, livestreamer),
        .sz = sizeof(((kr_task_params_info*)0)->livestreamer),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_livestreamer_params_info",
            .idx = KR_KR_LIVESTREAMER_PARAMS_INFO_META_INFO
          }
        }
      },
      {
        .name = "avprobe",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_params_info, avprobe),
        .sz = sizeof(((kr_task_params_info*)0)->avprobe),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_avprobe_params_info",
            .idx = KR_KR_AVPROBE_PARAMS_INFO_META_INFO
          }
        }
      },
      {
        .name = "ydl",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_params_info, ydl),
        .sz = sizeof(((kr_task_params_info*)0)->ydl),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_ydl_params_info",
            .idx = KR_KR_YDL_PARAMS_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 145,
    .is_typedef = 0
  },
  [KR_KR_TASK_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_task_info",
    .member_count = 5,
    .members = {
      {
        .name = "id",
        .arr = 40,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_info, id),
        .sz = sizeof(((kr_task_info*)0)->id),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 39,
            .notnull = 0
          }
        }
      },
      {
        .name = "status",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_info, status),
        .sz = sizeof(((kr_task_info*)0)->status),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_task_status",
            .idx = KR_KR_TASK_STATUS_META_INFO
          }
        }
      },
      {
        .name = "params",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_info, params),
        .sz = sizeof(((kr_task_info*)0)->params),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_task_params_info",
            .idx = KR_KR_TASK_PARAMS_INFO_META_INFO
          }
        }
      },
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_info, type),
        .sz = sizeof(((kr_task_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_task_type",
            .enum_init = "ydl",
            .idx = KR_KR_TASK_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 148
          }
        }
      }
    },
    .sz = sizeof(kr_task_info),
    .is_typedef = 1
  },
  [KR_KR_TASK_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_task_info_sub1",
    .member_count = 9,
    .members = {
      {
        .name = "valgrind_stressor",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_info, valgrind_stressor),
        .sz = sizeof(((kr_task_info*)0)->valgrind_stressor),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_valgrind_stressor_info",
            .idx = KR_KR_VALGRIND_STRESSOR_INFO_META_INFO
          }
        }
      },
      {
        .name = "quvi",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_info, quvi),
        .sz = sizeof(((kr_task_info*)0)->quvi),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_quvi_info",
            .idx = KR_KR_QUVI_INFO_META_INFO
          }
        }
      },
      {
        .name = "avconv",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_info, avconv),
        .sz = sizeof(((kr_task_info*)0)->avconv),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_avconv_info",
            .idx = KR_KR_AVCONV_INFO_META_INFO
          }
        }
      },
      {
        .name = "xmms",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_info, xmms),
        .sz = sizeof(((kr_task_info*)0)->xmms),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xmms_info",
            .idx = KR_KR_XMMS_INFO_META_INFO
          }
        }
      },
      {
        .name = "curl",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_info, curl),
        .sz = sizeof(((kr_task_info*)0)->curl),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_curl_info",
            .idx = KR_KR_CURL_INFO_META_INFO
          }
        }
      },
      {
        .name = "transmission_cli",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_info, transmission_cli),
        .sz = sizeof(((kr_task_info*)0)->transmission_cli),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_transmission_cli_info",
            .idx = KR_KR_TRANSMISSION_CLI_INFO_META_INFO
          }
        }
      },
      {
        .name = "livestreamer",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_info, livestreamer),
        .sz = sizeof(((kr_task_info*)0)->livestreamer),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_livestreamer_info",
            .idx = KR_KR_LIVESTREAMER_INFO_META_INFO
          }
        }
      },
      {
        .name = "avprobe",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_info, avprobe),
        .sz = sizeof(((kr_task_info*)0)->avprobe),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_avprobe_info",
            .idx = KR_KR_AVPROBE_INFO_META_INFO
          }
        }
      },
      {
        .name = "ydl",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_task_info, ydl),
        .sz = sizeof(((kr_task_info*)0)->ydl),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_ydl_info",
            .idx = KR_KR_YDL_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 147,
    .is_typedef = 0
  },
  [KR_KR_VALGRIND_STRESSOR_STATUS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_valgrind_stressor_status_info",
    .member_count = 0,
    .members = {
    },
    .sz = sizeof(kr_valgrind_stressor_status_info),
    .is_typedef = 1
  },
  [KR_KR_VALGRIND_STRESSOR_PARAMS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_valgrind_stressor_params_info",
    .member_count = 0,
    .members = {
    },
    .sz = sizeof(kr_valgrind_stressor_params_info),
    .is_typedef = 1
  },
  [KR_KR_VALGRIND_STRESSOR_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_valgrind_stressor_info",
    .member_count = 0,
    .members = {
    },
    .sz = sizeof(kr_valgrind_stressor_info),
    .is_typedef = 1
  },
  [KR_KR_XMMS_PARAMS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xmms_params_info",
    .member_count = 2,
    .members = {
      {
        .name = "filename",
        .arr = PATH_MAX,
        .arr_notnull = 0,
        .len_def = "PATH_MAX",
        .ptr = 0,
        .off = offsetof(kr_xmms_params_info, filename),
        .sz = sizeof(((kr_xmms_params_info*)0)->filename),
        .opt = 0,
        .type = 1,
        .type_info = {

        }
      },
      {
        .name = "env",
        .arr = 32,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xmms_params_info, env),
        .sz = sizeof(((kr_xmms_params_info*)0)->env),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 31,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xmms_params_info),
    .is_typedef = 1
  },
  [KR_KR_XMMS_STATUS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xmms_status_info",
    .member_count = 1,
    .members = {
      {
        .name = "coconut",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xmms_status_info, coconut),
        .sz = sizeof(((kr_xmms_status_info*)0)->coconut),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xmms_status_info),
    .is_typedef = 1
  },
  [KR_KR_XMMS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xmms_info",
    .member_count = 2,
    .members = {
      {
        .name = "params",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xmms_info, params),
        .sz = sizeof(((kr_xmms_info*)0)->params),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xmms_params_info",
            .idx = KR_KR_XMMS_PARAMS_INFO_META_INFO
          }
        }
      },
      {
        .name = "status",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xmms_info, status),
        .sz = sizeof(((kr_xmms_info*)0)->status),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xmms_status_info",
            .idx = KR_KR_XMMS_STATUS_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_xmms_info),
    .is_typedef = 1
  },
  [KR_KR_YDL_FORMAT_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_ydl_format_info",
    .member_count = 8,
    .members = {
      {
        .name = "url",
        .arr = 1024,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_format_info, url),
        .sz = sizeof(((kr_ydl_format_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 1023,
            .notnull = 0
          }
        }
      },
      {
        .name = "ext",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_format_info, ext),
        .sz = sizeof(((kr_ydl_format_info*)0)->ext),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 15,
            .notnull = 0
          }
        }
      },
      {
        .name = "format_id",
        .arr = 32,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_format_info, format_id),
        .sz = sizeof(((kr_ydl_format_info*)0)->format_id),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 31,
            .notnull = 0
          }
        }
      },
      {
        .name = "format",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_format_info, format),
        .sz = sizeof(((kr_ydl_format_info*)0)->format),
        .opt = 1,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "vcodec",
        .arr = 32,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_format_info, vcodec),
        .sz = sizeof(((kr_ydl_format_info*)0)->vcodec),
        .opt = 1,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 31,
            .notnull = 0
          }
        }
      },
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_format_info, width),
        .sz = sizeof(((kr_ydl_format_info*)0)->width),
        .opt = 1,
        .type = 7,
        .type_info = {

        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_format_info, height),
        .sz = sizeof(((kr_ydl_format_info*)0)->height),
        .opt = 1,
        .type = 7,
        .type_info = {

        }
      },
      {
        .name = "abr",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_format_info, abr),
        .sz = sizeof(((kr_ydl_format_info*)0)->abr),
        .opt = 1,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_ydl_format_info),
    .is_typedef = 1
  },
  [KR_KR_YDL_VIDEO_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_ydl_video_info",
    .member_count = 13,
    .members = {
      {
        .name = "duration",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_video_info, duration),
        .sz = sizeof(((kr_ydl_video_info*)0)->duration),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "description",
        .arr = 4096,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_video_info, description),
        .sz = sizeof(((kr_ydl_video_info*)0)->description),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 4095,
            .notnull = 0
          }
        }
      },
      {
        .name = "uploader_id",
        .arr = 64,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_video_info, uploader_id),
        .sz = sizeof(((kr_ydl_video_info*)0)->uploader_id),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 63,
            .notnull = 0
          }
        }
      },
      {
        .name = "uploader",
        .arr = 64,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_video_info, uploader),
        .sz = sizeof(((kr_ydl_video_info*)0)->uploader),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 63,
            .notnull = 0
          }
        }
      },
      {
        .name = "view_count",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_video_info, view_count),
        .sz = sizeof(((kr_ydl_video_info*)0)->view_count),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "title",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_video_info, title),
        .sz = sizeof(((kr_ydl_video_info*)0)->title),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "ext",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_video_info, ext),
        .sz = sizeof(((kr_ydl_video_info*)0)->ext),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 15,
            .notnull = 0
          }
        }
      },
      {
        .name = "id",
        .arr = 32,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_video_info, id),
        .sz = sizeof(((kr_ydl_video_info*)0)->id),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 31,
            .notnull = 0
          }
        }
      },
      {
        .name = "thumbnail",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_video_info, thumbnail),
        .sz = sizeof(((kr_ydl_video_info*)0)->thumbnail),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_video_info, width),
        .sz = sizeof(((kr_ydl_video_info*)0)->width),
        .opt = 0,
        .type = 7,
        .type_info = {

        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_video_info, height),
        .sz = sizeof(((kr_ydl_video_info*)0)->height),
        .opt = 0,
        .type = 7,
        .type_info = {

        }
      },
      {
        .name = "upload_date",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_video_info, upload_date),
        .sz = sizeof(((kr_ydl_video_info*)0)->upload_date),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 15,
            .notnull = 0
          }
        }
      },
      {
        .name = "formats",
        .arr = KR_YDL_MAX_FORMATS,
        .arr_notnull = 0,
        .len_def = "KR_YDL_MAX_FORMATS",
        .ptr = 0,
        .off = offsetof(kr_ydl_video_info, formats),
        .sz = sizeof(((kr_ydl_video_info*)0)->formats),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_ydl_format_info",
            .idx = KR_KR_YDL_FORMAT_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_ydl_video_info),
    .is_typedef = 1
  },
  [KR_KR_YDL_PARAMS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_ydl_params_info",
    .member_count = 1,
    .members = {
      {
        .name = "url",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_params_info, url),
        .sz = sizeof(((kr_ydl_params_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_ydl_params_info),
    .is_typedef = 1
  },
  [KR_KR_YDL_STATUS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_ydl_status_info",
    .member_count = 1,
    .members = {
      {
        .name = "video",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_status_info, video),
        .sz = sizeof(((kr_ydl_status_info*)0)->video),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_ydl_video_info",
            .idx = KR_KR_YDL_VIDEO_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_ydl_status_info),
    .is_typedef = 1
  },
  [KR_KR_YDL_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_ydl_info",
    .member_count = 2,
    .members = {
      {
        .name = "params",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_info, params),
        .sz = sizeof(((kr_ydl_info*)0)->params),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_ydl_params_info",
            .idx = KR_KR_YDL_PARAMS_INFO_META_INFO
          }
        }
      },
      {
        .name = "status",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_ydl_info, status),
        .sz = sizeof(((kr_ydl_info*)0)->status),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_ydl_status_info",
            .idx = KR_KR_YDL_STATUS_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_ydl_info),
    .is_typedef = 1
  },
  [KR_KR_TIMESTAMP_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_timestamp_info",
    .member_count = 2,
    .members = {
      {
        .name = "seconds",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_timestamp_info, seconds),
        .sz = sizeof(((kr_timestamp_info*)0)->seconds),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "nanoseconds",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_timestamp_info, nanoseconds),
        .sz = sizeof(((kr_timestamp_info*)0)->nanoseconds),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_timestamp_info),
    .is_typedef = 1
  },
  [KR_KR_EASING_META_INFO] = {
    .type = 1,
    .name = "kr_easing",
    .member_count = 8,
    .members = {
      {
        .name = "LINEAR",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EASEINSINE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EASEOUTSINE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EASEINOUTSINE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EASEINCUBIC",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EASEOUTCUBIC",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EASEINOUTCUBIC",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "EASEINOUTELASTIC",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_easing),
    .is_typedef = 1
  },
  [KR_KR_MEDIA_SOURCE_FORMAT_META_INFO] = {
    .type = 1,
    .name = "kr_media_source_format",
    .member_count = 1,
    .members = {
      {
        .name = "MEDIA_SOURCE_MTS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_media_source_format),
    .is_typedef = 1
  },
  [KR_KR_MEDIA_SOURCE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_media_source_info",
    .member_count = 1,
    .members = {
      {
        .name = "path",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_source_info, path),
        .sz = sizeof(((kr_media_source_info*)0)->path),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_media_source_info),
    .is_typedef = 1
  },
  [KR_KR_MEDIA_PACKET_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_media_packet_type",
    .member_count = 3,
    .members = {
      {
        .name = "KR_PKT_HEADER",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_PKT_KEY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_PKT_NORMAL",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_media_packet_type),
    .is_typedef = 1
  },
  [KR_KR_MEDIA_PACKET_META_INFO] = {
    .type = 2,
    .name = "kr_media_packet",
    .member_count = 6,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_packet, type),
        .sz = sizeof(((kr_media_packet*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_media_packet_type",
            .enum_init = "",
            .idx = KR_KR_MEDIA_PACKET_TYPE_META_INFO
          }
        }
      },
      {
        .name = "track",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_packet, track),
        .sz = sizeof(((kr_media_packet*)0)->track),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "pts",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_packet, pts),
        .sz = sizeof(((kr_media_packet*)0)->pts),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "dts",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_packet, dts),
        .sz = sizeof(((kr_media_packet*)0)->dts),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "buf",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 1,
        .off = offsetof(kr_media_packet, buf),
        .sz = sizeof(((kr_media_packet*)0)->buf),
        .opt = 0,
        .type = 6,
        .type_info = {

        }
      },
      {
        .name = "sz",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_media_packet, sz),
        .sz = sizeof(((kr_media_packet*)0)->sz),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_media_packet),
    .is_typedef = 1
  },
  [KR_KR_TRACK_NAME_META_INFO] = {
    .type = 2,
    .name = "kr_track_name",
    .member_count = 2,
    .members = {
      {
        .name = "name",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_track_name, name),
        .sz = sizeof(((kr_track_name*)0)->name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_track_name, len),
        .sz = sizeof(((kr_track_name*)0)->len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_track_name),
    .is_typedef = 1
  },
  [KR_KR_MUX_NAME_META_INFO] = {
    .type = 2,
    .name = "kr_mux_name",
    .member_count = 2,
    .members = {
      {
        .name = "name",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mux_name, name),
        .sz = sizeof(((kr_mux_name*)0)->name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mux_name, len),
        .sz = sizeof(((kr_mux_name*)0)->len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_mux_name),
    .is_typedef = 1
  },
  [KR_KR_DEMUX_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_demux_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_demux_info, type),
        .sz = sizeof(((kr_demux_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_container_type",
            .enum_init = "",
            .idx = KR_KR_CONTAINER_TYPE_META_INFO
          }
        }
      },
      {
        .name = "source",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_demux_info, source),
        .sz = sizeof(((kr_demux_info*)0)->source),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_demux_info),
    .is_typedef = 1
  },
  [KR_KR_MUX_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_mux_info",
    .member_count = 3,
    .members = {
      {
        .name = "input",
        .arr = 4,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mux_info, input),
        .sz = sizeof(((kr_mux_info*)0)->input),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_track_name",
            .idx = KR_KR_TRACK_NAME_META_INFO
          }
        }
      },
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mux_info, type),
        .sz = sizeof(((kr_mux_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_container_type",
            .enum_init = "",
            .idx = KR_KR_CONTAINER_TYPE_META_INFO
          }
        }
      },
      {
        .name = "ntracks",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_mux_info, ntracks),
        .sz = sizeof(((kr_mux_info*)0)->ntracks),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_mux_info),
    .is_typedef = 1
  },
  [KR_KR_VIDEO_ENCODE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_video_encode_info",
    .member_count = 2,
    .members = {
      {
        .name = "codec",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_video_encode_info, codec),
        .sz = sizeof(((kr_video_encode_info*)0)->codec),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_video_codec",
            .enum_init = "",
            .idx = KR_KR_VIDEO_CODEC_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 171
          }
        }
      }
    },
    .sz = sizeof(kr_video_encode_info),
    .is_typedef = 1
  },
  [KR_KR_VIDEO_ENCODE_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_video_encode_info_sub1",
    .member_count = 5,
    .members = {
      {
        .name = "daala",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_video_encode_info, daala),
        .sz = sizeof(((kr_video_encode_info*)0)->daala),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_daala_encoder_info",
            .idx = KR_KR_DAALA_ENCODER_INFO_META_INFO
          }
        }
      },
      {
        .name = "vpx",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_video_encode_info, vpx),
        .sz = sizeof(((kr_video_encode_info*)0)->vpx),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_vpx_encoder_info",
            .idx = KR_KR_VPX_ENCODER_INFO_META_INFO
          }
        }
      },
      {
        .name = "h264",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_video_encode_info, h264),
        .sz = sizeof(((kr_video_encode_info*)0)->h264),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_h264_encoder_info",
            .idx = KR_KR_H264_ENCODER_INFO_META_INFO
          }
        }
      },
      {
        .name = "theora",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_video_encode_info, theora),
        .sz = sizeof(((kr_video_encode_info*)0)->theora),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_theora_encoder_info",
            .idx = KR_KR_THEORA_ENCODER_INFO_META_INFO
          }
        }
      },
      {
        .name = "y4m",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_video_encode_info, y4m),
        .sz = sizeof(((kr_video_encode_info*)0)->y4m),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_y4m_encoder_info",
            .idx = KR_KR_Y4M_ENCODER_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 170,
    .is_typedef = 0
  },
  [KR_KR_AUDIO_ENCODE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_audio_encode_info",
    .member_count = 2,
    .members = {
      {
        .name = "codec",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_audio_encode_info, codec),
        .sz = sizeof(((kr_audio_encode_info*)0)->codec),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_audio_codec",
            .enum_init = "",
            .idx = KR_KR_AUDIO_CODEC_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 173
          }
        }
      }
    },
    .sz = sizeof(kr_audio_encode_info),
    .is_typedef = 1
  },
  [KR_KR_AUDIO_ENCODE_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_audio_encode_info_sub1",
    .member_count = 5,
    .members = {
      {
        .name = "flac",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_audio_encode_info, flac),
        .sz = sizeof(((kr_audio_encode_info*)0)->flac),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_flac_info",
            .idx = KR_KR_FLAC_INFO_META_INFO
          }
        }
      },
      {
        .name = "opus",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_audio_encode_info, opus),
        .sz = sizeof(((kr_audio_encode_info*)0)->opus),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_opus_info",
            .idx = KR_KR_OPUS_INFO_META_INFO
          }
        }
      },
      {
        .name = "vorbis",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_audio_encode_info, vorbis),
        .sz = sizeof(((kr_audio_encode_info*)0)->vorbis),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_vorbis_info",
            .idx = KR_KR_VORBIS_INFO_META_INFO
          }
        }
      },
      {
        .name = "mp3",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_audio_encode_info, mp3),
        .sz = sizeof(((kr_audio_encode_info*)0)->mp3),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_mp3_info",
            .idx = KR_KR_MP3_INFO_META_INFO
          }
        }
      },
      {
        .name = "aac",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_audio_encode_info, aac),
        .sz = sizeof(((kr_audio_encode_info*)0)->aac),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_aac_info",
            .idx = KR_KR_AAC_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 172,
    .is_typedef = 0
  },
  [KR_KR_ENCODE_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_encode_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_ENCODE_AUDIO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_ENCODE_VIDEO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_encode_type),
    .is_typedef = 1
  },
  [KR_KR_ENCODE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_encode_info",
    .member_count = 4,
    .members = {
      {
        .name = "in",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_encode_info, in),
        .sz = sizeof(((kr_encode_info*)0)->in),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_track_name",
            .idx = KR_KR_TRACK_NAME_META_INFO
          }
        }
      },
      {
        .name = "out",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_encode_info, out),
        .sz = sizeof(((kr_encode_info*)0)->out),
        .opt = 1,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_track_name",
            .idx = KR_KR_TRACK_NAME_META_INFO
          }
        }
      },
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_encode_info, type),
        .sz = sizeof(((kr_encode_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_encode_type",
            .enum_init = "",
            .idx = KR_KR_ENCODE_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 176
          }
        }
      }
    },
    .sz = sizeof(kr_encode_info),
    .is_typedef = 1
  },
  [KR_KR_ENCODE_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_encode_info_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "audio",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_encode_info, audio),
        .sz = sizeof(((kr_encode_info*)0)->audio),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_audio_encode_info",
            .idx = KR_KR_AUDIO_ENCODE_INFO_META_INFO
          }
        }
      },
      {
        .name = "video",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_encode_info, video),
        .sz = sizeof(((kr_encode_info*)0)->video),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_video_encode_info",
            .idx = KR_KR_VIDEO_ENCODE_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 175,
    .is_typedef = 0
  },
  [KR_KR_CAPTURE_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_capture_type",
    .member_count = 3,
    .members = {
      {
        .name = "KR_CAPTURE_AUDIO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CAPTURE_VIDEO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_CAPTURE_IMAGE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_capture_type),
    .is_typedef = 1
  },
  [KR_KR_RECORD_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_record_info",
    .member_count = 2,
    .members = {
      {
        .name = "name",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_record_info, name),
        .sz = sizeof(((kr_record_info*)0)->name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_record_info, len),
        .sz = sizeof(((kr_record_info*)0)->len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_record_info),
    .is_typedef = 1
  },
  [KR_KR_TRANSMISSION_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_transmission_info",
    .member_count = 2,
    .members = {
      {
        .name = "txmtr",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_transmission_info, txmtr),
        .sz = sizeof(((kr_transmission_info*)0)->txmtr),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xfer_transmitter_info",
            .idx = KR_KR_XFER_TRANSMITTER_INFO_META_INFO
          }
        }
      },
      {
        .name = "mount",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_transmission_info, mount),
        .sz = sizeof(((kr_transmission_info*)0)->mount),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_transmission_info),
    .is_typedef = 1
  },
  [KR_KR_OUTPUT_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_output_type",
    .member_count = 3,
    .members = {
      {
        .name = "KR_STUDIO_RECORD",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_STUDIO_UPLINK",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_STUDIO_TRANSMISSION",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_output_type),
    .is_typedef = 1
  },
  [KR_KR_OUTPUT_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_output_info",
    .member_count = 3,
    .members = {
      {
        .name = "input",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_output_info, input),
        .sz = sizeof(((kr_output_info*)0)->input),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_mux_name",
            .idx = KR_KR_MUX_NAME_META_INFO
          }
        }
      },
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_output_info, type),
        .sz = sizeof(((kr_output_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_output_type",
            .enum_init = "",
            .idx = KR_KR_OUTPUT_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 182
          }
        }
      }
    },
    .sz = sizeof(kr_output_info),
    .is_typedef = 1
  },
  [KR_KR_OUTPUT_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_output_info_sub1",
    .member_count = 3,
    .members = {
      {
        .name = "record",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_output_info, record),
        .sz = sizeof(((kr_output_info*)0)->record),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_record_info",
            .idx = KR_KR_RECORD_INFO_META_INFO
          }
        }
      },
      {
        .name = "uplink",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_output_info, uplink),
        .sz = sizeof(((kr_output_info*)0)->uplink),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_uplink_info",
            .idx = KR_KR_UPLINK_INFO_META_INFO
          }
        }
      },
      {
        .name = "tx",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_output_info, tx),
        .sz = sizeof(((kr_output_info*)0)->tx),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_transmission_info",
            .idx = KR_KR_TRANSMISSION_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 181,
    .is_typedef = 0
  },
  [KR_KR_CAPTURE_AUDIO_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_capture_audio_info",
    .member_count = 3,
    .members = {
      {
        .name = "track",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_audio_info, track),
        .sz = sizeof(((kr_capture_audio_info*)0)->track),
        .opt = 1,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_track_name",
            .idx = KR_KR_TRACK_NAME_META_INFO
          }
        }
      },
      {
        .name = "channels",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_audio_info, channels),
        .sz = sizeof(((kr_capture_audio_info*)0)->channels),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 2,
            .min = 1,
            .max = 2
          }
        }
      },
      {
        .name = "total_frames",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_audio_info, total_frames),
        .sz = sizeof(((kr_capture_audio_info*)0)->total_frames),
        .opt = 1,
        .type = 5,
        .type_info = {
          .int64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_capture_audio_info),
    .is_typedef = 1
  },
  [KR_KR_CAPTURE_VIDEO_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_capture_video_info",
    .member_count = 7,
    .members = {
      {
        .name = "track",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_video_info, track),
        .sz = sizeof(((kr_capture_video_info*)0)->track),
        .opt = 1,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_track_name",
            .idx = KR_KR_TRACK_NAME_META_INFO
          }
        }
      },
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_video_info, width),
        .sz = sizeof(((kr_capture_video_info*)0)->width),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_video_info, height),
        .sz = sizeof(((kr_capture_video_info*)0)->height),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "fps_num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_video_info, fps_num),
        .sz = sizeof(((kr_capture_video_info*)0)->fps_num),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "fps_den",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_video_info, fps_den),
        .sz = sizeof(((kr_capture_video_info*)0)->fps_den),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "format",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_video_info, format),
        .sz = sizeof(((kr_capture_video_info*)0)->format),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_pixel_fmt",
            .enum_init = "",
            .idx = KR_KR_PIXEL_FMT_META_INFO
          }
        }
      },
      {
        .name = "total_frames",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_video_info, total_frames),
        .sz = sizeof(((kr_capture_video_info*)0)->total_frames),
        .opt = 1,
        .type = 5,
        .type_info = {
          .int64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_capture_video_info),
    .is_typedef = 1
  },
  [KR_KR_CAPTURE_IMAGE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_capture_image_info",
    .member_count = 3,
    .members = {
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_image_info, width),
        .sz = sizeof(((kr_capture_image_info*)0)->width),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_image_info, height),
        .sz = sizeof(((kr_capture_image_info*)0)->height),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "format",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_image_info, format),
        .sz = sizeof(((kr_capture_image_info*)0)->format),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_pixel_fmt",
            .enum_init = "",
            .idx = KR_KR_PIXEL_FMT_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_capture_image_info),
    .is_typedef = 1
  },
  [KR_KR_CAPTURE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_capture_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_info, type),
        .sz = sizeof(((kr_capture_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_capture_type",
            .enum_init = "",
            .idx = KR_KR_CAPTURE_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 187
          }
        }
      }
    },
    .sz = sizeof(kr_capture_info),
    .is_typedef = 1
  },
  [KR_KR_CAPTURE_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_capture_info_sub1",
    .member_count = 3,
    .members = {
      {
        .name = "audio",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_info, audio),
        .sz = sizeof(((kr_capture_info*)0)->audio),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_capture_audio_info",
            .idx = KR_KR_CAPTURE_AUDIO_INFO_META_INFO
          }
        }
      },
      {
        .name = "video",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_info, video),
        .sz = sizeof(((kr_capture_info*)0)->video),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_capture_video_info",
            .idx = KR_KR_CAPTURE_VIDEO_INFO_META_INFO
          }
        }
      },
      {
        .name = "image",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_capture_info, image),
        .sz = sizeof(((kr_capture_info*)0)->image),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_capture_image_info",
            .idx = KR_KR_CAPTURE_IMAGE_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 186,
    .is_typedef = 0
  },
  [KR_KR_SOURCE_FILE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_source_file_info",
    .member_count = 3,
    .members = {
      {
        .name = "path",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_source_file_info, path),
        .sz = sizeof(((kr_source_file_info*)0)->path),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "size",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_source_file_info, size),
        .sz = sizeof(((kr_source_file_info*)0)->size),
        .opt = 0,
        .type = 5,
        .type_info = {
          .int64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "lastmod",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_source_file_info, lastmod),
        .sz = sizeof(((kr_source_file_info*)0)->lastmod),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_source_file_info),
    .is_typedef = 1
  },
  [KR_KR_SOURCE_REMOTE_FILE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_source_remote_file_info",
    .member_count = 1,
    .members = {
      {
        .name = "path",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_source_remote_file_info, path),
        .sz = sizeof(((kr_source_remote_file_info*)0)->path),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_source_remote_file_info),
    .is_typedef = 1
  },
  [KR_KR_STUDIO_SOURCE_STREAM_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_studio_source_stream_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_SOURCE_STREAM_ACCEPT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_SOURCE_STREAM_DOWNLINK",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_studio_source_stream_type),
    .is_typedef = 1
  },
  [KR_KR_SOURCE_STREAM_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_source_stream_info",
    .member_count = 5,
    .members = {
      {
        .name = "path",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_source_stream_info, path),
        .sz = sizeof(((kr_source_stream_info*)0)->path),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_source_stream_info, len),
        .sz = sizeof(((kr_source_stream_info*)0)->len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_source_stream_info, type),
        .sz = sizeof(((kr_source_stream_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_studio_source_stream_type",
            .enum_init = "",
            .idx = KR_KR_STUDIO_SOURCE_STREAM_TYPE_META_INFO
          }
        }
      },
      {
        .name = "num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_source_stream_info, num),
        .sz = sizeof(((kr_source_stream_info*)0)->num),
        .opt = 1,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "bytes",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_source_stream_info, bytes),
        .sz = sizeof(((kr_source_stream_info*)0)->bytes),
        .opt = 1,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_source_stream_info),
    .is_typedef = 1
  },
  [KR_KR_STUDIO_SOURCE_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_studio_source_type",
    .member_count = 3,
    .members = {
      {
        .name = "KR_SOURCE_LOCAL_FILE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_SOURCE_REMOTE_FILE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_SOURCE_STREAM",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_studio_source_type),
    .is_typedef = 1
  },
  [KR_KR_STUDIO_SOURCE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_studio_source_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_source_info, type),
        .sz = sizeof(((kr_studio_source_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_studio_source_type",
            .enum_init = "",
            .idx = KR_KR_STUDIO_SOURCE_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 194
          }
        }
      }
    },
    .sz = sizeof(kr_studio_source_info),
    .is_typedef = 1
  },
  [KR_KR_STUDIO_SOURCE_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_studio_source_info_sub1",
    .member_count = 3,
    .members = {
      {
        .name = "file",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_source_info, file),
        .sz = sizeof(((kr_studio_source_info*)0)->file),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_source_file_info",
            .idx = KR_KR_SOURCE_FILE_INFO_META_INFO
          }
        }
      },
      {
        .name = "remote",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_source_info, remote),
        .sz = sizeof(((kr_studio_source_info*)0)->remote),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_source_remote_file_info",
            .idx = KR_KR_SOURCE_REMOTE_FILE_INFO_META_INFO
          }
        }
      },
      {
        .name = "stream",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_source_info, stream),
        .sz = sizeof(((kr_studio_source_info*)0)->stream),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_source_stream_info",
            .idx = KR_KR_SOURCE_STREAM_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 193,
    .is_typedef = 0
  },
  [KR_KR_DECODE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_decode_info",
    .member_count = 1,
    .members = {
      {
        .name = "bongo",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_decode_info, bongo),
        .sz = sizeof(((kr_decode_info*)0)->bongo),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_decode_info),
    .is_typedef = 1
  },
  [KR_KR_PLAY_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_play_info",
    .member_count = 1,
    .members = {
      {
        .name = "input",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_play_info, input),
        .sz = sizeof(((kr_play_info*)0)->input),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_track_name",
            .idx = KR_KR_TRACK_NAME_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_play_info),
    .is_typedef = 1
  },
  [KR_KR_RENDER_IMAGE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_render_image_info",
    .member_count = 1,
    .members = {
      {
        .name = "id",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_render_image_info, id),
        .sz = sizeof(((kr_render_image_info*)0)->id),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 15,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_render_image_info),
    .is_typedef = 1
  },
  [KR_KR_RENDER_VIDEO_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_render_video_info",
    .member_count = 1,
    .members = {
      {
        .name = "id",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_render_video_info, id),
        .sz = sizeof(((kr_render_video_info*)0)->id),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 15,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_render_video_info),
    .is_typedef = 1
  },
  [KR_KR_RENDER_AUDIO_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_render_audio_info",
    .member_count = 1,
    .members = {
      {
        .name = "id",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_render_audio_info, id),
        .sz = sizeof(((kr_render_audio_info*)0)->id),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 15,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_render_audio_info),
    .is_typedef = 1
  },
  [KR_KR_RENDER_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_render_type",
    .member_count = 3,
    .members = {
      {
        .name = "KR_RENDER_IMAGE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_RENDER_VIDEO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_RENDER_AUDIO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_render_type),
    .is_typedef = 1
  },
  [KR_KR_RENDER_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_render_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_render_info, type),
        .sz = sizeof(((kr_render_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_render_type",
            .enum_init = "",
            .idx = KR_KR_RENDER_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 202
          }
        }
      }
    },
    .sz = sizeof(kr_render_info),
    .is_typedef = 1
  },
  [KR_KR_RENDER_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_render_info_sub1",
    .member_count = 3,
    .members = {
      {
        .name = "image",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_render_info, image),
        .sz = sizeof(((kr_render_info*)0)->image),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_render_image_info",
            .idx = KR_KR_RENDER_IMAGE_INFO_META_INFO
          }
        }
      },
      {
        .name = "video",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_render_info, video),
        .sz = sizeof(((kr_render_info*)0)->video),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_render_video_info",
            .idx = KR_KR_RENDER_VIDEO_INFO_META_INFO
          }
        }
      },
      {
        .name = "audio",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_render_info, audio),
        .sz = sizeof(((kr_render_info*)0)->audio),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_render_audio_info",
            .idx = KR_KR_RENDER_AUDIO_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 201,
    .is_typedef = 0
  },
  [KR_KR_AUDIO_GENERATE_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_audio_generate_type",
    .member_count = 3,
    .members = {
      {
        .name = "KR_GENERATE_NOISE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_GENERATE_TONE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_GENERATE_SILENCE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_audio_generate_type),
    .is_typedef = 1
  },
  [KR_KR_GENERATE_AUDIO_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_generate_audio_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_audio_info, type),
        .sz = sizeof(((kr_generate_audio_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_audio_generate_type",
            .enum_init = "",
            .idx = KR_KR_AUDIO_GENERATE_TYPE_META_INFO
          }
        }
      },
      {
        .name = "channels",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_audio_info, channels),
        .sz = sizeof(((kr_generate_audio_info*)0)->channels),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_generate_audio_info),
    .is_typedef = 1
  },
  [KR_KR_VIDEO_GENERATE_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_video_generate_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_GENERATE_COLOR",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_GENERATE_NO_SIGNAL",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_video_generate_type),
    .is_typedef = 1
  },
  [KR_KR_GENERATE_COLOR_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_generate_color_info",
    .member_count = 3,
    .members = {
      {
        .name = "r",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_color_info, r),
        .sz = sizeof(((kr_generate_color_info*)0)->r),
        .opt = 0,
        .type = 6,
        .type_info = {

        }
      },
      {
        .name = "g",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_color_info, g),
        .sz = sizeof(((kr_generate_color_info*)0)->g),
        .opt = 0,
        .type = 6,
        .type_info = {

        }
      },
      {
        .name = "b",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_color_info, b),
        .sz = sizeof(((kr_generate_color_info*)0)->b),
        .opt = 0,
        .type = 6,
        .type_info = {

        }
      }
    },
    .sz = sizeof(kr_generate_color_info),
    .is_typedef = 1
  },
  [KR_KR_GENERATE_VIDEO_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_generate_video_info",
    .member_count = 7,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_video_info, type),
        .sz = sizeof(((kr_generate_video_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_video_generate_type",
            .enum_init = "",
            .idx = KR_KR_VIDEO_GENERATE_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 208
          }
        }
      },
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_video_info, width),
        .sz = sizeof(((kr_generate_video_info*)0)->width),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_video_info, height),
        .sz = sizeof(((kr_generate_video_info*)0)->height),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "fps_num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_video_info, fps_num),
        .sz = sizeof(((kr_generate_video_info*)0)->fps_num),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "fps_den",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_video_info, fps_den),
        .sz = sizeof(((kr_generate_video_info*)0)->fps_den),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "format",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_video_info, format),
        .sz = sizeof(((kr_generate_video_info*)0)->format),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_pixel_fmt",
            .enum_init = "",
            .idx = KR_KR_PIXEL_FMT_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_generate_video_info),
    .is_typedef = 1
  },
  [KR_KR_GENERATE_VIDEO_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_generate_video_info_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "color",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_video_info, color),
        .sz = sizeof(((kr_generate_video_info*)0)->color),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_generate_color_info",
            .idx = KR_KR_GENERATE_COLOR_INFO_META_INFO
          }
        }
      },
      {
        .name = "with_coconut",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_video_info, with_coconut),
        .sz = sizeof(((kr_generate_video_info*)0)->with_coconut),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .root_idx = 207,
    .is_typedef = 0
  },
  [KR_KR_GENERATE_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_generate_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_GENERATE_AUDIO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_GENERATE_VIDEO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_generate_type),
    .is_typedef = 1
  },
  [KR_KR_GENERATE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_generate_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_info, type),
        .sz = sizeof(((kr_generate_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_generate_type",
            .enum_init = "",
            .idx = KR_KR_GENERATE_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 211
          }
        }
      }
    },
    .sz = sizeof(kr_generate_info),
    .is_typedef = 1
  },
  [KR_KR_GENERATE_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_generate_info_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "audio",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_info, audio),
        .sz = sizeof(((kr_generate_info*)0)->audio),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_generate_audio_info",
            .idx = KR_KR_GENERATE_AUDIO_INFO_META_INFO
          }
        }
      },
      {
        .name = "video",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_generate_info, video),
        .sz = sizeof(((kr_generate_info*)0)->video),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_generate_video_info",
            .idx = KR_KR_GENERATE_VIDEO_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 210,
    .is_typedef = 0
  },
  [KR_KR_STUDIO_PATH_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_studio_path_type",
    .member_count = 10,
    .members = {
      {
        .name = "KR_STUDIO_SOURCE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_STUDIO_GENERATE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_STUDIO_CAPTURE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_STUDIO_RENDER",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_STUDIO_PLAY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_STUDIO_ENCODE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_STUDIO_DECODE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_STUDIO_DEMUX",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_STUDIO_MUX",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_STUDIO_OUTPUT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_studio_path_type),
    .is_typedef = 1
  },
  [KR_KR_STUDIO_PATH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_studio_path_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_path_info, type),
        .sz = sizeof(((kr_studio_path_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_studio_path_type",
            .enum_init = "",
            .idx = KR_KR_STUDIO_PATH_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 214
          }
        }
      }
    },
    .sz = sizeof(kr_studio_path_info),
    .is_typedef = 1
  },
  [KR_KR_STUDIO_PATH_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_studio_path_info_sub1",
    .member_count = 10,
    .members = {
      {
        .name = "source",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_path_info, source),
        .sz = sizeof(((kr_studio_path_info*)0)->source),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_studio_source_info",
            .idx = KR_KR_STUDIO_SOURCE_INFO_META_INFO
          }
        }
      },
      {
        .name = "generate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_path_info, generate),
        .sz = sizeof(((kr_studio_path_info*)0)->generate),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_generate_info",
            .idx = KR_KR_GENERATE_INFO_META_INFO
          }
        }
      },
      {
        .name = "capture",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_path_info, capture),
        .sz = sizeof(((kr_studio_path_info*)0)->capture),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_capture_info",
            .idx = KR_KR_CAPTURE_INFO_META_INFO
          }
        }
      },
      {
        .name = "render",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_path_info, render),
        .sz = sizeof(((kr_studio_path_info*)0)->render),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_render_info",
            .idx = KR_KR_RENDER_INFO_META_INFO
          }
        }
      },
      {
        .name = "play",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_path_info, play),
        .sz = sizeof(((kr_studio_path_info*)0)->play),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_play_info",
            .idx = KR_KR_PLAY_INFO_META_INFO
          }
        }
      },
      {
        .name = "encode",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_path_info, encode),
        .sz = sizeof(((kr_studio_path_info*)0)->encode),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_encode_info",
            .idx = KR_KR_ENCODE_INFO_META_INFO
          }
        }
      },
      {
        .name = "decode",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_path_info, decode),
        .sz = sizeof(((kr_studio_path_info*)0)->decode),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_decode_info",
            .idx = KR_KR_DECODE_INFO_META_INFO
          }
        }
      },
      {
        .name = "demux",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_path_info, demux),
        .sz = sizeof(((kr_studio_path_info*)0)->demux),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_demux_info",
            .idx = KR_KR_DEMUX_INFO_META_INFO
          }
        }
      },
      {
        .name = "mux",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_path_info, mux),
        .sz = sizeof(((kr_studio_path_info*)0)->mux),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_mux_info",
            .idx = KR_KR_MUX_INFO_META_INFO
          }
        }
      },
      {
        .name = "output",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_studio_path_info, output),
        .sz = sizeof(((kr_studio_path_info*)0)->output),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_output_info",
            .idx = KR_KR_OUTPUT_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 213,
    .is_typedef = 0
  },
  [KR_KR_TRACK_BLOCK_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_track_block_type",
    .member_count = 8,
    .members = {
      {
        .name = "KR_TRACK_CODED_AUDIO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TRACK_CODED_VIDEO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TRACK_MUX",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TRACK_AUDIO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TRACK_VIDEO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TRACK_SUBTITLE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TRACK_METADATA",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TRACK_CONTROL",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_track_block_type),
    .is_typedef = 1
  },
  [KR_KR_TRACK_MODE_META_INFO] = {
    .type = 1,
    .name = "kr_track_mode",
    .member_count = 2,
    .members = {
      {
        .name = "KR_TRACK_FINITE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TRACK_INFINITE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_track_mode),
    .is_typedef = 1
  },
  [KR_KR_TRACK_EVENT_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_track_event_type",
    .member_count = 4,
    .members = {
      {
        .name = "KR_TRACK_ADD",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TRACK_REMOVE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TRACK_READ",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TRACK_WRITE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_track_event_type),
    .is_typedef = 1
  },
  [KR_KR_TRACK_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_track_info",
    .member_count = 6,
    .members = {
      {
        .name = "track",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_track_info, track),
        .sz = sizeof(((kr_track_info*)0)->track),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_track_name",
            .idx = KR_KR_TRACK_NAME_META_INFO
          }
        }
      },
      {
        .name = "codec",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_track_info, codec),
        .sz = sizeof(((kr_track_info*)0)->codec),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_codec",
            .idx = KR_KR_CODEC_META_INFO
          }
        }
      },
      {
        .name = "mode",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_track_info, mode),
        .sz = sizeof(((kr_track_info*)0)->mode),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_track_mode",
            .enum_init = "",
            .idx = KR_KR_TRACK_MODE_META_INFO
          }
        }
      },
      {
        .name = "duration",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_track_info, duration),
        .sz = sizeof(((kr_track_info*)0)->duration),
        .opt = 0,
        .type = 5,
        .type_info = {
          .int64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_track_info, type),
        .sz = sizeof(((kr_track_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_track_block_type",
            .enum_init = "",
            .idx = KR_KR_TRACK_BLOCK_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 219
          }
        }
      }
    },
    .sz = sizeof(kr_track_info),
    .is_typedef = 1
  },
  [KR_KR_TRACK_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_track_info_sub1",
    .member_count = 5,
    .members = {
      {
        .name = "audio",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_track_info, audio),
        .sz = sizeof(((kr_track_info*)0)->audio),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_audio_info",
            .idx = KR_KR_AUDIO_INFO_META_INFO
          }
        }
      },
      {
        .name = "video",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_track_info, video),
        .sz = sizeof(((kr_track_info*)0)->video),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_video_info",
            .idx = KR_KR_VIDEO_INFO_META_INFO
          }
        }
      },
      {
        .name = "coconut",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_track_info, coconut),
        .sz = sizeof(((kr_track_info*)0)->coconut),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "coconut2",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_track_info, coconut2),
        .sz = sizeof(((kr_track_info*)0)->coconut2),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "coconut3",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_track_info, coconut3),
        .sz = sizeof(((kr_track_info*)0)->coconut3),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .root_idx = 218,
    .is_typedef = 0
  },
  [KR_KR_TRACK_BLOCK_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_track_block_info",
    .member_count = 1,
    .members = {
      {
        .name = "tc",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_track_block_info, tc),
        .sz = sizeof(((kr_track_block_info*)0)->tc),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_track_block_info),
    .is_typedef = 1
  },
  [KR_KR_HLS_PLAYLIST_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_hls_playlist_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_HLS_PLAYLIST_MASTER",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_HLS_PLAYLIST_MEDIA",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_hls_playlist_type),
    .is_typedef = 1
  },
  [KR_KR_HLS_RENDITION_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_hls_rendition_type",
    .member_count = 4,
    .members = {
      {
        .name = "HLS_AUDIO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "HLS_VIDEO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "HLS_SUBTITLES",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "HLS_CLOSED_CAPTIONS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_hls_rendition_type),
    .is_typedef = 1
  },
  [KR_KR_HLS_MEDIA_PLAYLIST_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_hls_media_playlist_type",
    .member_count = 2,
    .members = {
      {
        .name = "HLS_VOD",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "HLS_LIVE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_hls_media_playlist_type),
    .is_typedef = 1
  },
  [KR_KR_HLS_VARIANT_META_INFO] = {
    .type = 2,
    .name = "kr_hls_variant",
    .member_count = 9,
    .members = {
      {
        .name = "name",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_variant, name),
        .sz = sizeof(((kr_hls_variant*)0)->name),
        .opt = 1,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "url",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_variant, url),
        .sz = sizeof(((kr_hls_variant*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "codecs",
        .arr = 128,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_variant, codecs),
        .sz = sizeof(((kr_hls_variant*)0)->codecs),
        .opt = 1,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 127,
            .notnull = 0
          }
        }
      },
      {
        .name = "bitrate",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_variant, bitrate),
        .sz = sizeof(((kr_hls_variant*)0)->bitrate),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "width",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_variant, width),
        .sz = sizeof(((kr_hls_variant*)0)->width),
        .opt = 1,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "height",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_variant, height),
        .sz = sizeof(((kr_hls_variant*)0)->height),
        .opt = 1,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "audio",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_variant, audio),
        .sz = sizeof(((kr_hls_variant*)0)->audio),
        .opt = 1,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "video",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_variant, video),
        .sz = sizeof(((kr_hls_variant*)0)->video),
        .opt = 1,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "subtitles",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_variant, subtitles),
        .sz = sizeof(((kr_hls_variant*)0)->subtitles),
        .opt = 1,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_hls_variant),
    .is_typedef = 1
  },
  [KR_KR_HLS_RENDITION_META_INFO] = {
    .type = 2,
    .name = "kr_hls_rendition",
    .member_count = 5,
    .members = {
      {
        .name = "name",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_rendition, name),
        .sz = sizeof(((kr_hls_rendition*)0)->name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "group",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_rendition, group),
        .sz = sizeof(((kr_hls_rendition*)0)->group),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "url",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_rendition, url),
        .sz = sizeof(((kr_hls_rendition*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_rendition, type),
        .sz = sizeof(((kr_hls_rendition*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_hls_rendition_type",
            .enum_init = "",
            .idx = KR_KR_HLS_RENDITION_TYPE_META_INFO
          }
        }
      },
      {
        .name = "def",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_rendition, def),
        .sz = sizeof(((kr_hls_rendition*)0)->def),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_hls_rendition),
    .is_typedef = 1
  },
  [KR_KR_HLS_MASTER_PLAYLIST_META_INFO] = {
    .type = 2,
    .name = "kr_hls_master_playlist",
    .member_count = 5,
    .members = {
      {
        .name = "url",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_master_playlist, url),
        .sz = sizeof(((kr_hls_master_playlist*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "nrenditions",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_master_playlist, nrenditions),
        .sz = sizeof(((kr_hls_master_playlist*)0)->nrenditions),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "nvariants",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_master_playlist, nvariants),
        .sz = sizeof(((kr_hls_master_playlist*)0)->nvariants),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "rendition",
        .arr = KR_HLS_MAX_RENDITIONS,
        .arr_notnull = 0,
        .len_def = "KR_HLS_MAX_RENDITIONS",
        .ptr = 0,
        .off = offsetof(kr_hls_master_playlist, rendition),
        .sz = sizeof(((kr_hls_master_playlist*)0)->rendition),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_hls_rendition",
            .idx = KR_KR_HLS_RENDITION_META_INFO
          }
        }
      },
      {
        .name = "variant",
        .arr = KR_HLS_MAX_VARIANTS,
        .arr_notnull = 0,
        .len_def = "KR_HLS_MAX_VARIANTS",
        .ptr = 0,
        .off = offsetof(kr_hls_master_playlist, variant),
        .sz = sizeof(((kr_hls_master_playlist*)0)->variant),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_hls_variant",
            .idx = KR_KR_HLS_VARIANT_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_hls_master_playlist),
    .is_typedef = 1
  },
  [KR_KR_HLS_MEDIA_SEGMENT_META_INFO] = {
    .type = 2,
    .name = "kr_hls_media_segment",
    .member_count = 3,
    .members = {
      {
        .name = "url",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_media_segment, url),
        .sz = sizeof(((kr_hls_media_segment*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "sequence_num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_media_segment, sequence_num),
        .sz = sizeof(((kr_hls_media_segment*)0)->sequence_num),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "duration",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_media_segment, duration),
        .sz = sizeof(((kr_hls_media_segment*)0)->duration),
        .opt = 0,
        .type = 10,
        .type_info = {
          .float_info = {
            .init = 0.00,
            .min = 0.00,
            .max = 0.00
          }
        }
      }
    },
    .sz = sizeof(kr_hls_media_segment),
    .is_typedef = 1
  },
  [KR_KR_HLS_MEDIA_PLAYLIST_META_INFO] = {
    .type = 2,
    .name = "kr_hls_media_playlist",
    .member_count = 5,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_media_playlist, type),
        .sz = sizeof(((kr_hls_media_playlist*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_hls_media_playlist_type",
            .enum_init = "",
            .idx = KR_KR_HLS_MEDIA_PLAYLIST_TYPE_META_INFO
          }
        }
      },
      {
        .name = "max_duration",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_media_playlist, max_duration),
        .sz = sizeof(((kr_hls_media_playlist*)0)->max_duration),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "sequence_num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_media_playlist, sequence_num),
        .sz = sizeof(((kr_hls_media_playlist*)0)->sequence_num),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "nsegments",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_media_playlist, nsegments),
        .sz = sizeof(((kr_hls_media_playlist*)0)->nsegments),
        .opt = 0,
        .type = 8,
        .type_info = {
          .uint32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "segment",
        .arr = KR_HLS_MAX_SEGMENTS,
        .arr_notnull = 0,
        .len_def = "KR_HLS_MAX_SEGMENTS",
        .ptr = 0,
        .off = offsetof(kr_hls_media_playlist, segment),
        .sz = sizeof(((kr_hls_media_playlist*)0)->segment),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_hls_media_segment",
            .idx = KR_KR_HLS_MEDIA_SEGMENT_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_hls_media_playlist),
    .is_typedef = 1
  },
  [KR_KR_HLS_PLAYLIST_META_INFO] = {
    .type = 2,
    .name = "kr_hls_playlist",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_playlist, type),
        .sz = sizeof(((kr_hls_playlist*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_hls_playlist_type",
            .enum_init = "",
            .idx = KR_KR_HLS_PLAYLIST_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 230
          }
        }
      }
    },
    .sz = sizeof(kr_hls_playlist),
    .is_typedef = 1
  },
  [KR_KR_HLS_PLAYLIST_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_hls_playlist_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "master",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_playlist, master),
        .sz = sizeof(((kr_hls_playlist*)0)->master),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_hls_master_playlist",
            .idx = KR_KR_HLS_MASTER_PLAYLIST_META_INFO
          }
        }
      },
      {
        .name = "media",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_hls_playlist, media),
        .sz = sizeof(((kr_hls_playlist*)0)->media),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_hls_media_playlist",
            .idx = KR_KR_HLS_MEDIA_PLAYLIST_META_INFO
          }
        }
      }
    },
    .root_idx = 229,
    .is_typedef = 0
  },
  [KR_KR_HLS_TUNER_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_hls_tuner_info",
    .member_count = 0,
    .members = {
    },
    .sz = sizeof(kr_hls_tuner_info),
    .is_typedef = 1
  },
  [KR_KR_FPS_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_fps_info",
    .member_count = 2,
    .members = {
      {
        .name = "num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_fps_info, num),
        .sz = sizeof(((kr_fps_info*)0)->num),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 30,
            .min = 1,
            .max = 2400
          }
        }
      },
      {
        .name = "den",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_fps_info, den),
        .sz = sizeof(((kr_fps_info*)0)->den),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 1,
            .min = 1,
            .max = 2400
          }
        }
      }
    },
    .sz = sizeof(kr_fps_info),
    .is_typedef = 1
  },
  [KR_KR_RADIO_MODE_META_INFO] = {
    .type = 1,
    .name = "kr_radio_mode",
    .member_count = 2,
    .members = {
      {
        .name = "KR_RADIO_MODE_AUDIO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_RADIO_MODE_AUDIOVIDEO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_radio_mode),
    .is_typedef = 1
  },
  [KR_KR_RADIO_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_radio_info",
    .member_count = 6,
    .members = {
      {
        .name = "callsign",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_radio_info, callsign),
        .sz = sizeof(((kr_radio_info*)0)->callsign),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 15,
            .notnull = 0
          }
        }
      },
      {
        .name = "name",
        .arr = 128,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_radio_info, name),
        .sz = sizeof(((kr_radio_info*)0)->name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 127,
            .notnull = 0
          }
        }
      },
      {
        .name = "started_on",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_radio_info, started_on),
        .sz = sizeof(((kr_radio_info*)0)->started_on),
        .opt = 0,
        .type = 5,
        .type_info = {
          .int64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "mode",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_radio_info, mode),
        .sz = sizeof(((kr_radio_info*)0)->mode),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_radio_mode",
            .enum_init = "",
            .idx = KR_KR_RADIO_MODE_META_INFO
          }
        }
      },
      {
        .name = "version",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_radio_info, version),
        .sz = sizeof(((kr_radio_info*)0)->version),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "commit",
        .arr = 48,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_radio_info, commit),
        .sz = sizeof(((kr_radio_info*)0)->commit),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 47,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_radio_info),
    .is_typedef = 1
  },
  [KR_KR_WEB_CLIENT_PROTOCOL_META_INFO] = {
    .type = 1,
    .name = "kr_web_client_protocol",
    .member_count = 3,
    .members = {
      {
        .name = "KR_PROTOCOL_HTTP",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_PROTOCOL_WEBSOCKET",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_PROTOCOL_TCP",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_web_client_protocol),
    .is_typedef = 1
  },
  [KR_KR_WEB_CLIENT_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_web_client_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_WEB_CLIENT_ACCEPT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WEB_CLIENT_CONNECT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_web_client_type),
    .is_typedef = 1
  },
  [KR_KR_WEB_CLIENT_STATE_META_INFO] = {
    .type = 1,
    .name = "kr_web_client_state",
    .member_count = 4,
    .members = {
      {
        .name = "KR_WEB_RESOLVING",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WEB_CONNECTING",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WEB_ACTIVE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WEB_FLUSHING",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_web_client_state),
    .is_typedef = 1
  },
  [KR_KR_WEB_SERVER_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_web_server_type",
    .member_count = 3,
    .members = {
      {
        .name = "KR_WEB_SERVER_HTTP",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WEB_SERVER_WEBSOCKET",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WEB_SERVER_HTTP_WEBSOCKET",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_web_server_type),
    .is_typedef = 1
  },
  [KR_KR_MULTIPART_STATE_META_INFO] = {
    .type = 1,
    .name = "kr_multipart_state",
    .member_count = 4,
    .members = {
      {
        .name = "KR_MULTIPART_DONE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_MULTIPART_INIT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_MULTIPART_INFO",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_MULTIPART_DATA",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_multipart_state),
    .is_typedef = 1
  },
  [KR_KR_WEB_SERVER_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_web_server_info",
    .member_count = 6,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_server_info, type),
        .sz = sizeof(((kr_web_server_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_web_server_type",
            .enum_init = "",
            .idx = KR_KR_WEB_SERVER_TYPE_META_INFO
          }
        }
      },
      {
        .name = "port",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_server_info, port),
        .sz = sizeof(((kr_web_server_info*)0)->port),
        .opt = 0,
        .type = 7,
        .type_info = {

        }
      },
      {
        .name = "addr",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_server_info, addr),
        .sz = sizeof(((kr_web_server_info*)0)->addr),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "iface",
        .arr = 128,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_server_info, iface),
        .sz = sizeof(((kr_web_server_info*)0)->iface),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 127,
            .notnull = 0
          }
        }
      },
      {
        .name = "conn_total",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_server_info, conn_total),
        .sz = sizeof(((kr_web_server_info*)0)->conn_total),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "created_on",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_server_info, created_on),
        .sz = sizeof(((kr_web_server_info*)0)->created_on),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_web_server_info),
    .is_typedef = 1
  },
  [KR_KR_WEB_SERVER_SETUP_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_web_server_setup_info",
    .member_count = 4,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_server_setup_info, type),
        .sz = sizeof(((kr_web_server_setup_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_web_server_type",
            .enum_init = "",
            .idx = KR_KR_WEB_SERVER_TYPE_META_INFO
          }
        }
      },
      {
        .name = "port",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_server_setup_info, port),
        .sz = sizeof(((kr_web_server_setup_info*)0)->port),
        .opt = 0,
        .type = 7,
        .type_info = {

        }
      },
      {
        .name = "addr",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_server_setup_info, addr),
        .sz = sizeof(((kr_web_server_setup_info*)0)->addr),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "iface",
        .arr = 128,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_server_setup_info, iface),
        .sz = sizeof(((kr_web_server_setup_info*)0)->iface),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 127,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_web_server_setup_info),
    .is_typedef = 1
  },
  [KR_KR_WEB_REMOTE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_web_remote_info",
    .member_count = 4,
    .members = {
      {
        .name = "port",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_remote_info, port),
        .sz = sizeof(((kr_web_remote_info*)0)->port),
        .opt = 0,
        .type = 7,
        .type_info = {

        }
      },
      {
        .name = "addr",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_remote_info, addr),
        .sz = sizeof(((kr_web_remote_info*)0)->addr),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "host",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_remote_info, host),
        .sz = sizeof(((kr_web_remote_info*)0)->host),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "host_len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_remote_info, host_len),
        .sz = sizeof(((kr_web_remote_info*)0)->host_len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_web_remote_info),
    .is_typedef = 1
  },
  [KR_KR_WEB_SOCKET_IO_STATE_META_INFO] = {
    .type = 2,
    .name = "kr_web_socket_io_state",
    .member_count = 3,
    .members = {
      {
        .name = "frame",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_socket_io_state, frame),
        .sz = sizeof(((kr_web_socket_io_state*)0)->frame),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "sz",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_socket_io_state, sz),
        .sz = sizeof(((kr_web_socket_io_state*)0)->sz),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "pos",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_socket_io_state, pos),
        .sz = sizeof(((kr_web_socket_io_state*)0)->pos),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_web_socket_io_state),
    .is_typedef = 1
  },
  [KR_KR_WEBSOCKET_FRAME_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_websocket_frame_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_WEBSOCKET_BINARY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WEBSOCKET_TEXT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_websocket_frame_type),
    .is_typedef = 1
  },
  [KR_KR_WEBSOCKET_PROTOCOL_META_INFO] = {
    .type = 1,
    .name = "kr_websocket_protocol",
    .member_count = 4,
    .members = {
      {
        .name = "KR_WS_PROTOCOL_UNKNOWN",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WS_PROTOCOL_KRAD",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WS_PROTOCOL_STREAM",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WS_PROTOCOL_XMMS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_websocket_protocol),
    .is_typedef = 1
  },
  [KR_KR_WEB_SOCKET_STATE_META_INFO] = {
    .type = 2,
    .name = "kr_web_socket_state",
    .member_count = 4,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_socket_state, type),
        .sz = sizeof(((kr_web_socket_state*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_websocket_frame_type",
            .enum_init = "",
            .idx = KR_KR_WEBSOCKET_FRAME_TYPE_META_INFO
          }
        }
      },
      {
        .name = "proto",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_socket_state, proto),
        .sz = sizeof(((kr_web_socket_state*)0)->proto),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_websocket_protocol",
            .enum_init = "",
            .idx = KR_KR_WEBSOCKET_PROTOCOL_META_INFO
          }
        }
      },
      {
        .name = "input",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_socket_state, input),
        .sz = sizeof(((kr_web_socket_state*)0)->input),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_web_socket_io_state",
            .idx = KR_KR_WEB_SOCKET_IO_STATE_META_INFO
          }
        }
      },
      {
        .name = "output",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_socket_state, output),
        .sz = sizeof(((kr_web_socket_state*)0)->output),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_web_socket_io_state",
            .idx = KR_KR_WEB_SOCKET_IO_STATE_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_web_socket_state),
    .is_typedef = 1
  },
  [KR_KR_HTTP_MESSAGE_STATE_META_INFO] = {
    .type = 1,
    .name = "kr_http_message_state",
    .member_count = 2,
    .members = {
      {
        .name = "KR_HTTP_IN_HEADER",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_HTTP_IN_CONTENT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_http_message_state),
    .is_typedef = 1
  },
  [KR_KR_WEB_HTTP_IO_STATE_META_INFO] = {
    .type = 2,
    .name = "kr_web_http_io_state",
    .member_count = 6,
    .members = {
      {
        .name = "state",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_http_io_state, state),
        .sz = sizeof(((kr_web_http_io_state*)0)->state),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_http_message_state",
            .enum_init = "",
            .idx = KR_KR_HTTP_MESSAGE_STATE_META_INFO
          }
        }
      },
      {
        .name = "sz",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_http_io_state, sz),
        .sz = sizeof(((kr_web_http_io_state*)0)->sz),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "pos",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_http_io_state, pos),
        .sz = sizeof(((kr_web_http_io_state*)0)->pos),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "num",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_http_io_state, num),
        .sz = sizeof(((kr_web_http_io_state*)0)->num),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "chunked",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_http_io_state, chunked),
        .sz = sizeof(((kr_web_http_io_state*)0)->chunked),
        .opt = 0,
        .type = 6,
        .type_info = {

        }
      },
      {
        .name = "multipart",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_http_io_state, multipart),
        .sz = sizeof(((kr_web_http_io_state*)0)->multipart),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_multipart_state",
            .enum_init = "",
            .idx = KR_KR_MULTIPART_STATE_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_web_http_io_state),
    .is_typedef = 1
  },
  [KR_KR_WEB_HTTP_STATE_META_INFO] = {
    .type = 2,
    .name = "kr_web_http_state",
    .member_count = 2,
    .members = {
      {
        .name = "input",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_http_state, input),
        .sz = sizeof(((kr_web_http_state*)0)->input),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_web_http_io_state",
            .idx = KR_KR_WEB_HTTP_IO_STATE_META_INFO
          }
        }
      },
      {
        .name = "output",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_http_state, output),
        .sz = sizeof(((kr_web_http_state*)0)->output),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_web_http_io_state",
            .idx = KR_KR_WEB_HTTP_IO_STATE_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_web_http_state),
    .is_typedef = 1
  },
  [KR_KR_WEB_CLIENT_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_web_client_info",
    .member_count = 9,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_client_info, type),
        .sz = sizeof(((kr_web_client_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_web_client_type",
            .enum_init = "",
            .idx = KR_KR_WEB_CLIENT_TYPE_META_INFO
          }
        }
      },
      {
        .name = "state",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_client_info, state),
        .sz = sizeof(((kr_web_client_info*)0)->state),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_web_client_state",
            .enum_init = "",
            .idx = KR_KR_WEB_CLIENT_STATE_META_INFO
          }
        }
      },
      {
        .name = "protocol",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_client_info, protocol),
        .sz = sizeof(((kr_web_client_info*)0)->protocol),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_web_client_protocol",
            .enum_init = "",
            .idx = KR_KR_WEB_CLIENT_PROTOCOL_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 251
          }
        }
      },
      {
        .name = "created_on",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_client_info, created_on),
        .sz = sizeof(((kr_web_client_info*)0)->created_on),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "bytes_in",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_client_info, bytes_in),
        .sz = sizeof(((kr_web_client_info*)0)->bytes_in),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "bytes_out",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_client_info, bytes_out),
        .sz = sizeof(((kr_web_client_info*)0)->bytes_out),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "port",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_client_info, port),
        .sz = sizeof(((kr_web_client_info*)0)->port),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 65535
          }
        }
      },
      {
        .name = "remote",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_client_info, remote),
        .sz = sizeof(((kr_web_client_info*)0)->remote),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_web_remote_info",
            .idx = KR_KR_WEB_REMOTE_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_web_client_info),
    .is_typedef = 1
  },
  [KR_KR_WEB_CLIENT_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_web_client_info_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "http",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_client_info, http),
        .sz = sizeof(((kr_web_client_info*)0)->http),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_web_http_state",
            .idx = KR_KR_WEB_HTTP_STATE_META_INFO
          }
        }
      },
      {
        .name = "ws",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_client_info, ws),
        .sz = sizeof(((kr_web_client_info*)0)->ws),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_web_socket_state",
            .idx = KR_KR_WEB_SOCKET_STATE_META_INFO
          }
        }
      }
    },
    .root_idx = 250,
    .is_typedef = 0
  },
  [KR_KR_WEB_CLIENT_SETUP_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_web_client_setup_info",
    .member_count = 2,
    .members = {
      {
        .name = "protocol",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_client_setup_info, protocol),
        .sz = sizeof(((kr_web_client_setup_info*)0)->protocol),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_web_client_protocol",
            .enum_init = "",
            .idx = KR_KR_WEB_CLIENT_PROTOCOL_META_INFO
          }
        }
      },
      {
        .name = "remote",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_client_setup_info, remote),
        .sz = sizeof(((kr_web_client_setup_info*)0)->remote),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_web_remote_info",
            .idx = KR_KR_WEB_REMOTE_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_web_client_setup_info),
    .is_typedef = 1
  },
  [KR_KR_WEB_PATH_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_web_path_type",
    .member_count = 3,
    .members = {
      {
        .name = "KR_WEB_SERVER",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WEB_CLIENT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WEB_PROXY",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_web_path_type),
    .is_typedef = 1
  },
  [KR_KR_WEB_PATH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_web_path_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_path_info, type),
        .sz = sizeof(((kr_web_path_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_web_path_type",
            .enum_init = "",
            .idx = KR_KR_WEB_PATH_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 255
          }
        }
      }
    },
    .sz = sizeof(kr_web_path_info),
    .is_typedef = 1
  },
  [KR_KR_WEB_PATH_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_web_path_info_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "server",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_path_info, server),
        .sz = sizeof(((kr_web_path_info*)0)->server),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_web_server_info",
            .idx = KR_KR_WEB_SERVER_INFO_META_INFO
          }
        }
      },
      {
        .name = "client",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_path_info, client),
        .sz = sizeof(((kr_web_path_info*)0)->client),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_web_client_info",
            .idx = KR_KR_WEB_CLIENT_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 254,
    .is_typedef = 0
  },
  [KR_KR_WEB_PATH_SETUP_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_web_path_setup_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_path_setup_info, type),
        .sz = sizeof(((kr_web_path_setup_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_web_path_type",
            .enum_init = "",
            .idx = KR_KR_WEB_PATH_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 257
          }
        }
      }
    },
    .sz = sizeof(kr_web_path_setup_info),
    .is_typedef = 1
  },
  [KR_KR_WEB_PATH_SETUP_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_web_path_setup_info_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "server",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_path_setup_info, server),
        .sz = sizeof(((kr_web_path_setup_info*)0)->server),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_web_server_setup_info",
            .idx = KR_KR_WEB_SERVER_SETUP_INFO_META_INFO
          }
        }
      },
      {
        .name = "client",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_web_path_setup_info, client),
        .sz = sizeof(((kr_web_path_setup_info*)0)->client),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_web_client_setup_info",
            .idx = KR_KR_WEB_CLIENT_SETUP_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 256,
    .is_typedef = 0
  },
  [KR_KR_PATH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_path_info",
    .member_count = 1,
    .members = {
      {
        .name = "name",
        .arr = 64,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_path_info, name),
        .sz = sizeof(((kr_path_info*)0)->name),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 63,
            .notnull = 0
          }
        }
      }
    },
    .sz = sizeof(kr_path_info),
    .is_typedef = 1
  },
  [KR_KR_PATH_LIST_META_INFO] = {
    .type = 2,
    .name = "kr_path_list",
    .member_count = 2,
    .members = {
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_path_list, len),
        .sz = sizeof(((kr_path_list*)0)->len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "path",
        .arr = 64,
        .arr_notnull = 1,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_path_list, path),
        .sz = sizeof(((kr_path_list*)0)->path),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_path_info",
            .idx = KR_KR_PATH_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_path_list),
    .is_typedef = 1
  },
  [KR_KR_UPLOAD_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_upload_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_UPLOAD_FILE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_UPLOAD_STASH",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_upload_type),
    .is_typedef = 1
  },
  [KR_KR_DOWNLOAD_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_download_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_DOWNLOAD_FILE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_DOWNLOAD_STASH",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_download_type),
    .is_typedef = 1
  },
  [KR_KR_UPLOAD_FILE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_upload_file_info",
    .member_count = 4,
    .members = {
      {
        .name = "url",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_upload_file_info, url),
        .sz = sizeof(((kr_upload_file_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      },
      {
        .name = "url_len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_upload_file_info, url_len),
        .sz = sizeof(((kr_upload_file_info*)0)->url_len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "path",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_upload_file_info, path),
        .sz = sizeof(((kr_upload_file_info*)0)->path),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      },
      {
        .name = "path_len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_upload_file_info, path_len),
        .sz = sizeof(((kr_upload_file_info*)0)->path_len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_upload_file_info),
    .is_typedef = 1
  },
  [KR_KR_UPLOAD_STASH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_upload_stash_info",
    .member_count = 4,
    .members = {
      {
        .name = "url",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_upload_stash_info, url),
        .sz = sizeof(((kr_upload_stash_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      },
      {
        .name = "url_len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_upload_stash_info, url_len),
        .sz = sizeof(((kr_upload_stash_info*)0)->url_len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "id",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_upload_stash_info, id),
        .sz = sizeof(((kr_upload_stash_info*)0)->id),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 15,
            .notnull = 0
          }
        }
      },
      {
        .name = "id_len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_upload_stash_info, id_len),
        .sz = sizeof(((kr_upload_stash_info*)0)->id_len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_upload_stash_info),
    .is_typedef = 1
  },
  [KR_KR_DOWNLOAD_FILE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_download_file_info",
    .member_count = 4,
    .members = {
      {
        .name = "url",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_download_file_info, url),
        .sz = sizeof(((kr_download_file_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      },
      {
        .name = "url_len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_download_file_info, url_len),
        .sz = sizeof(((kr_download_file_info*)0)->url_len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "path",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_download_file_info, path),
        .sz = sizeof(((kr_download_file_info*)0)->path),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      },
      {
        .name = "path_len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_download_file_info, path_len),
        .sz = sizeof(((kr_download_file_info*)0)->path_len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_download_file_info),
    .is_typedef = 1
  },
  [KR_KR_DOWNLOAD_STASH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_download_stash_info",
    .member_count = 4,
    .members = {
      {
        .name = "url",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_download_stash_info, url),
        .sz = sizeof(((kr_download_stash_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      },
      {
        .name = "url_len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_download_stash_info, url_len),
        .sz = sizeof(((kr_download_stash_info*)0)->url_len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "id",
        .arr = 16,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_download_stash_info, id),
        .sz = sizeof(((kr_download_stash_info*)0)->id),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 15,
            .notnull = 0
          }
        }
      },
      {
        .name = "id_len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_download_stash_info, id_len),
        .sz = sizeof(((kr_download_stash_info*)0)->id_len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_download_stash_info),
    .is_typedef = 1
  },
  [KR_KR_UPLOAD_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_upload_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_upload_info, type),
        .sz = sizeof(((kr_upload_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_upload_type",
            .enum_init = "",
            .idx = KR_KR_UPLOAD_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 267
          }
        }
      }
    },
    .sz = sizeof(kr_upload_info),
    .is_typedef = 1
  },
  [KR_KR_UPLOAD_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_upload_info_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "file",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_upload_info, file),
        .sz = sizeof(((kr_upload_info*)0)->file),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_upload_file_info",
            .idx = KR_KR_UPLOAD_FILE_INFO_META_INFO
          }
        }
      },
      {
        .name = "stash",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_upload_info, stash),
        .sz = sizeof(((kr_upload_info*)0)->stash),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_upload_stash_info",
            .idx = KR_KR_UPLOAD_STASH_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 266,
    .is_typedef = 0
  },
  [KR_KR_DOWNLOAD_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_download_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_download_info, type),
        .sz = sizeof(((kr_download_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_download_type",
            .enum_init = "",
            .idx = KR_KR_DOWNLOAD_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 269
          }
        }
      }
    },
    .sz = sizeof(kr_download_info),
    .is_typedef = 1
  },
  [KR_KR_DOWNLOAD_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_download_info_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "file",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_download_info, file),
        .sz = sizeof(((kr_download_info*)0)->file),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_download_file_info",
            .idx = KR_KR_DOWNLOAD_FILE_INFO_META_INFO
          }
        }
      },
      {
        .name = "stash",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_download_info, stash),
        .sz = sizeof(((kr_download_info*)0)->stash),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_download_stash_info",
            .idx = KR_KR_DOWNLOAD_STASH_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 268,
    .is_typedef = 0
  },
  [KR_KR_ACCEPT_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_accept_info",
    .member_count = 4,
    .members = {
      {
        .name = "path",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_accept_info, path),
        .sz = sizeof(((kr_accept_info*)0)->path),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "path_len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_accept_info, path_len),
        .sz = sizeof(((kr_accept_info*)0)->path_len),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "pos",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_accept_info, pos),
        .sz = sizeof(((kr_accept_info*)0)->pos),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_accept_info, len),
        .sz = sizeof(((kr_accept_info*)0)->len),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_accept_info),
    .is_typedef = 1
  },
  [KR_KR_SERVE_RANGE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_serve_range_info",
    .member_count = 2,
    .members = {
      {
        .name = "start",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_range_info, start),
        .sz = sizeof(((kr_serve_range_info*)0)->start),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "end",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_range_info, end),
        .sz = sizeof(((kr_serve_range_info*)0)->end),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_serve_range_info),
    .is_typedef = 1
  },
  [KR_KR_SERVE_RANGES_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_serve_ranges_info",
    .member_count = 5,
    .members = {
      {
        .name = "pos",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_ranges_info, pos),
        .sz = sizeof(((kr_serve_ranges_info*)0)->pos),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_ranges_info, len),
        .sz = sizeof(((kr_serve_ranges_info*)0)->len),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "sz",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_ranges_info, sz),
        .sz = sizeof(((kr_serve_ranges_info*)0)->sz),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "nranges",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_ranges_info, nranges),
        .sz = sizeof(((kr_serve_ranges_info*)0)->nranges),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "range",
        .arr = KR_XFER_NRANGES,
        .arr_notnull = 0,
        .len_def = "KR_XFER_NRANGES",
        .ptr = 0,
        .off = offsetof(kr_serve_ranges_info, range),
        .sz = sizeof(((kr_serve_ranges_info*)0)->range),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_serve_range_info",
            .idx = KR_KR_SERVE_RANGE_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_serve_ranges_info),
    .is_typedef = 1
  },
  [KR_KR_SERVE_FULL_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_serve_full_info",
    .member_count = 2,
    .members = {
      {
        .name = "pos",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_full_info, pos),
        .sz = sizeof(((kr_serve_full_info*)0)->pos),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_full_info, len),
        .sz = sizeof(((kr_serve_full_info*)0)->len),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_serve_full_info),
    .is_typedef = 1
  },
  [KR_KR_SERVE_INDEX_FORMAT_META_INFO] = {
    .type = 1,
    .name = "kr_serve_index_format",
    .member_count = 2,
    .members = {
      {
        .name = "KR_SERVE_INDEX_JSON",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_SERVE_INDEX_HTML",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_serve_index_format),
    .is_typedef = 1
  },
  [KR_KR_SERVE_INDEX_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_serve_index_info",
    .member_count = 2,
    .members = {
      {
        .name = "cnt",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_index_info, cnt),
        .sz = sizeof(((kr_serve_index_info*)0)->cnt),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "format",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_index_info, format),
        .sz = sizeof(((kr_serve_index_info*)0)->format),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_serve_index_format",
            .enum_init = "",
            .idx = KR_KR_SERVE_INDEX_FORMAT_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_serve_index_info),
    .is_typedef = 1
  },
  [KR_KR_SERVE_REDIRECT_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_serve_redirect_info",
    .member_count = 1,
    .members = {
      {
        .name = "coconut",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_redirect_info, coconut),
        .sz = sizeof(((kr_serve_redirect_info*)0)->coconut),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_serve_redirect_info),
    .is_typedef = 1
  },
  [KR_KR_SERVE_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_serve_type",
    .member_count = 4,
    .members = {
      {
        .name = "KR_SERVE_FULL",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_SERVE_RANGES",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_SERVE_INDEX",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_SERVE_REDIRECT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_serve_type),
    .is_typedef = 1
  },
  [KR_KR_SERVE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_serve_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_info, type),
        .sz = sizeof(((kr_serve_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_serve_type",
            .enum_init = "",
            .idx = KR_KR_SERVE_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 279
          }
        }
      }
    },
    .sz = sizeof(kr_serve_info),
    .is_typedef = 1
  },
  [KR_KR_SERVE_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_serve_info_sub1",
    .member_count = 4,
    .members = {
      {
        .name = "full",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_info, full),
        .sz = sizeof(((kr_serve_info*)0)->full),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_serve_full_info",
            .idx = KR_KR_SERVE_FULL_INFO_META_INFO
          }
        }
      },
      {
        .name = "ranges",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_info, ranges),
        .sz = sizeof(((kr_serve_info*)0)->ranges),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_serve_ranges_info",
            .idx = KR_KR_SERVE_RANGES_INFO_META_INFO
          }
        }
      },
      {
        .name = "index",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_info, index),
        .sz = sizeof(((kr_serve_info*)0)->index),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_serve_index_info",
            .idx = KR_KR_SERVE_INDEX_INFO_META_INFO
          }
        }
      },
      {
        .name = "redirect",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_serve_info, redirect),
        .sz = sizeof(((kr_serve_info*)0)->redirect),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_serve_redirect_info",
            .idx = KR_KR_SERVE_REDIRECT_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 278,
    .is_typedef = 0
  },
  [KR_KR_DOWNLINK_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_downlink_info",
    .member_count = 2,
    .members = {
      {
        .name = "url",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_downlink_info, url),
        .sz = sizeof(((kr_downlink_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_downlink_info, len),
        .sz = sizeof(((kr_downlink_info*)0)->len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_downlink_info),
    .is_typedef = 1
  },
  [KR_KR_UPLINK_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_uplink_info",
    .member_count = 3,
    .members = {
      {
        .name = "url",
        .arr = 256,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_uplink_info, url),
        .sz = sizeof(((kr_uplink_info*)0)->url),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 255,
            .notnull = 0
          }
        }
      },
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_uplink_info, len),
        .sz = sizeof(((kr_uplink_info*)0)->len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "legacy",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_uplink_info, legacy),
        .sz = sizeof(((kr_uplink_info*)0)->legacy),
        .opt = 1,
        .type = 6,
        .type_info = {

        }
      }
    },
    .sz = sizeof(kr_uplink_info),
    .is_typedef = 1
  },
  [KR_KR_SOURCE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_source_info",
    .member_count = 1,
    .members = {
      {
        .name = "coconut",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_source_info, coconut),
        .sz = sizeof(((kr_source_info*)0)->coconut),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_source_info),
    .is_typedef = 1
  },
  [KR_KR_TRANSMIT_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_transmit_info",
    .member_count = 1,
    .members = {
      {
        .name = "packed_header",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_transmit_info, packed_header),
        .sz = sizeof(((kr_transmit_info*)0)->packed_header),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_transmit_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_CLIENT_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_xfer_client_type",
    .member_count = 8,
    .members = {
      {
        .name = "KR_DOWNLOAD",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_UPLOAD",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_ACCEPT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_SERVE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_DOWNLINK",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_UPLINK",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_SOURCE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_TRANSMIT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_client_type),
    .is_typedef = 1
  },
  [KR_KR_XFER_RECONNECT_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xfer_reconnect_info",
    .member_count = 2,
    .members = {
      {
        .name = "max_tries",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_reconnect_info, max_tries),
        .sz = sizeof(((kr_xfer_reconnect_info*)0)->max_tries),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "ms",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_reconnect_info, ms),
        .sz = sizeof(((kr_xfer_reconnect_info*)0)->ms),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_reconnect_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_CLIENT_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xfer_client_info",
    .member_count = 7,
    .members = {
      {
        .name = "rx",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_client_info, rx),
        .sz = sizeof(((kr_xfer_client_info*)0)->rx),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "tx",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_client_info, tx),
        .sz = sizeof(((kr_xfer_client_info*)0)->tx),
        .opt = 0,
        .type = 9,
        .type_info = {
          .uint64_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_client_info, type),
        .sz = sizeof(((kr_xfer_client_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_xfer_client_type",
            .enum_init = "",
            .idx = KR_KR_XFER_CLIENT_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 287
          }
        }
      },
      {
        .name = "path",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_client_info, path),
        .sz = sizeof(((kr_xfer_client_info*)0)->path),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      },
      {
        .name = "client",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_client_info, client),
        .sz = sizeof(((kr_xfer_client_info*)0)->client),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_web_client_info",
            .idx = KR_KR_WEB_CLIENT_INFO_META_INFO
          }
        }
      },
      {
        .name = "reconnect",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_client_info, reconnect),
        .sz = sizeof(((kr_xfer_client_info*)0)->reconnect),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xfer_reconnect_info",
            .idx = KR_KR_XFER_RECONNECT_INFO_META_INFO
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_client_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_CLIENT_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_xfer_client_info_sub1",
    .member_count = 8,
    .members = {
      {
        .name = "download",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_client_info, download),
        .sz = sizeof(((kr_xfer_client_info*)0)->download),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_download_info",
            .idx = KR_KR_DOWNLOAD_INFO_META_INFO
          }
        }
      },
      {
        .name = "upload",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_client_info, upload),
        .sz = sizeof(((kr_xfer_client_info*)0)->upload),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_upload_info",
            .idx = KR_KR_UPLOAD_INFO_META_INFO
          }
        }
      },
      {
        .name = "accept",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_client_info, accept),
        .sz = sizeof(((kr_xfer_client_info*)0)->accept),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_accept_info",
            .idx = KR_KR_ACCEPT_INFO_META_INFO
          }
        }
      },
      {
        .name = "serve",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_client_info, serve),
        .sz = sizeof(((kr_xfer_client_info*)0)->serve),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_serve_info",
            .idx = KR_KR_SERVE_INFO_META_INFO
          }
        }
      },
      {
        .name = "downlink",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_client_info, downlink),
        .sz = sizeof(((kr_xfer_client_info*)0)->downlink),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_downlink_info",
            .idx = KR_KR_DOWNLINK_INFO_META_INFO
          }
        }
      },
      {
        .name = "uplink",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_client_info, uplink),
        .sz = sizeof(((kr_xfer_client_info*)0)->uplink),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_uplink_info",
            .idx = KR_KR_UPLINK_INFO_META_INFO
          }
        }
      },
      {
        .name = "source",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_client_info, source),
        .sz = sizeof(((kr_xfer_client_info*)0)->source),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_source_info",
            .idx = KR_KR_SOURCE_INFO_META_INFO
          }
        }
      },
      {
        .name = "transmit",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_client_info, transmit),
        .sz = sizeof(((kr_xfer_client_info*)0)->transmit),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_transmit_info",
            .idx = KR_KR_TRANSMIT_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 286,
    .is_typedef = 0
  },
  [KR_KR_XFER_ACCEPTOR_DIR_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xfer_acceptor_dir_info",
    .member_count = 2,
    .members = {
      {
        .name = "path",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_acceptor_dir_info, path),
        .sz = sizeof(((kr_xfer_acceptor_dir_info*)0)->path),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      },
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_acceptor_dir_info, len),
        .sz = sizeof(((kr_xfer_acceptor_dir_info*)0)->len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_acceptor_dir_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_ACCEPTOR_FILE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xfer_acceptor_file_info",
    .member_count = 1,
    .members = {
      {
        .name = "coconut",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_acceptor_file_info, coconut),
        .sz = sizeof(((kr_xfer_acceptor_file_info*)0)->coconut),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_acceptor_file_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_ACCEPTOR_BUF_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xfer_acceptor_buf_info",
    .member_count = 1,
    .members = {
      {
        .name = "coconut",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_acceptor_buf_info, coconut),
        .sz = sizeof(((kr_xfer_acceptor_buf_info*)0)->coconut),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_acceptor_buf_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_SHARE_DIR_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xfer_share_dir_info",
    .member_count = 2,
    .members = {
      {
        .name = "path",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_share_dir_info, path),
        .sz = sizeof(((kr_xfer_share_dir_info*)0)->path),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      },
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_share_dir_info, len),
        .sz = sizeof(((kr_xfer_share_dir_info*)0)->len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_share_dir_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_SHARE_FILE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xfer_share_file_info",
    .member_count = 1,
    .members = {
      {
        .name = "coconut",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_share_file_info, coconut),
        .sz = sizeof(((kr_xfer_share_file_info*)0)->coconut),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_share_file_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_SHARE_BUF_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xfer_share_buf_info",
    .member_count = 1,
    .members = {
      {
        .name = "coconut",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_share_buf_info, coconut),
        .sz = sizeof(((kr_xfer_share_buf_info*)0)->coconut),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_share_buf_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_ACCEPTOR_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_xfer_acceptor_type",
    .member_count = 3,
    .members = {
      {
        .name = "KR_XFER_ACCEPT_DIR",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_XFER_ACCEPT_FILE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_XFER_ACCEPT_BUF",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_acceptor_type),
    .is_typedef = 1
  },
  [KR_KR_XFER_ACCEPTOR_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xfer_acceptor_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_acceptor_info, type),
        .sz = sizeof(((kr_xfer_acceptor_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_xfer_acceptor_type",
            .enum_init = "",
            .idx = KR_KR_XFER_ACCEPTOR_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 296
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_acceptor_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_ACCEPTOR_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_xfer_acceptor_info_sub1",
    .member_count = 3,
    .members = {
      {
        .name = "dir",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_acceptor_info, dir),
        .sz = sizeof(((kr_xfer_acceptor_info*)0)->dir),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xfer_acceptor_dir_info",
            .idx = KR_KR_XFER_ACCEPTOR_DIR_INFO_META_INFO
          }
        }
      },
      {
        .name = "file",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_acceptor_info, file),
        .sz = sizeof(((kr_xfer_acceptor_info*)0)->file),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xfer_acceptor_file_info",
            .idx = KR_KR_XFER_ACCEPTOR_FILE_INFO_META_INFO
          }
        }
      },
      {
        .name = "buf",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_acceptor_info, buf),
        .sz = sizeof(((kr_xfer_acceptor_info*)0)->buf),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xfer_acceptor_buf_info",
            .idx = KR_KR_XFER_ACCEPTOR_BUF_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 295,
    .is_typedef = 0
  },
  [KR_KR_XFER_RECEIVER_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xfer_receiver_info",
    .member_count = 1,
    .members = {
      {
        .name = "sources",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_receiver_info, sources),
        .sz = sizeof(((kr_xfer_receiver_info*)0)->sources),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_receiver_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_TRANSMITTER_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_xfer_transmitter_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_XFER_TRANSMITTER_HTTP",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_XFER_TRANSMITTER_WS",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_transmitter_type),
    .is_typedef = 1
  },
  [KR_KR_XFER_TRANSMITTER_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xfer_transmitter_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_transmitter_info, type),
        .sz = sizeof(((kr_xfer_transmitter_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_xfer_transmitter_type",
            .enum_init = "",
            .idx = KR_KR_XFER_TRANSMITTER_TYPE_META_INFO
          }
        }
      },
      {
        .name = "clients",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_transmitter_info, clients),
        .sz = sizeof(((kr_xfer_transmitter_info*)0)->clients),
        .opt = 1,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_transmitter_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_SHARE_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_xfer_share_type",
    .member_count = 3,
    .members = {
      {
        .name = "KR_XFER_SHARE_DIR",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_XFER_SHARE_FILE",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_XFER_SHARE_BUF",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_share_type),
    .is_typedef = 1
  },
  [KR_KR_XFER_SHARE_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xfer_share_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_share_info, type),
        .sz = sizeof(((kr_xfer_share_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_xfer_share_type",
            .enum_init = "",
            .idx = KR_KR_XFER_SHARE_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 302
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_share_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_SHARE_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_xfer_share_info_sub1",
    .member_count = 3,
    .members = {
      {
        .name = "dir",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_share_info, dir),
        .sz = sizeof(((kr_xfer_share_info*)0)->dir),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xfer_share_dir_info",
            .idx = KR_KR_XFER_SHARE_DIR_INFO_META_INFO
          }
        }
      },
      {
        .name = "file",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_share_info, file),
        .sz = sizeof(((kr_xfer_share_info*)0)->file),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xfer_share_file_info",
            .idx = KR_KR_XFER_SHARE_FILE_INFO_META_INFO
          }
        }
      },
      {
        .name = "buf",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_share_info, buf),
        .sz = sizeof(((kr_xfer_share_info*)0)->buf),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xfer_share_buf_info",
            .idx = KR_KR_XFER_SHARE_BUF_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 301,
    .is_typedef = 0
  },
  [KR_KR_XFER_SERVER_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_xfer_server_type",
    .member_count = 4,
    .members = {
      {
        .name = "KR_XFER_SHARING",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_XFER_ACCEPTING",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_XFER_TRANSMITTER",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_XFER_RECEIVER",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_server_type),
    .is_typedef = 1
  },
  [KR_KR_XFER_SERVER_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xfer_server_info",
    .member_count = 4,
    .members = {
      {
        .name = "path",
        .arr = 512,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_server_info, path),
        .sz = sizeof(((kr_xfer_server_info*)0)->path),
        .opt = 0,
        .type = 1,
        .type_info = {
          .char_info = {
            .max = 511,
            .notnull = 0
          }
        }
      },
      {
        .name = "len",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_server_info, len),
        .sz = sizeof(((kr_xfer_server_info*)0)->len),
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_server_info, type),
        .sz = sizeof(((kr_xfer_server_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_xfer_server_type",
            .enum_init = "",
            .idx = KR_KR_XFER_SERVER_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 305
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_server_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_SERVER_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_xfer_server_info_sub1",
    .member_count = 4,
    .members = {
      {
        .name = "share",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_server_info, share),
        .sz = sizeof(((kr_xfer_server_info*)0)->share),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xfer_share_info",
            .idx = KR_KR_XFER_SHARE_INFO_META_INFO
          }
        }
      },
      {
        .name = "acceptor",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_server_info, acceptor),
        .sz = sizeof(((kr_xfer_server_info*)0)->acceptor),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xfer_acceptor_info",
            .idx = KR_KR_XFER_ACCEPTOR_INFO_META_INFO
          }
        }
      },
      {
        .name = "transmitter",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_server_info, transmitter),
        .sz = sizeof(((kr_xfer_server_info*)0)->transmitter),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xfer_transmitter_info",
            .idx = KR_KR_XFER_TRANSMITTER_INFO_META_INFO
          }
        }
      },
      {
        .name = "receiver",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_server_info, receiver),
        .sz = sizeof(((kr_xfer_server_info*)0)->receiver),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xfer_receiver_info",
            .idx = KR_KR_XFER_RECEIVER_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 304,
    .is_typedef = 0
  },
  [KR_KR_XFER_PATH_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_xfer_path_type",
    .member_count = 2,
    .members = {
      {
        .name = "KR_XFER_SERVER",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_XFER_CLIENT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_path_type),
    .is_typedef = 1
  },
  [KR_KR_XFER_PATH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xfer_path_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_path_info, type),
        .sz = sizeof(((kr_xfer_path_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_xfer_path_type",
            .enum_init = "",
            .idx = KR_KR_XFER_PATH_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 308
          }
        }
      }
    },
    .sz = sizeof(kr_xfer_path_info),
    .is_typedef = 1
  },
  [KR_KR_XFER_PATH_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_xfer_path_info_sub1",
    .member_count = 2,
    .members = {
      {
        .name = "server",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_path_info, server),
        .sz = sizeof(((kr_xfer_path_info*)0)->server),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xfer_server_info",
            .idx = KR_KR_XFER_SERVER_INFO_META_INFO
          }
        }
      },
      {
        .name = "client",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(kr_xfer_path_info, client),
        .sz = sizeof(((kr_xfer_path_info*)0)->client),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_xfer_client_info",
            .idx = KR_KR_XFER_CLIENT_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 307,
    .is_typedef = 0
  },
  [KR_KR_XPDR_TYPE_META_INFO] = {
    .type = 1,
    .name = "kr_xpdr_type",
    .member_count = 20,
    .members = {
      {
        .name = "KR_JACK",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WAYLAND",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_V4L2",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_DECKLINK",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_X11",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_ALSA",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AUX",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_JACK_IN",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_JACK_OUT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_WAYLAND_OUT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_V4L2_IN",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_DECKLINK_VIDEO_IN",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_DECKLINK_AUDIO_IN",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_X11_IN",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_ALSA_IN",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_ALSA_OUT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AUX_VIDEO_IN",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AUX_VIDEO_OUT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AUX_AUDIO_IN",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      },
      {
        .name = "KR_AUX_AUDIO_OUT",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 4,
        .type_info = {
          .int32_info = {
            .init = 0,
            .min = 0,
            .max = 0
          }
        }
      }
    },
    .sz = sizeof(kr_xpdr_type),
    .is_typedef = 1
  },
  [KR_KR_XPDR_PATH_INFO_META_INFO] = {
    .type = 2,
    .name = "kr_xpdr_path_info",
    .member_count = 2,
    .members = {
      {
        .name = "type",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, type),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->type),
        .opt = 0,
        .type = 14,
        .type_info = {
          .subenum_info = {
            .type_name = "kr_xpdr_type",
            .enum_init = "",
            .idx = KR_KR_XPDR_TYPE_META_INFO
          }
        }
      },
      {
        .name = "",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .opt = 0,
        .type = 16,
        .type_info = {
          .anon_info = {
            .info = NULL,
            .idx = 311
          }
        }
      }
    },
    .sz = sizeof(struct kr_xpdr_path_info),
    .is_typedef = 0
  },
  [KR_KR_XPDR_PATH_INFO_SUB1_META_INFO] = {
    .type = 5,
    .name = "kr_xpdr_path_info_sub1",
    .member_count = 20,
    .members = {
      {
        .name = "jack",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, jack),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->jack),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_jack_info",
            .idx = KR_KR_JACK_INFO_META_INFO
          }
        }
      },
      {
        .name = "wl",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, wl),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->wl),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_wayland_info",
            .idx = KR_KR_WAYLAND_INFO_META_INFO
          }
        }
      },
      {
        .name = "v4l2",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, v4l2),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->v4l2),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_v4l2_info",
            .idx = KR_KR_V4L2_INFO_META_INFO
          }
        }
      },
      {
        .name = "dl",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, dl),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->dl),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_decklink_info",
            .idx = KR_KR_DECKLINK_INFO_META_INFO
          }
        }
      },
      {
        .name = "x11",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, x11),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->x11),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_x11_info",
            .idx = KR_KR_X11_INFO_META_INFO
          }
        }
      },
      {
        .name = "alsa",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, alsa),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->alsa),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_alsa_info",
            .idx = KR_KR_ALSA_INFO_META_INFO
          }
        }
      },
      {
        .name = "aux",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, aux),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->aux),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_aux_info",
            .idx = KR_KR_AUX_INFO_META_INFO
          }
        }
      },
      {
        .name = "jack_in",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, jack_in),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->jack_in),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_jack_path_info",
            .idx = KR_KR_JACK_PATH_INFO_META_INFO
          }
        }
      },
      {
        .name = "jack_out",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, jack_out),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->jack_out),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_jack_path_info",
            .idx = KR_KR_JACK_PATH_INFO_META_INFO
          }
        }
      },
      {
        .name = "wl_out",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, wl_out),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->wl_out),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_wayland_path_info",
            .idx = KR_KR_WAYLAND_PATH_INFO_META_INFO
          }
        }
      },
      {
        .name = "v4l2_in",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, v4l2_in),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->v4l2_in),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_v4l2_path_info",
            .idx = KR_KR_V4L2_PATH_INFO_META_INFO
          }
        }
      },
      {
        .name = "dlv_in",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, dlv_in),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->dlv_in),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_decklink_path_info",
            .idx = KR_KR_DECKLINK_PATH_INFO_META_INFO
          }
        }
      },
      {
        .name = "dla_in",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, dla_in),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->dla_in),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_decklink_path_info",
            .idx = KR_KR_DECKLINK_PATH_INFO_META_INFO
          }
        }
      },
      {
        .name = "x11_in",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, x11_in),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->x11_in),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_x11_path_info",
            .idx = KR_KR_X11_PATH_INFO_META_INFO
          }
        }
      },
      {
        .name = "alsa_in",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, alsa_in),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->alsa_in),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_alsa_path_info",
            .idx = KR_KR_ALSA_PATH_INFO_META_INFO
          }
        }
      },
      {
        .name = "alsa_out",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, alsa_out),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->alsa_out),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_alsa_path_info",
            .idx = KR_KR_ALSA_PATH_INFO_META_INFO
          }
        }
      },
      {
        .name = "aux_video_in",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, aux_video_in),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->aux_video_in),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_aux_path_info",
            .idx = KR_KR_AUX_PATH_INFO_META_INFO
          }
        }
      },
      {
        .name = "aux_video_out",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, aux_video_out),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->aux_video_out),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_aux_path_info",
            .idx = KR_KR_AUX_PATH_INFO_META_INFO
          }
        }
      },
      {
        .name = "aux_audio_in",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, aux_audio_in),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->aux_audio_in),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_aux_path_info",
            .idx = KR_KR_AUX_PATH_INFO_META_INFO
          }
        }
      },
      {
        .name = "aux_audio_out",
        .arr = 0,
        .arr_notnull = 0,
        .len_def = "",
        .ptr = 0,
        .off = offsetof(struct kr_xpdr_path_info, aux_audio_out),
        .sz = sizeof(((struct kr_xpdr_path_info*)0)->aux_audio_out),
        .opt = 0,
        .type = 12,
        .type_info = {
          .substruct_info = {
            .type_name = "kr_aux_path_info",
            .idx = KR_KR_AUX_PATH_INFO_META_INFO
          }
        }
      }
    },
    .root_idx = 310,
    .is_typedef = 0
  },
};

