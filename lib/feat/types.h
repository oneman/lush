#if !defined(_feat_types_H)
# define _feat_types_H (1)

#define KR_FEATURE_NASPECTS 1

typedef struct kr_feature kr_feature;

typedef enum {
  KR_FEATURE_COMMENTS,
  KR_FEATURE_TAG,
  KR_FEATURE_HEADER,
  KR_FEATURE_FUNCTION,
  KR_FEATURE_STORY
} kr_feature_aspect_type;

typedef struct {
  char id[42];
  kr_feature_aspect_type type;
  char value[2048];
} kr_feature_aspect;

struct kr_feature {
  char id[42];
  char name[256];
  kr_feature *ref[128]; /* ignore */
  int ref_count; /* ignore */
  kr_feature_aspect aspects[KR_FEATURE_NASPECTS];
  int aspect_count; /* ignore */
};

#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"

#endif
