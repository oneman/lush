static int dispatch_studio(void *user, kr_crate *crate);
static int dispatch_mcx(void *user, kr_crate *crate);

static int dispatch_limits(void *user, kr_crate *crate) {
  printk("KR LIMITS dispatch!");
  if (crate->method == KR_GET) {
    crate->type = KR_LIMITS_INFO;
    memcpy(&crate->info.limits_info, &kr_limits, sizeof(kr_limits_info));
    return KR_INFORM;
  }
  return KR_BAD_REQUEST;
}

static int dispatch_info(void *user, kr_crate *crate) {
  int ret;
  int len;
  int names;
  char *component;
  kr_radio *radio;
  kr_path *path;
  printk("Radio INFO dispatch!");
  if (!user || !crate) return -1;
  radio = (kr_radio *)user;
  if (!radio) return KR_BAD_REQUEST;
  kr_path_alloca(path);
  ret = kr_path_parse(path, crate->address, strlen(crate->address));
  if (ret) return KR_BAD_REQUEST;
  len = kr_path_cur_name(path, &component);
  if (len < 1) return KR_BAD_REQUEST;
  if ((len != strlen("info"))
   || (memcmp(component, "info", len) != 0)) return KR_NONE;
  names = kr_path_steps_ahead(path);
  if (names > 0) {
    printke("RADIO INFO Dispatch: Wrong path components");
    return KR_BAD_REQUEST;
  }
  if (crate->method != KR_GET && crate->method != KR_PATCH) {
    return KR_BAD_REQUEST;
  }
  if (crate->method == KR_GET) {
    crate->type = KR_RADIO_INFO;
    ret = radio_get_info(radio, &crate->info.radio_info);
    if (ret) return KR_ERROR;
    return KR_INFORM;
  } else if (crate->method == KR_PATCH) {
    ret = radio_ctl(radio, &crate->patchset);
    if (ret) return KR_ERROR;
    return KR_OK;
  }
  return KR_ERROR;
}

static int dispatch_api_mounts(void *user, kr_crate *crate) {
  int ret;
  int len;
  int names;
  char *component;
  kr_radio *radio;
  kr_path *path;
  kr_path_list *list;
  printk("API Mounts dispatch!");
  if (!user || !crate) return -1;
  radio = (kr_radio *)user;
  if (!radio) return KR_BAD_REQUEST;
  kr_path_alloca(path);
  ret = kr_path_parse(path, crate->address, strlen(crate->address));
  if (ret) return KR_BAD_REQUEST;
  len = kr_path_cur_name(path, &component);
  if (len < 1) return KR_BAD_REQUEST;
  if ((len != strlen("api"))
   || (memcmp(component, "api", len) != 0)) return KR_NONE;
  names = kr_path_steps_ahead(path);
  if (names > 0) {
    printke("API Mounts Dispatch: Wrong path components");
    return KR_BAD_REQUEST;
  }
  if (crate->method != KR_GET) return KR_BAD_REQUEST;
  crate->type = KR_PATH_LIST;
  list = &crate->info.path_list;
  names_to_path_list(list, radio->api.mounts, KR_MOUNT_DISPATCH, "");
  return KR_INFORM;
}

static int dispatch_studio(void *user, kr_crate *crate) {
  int ret;
  int len;
  int names;
  uint8_t stash;
  uint8_t tracker;
  char *component;
  char *path_name;
  kr_radio *radio;
  kr_path *path;
  kr_name_ctx *ctx;
  kr_path_list *list;
  kr_name *new_name;
  kr_name name;
  kr_studio_path *sp;
  kr_studio_path_info info;
  kr_image_stash_item *item;
  kr_track *track;
  if (!user || !crate) return -1;
  component = NULL;
  ctx = NULL;
  path_name = NULL;
  new_name = NULL;
  sp = NULL;
  stash = 0;
  tracker = 0;
  radio = (kr_radio *)user;
  if (!radio) return KR_BAD_REQUEST;
  memset(&name, 0, sizeof(name));
  kr_path_alloca(path);
  ret = kr_path_parse(path, crate->address, strlen(crate->address));
  if (ret) return KR_BAD_REQUEST;
  len = kr_path_cur_name(path, &component);
  if (len < 1) return KR_BAD_REQUEST;
  if ((len != strlen("studio"))
   || (memcmp(component, "studio", len) != 0)) return KR_NONE;
  names = kr_path_steps_ahead(path);
  if (names > 2) {
    printke("Studio Dispatch: Wrong path components");
    return KR_BAD_REQUEST;
  }
  if (crate->method == KR_PUT && names < 1) return KR_BAD_REQUEST;
  if (names > 0) {
    kr_path_step(path);
    len = kr_path_cur_name(path, &path_name);
    if (len == 5 && !memcmp(path_name, "stash", 5)) {
      stash = 1;
    } else if (len == 7 && !memcmp(path_name, "tracker", 7)) {
      tracker = 1;
    }
    if (tracker || stash) {
      if (crate->method == KR_PUT) return KR_CONFLICT;
    } else {
      ctx = kr_name_resolve_type(radio->api.names, KR_STUDIO, path_name, len);
      if (crate->method == KR_PUT) {
        if (ctx) return KR_CONFLICT;
        name.len = len;
        memcpy(name.buf, path_name, name.len);
        new_name = kr_name_add(radio->api.names, &name);
        if (!new_name) return KR_ERROR;
      } else {
        if (!ctx) return KR_NOT_FOUND;
        sp = (kr_studio_path *)ctx->user;
      }
    }
  }
  switch (crate->method) {
    case KR_GET:
      printkd("Studio Dispatch: GET");
      if (names == 0) {
        crate->type = KR_PATH_LIST;
        list = &crate->info.path_list;
        names_to_path_list(list, radio->api.names, KR_STUDIO, "studio");
        return KR_INFORM;
      } else if (names == 1) {
        if (stash) {
          crate->type = KR_PATH_LIST;
          list = &crate->info.path_list;
          names_to_path_list(list, radio->api.names, KR_IMAGE_STASH, "studio/stash");
          return KR_INFORM;
        } else if (tracker) {
          crate->type = KR_PATH_LIST;
          list = &crate->info.path_list;
          tracker_track_list(list, radio->tracker);
          return KR_INFORM;
        }
        ret = kr_studio_path_info_get(sp, &crate->info.studio_path_info);
        if (!ret) crate->type = KR_STUDIO_PATH_INFO;
        return KR_INFORM;
      } else if (names == 2) {
        kr_path_step(path);
        len = kr_path_cur_name(path, &path_name);
        if (stash) {
          item = kr_image_stash_find_by_id(radio->stash, path_name, len);
          if (!item) return KR_NOT_FOUND;
          crate->type = KR_IMAGE_STASH_ITEM_INFO;
          kr_image_stash_item_info_get(item, &crate->info.image_stash_item_info);
        } else if (tracker) {
          track = kr_track_find_by_name(radio->tracker, path_name, len);
          if (!track) return KR_NOT_FOUND;
          crate->type = KR_TRACK_INFO;
          kr_track_info_get(track, &crate->info.track_info);
        }
        return KR_INFORM;
      }
      break;
    case KR_PUT:
      printkd("Studio Dispatch: PUT");
      if (crate->info.studio_path_info.type == KR_STUDIO_DEMUX) {
        return KR_ERROR;
      }
      if (crate->info.studio_path_info.type == KR_STUDIO_OUTPUT) {
        ctx = kr_name_resolve_type(radio->api.names, KR_STUDIO,
         crate->info.studio_path_info.output.input.name,
          crate->info.studio_path_info.output.input.len);
        if (!ctx) return KR_NOT_FOUND;
        sp = (kr_studio_path *)ctx->user;
        ret = kr_studio_connect(sp,
         &crate->info.studio_path_info, new_name);
      } else {
        ret = kr_studio_add(radio->studio,
         &crate->info.studio_path_info, new_name);
      }
      if (!ret) {
        return KR_CREATED;
      } else {
        kr_name_remove(radio->api.names, new_name);
        return KR_ERROR;
      }
      break;
    case KR_POST:
      printkd("Studio Dispatch: POST");
      if (names == 2) {
        kr_path_step(path);
        len = kr_path_cur_name(path, &component);
        if (len < 1) return KR_BAD_REQUEST;
        if (len == 6 && !memcmp(component, "decode", 6)) {
          kr_studio_path_info_get(sp, &info);
          if (info.type != KR_STUDIO_SOURCE) return KR_BAD_REQUEST;
          printk("Radio: studio wants to decode source %s!", path_name);
          ret = kr_studio_source_decode_image(sp);
          if (ret != 0) return KR_ERROR;
          return KR_OK;
        }
      }
      break;
    case KR_PATCH:
      printkd("Studio Dispatch: PATCH");
      ret = kr_studio_ctl(sp, &crate->patchset);
      if (ret) return KR_ERROR;
      return KR_OK;
      break;
    case KR_DELETE:
      printkd("Studio Dispatch: DELETE");
      if (stash && names == 2) {
        kr_path_step(path);
        len = kr_path_cur_name(path, &path_name);
        item = kr_image_stash_find_by_id(radio->stash, path_name, len);
        if (!item) return KR_NOT_FOUND;
        kr_image_stash_del(radio->stash, item);
        return KR_OK;
      }
      ret = kr_studio_remove(sp);
      if (ret) return KR_ERROR;
      return KR_OK;
      break;
    default:
      printke("Studio Dispatch: no useful method");
      return KR_BAD_REQUEST;
  }
  return KR_ERROR;
}

static int dispatch_mcx(void *user, kr_crate *crate) {
  int ret;
  int len;
  char *name[2];
  int name_len[2];
  int names;
  void *ctx;
  int type;
  int perm;
  kr_path_list *list;
  kr_name_ctx *found[2];
  kr_name_ctx *dfound;
  kr_name *reverse[2];
  char *component;
  kr_method meth;
  const char *mcx[] = {
    [KR_MIXER] = "mixer",
    [KR_COMPOSITOR] = "compositor",
    [KR_XPDR] = "xpdr"
  };
  kr_component com;
  kr_path *path;
  kr_radio *radio;
  kr_name *result;
  kr_name *result2;
  kr_name new;
  result = NULL;
  result2 = NULL;
  found[0] = NULL;
  found[1] = NULL;
  dfound = NULL;
  name[0] = NULL;
  name[1] = NULL;
  name_len[0] = 0;
  name_len[1] = 0;
  ret = 0;
  perm = 0;
  com = 0;
  type = 0;
  radio = (kr_radio *)user;
  if (!radio) return KR_BAD_REQUEST;
  kr_path_alloca(path);
  ret = kr_path_parse(path, crate->address, strlen(crate->address));
  len = kr_path_cur_name(path, &component);
  /* Ok this is the part were we double check the path to make sure is mcx */
  if (len < 1) return KR_BAD_REQUEST;
  if ((len == strlen(mcx[KR_MIXER]))
   && (memcmp(component, mcx[KR_MIXER], len) == 0)) {
    com = KR_MIXER;
  }
  if ((len == strlen(mcx[KR_COMPOSITOR]))
   && (memcmp(component, mcx[KR_COMPOSITOR], len) == 0)) {
    com = KR_COMPOSITOR;
  }
  if ((len == strlen(mcx[KR_XPDR]))
   && (memcmp(component, mcx[KR_XPDR], len) == 0)) {
    com = KR_XPDR;
  }
  if (com) {
    printkd("Radio: DISPATCHED mcx %s!", component);
  } else {
    /* printke("MCX: Not a mixer/comp/xpdr crate"); */
    return KR_NONE;
  }
  meth = crate->method;
  names = kr_path_steps_ahead(path);
  if (names > 2) {
    printke("MCX: Too many names");
    return KR_BAD_REQUEST;
  }
  /* Ok this is the part where we see if the names in the path exist */
  printkd("MCX Dispatch: %s %.*s names: %d", kr_method_to_str(meth), len,
   component, names);
  if (names > 0) {
    kr_path_step(path);
    name_len[0] = kr_path_cur_name(path, &name[0]);
    printkd("MCX: name 1: %.*s", name_len[0], name[0]);
    kr_path_step(path);
    found[0] = kr_name_resolve(radio->api.names, name[0], name_len[0]);
    if (names == 2) {
      name_len[1] = kr_path_cur_name(path, &name[1]);
      printkd("MCX: name 2: %.*s", name_len[1], name[1]);
      found[1] = kr_name_resolve(radio->api.names, name[1], name_len[1]);
    }
  }
  if (meth == KR_PUT && names < 1) return KR_BAD_REQUEST;
  /* Ok so, check the datatype for the following method / path combo */
  if (meth == KR_PUT && names == 1) {
    if (found[0] != NULL) return KR_CONFLICT;
    //really needs to check deeper for mixer bus
    if ((com == KR_MIXER) && (crate->type != KR_MIXER_PATH_INFO)) {
      printke("MCX: Wrong type for mixer bus create");
    }
    //really needs to check deeper for compositor bus
    if ((com == KR_COMPOSITOR) && (crate->type != KR_COMPOSITOR_PATH_INFO)) {
      printke("MCX: Wrong type for compositor bus create");
      return KR_BAD_REQUEST;
    }
    //really needs to check deeper for adapter path type
    if ((com == KR_XPDR) && (crate->type != KR_XPDR_PATH_INFO)) {
      printke("MCX: Wrong type for xpdr path open");
      return KR_BAD_REQUEST;
    }
    memcpy(new.buf, name[0], name_len[0]);
    new.len = name_len[0];
    result = kr_name_add(radio->api.names, &new);
    if (com == KR_MIXER) {
      ret = kr_mixer_bus(radio->mixer, &crate->info.mixer_path_info, result);
    }
    if (com == KR_COMPOSITOR) {
      ret = kr_compositor_bus(radio->compositor,
       &crate->info.compositor_path_info, result);
    }
    if (com == KR_XPDR) {
      ret = kr_xpdr_open(radio->xpdr, &crate->info.xpdr_path_info, result);
    }
  }
  if ((meth == KR_PUT) && (names == 2)) {
    if (com == KR_XPDR) {
      if (found[1] != NULL) return KR_CONFLICT;
      if (found[0] != NULL && found[0]->type == com) {
        memcpy(new.buf, name[1], name_len[1]);
        new.len = name_len[1];
        result2 = kr_name_add(radio->api.names, &new);
        reverse[0] = kr_name_reverse(radio->api.names, found[0]->user);
        result = kr_name_attach(radio->api.names, reverse[0], result2);
        ret = kr_xpdr_link(found[0]->user, &crate->info.xpdr_path_info, result, result2);
      }
    }
    if (com == KR_MIXER || com == KR_COMPOSITOR) {
      if (found[0] == NULL || found[1] == NULL) return KR_CONFLICT;
      if (found[0]->type == com && found[1]->type == com) {
        reverse[0] = kr_name_reverse(radio->api.names, found[0]->user);
        reverse[1] = kr_name_reverse(radio->api.names, found[1]->user);
        result = kr_name_attach(radio->api.names, reverse[0], reverse[1]);
        if (com == KR_MIXER) {
          ret = kr_mixer_link(found[0]->user, found[1]->user,
           &crate->info.mixer_path_info, result);
        }
        if (com == KR_COMPOSITOR) {
          ret = kr_compositor_link(found[0]->user, found[1]->user,
           &crate->info.compositor_path_info, result);
        }
      }
    }
  }
  if (meth == KR_PUT) {
    if (ret) {
      if (result != NULL) kr_name_remove(radio->api.names, result);
      return KR_ERROR;
    }
    /* Ok it worked, we put stuff */
    return KR_CREATED;
  }
  if (meth == KR_GET && names == 0) {
    crate->type = KR_PATH_LIST;
    list = &crate->info.path_list;
    names_to_path_list(list, radio->api.names, com, (char *)mcx[com]);
    return KR_INFORM;
  }
  if ((meth == KR_GET) || (meth == KR_PATCH) || (meth == KR_DELETE)) {
    if (!found[0] || (names == 2 && found[1] == NULL)) return KR_NOT_FOUND;
    if (names == 1) {
      ctx = found[0]->user;
      type = found[0]->type;
      perm = found[0]->perm;
    } else if (names == 2) {
      dfound = kr_name_resolve2(radio->api.names, name[0], name_len[0], name[1],
       name_len[1]);
      if (!dfound) return KR_NOT_FOUND;
      ctx = dfound->user;
      type = dfound->type;
      perm = dfound->perm;
    }
    if (com != type) return KR_BAD_REQUEST;
    if (com == KR_MIXER) {
      if (meth == KR_GET) {
        ret = kr_mixer_path_info_get(ctx, &crate->info.mixer_path_info);
        if (!ret) crate->type = KR_MIXER_PATH_INFO;
      }
      if (meth == KR_PATCH) {
        ret = kr_mixer_ctl(ctx, &crate->patchset);
      }
      if (meth == KR_DELETE) {
        if (perm == KR_NAME_NO_DELETE) {
          ret = -1;
        } else {
          ret = kr_mixer_remove(ctx);
        }
      }
    } else if (com == KR_COMPOSITOR) {
      if (meth == KR_GET) {
        ret = kr_compositor_path_info_get(ctx, &crate->info.compositor_path_info);
        if (!ret) crate->type = KR_COMPOSITOR_PATH_INFO;
      }
      if (meth == KR_PATCH) {
        ret = kr_compositor_ctl(ctx, &crate->patchset);
      }
      if (meth == KR_DELETE) {
        if (perm == KR_NAME_NO_DELETE) {
          ret = -1;
        } else {
          ret = kr_compositor_remove(ctx);
        }
      }
    } else if (com == KR_XPDR) {
      if (meth == KR_GET) {
        ret = kr_xpdr_path_info_get(ctx, &crate->info.xpdr_path_info);
        if (!ret) crate->type = KR_XPDR_PATH_INFO;
      }
      if (meth == KR_PATCH) {
        ret = kr_xpdr_ctl(ctx, &crate->patchset);
      }
      if (meth == KR_DELETE) {
        ret = kr_xpdr_remove(ctx);
      }
    }
    if (ret != 0) return KR_ERROR;
    if (ret == 0) {
      if (meth == KR_GET) return KR_INFORM;
      return KR_OK;
    }
  }
  return KR_NONE;
}
