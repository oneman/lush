#include <krad/radio/version.h>
#include <krad/app/types.h>
#include <krad/io/buffer.h>
#include <krad/hash/sha1.h>
#include <krad/hash/base64.h>
#include <krad/loop/loop.h>
#include "types.h"

#if !defined(_web_web_H)
# define _web_web_H (1)

typedef enum {
  KR_WEB_OPEN = 1,
  KR_WEB_CLOSE,
  KR_WEB_HEADERS,
  KR_WEB_DATA
} kr_web_event_type;

typedef struct kr_web kr_web;
typedef struct kr_web_setup kr_web_setup;
typedef struct kr_web_event kr_web_event;
typedef int (kr_web_event_cb)(kr_web_event *);
typedef struct kr_web_path kr_web_path;

struct kr_web_event {
  kr_web_event_type type;
  void *user;
  void *user_path;
  kr_web_path *path;
};

#include "client.h"
#include "server.h"

struct kr_web_setup {
  int npaths;
  void *user;
  kr_web_event_cb *event_cb;
  kr_loop *master_loop;
};

kr_loop *kr_web_loop(kr_web *web);
kr_web_path_info *kr_web_path_info_get(kr_web_path *path);
int kr_web_path_user_type_get(kr_web_path *path);
int kr_web_path_user_type_set(kr_web_path *path, int type);
void *kr_web_path_user_get(kr_web_path *path);
int kr_web_path_user_set(kr_web_path *path, void *user);
int kr_web_path_handler_set(kr_web_path *path, kr_web_event_cb *handler);
int kr_web_path_remove(kr_web_path *path);
int kr_web_path_create(kr_web *web, kr_web_path_setup_info *setup,
 void *user, kr_web_event_cb *handler);
int kr_web_destroy(kr_web *web);
kr_web *kr_web_create(kr_web_setup *setup);

#endif
