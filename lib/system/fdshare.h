#if !defined(_system_fdshare_H)
#define _system_fdshare_H (1)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/types.h>

ssize_t recv_fds(int ud, uint8_t *data, size_t len, int *fds, int *nfds);
ssize_t send_fds(int ud, char *buf, size_t sz, const int *fds, int nfds);

#endif
