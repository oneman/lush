#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <errno.h>
#include <fcntl.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include "stream.h"

static void kr_stream_read_http_headers(kr_stream *stream);
static void kr_stream_send_request_to_stream(kr_stream *stream);
static void kr_stream_send_request_for_stream(kr_stream *stream);
static kr_stream *kr_stream_connect(char *host, int port);

ssize_t kr_stream_send(kr_stream *stream, void *buffer, size_t len) {
  ssize_t ret;
  ret = write(stream->sd, buffer, len);
  if (ret > 0) {
    stream->position += ret;
  } else {
    if ((ret == -1) && (errno != EAGAIN)) {
      stream->err_no = errno;
      strerror_r(stream->err_no, stream->error_str, sizeof(stream->error_str));
      stream->position = 0;
      stream->half_ready = 0;
      stream->ready = 0;
      stream->connected = 0;
    }
  }
  return ret;
}

ssize_t kr_stream_recv(kr_stream *stream, void *buffer, size_t len) {
  ssize_t ret;
  ret = read(stream->sd, buffer, len);
  if (ret > 0) {
    stream->position += ret;
  }
  return ret;
}

int kr_stream_disconnect(kr_stream *stream) {
  if (stream == NULL) {
    return -2;
  }
  if (stream->sd != 0) {
    close(stream->sd);
    stream->sd = 0;
    return 0;
  }
  return -1;
}

int kr_stream_free(kr_stream **stream) {
  if ((stream == NULL) || (*stream == NULL)) {
    return -1;
  }
  if ((*stream)->host != NULL) {
    free((*stream)->host);
    (*stream)->host = NULL;
  }
  if ((*stream)->mount != NULL) {
    free((*stream)->mount);
    (*stream)->mount = NULL;
  }
  if ((*stream)->password != NULL) {
    free((*stream)->password);
    (*stream)->password = NULL;
  }
  free(*stream);
  *stream = NULL;
  return 0;
}

int kr_stream_destroy(kr_stream **stream) {
  if ((stream == NULL) || (*stream == NULL)) {
    return -1;
  }
  kr_stream_disconnect(*stream);
  return kr_stream_free(stream);
}

int kr_stream_reconnect(kr_stream *stream) {
  //kr_stream_disconnect (stream);
  //FIXME RECCONNECTING
  return -1;
}

static kr_stream *kr_stream_connect(char *host, int port) {
  kr_stream *stream;
  int ret;
  int flags;
  char port_string[6];
  struct in6_addr serveraddr;
  struct addrinfo hints;
  struct addrinfo *res;
  res = NULL;
  if ((host == NULL) || ((port < 0) || (port > 65535))) {
    return NULL;
  }
  stream = kr_allocz(1, sizeof(kr_stream));
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_NUMERICSERV;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  ret = inet_pton(AF_INET, host, &serveraddr);
  if (ret == 1) {
    hints.ai_family = AF_INET;
    hints.ai_flags |= AI_NUMERICHOST;
  } else {
    ret = inet_pton(AF_INET6, host, &serveraddr);
    if (ret == 1) {
      hints.ai_family = AF_INET6;
      hints.ai_flags |= AI_NUMERICHOST;
    }
  }
  snprintf(port_string, 6, "%d", port);
  ret = getaddrinfo(host, port_string, &hints, &res);
  if (ret != 0) {
    kr_stream_destroy(&stream);
    return NULL;
  }
  flags = res->ai_socktype;
  flags |= SOCK_NONBLOCK | SOCK_CLOEXEC;
  stream->sd = socket(res->ai_family, flags, res->ai_protocol);
  if (stream->sd == -1) {
    kr_stream_destroy(&stream);
  } else {
    ret = connect(stream->sd, res->ai_addr, res->ai_addrlen);
    if ((ret != 0) && (errno != EINPROGRESS)) {
      kr_stream_destroy(&stream);
    }
  }
  if (res != NULL) {
    freeaddrinfo(res);
  }
  return stream;
}

static void kr_stream_read_http_headers(kr_stream *stream) {
  ssize_t ret;
  int32_t i;
  char buf[1024];
  while (1) {
    if (stream->drain > 0) {
      ret = read(stream->sd, buf, stream->drain);
      if (ret < 1) {
        if ((ret == 0) || ((ret == -1) && (errno != EAGAIN))) {
          stream->position = 0;
          stream->half_ready = 0;
          stream->ready = 0;
          stream->connected = 0;
        }
      } else {
        stream->drain -= ret;
        if (stream->hle == 2) {
          stream->position = 0;
          stream->half_ready = 0;
          stream->ready = 1;
          stream->connected = 1;
          return;
        }
      }
    } else {
      ret = recv(stream->sd, buf, sizeof(buf), MSG_PEEK);
      if (ret < 1) {
        if ((ret == 0) || ((ret == -1) && (errno != EAGAIN))) {
          stream->position = 0;
          stream->half_ready = 0;
          stream->ready = 0;
          stream->connected = 0;
        }
        return;
      } else {
        for (i = 0; i < ret; i++) {
          if ((buf[i] == '\n') || (buf[i] == '\r')) {
            if (stream->hle_pos != ((stream->position + i) - 1)) {
              stream->hle = 0;
              stream->hler = 0;
            }
            stream->hle_pos = stream->position + i;
            if (buf[i] == '\n') {
              stream->hle += 1;
            }
            if (buf[i] == '\r') {
              stream->hler += 1;
            }
            if (stream->hle == 2) {
              stream->drain = i + 1;
              break;
            }
          }
        }
        if (stream->drain == 0) {
          stream->drain = ret;
        }
      }
    }
  }
}

static void kr_stream_send_request_to_stream(kr_stream *stream) {
  ssize_t ret;
  int len;
  char http_req[512];
  char auth[256];
  char auth_base64[256];
  len = 0;
  snprintf(auth, sizeof(auth), "source:%s", stream->password);
  kr_base64_encode(auth_base64, auth, sizeof(auth_base64));
  len = snprintf(http_req, sizeof(http_req) - len,
                 "SOURCE %s ICE/1.0\r\n",
                 stream->mount);
  len += snprintf(http_req + len, sizeof(http_req) - len,
                  "content-type: %s\r\n",
                  stream->content_type);
  len += snprintf(http_req + len, sizeof(http_req) - len,
                  "Authorization: Basic %s\r\n\r\n",
                  auth_base64);
  ret = kr_stream_send(stream,
                       http_req + stream->position,
                       len - stream->position);
  if (ret > 0) {
    if (stream->position == len) {
      stream->position = 0;
      stream->ready = 1;
      stream->connected = 1;
    }
  }
}

static void kr_stream_send_request_for_stream(kr_stream *stream) {
  ssize_t ret;
  int len;
  char http_req[512];
  len = snprintf(http_req, sizeof(http_req),
                 "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n",
                 stream->mount, stream->host);
  ret = kr_stream_send(stream,
                       http_req + stream->position,
                       len - stream->position);
  if (ret > 0) {
    if (stream->position == len) {
      stream->position = 0;
      stream->half_ready = 1;
    }
  }
}

kr_stream *kr_stream_create(char *host, int port,
                                char *mount, char *content_type,
                                char *password) {
  kr_stream *stream;
  if ((host == NULL) || (mount == NULL) || (password == NULL) ||
      (content_type == NULL) || ((port < 0) || (port > 65535))) {
    return NULL;
  }
  if (mount[0] != '/') {
    return NULL;
  }
  stream = kr_stream_connect(host, port);
  if (stream == NULL) {
    return NULL;
  }
  stream->host = strdup(host);
  stream->port = port;
  stream->mount = strdup(mount);
  stream->password = strdup(password);
  stream->content_type = strdup(content_type);
  stream->direction = 1;
  return stream;
}

kr_stream *kr_stream_open(char *host, int port, char *mount) {
  kr_stream *stream;
  if ((host == NULL) || (mount == NULL) || ((port < 0) || (port > 65535))) {
    return NULL;
  }
  if (mount[0] != '/') {
    return NULL;
  }
  stream = kr_stream_connect(host, port);
  if (stream == NULL) {
    return NULL;
  }
  stream->host = strdup(host);
  stream->port = port;
  stream->mount = strdup(mount);
  stream->direction = 0;
  return stream;
}

int32_t kr_stream_handle_headers(kr_stream *stream) {
  if (stream->ready == 0) {
    if (stream->direction == 1) {
      kr_stream_send_request_to_stream (stream);
    } else {
      if (stream->half_ready == 0) {
        kr_stream_send_request_for_stream (stream);
      }
      if (stream->half_ready == 1) {
        kr_stream_read_http_headers (stream);
      }
    }
  }
  return stream->ready;
}

void kr_stream_i_am_a_blocking_subscripter(kr_stream *stream) {
  int ret;
  struct pollfd sp[1];
  if (stream->direction == 1) {
    sp[0].events = POLLOUT;
  } else {
    sp[0].events = POLLIN;
  }
  sp[0].fd = stream->sd;
  ret = poll(sp, 1, -1);
  if (sp[0].revents & POLLERR) {
    fprintf(stderr, "Got poll err on %s\n", stream->mount);
  }
  if (sp[0].revents & POLLHUP) {
    fprintf(stderr, "Got poll POLLHUP on %s\n", stream->mount);
  }
  if (stream->direction == 1) {
    if (!(sp[0].revents & POLLOUT)) {
      fprintf(stderr, "Did NOT get POLLOUT on %s\n", stream->mount);
    }
  } else {
    if (!(sp[0].revents & POLLIN)) {
      fprintf(stderr, "Did NOT get POLLIN on %s\n", stream->mount);
    }
  }
  if (ret != 1) {
    fprintf(stderr, "poll failure\n");
    exit(1);
  }
}
