#include "render.h"
#include <krad/image/png.h>

static int render_image(kr_studio_path *path) {
  int ret;
  int fail;
  char *id;
  size_t len;
  studio_render *ren;
  kr_stash_image *img;
  kr_frame frame;
  kr_image_stash_item *item;
  fail = 0;
  id = path->info.render.image.id;
  len = strlen(id);
  ren = &path->render;
  memset(&frame, 0, sizeof(frame));
  item = kr_image_stash_find_by_id(path->studio->stash, id, len);
  if (item) {
    printk("Render: Aha! Gotcha %*s", len, id);
  } else {
    printke("Render: Ah damn, did not find the item %*s", len, id);
    fail = 1;
  }
  if (!fail) {
    img = kr_image_stash_item_image(item);
    if (!img) {
      printke("Render: ahh no stash image from stash item yaaar");
      fail = 1;
    }
  }
  if (!fail) {
    if (img->type != KR_IMAGE_RAW) {
      printke("Render: Yaaar, we can't be renderin a coded image can we");
      fail = 1;
    }
  }
  if (!fail) {
    frame.image = img->raw;
  }
  if (!fail) {
    ret = kr_compositor_write(ren->image.com_port, &frame);
    if (ret != 1) fail = 1;
  }
  return fail;
}

static int destroy_render_image(kr_studio_path *path) {
  int ret;
  studio_render *ren;
  ren = &path->render;
  ret = kr_compositor_remove(ren->image.com_port);
  ren->image.com_port = NULL;
  return ret;
}

static int setup_render_image(kr_studio_path *path) {
  int ret;
  kr_compositor *comp;
  studio_render *ren;
  kr_compositor_port_setup cp_setup;
  ret = 0;
  ren = &path->render;
  comp = path->studio->compositor;
  memset(&cp_setup, 0, sizeof(cp_setup));
  cp_setup.info.type = KR_COM_SOURCE;
  kr_compositor_source_info_init(&cp_setup.info.source);
  cp_setup.frame_user = path;
  cp_setup.control_user = path->user;
  ren->image.com_port = kr_compositor_port(comp, &cp_setup);
  if (!ren->image.com_port) {
    printke("Render Image: create source compositor port returned NULL");
    ret = -1;
  } else {
    ret = render_image(path);
    if (ret) destroy_render_image(path);
  }
  return ret;
}

int studio_render_destroy(kr_studio_path *path) {
  int ret;
  printk("Render: Destroy me!");
  if (path->info.render.type == KR_RENDER_IMAGE) {
    ret = destroy_render_image(path);
  } else {
    /* FIXME */
    printke("Render: Noooooo!!!");
    ret = -1;
  }
  return ret;
}

int studio_render_create(kr_studio_path *path) {
  int ret;
  if (path->info.render.type == KR_RENDER_IMAGE) {
    ret = setup_render_image(path);
  } else {
    /* FIXME */
    printke("Render: A/V not yet implemented :/");
    ret = -1;
  }
  return ret;
}
