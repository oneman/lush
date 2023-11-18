#include "types.h"

#if !defined(_track_output_H)
# define _track_output_H (1)

typedef struct {
  kr_studio_path *mux_path;
  union {
    kr_xfer_path *xfer;
    kr_file2 *file;
  };
  size_t file_pos;
  kr_mux *mux;
  kr_mux_output *mux_output;
  kr_amem(mem, 512)
} studio_output;

int studio_output_write(kr_studio_path *path);
int studio_output_of_mux(kr_studio_path *path, kr_studio_path *mux);
int studio_output_destroy(kr_studio_path *path);
int studio_output_create(kr_studio_path *path, kr_studio_path *mux);

#endif
