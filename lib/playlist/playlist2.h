typedef struct {
  char location[512];
  char identifier[256];
  char title[256];
  char creator[256];
  char annotation[1024];
  char info[1024];
  char image[512];
  char album[256];
  int trackNum;
  int duration;
} kr_playlist_track;

typedef struct {
  char title[256];
  char creator[256];
  char annotation[1024];
  char info[1024];
  char location[512];
  char identifier[256];
  char image[512];
  char date[128];
  kr_playlist_track track[32];
} kr_playlist;
