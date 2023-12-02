#define _GNU_SOURCE
#include <krad/io/dir.h>
#include <krad/io/file2.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include "doc/1a2b3c/stdiov.h"

typedef struct {
  char label[256];
  char name[256];
  struct iovec iovec[256];
} labeled_iovec;

/*
struct iovec {
  void   *iov_base;
  size_t  iov_len;
};
*/

static char *V = "demo system 26";

#define DEMO_MEMNOM 26000

typedef struct {
  uint64_t nbytes;
  int nfiles;
  int ndirs;
  int to_scan;
  int scanned;
  kr_dir fold[DEMO_MEMNOM];
} dirscan;

typedef struct {
  dirscan scan;
  labeled_iovec mem[DEMO_MEMNOM];
  struct utsname uname;
} demo_t;

int cbev(kr_file_event *ev) {
  /*uint8_t *user_ptr;
  user_ptr = ev->user;
  printf("cbev\n");*/
  return 0;
}

typedef struct {
 ino_t        d_ino;    /* 64-bit inode number */
 off_t        d_off;    /* 64-bit offset to next structure */
 unsigned short int d_reclen; /* Size of this dirent */
 unsigned char  d_type;   /* File type */
 char           d_name[256]; /* Filename (null-terminated) */
} dir_entry;

/*ssize_t getdents64(int fd, void dirp[.count], size_t count);*/
ssize_t get_dir_ls(unsigned int fd, dir_entry *dirp,
                   unsigned int count) {
  return syscall(SYS_getdents64, fd, dirp, count);
}

/*
static void uwls(uint8_t *bits, uint64_t sz) {
  uint64_t b;
  int wasntext = 0;
  int wasatext = 0;
  uint8_t byte;
  for (b = 0; b < sz; b++) {
    byte = bits[b];
    if ((wasatext == 78) || (wasntext == 78)) {
      printf("b: %lu wasatext: %d wasntext %d\n", b, wasatext, wasntext);
      exit(1);
    }
    if (!isascii_splf(byte)) {
      putchar(byte);
      if (wasntext) {
        wasntext = 0;
      }
      wasatext++;
    } else {
      wasntext++;
      if (wasatext) {
        putchar(LF);
        wasatext = 0;
      }
    }
  }
}

static int checkpath(kr_file_set *fs, char *path) {
  int ret;
  kr_file2 *in;
  size_t len;
  len = strlen(path);
  in = kr_file2_open(fs, path, len);
  if (!in) {
    printf("We could not open a file: %s\n", path);
    ret = 1;
  }
  if (ret == 0) {
    kr_file2_info info;
    ret = kr_file2_get_info(in, &info);
    if (ret) {
      printf("Could not get file info: %d\n", ret);
      return ret;
    }
    uint8_t *bits = kr_file2_get_data(in);
    if (!bits) {
      printf("Could not get the bits for: %s\n", info.path);
      ret += kr_file2_close(in);
      ret++;
      if (ret) printf("ret: %d\n", ret);
    }
  }
  return ret;
}
*/
/*
 *
 * How many files out of all the files I can read
 * have 1 partition when partitioned by byte LF
 * and are larger than PATH_MAX ie 4096
 *
 */

int addscan(demo_t *d, char *path) {
  int len;
  if (!d) return 0;
  if (!path) return 0;
  if (d->scan.to_scan == DEMO_MEMNOM) {
    printf("damnit wtf to many dirs\n");
    exit(1);
    return -1;
  }
  len = strlen(path);
  memcpy(d->scan.fold[d->scan.to_scan].name, path, len);
  d->scan.fold[d->scan.to_scan].name[len] = '\0';
  d->scan.to_scan++;
  return 1;
}

int scan_another_dir(demo_t *d) {
  int ret;
  kr_dir *dir;
  dir = &d->scan.fold[d->scan.scanned++];
  printf("Scanning:\n%s\n", dir->name);
  ret = kr_dir_open(dir, dir->name, strlen(dir->name));
  if (ret) {
    printf("kr_dir_open: %d\n", ret);
    return 1;
  }
  kr_dir_entry y;
  int nfiles = d->scan.nfiles;
  int ndirs = d->scan.ndirs;
  for (;;) {
    if (((d->scan.ndirs + d->scan.nfiles) % 26) == 0) {
      printf("%s status %d subdirs %d files\n", dir->name, d->scan.ndirs - ndirs,
        d->scan.nfiles - nfiles);
      printf("%lu megabytes\n", d->scan.nbytes / 1000000);
    };
    ret = kr_dir_get_entry(dir, &y);
    if (!ret) break;
    if (kr_dir_entry_is_file(&y)) {
      printf("%s/%s %lu bytes\n", dir->name, y.name, y.sz);
      d->scan.nbytes += y.sz;
      d->scan.nfiles++;
      continue;
    }
    if (kr_dir_entry_is_dir(&y)) {
      static char fullpath[4096];
      //printf("1Adding subscan!: %s / %s\n", dir->name, y.name);
      sprintf(fullpath, "%s/%s", dir->name, y.name);
      printf("subdir: %s\n", fullpath);
      addscan(d, fullpath);
      d->scan.ndirs++;
      continue;
    }
  }
  printf("Scan done: %s %d subdirs and %d files\n", dir->name, d->scan.ndirs - ndirs,
         d->scan.nfiles - nfiles);
  ret = kr_dir_close(dir);
  if (ret) {
    printf("kr_dir_close: %d\n", ret);
    exit(1);
  }
  return 0;
}

int run_dirscan(demo_t *d) {
  if (!d) return -1;
  if (d->scan.to_scan < 1) return -1;
  while (d->scan.to_scan != d->scan.scanned) {
    scan_another_dir(d);
    printf("Scanned %d entries. %d subdirs and %d files.\n", d->scan.scanned,
           d->scan.ndirs, d->scan.nfiles);
  }
  return 0;
}

/* explain everything in the universe from the context
 * of this one program parsing all the files ;) */
int scan_fs(demo_t *d) {
  if (!d) return -1;
  char *hom;
  hom = "/root";
  int ret;
  ret = 0;
  //int len;
  //len = 0;
  int uid;
  int euid;
  //char *path;
  //path = NULL;
  //kr_file2 *file;
  //file = NULL;
  static kr_fs_setup setup;
  setup.nfiles = 64;
  setup.user = d;
  setup.event_cb = cbev;
  static kr_file_set *fs;
  fs = kr_file_set_create(&setup);
  /*checkpath(fs, "/dev/mei0");*/ /* LOL */
  ret = uname(&d->uname);
  if (ret) {
    printf("wtf uname error: %s\n", strerror(errno));
    exit(1);
  }
  /*
  printf("sysname: %s\n", d->uname.sysname);
  printf("nodename: %s\n", d->uname.nodename);
  printf("release: %s\n", d->uname.release);
  printf("version: %s\n", d->uname.version);
  printf("machine: %s\n", d->uname.machine);*/
  uid = getuid();
  //printf("uid: %d\n", uid);
  euid = geteuid();
  //printf("euid: %d\n", euid);
  if (uid + euid != 0) {
    ret = setuid(0);
    if (ret) {
      printf("setuid: %s\n", strerror(errno));
      exit(1);
    }
  }
  if (!kr_dir_exists(hom)) {
    ret = mkdir(hom, S_IFDIR | S_IRWXU);
    if (ret) {
      printf("mkdir: %s\n", strerror(errno));
      exit(1);
    }
  }
  ret = chdir(hom);
  if (ret) {
    printf("chdir: %s\n", strerror(errno));
    exit(1);
  }
  printf("%s operating LiK# %s on %s\n", V, d->uname.release, d->uname.machine);
  printf("state: superGOOD & .plan: Now Comphrehensive fs scan!\n");
  //addscan(d, "/", 1);
  // ok so #1 we ..must encode in a sense a partial mirror of the kernel
  // probably best by using something to pull the needed types from the kernel
  // such as the "seven types of files" and that some files are apparently,
  // both a directory and a symlink, surprise surprise
  //
  // ok lets make a masterfile of say 256mb on every block device with a
  // partition, which we should mount all of them if avail
  //
  // need to get rid of lot of stupidnesses ..
  //
  // need to encode what "fs types" are "system" vs storage?
  //openat(AT_FDCWD, "/proc/self/mountinfo", O_RDONLY|O_CLOEXEC) = 3
  //newfstatat(AT_FDCWD, "/dev", {st_mode=S_IFDIR|0755, st_size=4240, ...}, 0) = 0
  //statfs("/dev", {f_type=TMPFS_MAGIC, f_bsize=4096, f_blocks=8184431, f_bfree=8184431, f_bavail=8184431, f_files=8184431, f_ffree=8183802, f_fsid={val=[0xef75550f, 0xeb1f3c3]}, f_namelen=255, f_frsize=4096, f_flags=ST_VALID|ST_NOSUID|ST_RELATIME}) = 0

  //
  //
  addscan(d, "/root");
  addscan(d, "/etc");
  addscan(d, "/usr");
  addscan(d, "/home");
  run_dirscan(d);
  /*
  DIR *dh;
  dh = opendir("/");
  if (!dh) {
    printf("opendir: %s\n", strerror(errno));
    exit(1);
  }
  int fd;
  char duf[4096*26];
  fd = dirfd(dh);
  ret = read(fd, duf, sizeof(duf));
  if (ret < 1) {
    printf("read: %s\n", strerror(errno));
    exit(1);
  }
  printf("whot: %d\n%s\n", ret, duf);
  */
  /*
  checkpath(fs, "/");
  checkpath(fs, "/proc/self/environ");
  checkpath(fs, "/proc/self/mounts");
  checkpath(fs, "/dev/null");
  checkpath(fs, "/bin/ls");
  checkpath(fs, "/usr/bin/demo");
  checkpath(fs, "/home/demo/src/lush/lush.symlink");
  checkpath(fs, "/home/demo/src/lush/bad.symlink");
  checkpath(fs, "/home/demo/src/lush/demo/.libs/demo");
  checkpath(fs, "/home/demo/src/lush/demo/demo");
  uint8_t some_page[4096 * 2];
  path = "/proc/self/environ";
  len = strlen(path);
  file = kr_file2_open(fs, path, len);
  ret = kr_file2_read(file, some_page, sizeof(some_page));
  printf("Read %d bytes from %s\n", ret, path);
  if (ret == sizeof(some_page)) {
    printf("wtf ur environment is to fking big\n");
    exit(1);
  }
  */
  ret = kr_file_set_destroy(fs);
  if (ret) printf("ret: %d\n", ret);
  return ret;
}

int seek_lush_source(demo_t *d) {
  if (!d) return -1;
  int ret;
  ret = 0;
  /*reconstruct_argc?*/
  /* Find lush src dir cd cwd $HOME/src/lush" *
   [~/src/lush]% pwd
/home/demo/src/lush
[~/src/lush]% cat README.md | grep "# LUSH" | wc -l
1
[~/src/lush]%
   */
  return ret;
}

/* do i have the lush src?
 * what username what cpu how much ram how much
 * disk space what kernel version lib uring? *
 * what date and time is it?
 */

int init_demo(int argc, char *argv[]) {
  demo_t *d;
  int ret;
  d = calloc(1, sizeof(demo_t));
  if (d) printf("cAlloc'ed %lu megabyte mistake buffer\n", sizeof(demo_t) / 1000000);
  ret = 0;
  ret = scan_fs(d);
  if (ret) return ret;
  ret = seek_lush_source(d);
  free(d);
  return ret;
}

int run_demo(int argc, char *argv[]) {
  int ret;
  ret = 0;
  if (1) {
    ret = init_demo(argc, argv);
  }
  printf("%s ran proper.\n", V);
  return ret;
}
