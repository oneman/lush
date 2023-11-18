#include <stdio.h>
#include <stdlib.h>
#include <krad/app/debug.h>
#include <krad/io/scan.h>

#ifdef KR_XMMS2
#define KR_XMMS2_TEST 1
#endif

#ifdef KR_XMMS2_TEST
#include <xmmsclient/xmmsclient.h>
int add_xmms2_file(xmmsc_connection_t *conn, char *dir, char *name) {
	xmmsc_result_t *res;
  xmmsv_t *val;
  int ret;
  char str[PATH_MAX];
  const char *url = str;
  const char *err;
  ret = snprintf(str, sizeof(str), "file://%s/%s", dir, name);
  if (ret < 0 || ret > sizeof(str) - 1) return -1;
	res = xmmsc_medialib_add_entry(conn, url);
  xmmsc_result_wait(res);
  val = xmmsc_result_get_value(res);
  if (xmmsv_is_error(val) && xmmsv_get_error(val, &err)) {
    printke("xmmsc_medialib_add_entry: %s\n[%s]", err, url);
    return -1;
  } else {
    /*printk("xmmsc_medialib_add_entry: %s", url);*/
  }
  xmmsc_result_unref(res);
  return 0;
}
#endif

typedef struct {
  char ext[8];
} media_desc;

typedef struct {
  media_desc *type;
  int ntypes;
  size_t bytes;
  uint64_t nmatches;
  uint64_t nerrors;
  kr_scan *scan;
#ifdef KR_XMMS2_TEST
  xmmsc_connection_t *conn;
#endif
} media_scan;

static const media_desc media_ext[] = {
  [0] = { .ext = ".ts" },
  [1] = { .ext = ".mkv" },
  [2] = { .ext = "webm" },
  [3] = { .ext = ".ogg" },
  [4] = { .ext = "opus" },
  [5] = { .ext = ".mp3" },
  [6] = { .ext = ".aac" },
  [7] = { .ext = "flac" }
};

static const int nexts = (sizeof(media_ext) / sizeof(media_ext[0]));

static int strbytes(char *str, size_t sz, size_t bytes) {
  const int kb = 1024;
  const int mb = kb * kb;
  const int gb = kb * 1000000;
  int len;
  len = 0;
  if (bytes < kb) {
    len = snprintf(str, sz, "%zu bytes", bytes);
  } else if (bytes < mb) {
    if (bytes < mb - kb) bytes += kb;
    len = snprintf(str, sz, "%zu KB", bytes / kb);
  } else if (bytes < gb) {
    if (bytes < gb - mb) bytes += mb;
    len = snprintf(str, sz, "%zu MB", bytes / mb);
  } else {
    bytes += mb * 10;
    len = snprintf(str, sz, "%zu.%zu GB", bytes / gb, bytes % gb / (mb * 10));
  }
  return len;
}

static void print_exts(media_scan *mscan) {
  int i;
  i = 0;
  printf("Looking for:");
  while (i < mscan->ntypes) {
    printf(" %s", mscan->type[i].ext);
    if ((++i % 8) == 0) printf("\n");
  }
  printf("\n");
}

static int get_ext(char **ext, char *str, size_t sz) {
  int i;
  int len;
  len = 0;
  if (sz < 4) return 0;
  for (i = sz; i > sz - 4; i--) {
    if (str[i] == '.') break;
    len++;
  }
  *ext = &str[i];
  return len;
}

static int match_ext(media_scan *mscan, char *ext, int len) {
  int i;
  char *mext;
  int mext_len;
  for (i = 0; i < mscan->ntypes; i++) {
    mext = mscan->type[i].ext;
    mext_len = strlen(mext);
    if (len != mext_len) continue;
    if (0 == memcmp(ext, mext, len)) return i;
  }
  return -1;
}

static void on_match(media_scan *mscan, int match, kr_dir_entry *entry) {
  int ret;
  int plen;
  char *path;
  int blen;
  char bytes[16];
  plen = kr_scan_path(mscan->scan, &path);
  blen = strbytes(bytes, sizeof(bytes), entry->sz);
  printf("%14.*s %.*s/%s\n", blen, bytes, plen, path, entry->name);
#ifdef KR_XMMS2_TEST
  ret = add_xmms2_file(mscan->conn, path, entry->name);
  if (ret) mscan->nerrors++;
#endif
}

static void on_file(media_scan *mscan, kr_dir_entry *entry) {
  char *ext;
  int len;
  int match;
  len = get_ext(&ext, entry->name, strlen(entry->name));
  if (!len) return;
  match = match_ext(mscan, ext, len);
  if (match < 0) return;
  mscan->nmatches++;
  mscan->bytes += entry->sz;
  on_match(mscan, match, entry);
}

int scan_test(char *path, size_t len) {
  int ret;
  char *cur;
  char bytes_str[16];
  int bytes_str_len;
  media_scan mscan;
  kr_scan_setup setup;
  kr_dir_entry entry;
  ret = -1;
  setup.mem = kr_allocz(1, kr_scan_sizeof());
  memset(&mscan, 0, sizeof(mscan));
  mscan.ntypes = nexts;
  mscan.type = (media_desc *)&media_ext;
  memset(&entry, 0, sizeof(entry));
  setup.len = len;
  if (setup.len >= sizeof(setup.path)) setup.len = sizeof(path) - 1;
  memcpy(setup.path, path, setup.len);
  setup.path[len] = '\0'; 
  if (setup.path[setup.len - 1] == '/') setup.path[--setup.len] = '\0';
  printf("Scanning %.*s\n", (int)setup.len, setup.path);
  print_exts(&mscan);
#ifdef KR_XMMS2_TEST
	mscan.conn = xmmsc_init("krad_media_scan");
	if (!xmmsc_connect(mscan.conn, getenv("XMMS_PATH"))) {
		printke("Unable to connect to XMMS2");
		return -1;
	}
#endif
  mscan.scan = kr_scan_create(&setup);
  if (!mscan.scan) return 1;
  for (;;) {
    ret = kr_scan_iter(mscan.scan, &entry);
    if (ret == 0) break;
    if (ret != 1) {
      fprintf(stderr, "kr_scan_iter ret %d\n", ret);
      break;
    }
    if (kr_dir_entry_is_file(entry.entry_mode)) {
      on_file(&mscan, &entry);
    } else if (kr_dir_entry_is_dir(entry.entry_mode)) {
      //printf("%20s  %s/\n", "", entry.name);
      kr_scan_probe(mscan.scan, entry.name);
      ret = kr_scan_path(mscan.scan, &cur);
      //printf("Sub scanning: %.*s\n", ret, cur);
    }
  }
#ifdef KR_XMMS2_TEST
	xmmsc_unref(mscan.conn);
#endif
  ret = kr_scan_destroy(mscan.scan);
  if (ret) printke("kr scan destroy ret %d", ret);
  bytes_str_len = strbytes(bytes_str, sizeof(bytes_str), mscan.bytes);
  printk("%d matches %.*s [%zu]", mscan.nmatches, bytes_str_len, bytes_str,
   mscan.bytes);
  if (mscan.nerrors) printke("%zu errors", mscan.nerrors);
  free(setup.mem);
  return ret;
}

int main(int argc, char const *argv[]) {
  int ret;
  size_t len;
  char cwd[PATH_MAX];
  char *path;
  if (argc != 2) {
    path = getcwd(cwd, sizeof(cwd)); 
  } else {
    path = (char *)argv[1];
  }
  len = strlen(path);
  ret = scan_test(path, len);
  if (ret != 0) {
    fprintf(stderr, "Failure.");
    return -1;
  }
  return 0;
}
