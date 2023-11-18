#if !defined(_tasks_types_H)
# define _tasks_types_H (1)

#include <krad/tasks/valgrind_stressor/types.h>
#include <krad/tasks/quvi/types.h>
#include <krad/tasks/avconv/types.h>
#include <krad/tasks/xmms/types.h>
#include <krad/tasks/curl/types.h>
#include <krad/tasks/transmission_cli/types.h>
#include <krad/tasks/livestreamer/types.h>
#include <krad/tasks/avprobe/types.h>
#include <krad/tasks/ydl/types.h>

typedef enum {
  KR_VALGRIND_STRESSOR = 1,
  KR_QUVI,
  KR_AVCONV,
  KR_XMMS,
  KR_CURL,
  KR_TRANSMISSION_CLI,
  KR_LIVESTREAMER,
  KR_AVPROBE,
  KR_YDL
} kr_task_type;

typedef enum {
  KR_TASK_READY = 1,
  KR_TASK_RUNNING,
  KR_TASK_FAILED,
  KR_TASK_TIMEDOUT,
  KR_TASK_SUCCEEDED,
  KR_TASK_CANCELLED,
  KR_TASK_SAME_STATE
} kr_task_state;

typedef enum {
  KR_TASK_CREATE = 1,
  KR_TASK_START,
  KR_TASK_PATCH,
  KR_TASK_PROGRESS,
  KR_TASK_FAILURE,
  KR_TASK_SUCCESS,
  KR_TASK_CANCEL,
  KR_TASK_TIMEOUT,
  KR_TASK_DESTROY,
} kr_task_event_type;

static const kr_task_state transitions[] = {
  [KR_TASK_CREATE] = KR_TASK_READY,
  [KR_TASK_START] = KR_TASK_RUNNING,
  [KR_TASK_PATCH] = KR_TASK_SAME_STATE,
  [KR_TASK_PROGRESS] = KR_TASK_RUNNING,
  [KR_TASK_FAILURE] = KR_TASK_FAILED,
  [KR_TASK_TIMEOUT] = KR_TASK_TIMEDOUT,
  [KR_TASK_CANCEL] = KR_TASK_CANCELLED,
  [KR_TASK_SUCCESS] = KR_TASK_SUCCEEDED,
  [KR_TASK_DESTROY] = 0
};

static const int task_internal_events[] = {
  [KR_TASK_CREATE] = 0,
  [KR_TASK_START] = 0,
  [KR_TASK_PATCH] = 0,
  [KR_TASK_PROGRESS] = 1,
  [KR_TASK_FAILURE] = 1,
  [KR_TASK_TIMEOUT] = 0,
  [KR_TASK_CANCEL] = 0,
  [KR_TASK_SUCCESS] = 1,
  [KR_TASK_DESTROY] = 0
};

static const int startable_states[] = {
  [KR_TASK_READY] = 1,
  [KR_TASK_RUNNING] = 0,
  [KR_TASK_FAILED] = 1,
  [KR_TASK_TIMEDOUT] = 1,
  [KR_TASK_CANCELLED] = 1,
  [KR_TASK_SUCCEEDED] = 0,
};

static const int stoppable_states[] = {
  [KR_TASK_READY] = 0,
  [KR_TASK_RUNNING] = 1,
  [KR_TASK_FAILED] = 0,
  [KR_TASK_TIMEDOUT] = 0,
  [KR_TASK_CANCELLED] = 0,
  [KR_TASK_SUCCEEDED] = 0,
};

static const int patchable_states[] = {
  [KR_TASK_READY] = 1,
  [KR_TASK_RUNNING] = 1,
  [KR_TASK_FAILED] = 0,
  [KR_TASK_TIMEDOUT] = 1,
  [KR_TASK_CANCELLED] = 1,
  [KR_TASK_SUCCEEDED] = 0,
};

static const int stop_events[] = {
  [KR_TASK_CREATE] = 0,
  [KR_TASK_START] = 0,
  [KR_TASK_PATCH] = 0,
  [KR_TASK_PROGRESS] = 0,
  [KR_TASK_FAILURE] = 1,
  [KR_TASK_TIMEOUT] = 1,
  [KR_TASK_CANCEL] = 1,
  [KR_TASK_SUCCESS] = 1,
  [KR_TASK_DESTROY] = 0
};

typedef struct {
  char reason[32];
  char details[256];
  char subfault[32];
  int should_retry;
  int attempt;
} kr_fail_info;

typedef struct {
  kr_task_state state;
  kr_timestamp_info create_time;
  kr_timestamp_info start_time;
  kr_timestamp_info stop_time;
  kr_fail_info fail;
} kr_task_status;

typedef struct {
  kr_task_type type; /* YDL */
  union {
    kr_valgrind_stressor_params_info valgrind_stressor;
    kr_quvi_params_info quvi;
    kr_avconv_params_info avconv;
    kr_xmms_params_info xmms;
    kr_curl_params_info curl;
    kr_transmission_cli_params_info transmission_cli;
    kr_livestreamer_params_info livestreamer;
    kr_avprobe_params_info avprobe;
    kr_ydl_params_info ydl;
  };
} kr_task_params_info;

typedef struct {
  char id[40];
  kr_task_status status;
  kr_task_params_info params;
  kr_task_type type; /* YDL */
  union {
    kr_valgrind_stressor_info valgrind_stressor;
    kr_quvi_info quvi;
    kr_avconv_info avconv;
    kr_xmms_info xmms;
    kr_curl_info curl;
    kr_transmission_cli_info transmission_cli;
    kr_livestreamer_info livestreamer;
    kr_avprobe_info avprobe;
    kr_ydl_info ydl;
  };
} kr_task_info;

#include "gen/types_to_text.h"
#include "gen/types_helpers.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"

#endif
