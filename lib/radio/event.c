static int radio_event(kr_radio_event *e);
static int track_event(kr_track_event *event);
static int stash_event(kr_image_stash_event *event);
static int studio_event(kr_studio_event *event);
static int xfer_event(kr_xfer_event *event);
static int web_event(kr_web_event *event);
static void mixer_event(kr_mixer_event *event);
static void compositor_event(kr_compositor_event *event);
static void xpdr_event(kr_xpdr_event *event);
static int media_event(kr_media_event *event);
static int task_event(kr_task_event *event);
static int signal_event(kr_event *event);

static int radio_event(kr_radio_event *e) {
  if (!e || e->type != KR_RADIO_PATCH) return -1;
  test_broadcast_patch(e->radio, "info", NULL, e->patchset);
  return 0;
}

static int track_event(kr_track_event *event) {
  kr_crate cr;
  kr_name new;
  kr_name *name;
  kr_track_info *info;
  kr_radio *radio;
  radio = (kr_radio *)event->user;
  if (!radio) return -1;
  info = &event->info;
  switch (event->type) {
    case KR_TRACK_ADD:
      printk("Radio: Tracker track add event");
      cr.method = KR_PUT;
      cr.disposition = KR_RESPONSE;
      cr.type = KR_TRACK_INFO;
      memcpy(cr.address, "/studio/tracker/", 16);
      memcpy(cr.address + 16, info->track.name, info->track.len);
      cr.addr_len = 16 + info->track.len;
      if (cr.addr_len < sizeof(cr.address)) {
        cr.info.track_info = *info;
        test_broadcast_create(radio, &cr);
      }
      break;
    case KR_TRACK_REMOVE:
      printk("Radio: Tracker track remove event");
      memcpy(new.buf, info->track.name, info->track.len);
      new.len = info->track.len;
      name = kr_name_add(radio->api.names, &new);
      if (!name) return KR_ERROR;
      test_broadcast_delete(radio, "studio/tracker", name);
      kr_name_remove(radio->api.names, name);
      break;
    default: break;
  }
  return 0;
}

static int stash_event(kr_image_stash_event *event) {
  kr_radio *radio;
  kr_name *name;
  kr_crate cr;
  kr_stash_image *image;
  kr_name new;
  kr_name_ctx ctx;
  radio = (kr_radio *)event->user;
  if (!radio) return -1;
  ctx.user = event->item;
  ctx.type = KR_IMAGE_STASH;
  ctx.perm = 0;
  name = kr_name_reverse(radio->api.names, event->item);
  if (name && event->type == KR_IMAGE_STASH_ADD) return KR_CONFLICT;
  if (!name && event->type != KR_IMAGE_STASH_ADD) return KR_ERROR;
  switch (event->type) {
    case KR_IMAGE_STASH_ADD:
      printk("Radio: Image stash item add event");
      new.len = kr_image_stash_item_uid(event->item, new.buf, sizeof(new.buf));
      name = kr_name_add(radio->api.names, &new);
      if (!name) return KR_ERROR;
      kr_name_link(radio->api.names, name, &ctx);
      cr.method = KR_PUT;
      cr.disposition = KR_RESPONSE;
      cr.type = KR_IMAGE_STASH_ITEM_INFO;
      memcpy(cr.address, "/studio/stash/", 14);
      cr.addr_len = 14;
      cr.addr_len += kr_name_snprint(name, cr.address + 14,
       sizeof(cr.address) - 14);
      if (cr.addr_len < sizeof(cr.address)) {
        cr.info.image_stash_item_info = event->info;
        test_broadcast_create(radio, &cr);
      }
      break;
    case KR_IMAGE_STASH_REMOVE:
      printk("Radio: Image stash item remove event");
      kr_name_unlink(radio->api.names, name, &ctx);
      kr_name_remove(radio->api.names, name);
      test_broadcast_delete(radio, "studio/stash", name);
      break;
    case KR_IMAGE_STASH_ENCODE:
      printk("Radio: Image stash item encode event");
      image = kr_image_stash_item_image(event->item);
      printk("encoded sz %zu", image->enc.info.sz);
      break;
    case KR_IMAGE_STASH_DECODE:
      printk("Radio: Image stash item decode event");
      image = kr_image_stash_item_image(event->item);
      printk("decoded %dx%d img", image->raw.info.w, image->raw.info.h);
      break;
    default: break;
  }
  return 0;
}

static int studio_event(kr_studio_event *event) {
  kr_radio *radio;
  kr_name *name;
  kr_name_ctx ctx;
  kr_crate cr;
  radio = (kr_radio *)event->user;
  if (!radio) return -1;
  ctx.user = event->path;
  ctx.type = KR_STUDIO;
  ctx.perm = 0;
  name = (kr_name *)event->user_path;
  if (event->type == KR_STUDIO_ADD && event->info.type == KR_STUDIO_DEMUX) {
    name = kr_name_add(radio->api.names, name);
    event->user_path = name;
  }
  switch (event->type) {
    case KR_STUDIO_ADD:
      printk("Radio: Studio path create event");
      kr_name_link(radio->api.names, name, &ctx);
      cr.method = KR_PUT;
      cr.disposition = KR_RESPONSE;
      cr.type = KR_STUDIO_PATH_INFO;
      memcpy(cr.address, "/studio/", 8);
      cr.addr_len = 8;
      cr.addr_len += kr_name_snprint(name, cr.address + 8,
       sizeof(cr.address) - 8);
      if (cr.addr_len < sizeof(cr.address)) {
        cr.info.studio_path_info = event->info;
        test_broadcast_create(radio, &cr);
      }
      break;
    case KR_STUDIO_PATCH:
      printk("Radio: Studio path patch event");
      test_broadcast_patch(radio, "studio", name, event->patchset);
      break;
    case KR_STUDIO_REMOVE:
      printk("Radio: Studio path delete event %.*s", name->len, name->buf);
      test_broadcast_delete(radio, "studio", name);
      kr_name_unlink(radio->api.names, name, &ctx);
      break;
    default:
      printke("Radio: Bad event from studio");
      break;
  }
  return 0;
}

static int xfer_event(kr_xfer_event *event) {
  int ret;
  kr_radio *radio;
  kr_web_path *web_path;
  ret = 0;
  radio = (kr_radio *)event->user;
  if (!radio) return -1;
  switch (event->type) {
    case KR_XFER_CREATE:
      printkd("Radio: XFER Create event");
      if (event->info.type == KR_XFER_CLIENT) {
        web_path = event->user_path;
        if (web_path) {
          kr_web_path_user_set(web_path, event->path);
          kr_web_path_user_type_set(web_path, KR_RADIO_XFER);
        }
      }
      break;
    case KR_XFER_PATCH:
      printkd("Radio: XFER Patch event");
      break;
    case KR_XFER_DESTROY:
      printkd("Radio: XFER Destroy event");
      if (event->info.type == KR_XFER_CLIENT) {
        web_path = event->user_path;
        if (web_path) {
          kr_web_path_user_set(web_path, NULL);
          kr_web_path_user_type_set(web_path, KR_RADIO_UNKNOWN);
        }
      }
      break;
    default:
      printkd("Radio: Some XFER event");
  }
  return ret;
}

static int deck_proxy(kr_radio *r, kr_web_path *path) {
  int ret;
  char *name;
  int i;
  kr_deck *deck;
  kr_web_path_info *info;
  kr_web_client *client;
  kr_http_request *req;
  kr_path *addr;
  name = NULL;
  info = kr_web_path_info_get(path);
  if (info->type != KR_WEB_CLIENT) return 0;
  client = kr_web_path_get_client(path);
  req = kr_web_client_get_request(client);
  kr_path_alloca(addr);
  kr_path_parse(addr, req->req_line.path, req->req_line.path_len);
  ret = kr_path_cur_name(addr, &name);
  if (ret != 5) return 0;
  for (i = 0; i < KR_NDECKS; i++) {
    deck = &r->deck[i];
    if (deck->type == KR_DECK_XMMS) {
      if (kr_proxy_accept(deck->xmms.proxy, path, addr)) return 1;
    }
  }
  return 0;
}

static int web_event(kr_web_event *event) {
  int ret;
  kr_radio *radio;
  radio_client *rc;
  radio_client_setup rc_setup;
  kr_web_path_info *info;
  kr_web_client *client;
  kr_http_request *req;
  kr_path *addr;
  kr_radio_web_path_type type;
  kr_xfer_path *xf;
  client = NULL;
  rc = NULL;
  xf = NULL;
  type = KR_RADIO_UNKNOWN;
  radio = (kr_radio *)event->user;
  if (radio == NULL) return -1;
  info = kr_web_path_info_get(event->path);
  type = kr_web_path_user_type_get(event->path);
  if (info->type == KR_WEB_CLIENT) {
    client = kr_web_path_get_client(event->path);
  }
  ret = -1;
  switch (event->type) {
    case KR_WEB_OPEN:
      printkd("Radio: Open event from InterWeb");
      ret = 0;
      break;
    case KR_WEB_CLOSE:
      printkd("Radio: Close event from InterWeb");
      if (info->type != KR_WEB_CLIENT) break;
      if (type == KR_RADIO_API) {
        rc = kr_web_path_user_get(event->path);
        if (rc) {
          ret = client_destroy(rc);
          if (ret) printke("Radio: client destroy error %d", ret);
        }
      }
      if (type == KR_RADIO_XFER) {
        xf = kr_web_path_user_get(event->path);
        kr_xfer_remove(xf);
      }
      ret = 0;
      break;
    case KR_WEB_HEADERS:
      printkd("Radio: Headers event from InterWeb");
      req = kr_web_client_get_request(client);
      kr_path_alloca(addr);
      kr_path_parse(addr, req->req_line.path, req->req_line.path_len);
      if (info->client.protocol == KR_PROTOCOL_HTTP) {
        ret = radio_api(&radio->api, addr);
      }
      if (ret || info->client.protocol == KR_PROTOCOL_WEBSOCKET) {
        if (info->client.ws.proto == KR_WS_PROTOCOL_STREAM) {
          req->req_line.method = KR_HTTP_PUT;
        }
        if (kr_xfer_match(radio->xfer, req->req_line.method, addr)) {
          ret = kr_xfer_accept(radio->xfer, client, event->path);
          if (ret == 0) {
            xf = kr_web_path_user_get(event->path);
            ret = kr_xfer_headers(xf);
            if (ret == 0) {
              if (kr_xfer_done(xf)) {
                kr_xfer_remove(xf);
              }
              break;
            }
          }
        }
        if (type == KR_RADIO_API) {
          rc = kr_web_path_user_get(event->path);
        }
        if (!rc) {
          if (deck_proxy(radio, event->path)) {
            printk("Deck proxy!");
            ret = 0;
            break;
          }
          printkd("Not deck proxy");
          memset(&rc_setup, 0, sizeof(rc_setup));
          rc_setup.radio = radio;
          rc_setup.info.type = KR_RADIO_CLIENT_WEB;
          rc_setup.info.web = info->client;
          rc = client_create(&rc_setup);
          rc->path = event->path;
          kr_web_path_user_set(event->path, rc);
          kr_web_path_user_type_set(event->path, KR_RADIO_API);
        }
        if (info->client.protocol == KR_PROTOCOL_HTTP) {
          memset(&rc->crate, 0, sizeof(rc->crate));
          req = kr_web_client_get_request(client);
          ret = kr_http_request_to_crate(&rc->crate, req);
          if ((req->req_line.method == KR_HTTP_GET)
           || (req->req_line.method == KR_HTTP_DELETE)) {
            client_crate(rc, &rc->crate);
          }
        }
        ret = 0;
        break;
      } else {
        if (type == KR_RADIO_API) {
          rc = kr_web_path_user_get(event->path);
          if (rc) {
            ret = client_destroy(rc);
            if (ret) printke("Radio: client destroy error %d", ret);
            kr_web_path_user_set(event->path, NULL);
            kr_web_path_user_type_set(event->path, KR_RADIO_UNKNOWN);
          }
        }
        if (type == KR_RADIO_XFER) {
          xf = kr_web_path_user_get(event->path);
        }
        if (!xf) {
          ret = kr_xfer_accept(radio->xfer, client, event->path);
          if (ret == 0) {
            xf = kr_web_path_user_get(event->path);
            ret = kr_xfer_headers(xf);
            if (ret == 0) {
              if (kr_xfer_done(xf)) {
                kr_xfer_remove(xf);
              }
              break;
            }
          }
        } else {
          ret = kr_xfer_headers(xf);
          if (ret == 0) {
            if (kr_xfer_done(xf)) {
              kr_xfer_remove(xf);
            }
            break;
          }
        }
      }
      radio_404(client);
      ret = 0;
      break;
    case KR_WEB_DATA:
      if (info->type != KR_WEB_CLIENT) break;
      printkd("Radio: Data event from InterWeb");
      if (type == KR_RADIO_API) {
        rc = kr_web_path_user_get(event->path);
        if (rc) {
          client_event(rc);
          ret = 0;
        } else {
          ret = -1;
          printke("Radio: Hrm weird connection");
        }
      }
      if (type == KR_RADIO_XFER) {
        xf = kr_web_path_user_get(event->path);
        ret = kr_xfer_data(xf);
        if (ret < 0) radio_404(client);
        if (kr_xfer_done(xf)) {
          kr_xfer_remove(xf);
        }
      }
      break;
    default:
      printke("Radio: Unknown event from InterWeb");
      ret = -1;
      break;
  }
  return ret;
}

static int queued_broadcast_event(kr_event *e) {
  int ret;
  kr_radio *r;
  kr_broadcast_data *bd;
  kr_queue_item item;
  static int count = 0;
  if (!e) return -1;
  r = (kr_radio *)e->user;
  if (!r) return -1;
  memset(&item, 0, sizeof(item));
  ret = kr_queue_read(r->events, &item);
  if (ret == 1) {
    //printk("Queue read num %d %d com %d %p", count, ret, item.user, item.ptr);
    bd = (kr_broadcast_data *)item.ptr;
    test_broadcast_patch2(r, bd);
    kr_pool_atomic_release(r->broadcasts, item.ptr);
    count++;
  }
  return 0;
}

static void mixer_event(kr_mixer_event *event) {
  kr_radio *radio;
  kr_name *name;
  kr_crate cr;
  kr_name_ctx ctx;
  kr_mixer_path_info *info;
  radio = (kr_radio *)event->user;
  if (!radio) return;
  ctx.user = event->path;
  ctx.type = KR_MIXER;
  ctx.perm = 0;
  name = (kr_name *)event->user_path;
  info = &event->info;
  switch (event->type) {
    case KR_MIXER_CREATE:
      printkd("Radio: Mixer path create event");
      if (info->type == KR_MXR_SOURCE || info->type == KR_MXR_OUTPUT) {
        ctx.perm = KR_NAME_NO_DELETE;
      }
      kr_name_link(radio->api.names, name, &ctx);
      cr.method = KR_PUT;
      cr.disposition = KR_RESPONSE;
      cr.type = KR_MIXER_PATH_INFO;
      memcpy(cr.address, "/mixer/", 7);
      cr.addr_len = 7;
      cr.addr_len += kr_name_snprint(name, cr.address + 7,
       sizeof(cr.address) - 7);
      if (cr.addr_len < sizeof(cr.address)) {
        cr.info.mixer_path_info = event->info;
        test_broadcast_create(radio, &cr);
      }
      break;
    case KR_MIXER_PATCH:
      printkd("Radio: Mixer path patch event");
      test_broadcast_patch(radio, "mixer", name, event->patchset);
      break;
    case KR_MIXER_METER:
      printkd("Radio: Mixer meter event");
      queue_broadcast_patch(radio, "mixer", name, event->patchset);
      break;
    case KR_MIXER_DESTROY:
      printkd("Radio: Mixer path delete event");
      test_broadcast_delete(radio, "mixer", name);
      kr_name_unlink(radio->api.names, name, &ctx);
      break;
    default:
      printke("Radio: Bad event from mixer");
      break;
  }
}

static void compositor_event(kr_compositor_event *event) {
  kr_radio *radio;
  kr_name *name;
  kr_crate cr;
  kr_name_ctx ctx;
  kr_compositor_path_info *info;
  radio = (kr_radio *)event->user;
  if (!radio) return;
  ctx.user = event->path;
  ctx.type = KR_COMPOSITOR;
  ctx.perm = 0;
  name = (kr_name *)event->user_path;
  info = &event->info;
  switch (event->type) {
    case KR_COM_CREATE:
      printkd("Radio: Compositor path create event");
      if (info->type == KR_COM_SOURCE || info->type == KR_COM_OUTPUT) {
        ctx.perm = KR_NAME_NO_DELETE;
      }
      kr_name_link(radio->api.names, name, &ctx);
      cr.method = KR_PUT;
      cr.disposition = KR_RESPONSE;
      cr.type = KR_COMPOSITOR_PATH_INFO;
      memcpy(cr.address, "/compositor/", 12);
      cr.addr_len = 12;
      cr.addr_len += kr_name_snprint(name, cr.address + 12,
       sizeof(cr.address) - 12);
      if (cr.addr_len < sizeof(cr.address)) {
        cr.info.compositor_path_info = event->info;
        test_broadcast_create(radio, &cr);
      }
      break;
    case KR_COM_PATCH:
      printkd("Radio: Compositor path patch event");
      test_broadcast_patch(radio, "compositor", name, event->patchset);
      break;
    case KR_COM_DESTROY:
      printkd("Radio: Compositor path delete event");
      test_broadcast_delete(radio, "compositor", name);
      kr_name_unlink(radio->api.names, name, &ctx);
      break;
    default:
      printke("Radio: Bad event from compositor");
      break;
  }
}

static void xpdr_event(kr_xpdr_event *event) {
  kr_radio *radio;
  kr_name *name;
  kr_crate cr;
  kr_name_ctx ctx;
  radio = (kr_radio *)event->user;
  if (!radio) return;
  ctx.user = event->path;
  ctx.type = KR_XPDR;
  ctx.perm = 0;
  name = (kr_name *)event->user_path;
  switch (event->type) {
    case KR_XPDR_CREATE:
      printkd("Radio: XPDR path create event");
      kr_name_link(radio->api.names, name, &ctx);
      cr.method = KR_PUT;
      cr.disposition = KR_RESPONSE;
      cr.type = KR_XPDR_PATH_INFO;
      memcpy(cr.address, "/xpdr/", 6);
      cr.addr_len = 6;
      cr.addr_len += kr_name_snprint(name, cr.address + 6,
       sizeof(cr.address) - 6);
      if (cr.addr_len < sizeof(cr.address)) {
        cr.info.xpdr_path_info = event->info;
        test_broadcast_create(radio, &cr);
      }
      break;
    case KR_XPDR_PATCH:
      printkd("Radio: XPDR path patch event");
      test_broadcast_patch(radio, "xpdr", name, event->patchset);
      break;
    case KR_XPDR_DESTROY:
      printkd("Radio: XPDR path delete event");
      test_broadcast_delete(radio, "xpdr", name);
      kr_name_unlink(radio->api.names, name, &ctx);
      break;
    default:
      printke("Radio: Bad event from compositor");
      break;
  }
}

static int media_event(kr_media_event *event) {
  printk("media event %s happened!",
   kr_media_event_type_to_str(event->type));
  return 0;
}

static int task_event(kr_task_event *event) {
  printk("task event %s happened!",
   kr_task_event_type_to_str(event->type));
  return 0;
}

static int signal_event(kr_event *event) {
  ssize_t s;
  kr_radio *radio;
  struct signalfd_siginfo fdsi;
  radio = (kr_radio *)event->user;
  char *name = radio->info.callsign;
  for (;;) {
    s = read(event->fd, &fdsi, sizeof(fdsi));
    if ((s == -1) && (errno == EAGAIN)) break;
    if (s != sizeof(fdsi)) {
      failfast("%s signal: Error reading signalfd", name);
      exit(1);
    }
    switch (fdsi.ssi_signo) {
      case SIGHUP:
        printk("%s signal: Got HANGUP Signal!", name);
        break;
      case SIGINT:
        printk("\n%s: Got INT Signal!", name);
        printk("%s: Shutting down", name);
        return -1;
      case SIGTERM:
        printk("%s: Got TERM Signal!", name);
        printk("%s: Shutting down", name);
        return -1;
      default:
        printk("%s: Got Signal %u", name, fdsi.ssi_signo);
    }
  }
  return 0;
}
