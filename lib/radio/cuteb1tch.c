typedef struct {
  uint8_t bytes[4096];
} boat;


int cbev(kr_file_event *ev) {
  uint8_t *user_ptr;
  user_ptr = ev->user;
  printf("cbev\n");
  return 0;
}

static void sysrecheck(void) {
  printf("demo running!!!\n");
  int ret;
  static boat demo;
  static kr_fs_setup setup;
  setup.nfiles = 64;
  setup.user = &demo;
  setup.event_cb = cbev;
  static kr_file_set *set;
  set = kr_file_set_create(&setup);
  kr_file2 *intxt;
  char *path = "/home/demo/.bashrc";
  size_t len;
  len = strlen(path);
  intxt = kr_file2_open(set, path, len);
  /*kr_file2 *outtxt;*/
  /*outtxt = kr_file2_create(set, outpath, len, size_t sz);*/
  ret = kr_file2_close(intxt);
  if (ret) printf("ret: %d\n", ret);
  ret = kr_file_set_destroy(set);
  if (ret) printf("ret: %d\n", ret);
}
