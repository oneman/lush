#if !defined(_ops_media_types_H)
# define _ops_media_types_H (1)

#define KR_MEDIA_INFO_NITEMS 4

#include <krad/time/types.h>

typedef enum {
  KR_MEDIA_CREATE = 1,
  KR_MEDIA_ITEM_ADD,
  KR_MEDIA_FILE_CREATE,
  KR_MEDIA_DESTROY,
} kr_media_event_type;

typedef enum {
  KR_MEDIA_URL = 1,
  KR_MEDIA_METADATA,
  KR_MEDIA_FILE
} kr_media_item_type;

typedef struct {
  char name[512];
} kr_media_file_info;

typedef struct {
  char name[512];
  double duration;
  int width;
  int height;
} kr_media_metadata_info;

typedef struct {
  char url[1024];
} kr_media_url_info;

typedef struct {
  kr_timestamp_info create_time;
  char task[40]; /* this is the task that created the item */
  kr_media_item_type type;
  union {
    kr_media_url_info url;
    kr_media_metadata_info metadata;
    kr_media_file_info file;
  };
} kr_media_item_info;

typedef struct {
  char id[40];
  int nitems;
  kr_timestamp_info create_time;
  kr_media_item_info item[KR_MEDIA_INFO_NITEMS];
  char media_dir[512];
} kr_media_info;

#include "gen/types_to_text.h"
#include "gen/types_helpers.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"

#endif
