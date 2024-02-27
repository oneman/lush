static void handle_seat_capabilities(void *data, struct wl_seat *seat,
 enum wl_seat_capability caps);
static void keyboard_handle_modifiers(void *data, struct wl_keyboard *keyboard,
 uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched,
 uint32_t mods_locked, uint32_t group);
static void keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard,
 uint32_t format, int fd, uint32_t size);
static void keyboard_handle_enter(void *data, struct wl_keyboard *keyboard,
 uint32_t serial, struct wl_surface *surface, struct wl_array *keys);
static void keyboard_handle_leave(void *data, struct wl_keyboard *keyboard,
 uint32_t serial, struct wl_surface *surface);
static void keyboard_handle_key(void *data, struct wl_keyboard *keyboard,
 uint32_t serial, uint32_t time, uint32_t key, uint32_t state_w);
static void pointer_handle_enter(void *data,
 struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface,
 wl_fixed_t sx_w, wl_fixed_t sy_w);
static void pointer_handle_leave(void *data,
 struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface);
static void pointer_handle_motion(void *data,
 struct wl_pointer *pointer, uint32_t time, wl_fixed_t sx_w, wl_fixed_t sy_w);
static void pointer_handle_button(void *data,
 struct wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button,
 uint32_t state_w);
static void pointer_handle_axis(void *data,
 struct wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value);

static void handle_seat_capabilities(void *data, struct wl_seat *seat,
 enum wl_seat_capability caps) {
  kr_wayland *kw;
  kw = (kr_wayland *)data;
  if ((caps & WL_SEAT_CAPABILITY_POINTER) && !kw->pointer) {
    kw->pointer = wl_seat_get_pointer(seat);
    /*wl_pointer_set_user_data(wayland->pointer, wayland);*/
    wl_pointer_add_listener(kw->pointer, &kw->pointer_listener, kw);
  } else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && kw->pointer) {
    wl_pointer_destroy(kw->pointer);
    kw->pointer = NULL;
  }
  if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !kw->keyboard) {
    kw->keyboard = wl_seat_get_keyboard(seat);
    /*wl_keyboard_set_user_data(wayland->keyboard, wayland);*/
    wl_keyboard_add_listener(kw->keyboard, &kw->keyboard_listener, kw);
  } else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD) && kw->keyboard) {
    wl_keyboard_destroy(kw->keyboard);
    kw->keyboard = NULL;
  }
}

static void keyboard_handle_modifiers(void *data, struct wl_keyboard *keyboard,
 uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched,
 uint32_t mods_locked, uint32_t group) {
  /* Nothing here */
}

static void keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard,
 uint32_t format, int fd, uint32_t size) {
  kr_wayland *wayland = data;
  char *map_str;
  if (!data) {
    close(fd);
    return;
  }
  if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
    close(fd);
    return;
  }
  map_str = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
  if (map_str == MAP_FAILED) {
    close(fd);
    return;
  }
  wayland->xkb.keymap = xkb_map_new_from_string(wayland->xkb.context,
   map_str, XKB_KEYMAP_FORMAT_TEXT_V1, 0);
  munmap(map_str, size);
  close(fd);
  if (!wayland->xkb.keymap) {
    fprintf(stderr, "failed to compile keymap\n");
    return;
  }
  wayland->xkb.state = xkb_state_new(wayland->xkb.keymap);
  if (!wayland->xkb.state) {
    fprintf(stderr, "failed to create XKB state\n");
    xkb_map_unref(wayland->xkb.keymap);
    wayland->xkb.keymap = NULL;
    return;
  }
  wayland->xkb.control_mask =
    1 << xkb_map_mod_get_index(wayland->xkb.keymap, "Control");
  wayland->xkb.alt_mask =
    1 << xkb_map_mod_get_index(wayland->xkb.keymap, "Mod1");
  wayland->xkb.shift_mask =
    1 << xkb_map_mod_get_index(wayland->xkb.keymap, "Shift");
}

static void keyboard_handle_enter(void *data, struct wl_keyboard *keyboard,
 uint32_t serial, struct wl_surface *surface, struct wl_array *keys) {
  int i;
  kr_wayland *wayland;
  wayland = data;
  if (!surface) {
    return;
  }
  for (i = 0; i < KR_WL_MAX_WINDOWS; i++) {
    if (wayland->window[i].active == 1) {
      if (wayland->window[i].surface == surface) {
        if (wayland->key_window != &wayland->window[i]) {
          wayland->key_window = &wayland->window[i];
          break;
        }
      }
    }
  }
  if (i == KR_WL_MAX_WINDOWS) {
    return;
  }
}

static void keyboard_handle_leave(void *data, struct wl_keyboard *keyboard,
 uint32_t serial, struct wl_surface *surface) {
  int i;
  kr_wayland *wayland;
  wayland = data;
  for (i = 0; i < KR_WL_MAX_WINDOWS; i++) {
    if (wayland->window[i].active == 1) {
      if (wayland->window[i].surface == surface) {
        if (wayland->key_window == &wayland->window[i]) {
          wayland->key_window = NULL;
          break;
        }
      }
    }
  }
  if (i == KR_WL_MAX_WINDOWS) {
    return;
  }
}

static void keyboard_handle_key(void *data, struct wl_keyboard *keyboard,
 uint32_t serial, uint32_t time, uint32_t key, uint32_t state_w) {
  kr_wayland *wayland;
  kr_wayland_event wayland_event;
  kr_wayland_path *window;
  uint32_t code, num_syms;
  enum wl_keyboard_key_state state = state_w;
  const xkb_keysym_t *syms;
  xkb_keysym_t sym;
  int ret;
  //struct itimerspec its;
  //input->display->serial = serial;
  wayland = data;
  if (wayland->key_window == NULL) {
    return;
  } else {
    window = wayland->key_window;
  }
  code = key + 8;
  if (!wayland->xkb.state)
    return;
  num_syms = xkb_key_get_syms(wayland->xkb.state, code, &syms);
  sym = XKB_KEY_NoSymbol;
  if (num_syms == 1) {
    sym = syms[0];
  }
  if (((sym == XKB_KEY_F11) || (sym == XKB_KEY_f))
    && (state == WL_KEYBOARD_KEY_STATE_PRESSED)) {
    if (window->fullscreen == 1) {
      window->fullscreen = 0;
      //wl_shell_surface_set_toplevel(window->shell_surface);
    } else {
      window->fullscreen = 1;
      //wl_shell_surface_set_fullscreen(window->shell_surface, 1, 0, NULL);
    }
  }
/*
  if (sym == XKB_KEY_F5 && input->modifiers == MOD_ALT_MASK) {
    if (state == WL_KEYBOARD_KEY_STATE_PRESSED)
      window_set_maximized(window,
               window->type != TYPE_MAXIMIZED);
  } else if (sym == XKB_KEY_F11 &&
       window->fullscreen_handler &&
       state == WL_KEYBOARD_KEY_STATE_PRESSED) {
    window->fullscreen_handler(window, window->user_data);
  } else if (sym == XKB_KEY_F4 &&
       input->modifiers == MOD_ALT_MASK &&
       state == WL_KEYBOARD_KEY_STATE_PRESSED) {
    if (window->close_handler)
      window->close_handler(window->parent,
                window->user_data);
    else
      display_exit(window->display);
  } else if (window->key_handler) {
    (*window->key_handler)(window, input, time, key,
               sym, state, window->user_data);
  }

  if (state == WL_KEYBOARD_KEY_STATE_RELEASED &&
      key == input->repeat_key) {
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 0;
    timerfd_settime(input->repeat_timer_fd, 0, &its, NULL);
  } else if (state == WL_KEYBOARD_KEY_STATE_PRESSED) {
    input->repeat_sym = sym;
    input->repeat_key = key;
    input->repeat_time = time;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 25 * 1000 * 1000;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 400 * 1000 * 1000;
    timerfd_settime(input->repeat_timer_fd, 0, &its, NULL);
  }
*/
  if (state == WL_KEYBOARD_KEY_STATE_PRESSED) {
    wayland_event.key_event.down = 1;
  } else {
    wayland_event.key_event.down = 0;
  }
  if ((wayland->key_window != NULL)
      && (wayland->key_window->user_callback != NULL)) {
    wayland_event.type = KR_WL_KEY;
    wayland_event.key_event.key = sym;
    ret =
     wayland->key_window->user_callback(wayland->key_window,
     &wayland_event);
    if (ret < 0) {
    }
  }
}

static void pointer_handle_enter(void *data, struct wl_pointer *pointer,
 uint32_t serial, struct wl_surface *surface, wl_fixed_t x, wl_fixed_t y) {
  int ret;
  int i;
  kr_wayland *wayland;
  kr_wayland_event wayland_event;
  if (!surface) {
    return;
  }
  wayland = data;
  for (i = 0; i < KR_WL_MAX_WINDOWS; i++) {
    if (wayland->window[i].active == 1) {
      if (wayland->window[i].surface == surface) {
        if (wayland->pointer_window != &wayland->window[i]) {
          wayland->pointer_window = &wayland->window[i];
          break;
        }
      }
    }
  }
  if (i == KR_WL_MAX_WINDOWS) {
    return;
  }
  wayland->pointer_window->pointer_x = wl_fixed_to_int(x);
  wayland->pointer_window->pointer_y = wl_fixed_to_int(y);
  wayland->pointer_window->pointer_in = 1;
  wayland->pointer_window->pointer_out = 0;
  wl_pointer_set_cursor(pointer, serial, NULL, 0, 0);
  if (wayland->pointer_window->user_callback != NULL) {
    wayland_event.type = KR_WL_POINTER;
    wayland_event.pointer_event.x = wayland->pointer_window->pointer_x;
    wayland_event.pointer_event.y = wayland->pointer_window->pointer_y;
    wayland_event.pointer_event.click = wayland->pointer_window->click;
    wayland_event.pointer_event.pointer_in = wayland->pointer_window->pointer_in;
    wayland_event.pointer_event.pointer_out = wayland->pointer_window->pointer_out;
    ret =
     wayland->pointer_window->user_callback(wayland->pointer_window,
      &wayland_event);
    if (ret < 0) {
    }
  }
}

static void pointer_handle_leave(void *data, struct wl_pointer *pointer,
 uint32_t serial, struct wl_surface *surface) {
  int ret;
  int i;
  kr_wayland *wayland;
  kr_wayland_event wayland_event;
  wayland = data;
  if (!surface) {
    return;
  }
  for (i = 0; i < KR_WL_MAX_WINDOWS; i++) {
    if (wayland->window[i].active == 1) {
      if (wayland->window[i].surface == surface) {
        if (wayland->pointer_window == &wayland->window[i]) {
          break;
        }
      }
    }
  }
  if (i == KR_WL_MAX_WINDOWS) {
    return;
  }
  wayland->pointer_window->pointer_x = -1;
  wayland->pointer_window->pointer_y = -1;
  wayland->pointer_window->pointer_in = 0;
  wayland->pointer_window->pointer_out = 1;
  wayland->pointer_window->click = 0;
  if (wayland->pointer_window->user_callback != NULL) {
    wayland_event.type = KR_WL_POINTER;
    wayland_event.pointer_event.x = wayland->pointer_window->pointer_x;
    wayland_event.pointer_event.y = wayland->pointer_window->pointer_y;
    wayland_event.pointer_event.click = wayland->pointer_window->click;
    wayland_event.pointer_event.pointer_in = wayland->pointer_window->pointer_in;
    wayland_event.pointer_event.pointer_out = wayland->pointer_window->pointer_out;
    ret =
     wayland->pointer_window->user_callback(wayland->pointer_window,
     &wayland_event);
    if (ret < 0) {
    }
  }
  wayland->pointer_window = NULL;
}

static void pointer_handle_motion(void *data, struct wl_pointer *pointer,
 uint32_t time, wl_fixed_t x, wl_fixed_t y) {
  int ret;
  kr_wayland *wayland;
  kr_wayland_event wayland_event;
  wayland = data;
  wayland->pointer_window->pointer_x = wl_fixed_to_int(x);
  wayland->pointer_window->pointer_y = wl_fixed_to_int(y);
  wayland->pointer_window->pointer_in = 0;
  wayland->pointer_window->pointer_out = 0;
  if ((wayland->pointer_window != NULL)
      && (wayland->pointer_window->user_callback != NULL)) {
    wayland_event.type = KR_WL_POINTER;
    wayland_event.pointer_event.x = wayland->pointer_window->pointer_x;
    wayland_event.pointer_event.y = wayland->pointer_window->pointer_y;
    wayland_event.pointer_event.click = wayland->pointer_window->click;
    wayland_event.pointer_event.pointer_in = 0;
    wayland_event.pointer_event.pointer_out = 0;
    ret =
     wayland->pointer_window->user_callback(wayland->pointer_window,
     &wayland_event);
    if (ret < 0) {
    }
  }
}

static void pointer_handle_button(void *data, struct wl_pointer *pointer,
 uint32_t serial, uint32_t time, uint32_t button, uint32_t state_w) {
  int ret;
  kr_wayland *wayland;
  kr_wayland_event wayland_event;
  enum wl_pointer_button_state state = state_w;
  wayland = data;
  if (wayland->pointer_window != NULL) {
    if (state == WL_POINTER_BUTTON_STATE_PRESSED) {
      wayland->pointer_window->click = 1;
    }
    if (state == WL_POINTER_BUTTON_STATE_RELEASED) {
      wayland->pointer_window->click = 0;
    }
    wayland_event.type = KR_WL_POINTER;
    wayland_event.pointer_event.x = wayland->pointer_window->pointer_x;
    wayland_event.pointer_event.y = wayland->pointer_window->pointer_y;
    wayland_event.pointer_event.click = wayland->pointer_window->click;
    wayland_event.pointer_event.pointer_in = 0;
    wayland_event.pointer_event.pointer_out = 0;
    if (wayland->pointer_window->user_callback != NULL) {
      ret =
       wayland->pointer_window->user_callback(wayland->pointer_window,
       &wayland_event);
      if (ret < 0) {
      }
    }
  }
}

static void pointer_handle_axis(void *data, struct wl_pointer *pointer,
 uint32_t time, uint32_t axis, wl_fixed_t value) {
  /* Nothing here */
}
