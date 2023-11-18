#if !defined(_adapter_hw_alsa_types_H)
# define _adapter_hw_alsa_types_H (1)

typedef struct {
  int card;
  char name[40];
} kr_alsa_info;

typedef struct {
  int card_num;
} kr_alsa_path_info;

#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_helpers.h"

#endif
