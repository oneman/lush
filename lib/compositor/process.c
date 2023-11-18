static int process_output(kr_compositor_path *output) {
  if (path_writable(output)) {
    output_render(output);
    write_frame(output);
    return 1;
  }
  return 0;
}

static int process_source(kr_compositor_path *source) {
  kr_graph *g;
  int ret;
  int i;
  int n;
  void *output_users[KR_SOME_LIMIT];
  kr_compositor_path *output;
  g = source->compositor->graph;
  n = kr_graph_outputs(g, &source->g.elem, output_users, KR_SOME_LIMIT);
  for (i = 0; i < n; i++) {
    output = (kr_compositor_path *)output_users[i];
    ret = process_output(output);
    if (!ret) output->damage = 1;
  }
  return 0;
}

static int process_port(kr_compositor_path *path) {
  int ret;
  if (path == NULL) return -1;
  if (path->state == PATH_DESTROY) {
    path_release(path);
    return 0;
  }
  ret = read_frame(path);
  if (ret != 1) {
    printke("Compositor: process port read frame ret %d", ret);
    return ret;
  }
  if (path->info.type == KR_COM_OUTPUT) {
    if (path->damage) {
      ret = process_output(path);
      if (ret) path->damage = 0;
    }
    return 0;
  } else {
    process_source(path);
  }
  return 0;
}

static int output_global_damage(kr_compositor *com) {
  int ret;
  int i;
  i = 0;
  kr_compositor_path *path;
  while ((path = kr_pool_iterate_active(com->path_pool, &i))) {
    if (path->info.type != KR_COM_OUTPUT) continue;
    ret = process_output(path);
    if (!ret) path->damage = 1;
  }
  return 0;
}

static int graph_update_process(kr_compositor *com) {
  uint64_t notes;
  if (!com) return 0;
  notes = kr_take_notice(com->graph_fd);
  printk("Compositor: Processing %"PRIu64" graph updates!", notes);
  output_global_damage(com);
  return 0;
}

static int path_patch_process(kr_compositor *com) {
  uint64_t notes;
  if (!com) return 0;
  notes = kr_take_notice(com->path_fd);
  if (notes > 1) {
    printk("Compositor: Processing %"PRIu64" path patchs!", notes);
  }
  output_global_damage(com);
  return 0;
}

static int graph_event(kr_event *event) {
  kr_compositor *com;
  if (event == NULL) return -1;
  if (event->user == NULL) return -1;
  com = (kr_compositor *)event->user;
  if (!(event->events & EPOLLIN)) {
    printke("Compositor: Graph event unknown");
    return -1;
  }
  graph_update_process(com);
  return 0;
}

static int path_patch_event(kr_event *event) {
  kr_compositor *com;
  if (event == NULL) return -1;
  if (event->user == NULL) return -1;
  com = (kr_compositor *)event->user;
  if (!(event->events & EPOLLIN)) {
    printke("Compositor: Patch event unknown");
    return -1;
  }
  path_patch_process(com);
  return 0;
}

static int port_event(kr_event *event) {
  kr_compositor_path *path;
  if (event == NULL) return -1;
  if (event->user == NULL) return -1;
  path = (kr_compositor_path *)event->user;
  if (!(event->events & EPOLLIN)) {
    printke("Compositor: Port event unknown");
    return -1;
  }
  process_port(path);
  return 0;
}
