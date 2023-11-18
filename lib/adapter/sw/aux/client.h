#if !defined(_adapter_sw_aux_client_H)
# define _adapter_sw_aux_client_H (1)

#include <krad/app/debug.h>
#include <krad/video/frame.h>

#include "types.h"

typedef struct kr_aux kr_aux;

typedef struct {
  char adapter[26];
  char port[26];
  int type;
} kr_aux_params;

int kr_audio_out(kr_radio_client *client, char *aux_name, char *port_name);
int kr_audio_in(kr_radio_client *client, char *aux_name, char *port_name);
int kr_video_out(kr_radio_client *client, char *aux_name, char *port_name);
int kr_video_in(kr_radio_client *client, char *aux_name, char *port_name);
int kr_aux_port_destroy(kr_radio_client *client, char *aux, char *name);
int kr_aux_port_create(kr_radio_client *cl, int type, char *aux, char *name);
int kr_aux_destroy(kr_radio_client *client, char *name);
int kr_aux_create(kr_radio_client *client, char *name);

int kr_aux_get_fd(kr_aux *aux);
int kr_aux_close(kr_radio_client *client, kr_aux *aux);
kr_aux *kr_aux_connect(kr_radio_client *client, kr_aux_params *params);

int kr_aux_frame_acquire(kr_aux *aux, kr_frame *frame);
int kr_aux_frame_release(kr_aux *aux, kr_frame *frame);
int kr_aux_frame_read(kr_aux *aux, kr_frame *frame);
int kr_aux_frame_write(kr_aux *aux, kr_frame *frame);

int kr_aux_audio_acquire(kr_aux *aux, kr_audio *audio);
int kr_aux_audio_read(kr_aux *aux, kr_audio *audio);
int kr_aux_audio_write(kr_aux *aux, kr_audio *audio);

#endif
