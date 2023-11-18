#define KR_SOME_LIMIT 16

static void printr(kr_rect *r, char *s) {
  printk("%s %d,%d [%dx%d]", s, r->x, r->y, r->w, r->h);
}

static int bound(kr_rect *r, int w, int h) {
  if (!r) return -1;
  if (r->x >= w) return -1;
  if (r->y >= h) return -1;
  if (r->x < 0) {
    if (r->x <= (w * -1)) return -1;
    if (r->x + r->w < 1) return -1;
    r->w += r->x;
    r->x = 0;
  }
  if (r->y < 0) {
    if (r->y <= (h * -1)) return -1;
    if (r->y + r->h < 1) return -1;
    r->h += r->y;
    r->y = 0;
  }
  if (r->x + r->w > w) r->w = w - r->x;
  if (r->y + r->h > h) r->h = h - r->y;
  return 0;
}

static int transfigure(kr_rect *base, kr_rect *mod) {
  float wscale;
  float hscale;
  int ret;
  kr_rect m;
  m = *mod;
  mod = &m;
  if (0) {
    printr(base, "Base");
    printr(mod, "Mod");
  }
  wscale = base->w / (float)KR_COMPOSITOR_WIDTH;
  hscale = base->h / (float)KR_COMPOSITOR_HEIGHT;
  mod->x *= wscale;
  mod->y *= hscale;
  mod->w *= wscale;
  mod->h *= hscale;
  ret = bound(mod, base->w, base->h);
  if (ret) return ret;
  base->x += mod->x;
  base->y += mod->y;
  base->w = mod->w;
  base->h = mod->h;
  return 0;
}

static int crop_bussed_input(kr_rect *r, kr_rect *pos, kr_rect *crop) {
  kr_rect d;
  kr_rect c;
  kr_rect p;
  p = *pos;
  pos = &p;
  c = *crop;
  crop = &c;
  bound(pos, KR_COMPOSITOR_WIDTH, KR_COMPOSITOR_HEIGHT);
  bound(crop, KR_COMPOSITOR_WIDTH, KR_COMPOSITOR_HEIGHT);
  if (crop->x > pos->x + pos->w) return -1;
  if (crop->y > pos->y + pos->h) return -1;
  if (crop->x + crop->w < pos->x) return -1;
  if (crop->y + crop->h < pos->y) return -1;
  d.x = pos->x;
  d.y = pos->y;
  d.w = pos->w;
  d.h = pos->h;
  if (crop->x > pos->x) {
    d.x += crop->x - pos->x;
  }
  if (crop->y > pos->y) {
    d.y += crop->y - pos->y;
  }
  if (crop->x + crop->w < pos->x + pos->w) {
    d.w -= ((pos->x + pos->w) - (crop->x + crop->w));
  }
  if (crop->y + crop->h < pos->y + pos->h) {
    d.h -= ((pos->y + pos->h) - (crop->y + crop->h));
  }
  bound(&d, KR_COMPOSITOR_WIDTH, KR_COMPOSITOR_HEIGHT);
  if (1) {
    printr(pos, "Pos");
    printr(crop, "Crop");
    printr(&d, "Cropped Pos");
  }
  transfigure(r, &d);
  return 0;
}

static int crop_source(kr_rect *src_crop, kr_rect *crop) {
  kr_rect c;
  int ret;
  c = *crop;
  ret = bound(&c, KR_COMPOSITOR_WIDTH, KR_COMPOSITOR_HEIGHT);
  if (ret) return ret;
  *src_crop = c;
  return 0;
}

#define TEMP_NINPUTS 16
#define TEMP_INDIR 1

static void bus_extents(kr_rect *ext, kr_compositor_path *bus) {
  int i;
  int ret;
  kr_rect b;
  kr_compositor_path *inputs[TEMP_NINPUTS];
  kr_graph *graph;
  //kr_rect_init(&ext);
  ext->x = KR_COMPOSITOR_WIDTH - 1;
  ext->y = KR_COMPOSITOR_HEIGHT - 1;
  ext->w = 1;
  ext->h = 1;
  graph = bus->compositor->graph;
  ret = kr_graph_in_out_links(graph, &bus->g.elem, TEMP_INDIR, (void **)inputs,
   TEMP_NINPUTS);
  for (i = 0; i < ret; i++) {
    b = inputs[i]->info.input.pos;
    bound(&b, KR_COMPOSITOR_WIDTH, KR_COMPOSITOR_HEIGHT);
    if (b.x < ext->x) ext->x = b.x;
    if (b.y < ext->y) ext->y = b.y;
    if (b.x + b.w > ext->x + ext->w) ext->w = (b.x + b.w) - ext->x;
    if (b.y + b.h > ext->y + ext->h) ext->h = (b.y + b.h) - ext->y;
  }
  /*printr(ext, "Bus Extents");*/
}

static int rot_fade(kr_compositor_input_info *to,
 kr_compositor_path_info *from) {
  if (to == NULL) return -1;
  if (from == NULL) return -1;
  switch (from->type) {
    case KR_COM_SOURCE:
      //printk("source");
      to->opacity *= from->source.opacity;
      break;
    case KR_COM_INPUT:
      //printk("input");
      to->opacity *= from->input.opacity;
      to->rotation += from->input.rotation;
      break;
    case KR_COM_BUS:
      //printk("bus");
      to->opacity *= from->bus.opacity;
      break;
    case KR_COM_OUTPUT:
      //printk("output");
      to->opacity *= from->output.opacity;
      break;
    default:
     return -1;
  }
  return 0;
}

static int chain_transfer(kr_insert *trans, kr_chain *chain) {
  int i;
  int ret;
  kr_compositor_path *path;
  kr_rect *src_pos;
  kr_rect bus_ext;
  kr_compositor_input_info info;
  src_pos = NULL;
  kr_compositor_input_info_init(&info);
  for (i = 0; i < chain->len; i++) {
    path = (kr_compositor_path *)chain->users[i];
    if (path == NULL) continue; /* FIXME this should not be needed.. */
    //printk("chain trans %d - %d", i, chain->len);
    if (path->info.type == KR_COM_INPUT) {
      if (i == 1) {
        ret = crop_source(&info.crop, &path->info.input.crop);
        if (ret) return ret;
        /* the first input is an input from a source .. */
      } else {
        if (src_pos == NULL) return -1;
        if (0) {
          ret = crop_bussed_input(&info.crop, src_pos,
           &path->info.input.crop);
          if (ret) return ret;
        }
      }
      src_pos = &path->info.input.pos;
    }
    rot_fade(&info, &path->info);
  }
  kr_rect_init(&bus_ext);
  for (i = chain->len - 1; i > -1; i--) {
    path = (kr_compositor_path *)chain->users[i];
    if (path == NULL) continue; /* FIXME this should not be needed.. */
    if (path->info.type == KR_COM_BUS) {
      bus_extents(&bus_ext, path);
    }
    if (path->info.type == KR_COM_INPUT) {
      ret = transfigure(&info.pos, &path->info.input.pos);
      if (ret) return ret;
    }
  }
  trans->src_crop = info.crop;
  trans->dst_area = info.pos;
  trans->rotation = info.rotation;
  trans->opacity = info.opacity;
  return 0;
}

static void output_source_render(kr_compositor_path *output,
 kr_compositor_path *source) {
  double sf;
  int n;
  int i;
  int ret;
  kr_image *oi;
  kr_image *si;
  kr_chain user_chains[KR_SOME_LIMIT];
  kr_insert insert;
  if (output == NULL) {
    printke("Compositor Render: null output on output_source_render");
    return;
  }
  if (source == NULL) {
    printke("Compositor Render: null source on output_source_render");
    return;
  }
  n = kr_graph_chains_full(output->compositor->graph, &output->g.elem,
   &source->g.elem, user_chains, KR_SOME_LIMIT);
  for (i = 0; i < n; i++) {
    ret = chain_transfer(&insert, &user_chains[i]);
    if (ret) continue;
    oi = &output->frame.image;
    si = &source->frame.image;
    sf = kr_rect_scaler(oi->info.w, oi->info.h, KR_COMP_W, KR_COMP_H);
    /*printk("need fit %d,%d %dx%d into %dx%d", insert.dst_area.x, insert.dst_area.y, insert.dst_area.w, insert.dst_area.h,
     oi->info.w, oi->info.h);*/
    kr_rect_scale(&insert.dst_area, sf);
    sf = kr_rect_scaler(si->info.w, si->info.h, KR_COMP_W, KR_COMP_H);
    kr_rect_scale(&insert.src_crop, sf);
    ret = kr_image_insert_adv(oi, si, &insert);
    if (ret != 0) {
      printke("render: kr_image_insert_adv failed %d", ret);
    }
  }
}

static void output_render(kr_compositor_path *output) {
  void *source_users[KR_SOME_LIMIT];
  kr_compositor_path *source;
  int i;
  int n;
  n = kr_graph_sources(output->compositor->graph, &output->g.elem,
   source_users, KR_SOME_LIMIT);
  kr_image_fill(&output->frame.image, 0, 0, 0);
  for (i = 0; i < n; i++) {
    source = (kr_compositor_path *)source_users[i];
    output_source_render(output, source);
  }
}
