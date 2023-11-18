#include <xmmsclient/xmmsclient.h>
#include <krad/tags/tags.h>

typedef enum {
  PREV = 1,
  PLAY,
  PAUSE,
  STOP,
  NEXT
} krad_xmms_playback_cmd_t;

typedef struct krad_xmms_St krad_xmms_t;

struct krad_xmms_St {
  char sysname[64];
  char ipc_path[256];
  xmmsc_connection_t *connection;
  int fd;
  int connected;
  int destroying;
  pthread_t handler_thread;
  int handler_running;
  int handler_thread_socketpair[2];
  int playback_status;
  int playtime;
  int playing_id;
  int active_playlist_position;
  char active_playlist[256];
  char playtime_string[128];
  char now_playing[1024];
  char title[512];
  char artist[512];
  kr_tags *tags;
};

void krad_xmms_playback_cmd(krad_xmms_t *krad_xmms, krad_xmms_playback_cmd_t cmd);
void krad_xmms_destroy(krad_xmms_t *krad_xmms);
krad_xmms_t *krad_xmms_create(char *name, char *ipc_path, kr_tags *tags);
