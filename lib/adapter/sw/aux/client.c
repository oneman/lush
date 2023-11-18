#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <krad/app/crate.h>
#include <krad/app/notice.h>
#include <krad/app/seq.h>
#include <krad/audio/pool.h>
#include <krad/radio/client_internal.h>
#include <krad/gen/metainfo.h>
#include <krad/gen/info_types.h>
#include <krad/mem/pool.h>
#include <krad/util/util.h>
#include <krad/image/image.h>
#include "client.h"

struct kr_aux {
  int in_fd;
  int out_fd;
  kr_pool *pool;
  union {
    kr_frame frame[KR_AUX_PORT_NFRAMES];
    kr_audio audio[KR_AUX_PORT_NPERIODS];
  };
  kr_seq *seq;
  int type;
  kr_amem(mem, 32)
};

static int aux_close(kr_aux *aux);
static int aux_create(kr_radio_client *cli, int type, char *aux, char *port);

static int aux_close(kr_aux *aux) {
  int ret;
  int fail;
  void *save;
  fail = 0;
  printkd("Aux: Closing");
  if (aux->out_fd > -1) {
    ret = kr_notifier_destroy(aux->out_fd);
    fail += ret;
    if (ret) printke("AUX Port: close out fd");
  }
  if (aux->out_fd > -1) {
    ret = kr_notifier_destroy(aux->in_fd);
    fail += ret;
    if (ret) printke("AUX Port: close in fd");
  }
  if (aux->pool) {
    ret = kr_pool_destroy(aux->pool);
    fail += ret;
    if (ret) printke("AUX Port: destroy pool");
    aux->pool = NULL;
  }
  save = (void *)aux;
  memset(aux, 0, sizeof(*aux));
  free(save);
  printkd("Aux: Closed");
  return fail;
}

static int aux_create(kr_radio_client *cli, int type, char *aux, char *port) {
  kr_xpdr_path_info *info;
  int ret;
  kr_crate crate;
  char string[8192];
  if (!cli) return -1;
  if (!aux) return -2;
  if (type != KR_AUX && !port) return -3;
  memset(&crate, 0, sizeof(crate));
  info = &crate.info.xpdr_path_info;
  info->type = type;
  crate.method = KR_PUT;
  crate.type = KR_XPDR_PATH_INFO;
  if (type == KR_AUX) {
    snprintf(crate.address, sizeof(crate.address), "/xpdr/%s", aux);
    kr_aux_info_init(&info->aux);
    info->aux.state = 0;
    kr_aux_info_to_text(string, &info->aux, sizeof(string));
    printk("Aux: create: \n%s", string);
    ret = kr_crate_send(cli, &crate);
    return ret;
  }
  snprintf(crate.address, sizeof(crate.address), "/xpdr/%s/%s", aux, port);
  //kr_aux_path_info_init(&info->aux_video_in);
  kr_xpdr_path_info_pack_json(string, &info, sizeof(string));
  printk("Aux Port: create: \n%s", string);
  ret = kr_crate_send(cli, &crate);
  return ret;
}

int kr_video_in(kr_radio_client *client, char *aux_name, char *port_name) {
  if (!client) return -1;
  if (!aux_name) return -2;
  if (!port_name) return -3;
  return aux_create(client, KR_AUX_VIDEO_IN, aux_name, port_name);
}

int kr_video_out(kr_radio_client *client, char *aux_name, char *port_name) {
  if (!client) return -1;
  if (!aux_name) return -2;
  if (!port_name) return -3;
  return aux_create(client, KR_AUX_VIDEO_OUT, aux_name, port_name);
}

int kr_audio_in(kr_radio_client *client, char *aux_name, char *port_name) {
  if (!client) return -1;
  if (!aux_name) return -2;
  if (!port_name) return -3;
  return aux_create(client, KR_AUX_AUDIO_IN, aux_name, port_name);
}

int kr_audio_out(kr_radio_client *client, char *aux_name, char *port_name) {
  if (!client) return -1;
  if (!aux_name) return -2;
  if (!port_name) return -3;
  return aux_create(client, KR_AUX_AUDIO_OUT, aux_name, port_name);
}

int kr_aux_port_destroy(kr_radio_client *client, char *aux, char *name) {
  char path[512];
  if (!client) return -1;
  if (!aux) return -2;
  if (!name) return -3;
  snprintf(path, sizeof(path), "/xpdr/%s/%s", aux, name);
  return kr_delete(client, path);
}

int kr_aux_port_create(kr_radio_client *cl, int type, char *aux, char *name) {
  if (!cl) return -1;
  if (!aux) return -2;
  if (!name) return -3;
  if ((type != KR_AUX_VIDEO_IN) && (type != KR_AUX_VIDEO_OUT)
   && (type != KR_AUX_AUDIO_IN) && (type != KR_AUX_AUDIO_OUT)) {
    return -4;
  }
  return aux_create(cl, type, aux, name);
}

int kr_aux_destroy(kr_radio_client *client, char *name) {
  char addr[512];
  if (!client) return -1;
  if (!name) return -2;
  snprintf(addr, sizeof(addr), "/xpdr/%s", name);
  return kr_delete(client, addr);
}

int kr_aux_create(kr_radio_client *client, char *name) {
  if (!client) return -1;
  if (!name) return -2;
  return aux_create(client, KR_AUX, name, NULL);
}

int kr_aux_get_fd(kr_aux *aux) {
  if (!aux) return -1;
  if (aux->in_fd < 0) return -1;
  return aux->in_fd;
}

int kr_aux_close(kr_radio_client *client, kr_aux *aux) {
  if (!client || !aux) return -1;
  kr_client_on_disconnect_rm(client, aux);
  aux_close(aux);
  return 0;
}

static void setup_audio(kr_aux *aux) {
  int i;
  int ret;
  kr_audio_setup setup;
  for (i = 0; i < KR_AUX_PORT_NPERIODS; i++) {
    setup.mem = kr_pool_slice(aux->pool);
    setup.audio = &aux->audio[i];
    setup.channels = KR_AUX_KLUDGE_NCHANNELS;
    setup.rate = 48000;
    setup.count = KR_AUX_KLUDGE_PERIOD_NFRAMES;
    ret = kr_audio_init(&setup);
    if (ret) printk("AUX Port: setup audio");
  }
}

static void setup_frames(kr_aux *aux) {
  int i;
  int ret;
  kr_image_setup setup;
  for (i = 0; i < KR_AUX_PORT_NFRAMES; i++) {
    aux->frame[i].type = KR_IMAGE_RAW;
    setup.mem = kr_pool_slice(aux->pool);
    setup.img = &aux->frame[i].image;
    setup.info.w = KR_AUX_KLUDGE_WIDTH;
    setup.info.h = KR_AUX_KLUDGE_HEIGHT;
    setup.info.fmt = KR_AUX_KLUDGE_FMT;
    ret = kr_image_init2(&setup);
    if (ret) printk("AUX Port: setup frames");
    aux->frame[i].image.release = NULL;
    aux->frame[i].image.owner = NULL;
  }
}

static int create_frame_pool(kr_aux *aux) {
  kr_pool_setup setup;
  kr_image_info img_info;
  img_info.w = KR_AUX_KLUDGE_WIDTH;
  img_info.h = KR_AUX_KLUDGE_HEIGHT;
  img_info.fmt = KR_AUX_KLUDGE_FMT;
  memset(&setup, 0, sizeof(setup));
  setup.shared = 1;
  setup.slices = KR_AUX_PORT_NFRAMES;
  setup.size = kr_image_sizeof(&img_info);
  aux->pool = kr_pool_create(&setup);
  if (!aux->pool) return -1;
  return 0;
}

static int create_audio_pool(kr_aux *aux) {
  int ret;
  kr_audio_pool_setup s;
  aux->seq = (kr_seq *)aux->mem;
  ret = kr_seq_init(aux->seq, KR_AUX_PORT_NPERIODS);
  s.nframes = KR_AUX_KLUDGE_PERIOD_NFRAMES;
  s.count = KR_AUX_PORT_NPERIODS;
  s.channels = KR_AUX_KLUDGE_NCHANNELS;
  s.shared = 1;
  aux->pool = kr_audio_pool_create(&s);
  return ret;
}

kr_aux *kr_aux_connect(kr_radio_client *client, kr_aux_params *params) {
  kr_aux *aux;
  int ret;
  int err;
  int fail;
  int meta_type;
  char *param_path;
  kr_path *path;
  kr_patchset patchset;
  kr_crate crate;
  if (!client || !params) return NULL;
  if (!kr_client_local(client)) return NULL;
  memset(&crate, 0, sizeof(crate));
  fail = 0;
  aux = NULL;
  aux = kr_allocz(1, sizeof(*aux));
  aux->type = params->type;
  if (aux->type == KR_AUX_VIDEO_IN) param_path = "/aux_video_in/state";
  if (aux->type == KR_AUX_VIDEO_OUT) param_path = "/aux_video_out/state";
  if (aux->type == KR_AUX_AUDIO_IN) param_path = "/aux_audio_in/state";
  if (aux->type == KR_AUX_AUDIO_OUT) param_path = "/aux_audio_out/state";
  memset(&patchset, 0, sizeof(patchset));
  if (aux->type == KR_AUX_VIDEO_IN || aux->type == KR_AUX_VIDEO_OUT) {
    fail = create_frame_pool(aux);
    if (!fail) setup_frames(aux);
  } else if (aux->type == KR_AUX_AUDIO_IN || aux->type == KR_AUX_AUDIO_OUT) {
    fail = create_audio_pool(aux);
    if (!fail) setup_audio(aux);
  }
  if (!fail) {
    aux->in_fd = kr_notifier_create();
    if (aux->in_fd < 0) fail = 1;
  }
  if (!fail) {
    aux->out_fd = kr_notifier_create();
    if (aux->out_fd < 0) fail = 1;
  }
  if (!fail) {
    kr_client_on_disconnect(client, (kr_destroy_cb *)aux_close, aux);
  }
  if (!fail) {
    crate.method = KR_PATCH;
    crate.type = KR_XPDR_PATH_INFO;
    kr_path_alloca(path);
    kr_path_parse(path, param_path, strlen(param_path));
    patchset.type = KR_XPDR_PATH_INFO;
    patchset.len = 1;
    patchset.on = &crate.info;
    meta_type = kr_info_type_to_metainfo_type(crate.type);
    printk("AUX Client: meta type is %d", meta_type);
    ret = path_get_type(path, meta_type);
    printk("AUX Client: path type is %d", ret);
    patchset.patch[0].integer = KR_AUX_PORT_CONNECTED;
    ret = patch_path(&patchset.patch[0], path, meta_type);
    if (ret) {
      printke("AUX Client: creating patchset failed %d", ret);
      fail = 1;
    }
  }
  if (!fail) {
    char string[8192];
    kr_xpdr_path_info info;
    kr_xpdr_path_info_init(&info);
    info.type = aux->type;
    kr_aux_path_info_init(&info.aux_video_in);
    kr_xpdr_path_info_to_text(string, &info, sizeof(string));
    printk("Aux Port before: \n%s", string);
    ret = kr_patchset_apply(&info, &patchset);
    if (ret) printke("patchset apply");
    kr_xpdr_path_info_to_text(string, &info, sizeof(string));
    printk("Aux Port after: \n%s", string);
    kr_patchset_to_json(string, sizeof(string), &patchset);
    printk("Crate: \n%s", string);
  }
  if (!fail) {
    crate.patchset = patchset;
    kr_crate_attach_fd(&crate, aux->in_fd);
    kr_crate_attach_fd(&crate, aux->out_fd);
    kr_crate_attach_fd(&crate, kr_pool_fd(aux->pool));
    snprintf(crate.address, sizeof(crate.address), "/xpdr/%s/%s",
     params->adapter, params->port);
    fail = kr_crate_send(client, &crate);
  }
  if (!fail) {
    return aux;
  }
  err = errno;
  fprintf(stderr, "Aux: %s\n", strerror(err));
  aux_close(aux);
  return NULL;
}

int kr_aux_frame_acquire(kr_aux *aux, kr_frame *frame) {
  int frameno;
  if (!aux || !frame) return -1;
  frameno = kr_seq_pos(aux->seq);
  kr_seq_advance(aux->seq);
  *frame = aux->frame[frameno];
  return 0;
}

int kr_aux_frame_release(kr_aux *aux, kr_frame *frame) {
  int ret;
  if (!aux || !frame) return -1;
  ret = -666;
  //  ret = kr_give_notice(aux->out_fd);
  return ret;
}

int kr_aux_frame_read(kr_aux *aux, kr_frame *frame) {
  int ret;
  if (!aux || !frame) return -1;
  ret = kr_take_notice(aux->in_fd);
  return ret;
}

int kr_aux_frame_write(kr_aux *aux, kr_frame *frame) {
  int ret;
  if (!aux || !frame) return -1;
  ret = kr_give_notice(aux->out_fd);
  return ret;
}

int kr_aux_audio_acquire(kr_aux *aux, kr_audio *audio) {
  int pos;
  if (!aux || !audio) return -1;
  pos = kr_seq_pos(aux->seq);
  kr_seq_advance(aux->seq);
  *audio = aux->audio[pos];
  //printk("Aux audio aquire %d", pos);
  return 0;
}

int kr_aux_audio_read(kr_aux *aux, kr_audio *audio) {
  int ret;
  if (!aux || !audio) return -1;
  ret = kr_take_notice(aux->in_fd);
  //printk("Aux audio read");
  return ret;
}

int kr_aux_audio_write(kr_aux *aux, kr_audio *audio) {
  int ret;
  if (!aux || !audio) return -1;
  ret = kr_give_notice(aux->out_fd);
  //printk("Aux audio write");
  return ret;
}
