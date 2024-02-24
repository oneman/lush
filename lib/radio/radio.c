#include <krad/mixer/mixer.h>
#include <krad/compositor/compositor.h>
#include <krad/xpdr/xpdr.h>
#include <krad/web/web.h>
#include <krad/web/crate.h>
#include <krad/xfer/xfer.h>
#include <krad/app/server.h>
#include <krad/app/name.h>
#include <krad/app/queue.h>
#include <krad/io/file.h>
#include <krad/io/file2.h>
#include <krad/time/timer.h>
#include <krad/track/studio.h>
#include <krad/tasks/pool.h>
#include <krad/ops/media/media.h>
#include <krad/ops/media/pool.h>
#include <krad/util/strton.h>
#include <krad/image/stash.h>
#include <krad/proxy/proxy.h>
#include <krad/gen/limits_gen.h>
#include "types.h"
#include "limits.h"

/* kradradio! */

typedef struct {
  char addr[256];
  size_t addr_len;
  kr_patchset ps;
} kr_broadcast_data;

typedef enum {
  KR_MOUNT_DISPATCH = 1,
  KR_MOUNT_INFO_TYPE
} kr_mount_ctx_types;

typedef enum {
  KR_MIXER = 1,
  KR_COMPOSITOR,
  KR_XPDR,
  KR_STUDIO,
  KR_IMAGE_STASH,
  KR_XFER
} kr_component;

typedef struct kr_radio kr_radio;
typedef struct radio_client radio_client;

typedef struct {
  struct {
    radio_client *client;
    char *address;
    size_t addr_len;
  } exclude;
  kr_names *mounts;
  kr_names *names;
} kr_api;

typedef enum {
  KR_RADIO_PATCH = 1
} kr_radio_event_type;

typedef struct {
  kr_radio *radio;
  kr_radio_event_type type;
  kr_patchset *patchset;
} kr_radio_event;

typedef int (kr_radio_event_cb)(kr_radio_event *);
typedef int (kr_dispatcher)(void *user, kr_crate *crate);

#define KR_NDECKS 2

typedef enum {
  KR_DECK_PLAYER = 1,
  KR_DECK_XMMS
} kr_deck_type;

typedef struct {
  kr_deck_type type;
  union {
    struct {
      int coconut;
    } player;
    struct {
      uint8_t *mem;
      kr_proxy *proxy;
    } xmms;
  };
} kr_deck;

struct kr_radio {
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_xpdr *xpdr;
  kr_web *web;
  kr_xfer *xfer;
  kr_studio *studio;
  kr_app_server *app;
  kr_pool *clients;
  kr_queue *events;
  kr_pool *broadcasts;
  kr_media_pool *media;
  kr_task_pool *tasks;
  kr_image_stash *stash;
  kr_tracker *tracker;
  kr_radio_info info;
  kr_radio_event_cb *event_cb;
  kr_api api;
  struct {
    kr_xfer_path *uploader;
    kr_xfer_path *receiver;
    kr_xfer_path *web_share;
    kr_deck deck[KR_NDECKS];
  };
};

typedef enum {
  KR_RADIO_UNKNOWN,
  KR_RADIO_API,
  KR_RADIO_XFER
} kr_radio_web_path_type;

typedef struct {
  int bongo;
} kr_local_client_info;

typedef enum {
  KR_RADIO_CLIENT_LOCAL = 1,
  KR_RADIO_CLIENT_WEB
} kr_radio_client_type;

typedef struct {
  kr_radio_client_type type;
  union {
    kr_local_client_info local;
    kr_web_client_info web;
  };
} kr_radio_client_info;

struct radio_client {
  kr_radio *radio;
  kr_radio_client_info info;
  union {
    kr_app_server_client *local_client;
    kr_web_path *path;
  };
  kr_crate crate;
};

typedef struct {
  kr_radio *radio;
  kr_app_server_client *local_client;
  kr_radio_client_info info;
} radio_client_setup;

static int radio_client_fds_get(void *ptr, int *fds, int nfds);
static int client_crate(radio_client *rc, kr_crate *crate);
static int client_crate_type(radio_client *rc, kr_crate *crate);
static void client_event(radio_client *rc);
static void local_client_data(radio_client *rc);
static int local_client_event(kr_app_server_client_event *e);
static int client_destroy(radio_client *client);
static radio_client *client_create(radio_client_setup *setup);
static void init_xmms_deck(kr_radio *radio, int port);
static int setup_serving(kr_radio *radio);
static int setup_routing(kr_radio *radio);
static int radio_get_info(kr_radio *radio, kr_radio_info *info);
static int radio_api(kr_api *api, kr_path *addr);
static int radio_ctl(kr_radio *radio, kr_patchset *patchset);
static void close_clients(kr_radio *radio);
static void destroy_decks(kr_radio *radio);
static int radio_destroy(void *user);
static int radio_create(kr_app_server *app, void *user);

#include "receiver.c"
#include "broadcast.c"
#include "reply.c"
#include "event.c"
#include "dispatch.c"

static int radio_client_fds_get(void *ptr, int *fds, int nfds) {
  kr_radio *radio;
  if (!ptr || !fds || nfds < 1) return -1;
  radio = (kr_radio *)ptr;
  return kr_app_server_fds_get(radio->app, fds, nfds);
}

static int client_crate(radio_client *rc, kr_crate *crate) {
  int ret;
  char *component;
  kr_dispatcher *disp;
  kr_name_ctx *ctx;
  kr_radio *radio;
  kr_path *path;
  kr_response res;
  radio = rc->radio;
  kr_path_alloca(path);
  ret = kr_path_parse(path, crate->address, crate->addr_len);
  if (ret != 0) return -1;
  broadcast_suppress(rc, crate);
  ret = kr_path_cur_name(path, &component);
  ctx = kr_name_resolve_type(radio->api.mounts, KR_MOUNT_DISPATCH,
   component, ret);
  if (!ctx) return -2;
  disp = (kr_dispatcher *)ctx->user;
  res = disp(radio, crate);
  broadcast_unsuppress(radio);
  if (res == KR_NONE) res = KR_NOT_FOUND;
  client_reply(rc, res, crate);
  kr_crate_reset(crate);
  return 0;
}

static int client_crate_type(radio_client *rc, kr_crate *crate) {
  int ret;
  char *component;
  kr_name_ctx *ctx;
  kr_radio *radio;
  kr_path *path;
  radio = rc->radio;
  kr_path_alloca(path);
  ret = kr_path_parse(path, crate->address, crate->addr_len);
  if (ret != 0) return -1;
  ret = kr_path_cur_name(path, &component);
  ctx = kr_name_resolve_type(radio->api.mounts, KR_MOUNT_INFO_TYPE,
   component, ret);
  if (!ctx) return -2;
  crate->type = (kr_info_type)ctx->user;
  return 0;
}

static void client_event(radio_client *rc) {
  ssize_t ret;
  size_t len;
  kr_web_client *wc;
  kr_crate *crate;
  uint8_t buf[8192];
  wc = kr_web_path_get_client(rc->path);
  crate = &rc->crate;
  crate->client = rc;
  printkd("Radio: Client event ... data for me!");
  for (;;) {
    ret = kr_web_client_unpack(wc, buf, sizeof(buf));
    if (ret == 0) {
      printkd("Radio: Got all the client data");
      break;
    }
    len = ret;
    printkd("Radio API: Got %zu bytes!\n%.*s\n", len, len, (char *)buf);
    if (!crate->method || crate->api) {
      ret = kr_crate_addr_meth_unpack_json(crate, (char*)buf, len);
      if (ret != 0) {
        printke("Radio: could not unpack incoming json to crate"
        " address and method. %d", ret);
        crate->method = 0;
        return;
      }
    }
    ret = client_crate_type(rc, crate);
    if (ret != 0) {
      printke("Radio: could not get crate type %d", ret);
      crate->method = 0;
      return;
    }
    ret = kr_crate_unpack_json(crate, (char*)buf, len);
    if (ret < 0) {
      printke("Radio: could not unpack incoming json to crate. %d", ret);
    } else {
      ret = client_crate(rc, crate);
      if (ret != 0) printke("Radio: client_crate fail");
    }
  }
}

static void local_client_data(radio_client *rc) {
  int ret;
  int len;
  kr_crate *crate;
  kr_packing_plan plan;
  uint8_t buf[8192];
  crate = &rc->crate;
  crate->client = rc;
  plan.format = KR_FORMAT_EBML;
  for (;;) {
    len = kr_app_client_peek(rc->local_client, buf, sizeof(buf));
    if (len < 1) break;
    plan.sz = len;
    plan.buf = buf;
    ret = kr_crate_unpack(crate, &plan);
    if (ret > 0) {
      kr_app_client_took(rc->local_client, ret);
    } else if (ret == 0) {
      break;
    }
    if (crate->method) {
      printk("Radio: crate method is %d", crate->method);
    }
    if (crate->addr_len) {
      printk("Radio: %*s", crate->addr_len, crate->address);
    }
    ret = client_crate(rc, crate);
    if (ret != 0) {
      printke("Radio: client_crate fail");
      break;
    }
  }
}

static int local_client_event(kr_app_server_client_event *e) {
  int ret;
  kr_radio *r;
  radio_client *c;
  radio_client_setup setup;
  if (!e) return -1;
  r = (kr_radio *)kr_app_server_get_user(e->server);
  switch (e->type) {
  case KR_APP_SERVER_CLIENT_CONNECT:
    printk("Radio: Local client connect");
    setup.radio = r;
    setup.info.type = KR_RADIO_CLIENT_LOCAL;
    setup.info.local.bongo = 45;
    setup.local_client = e->client;
    c = client_create(&setup);
    if (!c) {
      printke("Radio: Couldn't create a client");
      return -1;
    }
    ret = kr_app_client_set_user(e->client, c);
    if (ret) {
      printke("Radio: wtf can't set user on app client?");
    }
    break;
  case KR_APP_SERVER_CLIENT_DATA:
    printk("Radio: Local client data");
    c = (radio_client *)kr_app_client_get_user(e->client);
    if (!c) {
      printke("Radio: client data handler could not get radio client from app"
       "client...");
      return -1;
    }
    local_client_data(c);
    break;
  case KR_APP_SERVER_CLIENT_DISCONNECT:
    printk("Radio: Local client disconnect");
    c = (radio_client *)kr_app_client_get_user(e->client);
    if (!c) {
      printke("Radio: client disconnect handler could not get radio client"
       "from app client...");
      return -1;
    }
    return client_destroy(c);
    break;
  default:
    printke("Radio: Unknown local client event!");
    return -1;
  }
  return 0;
}

static int client_destroy(radio_client *client) {
  int ret;
  kr_radio *radio;
  if (!client) return -1;
  radio = client->radio;
  printkd("Radio: client destroy");
  ret = kr_pool_release(radio->clients, client);
  return ret;
}

static radio_client *client_create(radio_client_setup *setup) {
  kr_radio *radio;
  radio_client *client;
  radio = setup->radio;
  client = kr_pool_slice(radio->clients);
  if (client == NULL) {
    printke("Radio: out of client slots");
    return NULL;
  }
  memset(client, 0, sizeof(*client));
  client->radio = radio;
  client->info = setup->info;
  if (setup->info.type == KR_RADIO_CLIENT_LOCAL) {
    client->local_client = setup->local_client;
  }
  printkd("Radio: client create");
  return client;
}

static void init_xmms_deck(kr_radio *radio, int port) {
  static int deck_num = 0;
  kr_deck *deck;
  kr_proxy_setup proxy_setup;
  deck = &radio->deck[deck_num++];
  deck->type = KR_DECK_XMMS;
  memset(&proxy_setup, 0, sizeof(proxy_setup));
  proxy_setup.web = radio->web;
  proxy_setup.info.type = KR_PROXY_WEBSOCKET_TO_TCP;
  proxy_setup.info.dst.host_len = snprintf(proxy_setup.info.dst.host,
   sizeof(proxy_setup.info.dst.host), "%s", "127.0.0.1");
  deck->xmms.mem = kr_allocz(1, kr_proxy_sizeof());
  snprintf(proxy_setup.info.path, sizeof(proxy_setup.info.path), "%s%d",
   "deck", deck_num);
  proxy_setup.info.dst.port = port;
  proxy_setup.mem = deck->xmms.mem;
  deck->xmms.proxy = kr_proxy_create(&proxy_setup);
}

static int yay_for_that(kr_file2 *file, void *user) {
  int ret;
  kr_radio *radio;
  kr_task *task;
  kr_task_params_info info;
  kr_task_setup setup;
  kr_file2_info file_info;
  if (!file || !user) return -1;
  radio = (kr_radio *)user;
  printk("yay we got a completely uploaded file!");
  setup.user = radio;
  info.type = KR_XMMS;
  ret = kr_file2_get_info(file, &file_info);
  if (ret) return ret;
  snprintf(info.xmms.filename, sizeof(info.xmms.filename), "%.*s",
   file_info.len, file_info.path);
  snprintf(info.xmms.env, sizeof(info.xmms.env),
   "XMMS_PATH=tcp://0.0.0.0:%d", 4500);
  setup.info = &info;
  task = kr_task_create(radio->tasks, &setup);
  kr_task_start(task);
  snprintf(info.xmms.env, sizeof(info.xmms.env),
   "XMMS_PATH=tcp://0.0.0.0:%d", 4600);
  setup.info = &info;
  task = kr_task_create(radio->tasks, &setup);
  kr_task_start(task);
  return 0;
}

static int setup_serving(kr_radio *radio) {
  int fail;
  int res;
  fail = 0;
  kr_xfer_server_setup setup;
  kr_xfer_share_info *assets;
  kr_xfer_acceptor_info *acceptor;
  static const char *dir_path;
  static const char *share_path;
  static const char *in_path;
  static const char *accept_path = "upload";
  static const char *receive_path = "incoming";
  char *web_port;
  dir_path = getenv("KRAD_WEB_ROOT");
  in_path = getenv("KRAD_XFER_INCOMING");
  web_port = getenv("KRAD_WEB_PORT");
  memset(&setup, 0, sizeof(setup));
  setup.user = radio;
  setup.info.type = KR_XFER_SHARING;
  assets = &setup.info.share;
  assets->type = KR_XFER_SHARE_DIR;
  if (!dir_path) {
    printke("Web Setup: No KRAD_WEB_ROOT env var");
    fail = 1;
  }
  if (!in_path) {
    printk("Web Setup: No KRAD_XFER_INCOMING env var");
  }
  if (!fail) {
    share_path = strrchr(dir_path, '/') + 1;
    printk("Using hardcoded web root: %s -> /%s", dir_path, share_path);
    assets->dir.len = strlen(dir_path);
    strncpy(assets->dir.path, dir_path, sizeof(assets->dir.path) - 1);
    strncpy(setup.info.path, share_path, sizeof(setup.info.path) - 1);
    setup.info.len = strlen(share_path);
    radio->web_share = kr_xfer_server(radio->xfer, &setup);
    if (!radio->web_share) fail = 1;
  }
  if (!fail && in_path) {
    setup.acptr.accepted_cb = yay_for_that;
    setup.acptr.user = radio;
    acceptor = &setup.info.acceptor;
    setup.info.type = KR_XFER_ACCEPTING;
    setup.info.len = snprintf(setup.info.path, sizeof(setup.info.path), "%s",
     accept_path);
    acceptor->type = KR_XFER_ACCEPT_DIR;
    acceptor->dir.len = snprintf(acceptor->dir.path,
      sizeof(acceptor->dir.path), "%s", in_path);
    radio->uploader = kr_xfer_server(radio->xfer, &setup);
    if (!radio->uploader) fail = 1;
  }
  if (!fail) {
    kr_xfer_receiver_setup rsetup;
    rsetup.user = radio;
    rsetup.add_client = receiver_add_source;
    rsetup.del_client = receiver_del_source;
    rsetup.client_write = receiver_read_source;
    setup.info.type = KR_XFER_RECEIVER;
    setup.info.len = snprintf(setup.info.path, sizeof(setup.info.path), "%s",
     receive_path);
    setup.rcvr = rsetup;
    radio->receiver = kr_xfer_server(radio->xfer, &setup);
    if (!radio->receiver) fail = 1;
  }
  if (!fail) {
    if (web_port) {
      /* KLUDGE */
      kr_web_path_setup_info new;
      memset(&new, 0, sizeof(new));
      new.type = KR_WEB_SERVER;
      new.server.type = KR_WEB_SERVER_HTTP;
      snprintf(new.server.iface, sizeof(new.server.iface), "0.0.0.0");
      res = kr_strtou16(&new.server.port, web_port, strlen(web_port));
      if (res != 0) return -1;
      fail = kr_web_path_create(radio->web, &new, "coconutptr", NULL);
      /* END KLUDGE */
    }
  }
  if (!fail) {
    init_xmms_deck(radio, 4500);
    init_xmms_deck(radio, 4600);
  }
  return fail;
}

static int setup_routing(kr_radio *radio) {
  kr_name_ctx ctx;
  kr_name *name;
  if (!radio) return -1;
  ctx.perm = 0;
  name = kr_name_add_str(radio->api.mounts, "mixer", 5);
  ctx.user = dispatch_mcx;
  ctx.type = KR_MOUNT_DISPATCH;
  kr_name_link(radio->api.mounts, name, &ctx);
  ctx.user = (void *)KR_MIXER_PATH_INFO;
  ctx.type = KR_MOUNT_INFO_TYPE;
  kr_name_link(radio->api.mounts, name, &ctx);
  if (radio->info.mode == KR_RADIO_MODE_AUDIOVIDEO) {
    name = kr_name_add_str(radio->api.mounts, "compositor", 10);
    ctx.user = dispatch_mcx;
    ctx.type = KR_MOUNT_DISPATCH;
    kr_name_link(radio->api.mounts, name, &ctx);
    ctx.user = (void *)KR_COMPOSITOR_PATH_INFO;
    ctx.type = KR_MOUNT_INFO_TYPE;
    kr_name_link(radio->api.mounts, name, &ctx);
  }
  name = kr_name_add_str(radio->api.mounts, "studio", 6);
  ctx.user = dispatch_studio;
  ctx.type = KR_MOUNT_DISPATCH;
  kr_name_link(radio->api.mounts, name, &ctx);
  ctx.user = (void *)KR_STUDIO_PATH_INFO;
  ctx.type = KR_MOUNT_INFO_TYPE;
  kr_name_link(radio->api.mounts, name, &ctx);
  name = kr_name_add_str(radio->api.mounts, "xpdr", 4);
  ctx.user = dispatch_mcx;
  ctx.type = KR_MOUNT_DISPATCH;
  kr_name_link(radio->api.mounts, name, &ctx);
  ctx.user = (void *)KR_XPDR_PATH_INFO;
  ctx.type = KR_MOUNT_INFO_TYPE;
  kr_name_link(radio->api.mounts, name, &ctx);
  name = kr_name_add_str(radio->api.mounts, "api", 3);
  ctx.user = dispatch_api_mounts;
  ctx.type = KR_MOUNT_DISPATCH;
  kr_name_link(radio->api.mounts, name, &ctx);
  name = kr_name_add_str(radio->api.mounts, "info", 4);
  ctx.user = dispatch_info;
  ctx.type = KR_MOUNT_DISPATCH;
  kr_name_link(radio->api.mounts, name, &ctx);
  ctx.user = (void *)KR_RADIO_INFO;
  ctx.type = KR_MOUNT_INFO_TYPE;
  kr_name_link(radio->api.mounts, name, &ctx);
  name = kr_name_add_str(radio->api.mounts, "limits", 6);
  ctx.user = dispatch_limits;
  ctx.type = KR_MOUNT_DISPATCH;
  kr_name_link(radio->api.mounts, name, &ctx);
  ctx.user = (void *)KR_LIMITS_INFO;
  ctx.type = KR_MOUNT_INFO_TYPE;
  kr_name_link(radio->api.mounts, name, &ctx);
  return 0;
}

static int radio_get_info(kr_radio *radio, kr_radio_info *info) {
  if (!radio || !info) return -1;
  *info = radio->info;
  return 0;
}

static int radio_api(kr_api *api, kr_path *addr) {
  int len;
  char *name;
  kr_name *found;
  len = kr_path_cur_name(addr, &name);
  if (len < 1) return 0;
  found = kr_name_find(api->mounts, name, len);
  if (found) {
    printkd("Radio: %s is an api path!", name);
    return 1;
  }
  return 0;
}

static int radio_ctl(kr_radio *radio, kr_patchset *patchset) {
  kr_radio_event e;
  if (!radio || !patchset) return -1;
  kr_patchset_apply(&radio->info, patchset);
  patchset->on = &radio->info;
  e.type = KR_RADIO_PATCH;
  e.patchset = patchset;
  e.radio = radio;
  radio->event_cb(&e);
  return 0;
}

static void close_clients(kr_radio *radio) {
  int i;
  radio_client *rc;
  i = 0;
  while ((rc = kr_pool_iterate_active(radio->clients, &i))) {
    if (rc->info.type == KR_RADIO_CLIENT_LOCAL) {
      kr_app_client_close(rc->local_client);
    }
  }
}

static void destroy_decks(kr_radio *radio) {
  int i;
  kr_deck *deck;
  for (i = 0; i < KR_NDECKS; i++) {
    deck = &radio->deck[i];
    if (deck->type == KR_DECK_XMMS) {
      if (deck->xmms.proxy) {
        kr_proxy_destroy(deck->xmms.proxy);
        deck->xmms.proxy = NULL;
      }
      if (deck->xmms.mem) {
        free(deck->xmms.mem);
        deck->xmms.mem = NULL;
      }
    }
  }
}

static int radio_destroy(void *user) {
  kr_radio *radio;
  kr_timer *timer;
  radio = (kr_radio *)user;
  if (radio == NULL) return -1;
  printk("Radio: Destroying");
  timer = kr_timer_alloca();
  kr_timer_name_set(timer, "Destroy");
  kr_timer_start(timer);
  if (radio->web_share) {
    kr_xfer_remove(radio->web_share);
    radio->web_share = NULL;
  }
  if (radio->uploader) {
    kr_xfer_remove(radio->uploader);
    radio->uploader = NULL;
  }
  if (radio->tasks) {
    kr_task_pool_destroy(radio->tasks);
    radio->tasks = NULL;
  }
  if (radio->studio) {
    kr_studio_destroy(radio->studio);
    radio->studio = NULL;
  }
  if (radio->stash) {
    kr_image_stash_destroy(radio->stash);
    free(radio->stash);
    radio->stash = NULL;
  }
  destroy_decks(radio);
  if (radio->web) {
    kr_web_destroy(radio->web);
    radio->web = NULL;
  }
  if (radio->xfer) {
    kr_xfer_destroy(radio->xfer);
    radio->xfer = NULL;
  }
  if (radio->xpdr) {
    kr_xpdr_destroy(radio->xpdr);
    radio->xpdr = NULL;
  }
  kr_timer_status(timer);
  if (radio->tracker) {
    kr_tracker_destroy(radio->tracker);
    radio->tracker = NULL;
  }
  kr_timer_status(timer);
  if (radio->mixer) {
    kr_mixer_destroy(radio->mixer);
    radio->mixer = NULL;
  }
  kr_timer_status(timer);
  if (radio->compositor) {
    kr_compositor_destroy(radio->compositor);
    radio->compositor = NULL;
  }
  if (radio->media) {
    kr_media_pool_destroy(radio->media);
    radio->media = NULL;
  }
  if (radio->clients) {
    close_clients(radio);
    kr_pool_destroy(radio->clients);
    radio->clients = NULL;
  }
  kr_timer_status(timer);
  if (radio->api.names) {
    kr_names_destroy(radio->api.names);
    radio->api.names = NULL;
  }
  if (radio->api.mounts) {
    kr_names_destroy(radio->api.mounts);
    radio->api.mounts = NULL;
  }
  if (radio->events) {
    kr_queue_destroy(radio->events);
    radio->events = NULL;
  }
  if (radio->broadcasts) {
    kr_pool_destroy(radio->broadcasts);
    radio->broadcasts = NULL;
  }
  kr_timer_status(timer);
  kr_timer_finish(timer);
  printk("Radio: Destroyed in %"PRIu64"ms", kr_timer_duration_ms(timer));
  return 0;
}

static int radio_create(kr_app_server *app, void *user) {
  int fail;
  int ret;
  fail = 0;
  kr_radio *radio;
  kr_event eh;
  kr_pool_setup broadcast_pool_setup;
  kr_pool_setup cpool_setup;
  kr_web_setup web_setup;
  kr_mixer_setup mixer_setup;
  kr_compositor_setup compositor_setup;
  kr_xpdr_setup xpdr_setup;
  kr_xfer_setup xfer_setup;
  kr_studio_setup studio_setup;
  kr_task_pool_setup tasks_setup;
  kr_media_pool_setup mpool_setup;
  kr_names_setup mounts_setup;
  kr_names_setup names_setup;
  kr_image_stash_setup stash_setup;
  kr_tracker_setup tracker_setup;
  kr_queue_setup queue_setup;
  radio = (kr_radio *)user;
  radio->app = app;
  radio->event_cb = radio_event;
  if (!fail) {
    memset(&broadcast_pool_setup, 0, sizeof(broadcast_pool_setup));
    broadcast_pool_setup.size = sizeof(kr_broadcast_data);
    broadcast_pool_setup.slices = 512;
    radio->broadcasts = kr_pool_create(&broadcast_pool_setup);
    if (!radio->broadcasts) fail = 1;
  }
  if (!fail) {
    queue_setup.nproducers = 2;
    queue_setup.producer[0].user = KR_MIXER;
    queue_setup.producer[0].sz = 512;
    queue_setup.producer[1].user = KR_COMPOSITOR;
    queue_setup.producer[1].sz = 32;
    radio->events = kr_queue_create(&queue_setup);
    if (!radio->events) fail = 1;
  }
  if (!fail) {
    memset(&eh, 0, sizeof(eh));
    eh.fd = kr_queue_get_fd(radio->events);
    eh.events = EPOLLIN;
    eh.user = radio;
    eh.handler = queued_broadcast_event;
    ret = kr_loop_add(kr_app_server_mainloop_get(radio->app), &eh);
    if (ret != 0) {
      fail = 1;
      printke("Queue: failed to setup queued broadcast handler");
    }
  }
  if (!fail) {
    mounts_setup.nnodes = 16;
    radio->api.mounts = kr_names_create(&mounts_setup);
    if (!radio->api.mounts) fail = 1;
  }
  if (!fail) {
    names_setup.nnodes = 64;
    radio->api.names = kr_names_create(&names_setup);
    if (!radio->api.names) fail = 1;
  }
  if (!fail) {
    memset(&cpool_setup, 0, sizeof(cpool_setup));
    cpool_setup.size = sizeof(radio_client);
    cpool_setup.slices = KR_RADIO_CLIENTS_MAX;
    radio->clients = kr_pool_create(&cpool_setup);
    if (!radio->clients) fail = 1;
  }
  if (!fail) {
    mixer_setup.path_count = KR_MIXER_NPATHS;
    mixer_setup.user = radio;
    mixer_setup.event_cb = mixer_event;
    radio->mixer = kr_mixer_create(&mixer_setup);
    if (!radio->mixer) fail = 1;
  }
  if (!fail && radio->info.mode == KR_RADIO_MODE_AUDIOVIDEO) {
    memset(&compositor_setup, 0, sizeof(kr_compositor_setup));
    compositor_setup.event_cb = compositor_event;
    compositor_setup.user = radio;
    radio->compositor = kr_compositor_create(&compositor_setup);
    if (!radio->compositor) fail = 1;
  }
  if (!fail) {
    xpdr_setup.mixer = radio->mixer;
    xpdr_setup.compositor = radio->compositor;
    xpdr_setup.user = radio;
    xpdr_setup.event_cb = xpdr_event;
    xpdr_setup.fds_get_cb = radio_client_fds_get;
    xpdr_setup.path_count = KR_XPDR_NPATHS;
    xpdr_setup.master_loop = kr_app_server_mainloop_get(radio->app);
    radio->xpdr = kr_xpdr_create(&xpdr_setup);
    if (!radio->xpdr) fail = 1;
  }
  if (!fail) {
    memset(&web_setup, 0, sizeof(kr_web_setup));
    web_setup.event_cb = web_event;
    web_setup.user = radio;
    web_setup.npaths = KR_WEB_NPATHS;
    web_setup.master_loop = kr_app_server_mainloop_get(radio->app);
    radio->web = kr_web_create(&web_setup);
    if (!radio->web) fail = 1;
  }
  if (!fail) {
    memset(&xfer_setup, 0, sizeof(kr_xfer_setup));
    xfer_setup.event_cb = xfer_event;
    xfer_setup.user = radio;
    xfer_setup.npaths = KR_XFER_NPATHS;
    xfer_setup.web = radio->web;
    radio->xfer = kr_xfer_create(&xfer_setup);
    if (!radio->xfer) fail = 1;
  }
  if (!fail && (radio->compositor != NULL)) {
    stash_setup.mem = kr_alloc(kr_image_stash_sizeof());
    stash_setup.stash_size = 45 * 6;
    stash_setup.event_cb = stash_event;
    stash_setup.user = radio;
    radio->stash = kr_image_stash_init(&stash_setup);
    if (!radio->stash) fail = 1;
  }
  if (!fail) {
    tracker_setup.user = radio;
    tracker_setup.ntracks = KR_TRACKER_NTRACKS;
    //tracker_setup.tracker_ev_cb = tracker_event;
    tracker_setup.track_ev_cb = track_event;
    tracker_setup.master_loop = NULL;
    radio->tracker = kr_tracker_create(&tracker_setup);
  }
  if (!fail) {
    memset(&studio_setup, 0, sizeof(kr_studio_setup));
    studio_setup.event_cb = studio_event;
    studio_setup.user = radio;
    studio_setup.npaths = KR_STUDIO_NPATHS;
    studio_setup.nprocs = KR_STUDIO_NPROCS;
    studio_setup.mixer = radio->mixer;
    studio_setup.compositor = radio->compositor;
    studio_setup.xfer = radio->xfer;
    studio_setup.stash = radio->stash;
    studio_setup.tracker = radio->tracker;
    radio->studio = kr_studio_create(&studio_setup);
    if (!radio->studio) fail = 1;
  }
  if (!fail) {
    memset(&mpool_setup, 0, sizeof(mpool_setup));
    mpool_setup.nmedia = KR_RADIO_CLIENTS_MAX;
    mpool_setup.loop = kr_app_server_mainloop_get(radio->app);
    mpool_setup.event_cb = media_event;
    radio->media = kr_media_pool_create(&mpool_setup);
    if (radio->media == NULL) fail = 1;
  }
  if (!fail) {
    memset(&tasks_setup, 0, sizeof(kr_task_pool_setup));
    tasks_setup.ntasks = KR_RADIO_CLIENTS_MAX;
    tasks_setup.loop = kr_app_server_mainloop_get(radio->app);
    tasks_setup.event_cb = task_event;
    tasks_setup.media_pool = radio->media;
    tasks_setup.cmdr = kr_app_server_get_cmdr(radio->app);
    if (tasks_setup.cmdr) {
      radio->tasks = kr_task_pool_create(&tasks_setup);
    }
    if (!radio->tasks) fail = 1;
  }
  if (!fail) {
    fail = setup_serving(radio);
  }
  if (!fail) {
    fail = setup_routing(radio);
  }
  if (!fail) {
    fail = kr_xpdr_monitor(radio->xpdr, 1);
  }
  if (!fail) {
    printk("Radio: Created");
    return 0;
  }
  radio_destroy(radio);
  return -1;
}

void krad_radio(char *callsign) {
  static kr_radio radio;
  static kr_app_server_setup setup;
  char *mode;
  printk("Radio: Creating %s", callsign);
  strncpy(radio.info.callsign, callsign, sizeof(radio.info.callsign) - 1);
  strncpy(radio.info.name, callsign, sizeof(radio.info.name) - 1);
  radio.info.started_on = time(NULL);
  mode = getenv("KR_RADIO_MODE");
  if (mode && str_to_kr_radio_mode(mode) == KR_RADIO_MODE_AUDIO) {
    radio.info.mode = KR_RADIO_MODE_AUDIO;
  } else {
    radio.info.mode = KR_RADIO_MODE_AUDIOVIDEO;
  }
  radio.info.version = KR_VERSION;
  strncpy(radio.info.commit, KR_GIT_VERSION, sizeof(radio.info.commit) - 1);
  strncpy(setup.appname, "krad_radio", sizeof(setup.appname) - 1);
  strncpy(setup.sysname, callsign, sizeof(setup.sysname) - 1);
  setup.cmdr = 1;
  setup.user = &radio;
  setup.client_handler = local_client_event;
  setup.signal_handler = signal_event;
  setup.startup_handler = radio_create;
  setup.shutdown_handler = radio_destroy;
  krad_app_server(&setup);
}
