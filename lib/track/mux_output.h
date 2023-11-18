typedef struct {
  void *user;
  uint8_t *mem;
  int64_t tc;
  int backbuf;
} kr_mux_output_setup;

typedef struct {
  void *user;
  kr_mux *mux;
  kr_track_head *head;
  int header_sent;
  int pos;
  kr_track_block block;
} kr_mux_output;

ssize_t kr_mux_read(kr_mux_output *out, uint8_t *buf, size_t sz);
ssize_t kr_mux_read_n(kr_mux_output *out, uint8_t *buf, size_t sz, int n);
int kr_mux_detach(kr_mux_output *out);
kr_mux_output *kr_mux_attach(kr_mux *m, kr_mux_output_setup *s);
size_t kr_mux_output_sizeof();
