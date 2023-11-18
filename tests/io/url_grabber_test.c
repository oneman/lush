#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <sys/epoll.h>
#include <krad/io/stream.h>
#include <krad/io/file.h>
#include <krad/time/timer.h>

#define NEVENTS 1

typedef struct {
  char host[256];
  int port;
  char mount[256];
} url_info;

typedef struct {
  size_t sz;
  char ctype[256];
  char *body;
} resource_info;

int resource_header_to_info(resource_info *info, char *header, int size) {
  char *header_ending;
  int total;
  char *ctype;
  char *clength;
  char *brk;
  header_ending = strstr(header,"\r\n\r\n");
  if (header_ending == NULL) {
    printf("Frak, header is fraked up\n");
    return -1;
  }
  header_ending = &header_ending[4];
  total = size - (header_ending - header);
  info->body = header_ending;
  ctype = strstr(header, "Content-Type: ");
  if (ctype) {
    brk = strstr(ctype, "\r\n");
    if (brk) {
      brk[0] = '\0';
      strcpy(info->ctype, &ctype[14]);
      brk[0] = '\r';
    }
  }
  clength = strstr(header, "Content-Length: ");
  if (clength) {
    brk = strstr(clength, "\r\n");
    if (brk) {
      brk[0] = '\0';
      info->sz = strtod(&clength[16], NULL);
      brk[0] = '\r';
    }
  }
  return total;
}

int grabber(kr_file *file, url_info *url_info) {
  kr_stream *in_stream;
  kr_timer *timer;
  resource_info res_info;
  char buffer[8092];
  int res;
  int ret;
  int err;
  int pd;
  int total;
  int curr_perc;
  int last_perc;
  struct epoll_event ev;
  struct epoll_event events[NEVENTS];
  pd = epoll_create1(EPOLL_CLOEXEC);
  in_stream = kr_stream_open(url_info->host, url_info->port, url_info->mount);
  if (in_stream == NULL) return 1;
  memset(&ev, 0, sizeof(ev));
  ev.events = EPOLLOUT;
  ev.data.fd = in_stream->sd;
  res = epoll_ctl(pd, EPOLL_CTL_ADD, in_stream->sd, &ev);
  if (res == -1) {
    err = errno;
    fprintf(stderr, "Error: %s\n", strerror(err));
    return 1;
  }
  res = epoll_wait(pd, events, NEVENTS, -1);
  res = kr_stream_handle_headers(in_stream);
  ev.events = EPOLLIN;
  res = epoll_ctl(pd, EPOLL_CTL_MOD, in_stream->sd, &ev);
  if (res == -1) {
    err = errno;
    fprintf(stderr, "Error: %s\n", strerror(err));
    return 1;
  }
  res = epoll_wait(pd, events, NEVENTS, -1);
  memset(buffer, 0, sizeof(buffer));
  timer = kr_timer_create();
  kr_timer_start(timer);
  res = kr_stream_recv(in_stream, buffer, sizeof(buffer));
  if (res <= 0) return 1;
  memset(&res_info, 0, sizeof(resource_info));
  res = resource_header_to_info(&res_info, buffer, res);
  if (res < 0) return 1;
  kr_file_write(file, res_info.body, res);
  total = res;
  last_perc = -1;
  do {
    ret = epoll_wait(pd, events, NEVENTS, 50);
    if (ret < 0) return 1;
    if (ret > 0) {
      res = kr_stream_recv(in_stream, buffer, sizeof(buffer));
      if (res > 0) {
        kr_file_write(file, buffer, res);
      }
      total += res;
      if (res_info.sz != 0) {
        curr_perc = floor( ((float)total/(float)res_info.sz) * 100.00f);
        if ((curr_perc != last_perc)) {
          printf("%d%%\n",curr_perc);
          last_perc = curr_perc;
        }
      }
    }
  } while (res > 0);
  kr_timer_finish(timer);
  printf("total grabbed: %d bytes in %"PRIi64"ms, ~%"PRIi64"KB/s\n",
    total,kr_timer_duration_ms(timer),total/kr_timer_duration_ms(timer));
  kr_timer_destroy(timer);
  kr_stream_disconnect(in_stream);
  kr_stream_destroy(&in_stream);
  close(pd);
  return 0;
}

int url_parser(url_info *url_info, char *url) {
  char *host;
  char *port;
  char *mount;
  if (url_info == NULL || url == NULL) return 1;
  host = strstr(url, "http://");
  if (host == NULL) return 1;
  host = &host[7];
  port = strchr(host, ':');
  if (port == NULL) {
    url_info->port = 80;
  } else {
    url_info->port = strtod(&port[1], NULL);
    if (url_info->port == 0) return 1;
    port[0] = '\0';
    strncpy(url_info->host, host, sizeof(url_info->host));
    port[0] = ':';
  }
  mount = strchr(host, '/');
  if (mount == NULL) return 1;
  mount[0] = '\0';
  if (port == NULL) {
    strncpy(url_info->host, host, sizeof(url_info->host));
  }
  mount[0] = '/';
  strncpy(url_info->mount, mount, sizeof(url_info->mount));
  return 0;
}

int url_grabber(char *out, char *url) {
  int ret;
  url_info uinfo;
  kr_file *file;
  memset(&uinfo, 0, sizeof(uinfo));
  ret = url_parser(&uinfo, url);
  if (ret != 0) return 1;
  file = kr_file_create(out);
  if (file == NULL) return 1;
  ret = grabber(file, &uinfo);
  kr_file_close(&file);
  if (ret != 0) return 1;
  return 0;
}

void print_usage(char *cmd) {
  printf("Grabs a resource from a given src url to a dst file.\n");
  printf("Usage: %s dst_file src_url\n",cmd);
  printf("i.e. %s out.png http://space.kradradio.com/krads.png\n",cmd);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    print_usage(argv[0]);
    return 1;
  }
  url_grabber(argv[1], argv[2]);
  return 0;
}
