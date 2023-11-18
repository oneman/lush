#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <krad/app/debug.h>
#include <krad/compositor/compositor.h>

static void compositor_event(kr_compositor_event *event) {
  switch (event->type) {
    case KR_COM_CREATE:
      printk("Compositor path create event");
      break;
    case KR_COM_PATCH:
      printk("Compositor path patch event");
      break;
    case KR_COM_DESTROY:
      printk("Compositor path delete event");
      break;
    default:
      printke("Bad event from compositor");
      break;
  }
}

int ctl_test1() {
  char string[4096];
  char *path1 = "/crop/w";
  char *path2 = "/crop/h";
  char *path3 = "/opacity";
  int value;
  float real;
  int ret;
  int len;
  kr_compositor_setup csetup;
  kr_compositor *comp;
  kr_compositor_path *path;
  kr_compositor_port_setup psetup;
  kr_compositor_path_info info;
  kr_patch patch;
  kr_path *kpath;
  printk("sizeof kr_patch: %zu", sizeof(kr_patch));
  kr_path_alloca(kpath);
  memset(&csetup, 0, sizeof(csetup));
  memset(&psetup, 0, sizeof(psetup));
  memset(&patch, 0, sizeof(patch));
  csetup.event_cb = compositor_event;
  comp = kr_compositor_create(&csetup);
  if (comp == NULL) return -1;
  psetup.info.type = KR_COM_SOURCE;
  psetup.control_user = comp;
  path = kr_compositor_port(comp, &psetup);
  if (path == NULL) return -1;
  memset(string, 0, sizeof(string));
  kr_compositor_path_info_get(path, &info);
  kr_compositor_path_info_to_text(string, &info, sizeof(string));
  printk("Comp Path Info:\n%s",string);
  value = 640;
  len = strlen(path1);
  ret = kr_path_parse(kpath, path1, len);
  if (ret < 0) {
    printk("Path parse ret: %d", ret);
    return -1;
  }
  kr_compositor_source_info_patch_path(&patch, kpath);
  patch.integer = value;
  kr_compositor_ctl(path, &patch);
  value = 360;
  len = strlen(path2);
  ret = kr_path_parse(kpath, path2, len);
  if (ret < 0) {
    printk("Path parse ret: %d", ret);
    return -1;
  }
  memset(&patch, 0, sizeof(patch));
  kr_compositor_source_info_patch_path(&patch, kpath);
  patch.integer = value;
  kr_compositor_ctl(path, &patch);
  real = 0.55f;
  len = strlen(path3);
  ret = kr_path_parse(kpath, path3, len);
  if (ret < 0) {
    printk("Path parse ret: %d", ret);
    return -1;
  }
  memset(&patch, 0, sizeof(patch));
  kr_compositor_source_info_patch_path(&patch, kpath);
  patch.real = real;
  kr_compositor_ctl(path, &patch);
  kr_compositor_path_info_get(path, &info);
  kr_compositor_path_info_to_text(string, &info, sizeof(string));
  printk("Comp Path Info:\n%s",string);
  return 0;
}

int main(int argc, char *argv[]) {
  int ret;
  ret = ctl_test1();
  printk("ctl_test1 result: %d\n", ret);
  return ret;
}
