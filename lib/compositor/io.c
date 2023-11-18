#define KR_MM __ATOMIC_SEQ_CST

static int read_frame(kr_compositor_path *path);
static int write_frame(kr_compositor_path *path);
static int path_writable(kr_compositor_path *path);

static int read_frame(kr_compositor_path *path) {
  uint64_t notes;
  int have_in;
  kr_compositor_path_type type;
  type = path->info.type;
  if (type != KR_COM_SOURCE && type != KR_COM_OUTPUT) return -1;
  have_in = __atomic_load_n(&path->have_in, KR_MM);
  if (!have_in) {
    printke("Compositor: read_frame but no in_frame");
    return 0;
  }
  notes = kr_take_notice(path->in_fd);
  if (!notes) {
    printke("Compositor: read_frame couldn't read port fd");
    return -2;
  }
  if (notes != 1) {
    printke("Compositor: Port read should have been 1 but was %"PRIu64"", notes);
  }
  if (type == KR_COM_SOURCE && path->have_frame) {
    write_frame(path);
  }
  path->frame = path->in;
  __atomic_store_n(&path->have_in, 0, KR_MM);
  __atomic_store_n(&path->have_frame, 1, KR_MM);
  return 1;
}

static int write_frame(kr_compositor_path *path) {
  int ret;
  int have_out;
  kr_compositor_path_type type;
  type = path->info.type;
  if (type != KR_COM_SOURCE && type != KR_COM_OUTPUT) return -1;
  have_out = __atomic_load_n(&path->have_out, KR_MM);
  if (have_out) {
    printke("Compositor: write_frame but out frame not null");
    return 0;
  }
  path->out = path->frame;
  __atomic_store_n(&path->have_frame, 0, KR_MM);
  __atomic_store_n(&path->have_out, 1, KR_MM);
  ret = kr_give_notice(path->out_fd);
  if (ret) {
    printke("Compositor: write frame to out fd");
    return -2;
  }
  return 1;
}

static int path_writable(kr_compositor_path *path) {
  int have_frame;
  kr_compositor_path_type type;
  type = path->info.type;
  if (type != KR_COM_OUTPUT) return -1;
  have_frame = __atomic_load_n(&path->have_frame, KR_MM);
  if (!have_frame) return 0;
  return 1;
}

int kr_compositor_port_fd(kr_compositor_path *path) {
  kr_compositor_path_type type;
  if (!path) return -1;
  type = path->info.type;
  if (type != KR_COM_SOURCE && type != KR_COM_OUTPUT) return -2;
  return path->out_fd;
}

int kr_compositor_read(kr_compositor_path *path, kr_frame *frame) {
  int have_out;
  uint64_t notes;
  kr_compositor_path_type type;
  kr_av_event event;
  if (!path || !frame) return -1;
  type = path->info.type;
  if (type != KR_COM_SOURCE && type != KR_COM_OUTPUT) return -1;
  have_out = __atomic_load_n(&path->have_out, KR_MM);
  if (!have_out) {
    if (type == KR_COM_SOURCE) printke("Compositor: source read, no out");
    if (type == KR_COM_OUTPUT) printke("Compositor: output read, no out");
    return 0;
  }
  notes = kr_take_notice(path->out_fd);
  if (!notes) {
    printke("Compositor: kr_compositor_read couldn't read port fd");
    return -2;
  }
  if (notes != 1) {
    printke("Compositor: Port read should have been 1 but was %"PRIu64"", notes);
  }
  *frame = path->out;
  __atomic_store_n(&path->have_out, 0, KR_MM);
  event.user = frame->image.owner;
  event.frame = frame;
  if (type == KR_COM_OUTPUT) {
    if (frame->image.ready) frame->image.ready(&event);
  } else {
    if (frame->image.release && frame->image.owner) {
      frame->image.release(frame->image.owner);
    }
  }
  return 1;
}

int kr_compositor_write(kr_compositor_path *path, kr_frame *frame) {
  int ret;
  int have_in;
  kr_compositor_path_type type;
  if (!path || !frame) return -1;
  type = path->info.type;
  if (type != KR_COM_SOURCE && type != KR_COM_OUTPUT) return -1;
  have_in = __atomic_load_n(&path->have_in, KR_MM);
  if (have_in) {
    if (type == KR_COM_SOURCE) printke("Compositor: source write, have in");
    if (type == KR_COM_OUTPUT) printke("Compositor: output write, have in");
    return 0;
  }
  path->in = *frame;
  __atomic_store_n(&path->have_in, 1, KR_MM);
  ret = kr_give_notice(path->in_fd);
  if (ret) {
    printke("Compositor: notify port have in frame failed");
    return -2;
  }
  return 1;
}
