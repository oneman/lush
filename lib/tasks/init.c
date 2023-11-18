#include <krad/tasks/internal.h>

static const kr_task_type kr_task_types[] = {
  [KR_VALGRIND_STRESSOR] = KR_VALGRIND_STRESSOR,
  [KR_QUVI] = KR_QUVI,
  [KR_AVCONV] = KR_AVCONV,
  [KR_XMMS] = KR_XMMS,
  [KR_CURL] = KR_CURL,
  [KR_TRANSMISSION_CLI] = KR_TRANSMISSION_CLI,
  [KR_LIVESTREAMER] = KR_LIVESTREAMER,
  [KR_AVPROBE] = KR_AVPROBE,
  [KR_YDL] = KR_YDL
};

static const kr_task_spec media_tasks[] = {
  [KR_VALGRIND_STRESSOR] = {
    .create = kr_valgrind_stressor_create,
    .ctl = kr_valgrind_stressor_ctl,
    .start = kr_valgrind_stressor_start,
    .stop = kr_valgrind_stressor_stop,
    .destroy = kr_valgrind_stressor_destroy,
  },
  [KR_QUVI] = {
    .create = kr_quvi_create,
    .ctl = kr_quvi_ctl,
    .start = kr_quvi_start,
    .stop = kr_quvi_stop,
    .destroy = kr_quvi_destroy,
  },
  [KR_AVCONV] = {
    .create = kr_avconv_create,
    .ctl = kr_avconv_ctl,
    .start = kr_avconv_start,
    .stop = kr_avconv_stop,
    .destroy = kr_avconv_destroy,
  },
  [KR_XMMS] = {
    .create = kr_xmms_create,
    .ctl = kr_xmms_ctl,
    .start = kr_xmms_start,
    .stop = kr_xmms_stop,
    .destroy = kr_xmms_destroy,
  },
  [KR_CURL] = {
    .create = kr_curl_create,
    .ctl = kr_curl_ctl,
    .start = kr_curl_start,
    .stop = kr_curl_stop,
    .destroy = kr_curl_destroy,
  },
  [KR_TRANSMISSION_CLI] = {
    .create = kr_transmission_cli_create,
    .ctl = kr_transmission_cli_ctl,
    .start = kr_transmission_cli_start,
    .stop = kr_transmission_cli_stop,
    .destroy = kr_transmission_cli_destroy,
  },
  [KR_LIVESTREAMER] = {
    .create = kr_livestreamer_create,
    .ctl = kr_livestreamer_ctl,
    .start = kr_livestreamer_start,
    .stop = kr_livestreamer_stop,
    .destroy = kr_livestreamer_destroy,
  },
  [KR_AVPROBE] = {
    .create = kr_avprobe_create,
    .ctl = kr_avprobe_ctl,
    .start = kr_avprobe_start,
    .stop = kr_avprobe_stop,
    .destroy = kr_avprobe_destroy,
  },
  [KR_YDL] = {
    .create = kr_ydl_create,
    .ctl = kr_ydl_ctl,
    .start = kr_ydl_start,
    .stop = kr_ydl_stop,
    .destroy = kr_ydl_destroy,
  }
};

static const int nmt_types = (sizeof(media_tasks) / sizeof(media_tasks[0]));
