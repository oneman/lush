#include "types.h"

#if !defined(_track_source_H)
# define _track_source_H (1)

typedef enum {
  SOURCE_INIT = 1,
  SOURCE_PROBING,
  SOURCE_DEMUXING
} studio_source_state;

typedef struct {
  union {
    kr_xfer_path *xfer;
    kr_file2 *file;
  };
  kr_studio_path *demuxer;
  uint8_t buffer[8192 * 8];
  size_t len;
  studio_source_state state;
} studio_source;

int source_file_decode(kr_studio_path *path);
int studio_source_destroy(kr_studio_path *path);
int studio_source_create(kr_studio_path *path);

#endif
