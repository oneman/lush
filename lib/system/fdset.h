#if !defined(_system_fdset_H)
#define _system_fdset_H (1)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/types.h>

#define KR_NFDS 128

typedef struct kr_fdset kr_fdset;

size_t kr_fdset_sizeof();
int kr_fdset_init(kr_fdset *fdset);
int kr_fdset_count(kr_fdset *fdset);
int kr_fdset_put(kr_fdset *fdset, int *fds, int n);
int kr_fdset_get(kr_fdset *fdset, int *fds, int n);
int kr_fdset_close(kr_fdset *fdset);

#endif
