#if !defined(_container_types_H)
# define _container_types_H (1)

typedef enum {
  KR_CONTAINER_RAW = 1,
  KR_CONTAINER_MKV,
  KR_CONTAINER_WEBM,
  KR_CONTAINER_OGG,
  KR_CONTAINER_FLAC,
  KR_CONTAINER_Y4M,
  KR_CONTAINER_TXT,
  KR_CONTAINER_TS,
  KR_CONTAINER_OCC
} kr_container_type;

#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"

#endif
