#if !defined(_adapter_adapter_H)
# define _adapter_adapter_H (1)

typedef struct kr_adapter kr_adapter;
typedef struct kr_adapter_spec kr_adapter_spec;
typedef struct kr_adapter_path kr_adapter_path;

#include <krad/xpdr/types.h>
#include <krad/xpdr/event.h>
#include <krad/loop/loop.h>

typedef enum {
  KR_ADAPTER_PROCESS, /* Process all paths: jack,alsa,decklink */
  KR_ADAPTER_DISCONNECTED,
  KR_ADAPTER_RECONNECTED
} kr_adapter_event_type;

typedef struct {
  kr_adapter *adapter;
  void *user;
  kr_adapter_event_type type;
} kr_adapter_event;

typedef void (kr_adapter_event_cb)(kr_adapter_event *);

struct kr_adapter {
  void *handle;
  int fd;
  void *user;
  kr_adapter_event_cb *event_cb;
  kr_xpdr_path_info *info;
  kr_loop *loop;
  kr_loop *master_loop;
};

typedef int (kr_avio)(void *, kr_av *);

struct kr_adapter_path {
  void *handle;
  void *user;
  int fd;
  kr_avio *read;
  kr_avio *write;
  kr_av_handler *av_handler;
  kr_xpdr_path_info *info;
  int aux_fds[4];
  int naux_fds;
};

typedef int (kr_adapter_lctl)(kr_adapter_path *, kr_patchset *);
typedef int (kr_adapter_unlink)(kr_adapter_path *);
typedef int (kr_adapter_link)(kr_adapter *, kr_adapter_path *);
typedef int (kr_adapter_ctl)(kr_adapter *, kr_patchset *);
typedef int (kr_adapter_close)(kr_adapter *);
typedef int (kr_adapter_open)(kr_adapter *);

struct kr_adapter_spec {
  kr_adapter_lctl *lctl;
  kr_adapter_unlink *unlink;
  kr_adapter_link *link;
  kr_adapter_ctl *ctl;
  kr_adapter_close *close;
  kr_adapter_open *open;
};

#endif
