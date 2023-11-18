#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <krad/app/debug.h>
#include <krad/mem/pool.h>
#include <krad/util/strton.h>
#include "client_internal.h"

#include "chunk.c"

static void client_nodelay(kr_web_client *client);
static int client_event(kr_event *event);
static ssize_t client_pack(kr_web_client *client, uint8_t *buf, size_t sz);
static ssize_t client_pushed(kr_web_client *client, size_t sz);
static ssize_t client_packed(kr_web_client *client);
static kr_web_client_info *client_info(kr_web_client *client);

static void client_nodelay(kr_web_client *client) {
  int ret;
  const int val = 1;
  ret = setsockopt(client->sd, IPPROTO_TCP, TCP_NODELAY, (const void *)&val,
   (socklen_t)sizeof(val));
  if (ret != 0) {
    printke("Web: Could not set socket TCP_NODELAY");
  }
}

static int client_upgrade_websocket(kr_web_path *path) {
  int ret;
  char accept_key[64];
  char ws_proto[32];
  char *buffer;
  int pos;
  kr_web_client_info *info;
  kr_http_header key;
  kr_http_header ver;
  kr_http_header proto;
  kr_web_client *client;
  kr_http_request *req;
  client = &path->client;
  info = &path->info.client;
  req = &client->http.request;
  ret = 0;
  ret = kr_http_get_header(&proto, "Sec-WebSocket-Protocol", 22, req);
  if (ret != 0) return ret;
  ret = kr_http_get_header(&ver, "Sec-WebSocket-Version", 21, req);
  if (ret != 0) return ret;
  if ((ver.value_len != 2) || (memcmp(ver.value, "13", 2) != 0)) {
    printke("WebClient: Bad websockets version");
    return -1;
  }
  ret = kr_http_get_header(&key, "Sec-WebSocket-Key", 17, req);
  if (ret != 0) return ret;
  /* FIXME check subprotocols against server? */
  memset(accept_key, 0, sizeof(accept_key));
  ret = kr_websocket_accept_key(accept_key, sizeof(accept_key), key.value,
   key.value_len);
  if (ret < 1) {
    printke("WebClient: Error generating client accept key");
    return -1;
  }
  buffer = (char *)path->client.out->buf;
  pos = 0;
  pos += sprintf(buffer + pos, "HTTP/1.1 101 Switching Protocols\r\n");
  pos += sprintf(buffer + pos, "Upgrade: websocket\r\n");
  pos += sprintf(buffer + pos, "Connection: Upgrade\r\n");
  pos += sprintf(buffer + pos, "Sec-WebSocket-Accept: %s\r\n", accept_key);
  pos += sprintf(buffer + pos, "Sec-WebSocket-Protocol: %.*s\r\n",
   (int)proto.value_len, proto.value);
  pos += sprintf(buffer + pos, "\r\n");
  printkd("Websocket Client:\n%s", buffer);
  ret = client_pushed(&path->client, pos);
  if (ret != pos) {
    printke("Web Client %zu: Couldn't pack WebSocket upgrade?", client->num);
    return -1;
  }
  ret = client_packed(&path->client);
  if (ret != 0) {
    printke("Web Client %zu: pkin ws upgrade: could not set path writable",
     client->num);
    return -1;
  }
  path->info.client.protocol = KR_PROTOCOL_WEBSOCKET;
  info->ws.input.pos = 0;
  info->ws.input.sz = 0;
  info->ws.input.frame = 0;
  memset(&client->ws, 0, sizeof(client->ws));
  if (proto.value_len > sizeof(ws_proto)) {
    printke("Websocket: WS proto too damn long!");
    return -1;
  }
  memset(ws_proto, 0, sizeof(ws_proto));
  memcpy(ws_proto, proto.value, proto.value_len);
  ret = str_to_kr_websocket_protocol(ws_proto);
  if (ret < 0) {
    printke("Websocket: WS proto %.*s not valid!",
     proto.value_len, proto.value);
    return -1;
  }
  info->ws.proto = ret;
  if (info->ws.proto == KR_WS_PROTOCOL_KRAD) {
    info->ws.type = KR_WEBSOCKET_TEXT;
    printk("Websocket: %.*s proto is text", proto.value_len, proto.value);
  } else if (info->ws.proto == KR_WS_PROTOCOL_STREAM) {
    info->ws.type = KR_WEBSOCKET_BINARY;
    printk("Websocket: %.*s proto is binary", proto.value_len, proto.value);
  } else if (info->ws.proto == KR_WS_PROTOCOL_XMMS) {
    info->ws.type = KR_WEBSOCKET_BINARY;
    printk("Websocket: %.*s proto is binary", proto.value_len, proto.value);
  } else {
    printke("Websocket: protocol %.*s unknown!", proto.value_len, proto.value);
    return -1;
  }
  printkd("Web Client %zu: Upgraded to websocket", client->num);
  path_event(path, KR_WEB_HEADERS);
  return 0;
}

static int client_upgrade(kr_web_path *path) {
  int ret;
  kr_http_header header;
  kr_web_client *client;
  kr_http_request *req;
  client = &path->client;
  req = &client->http.request;
  ret = kr_http_get_header(&header, "Upgrade", 7, req);
  if (ret != 0) ret = kr_http_get_header(&header, "upgrade", 7, req);
  if (ret != 0) {
    printk("WebClient: No upgrade header");
    return -1;
  }
  printkd("WebClient: Upgrade desired");
  if ((header.value_len == 9) && (memcmp("websocket", header.value, 9) == 0)) {
    return client_upgrade_websocket(path);
  }
  printke("WebClient: Unknown upgrade type");
  return -1;
}

static int get_multipart_boundary(kr_http_request *req,
 uint8_t *boundary, uint8_t *len) {
  int ret;
  kr_http_header hdr;
  ret = kr_http_get_header(&hdr, "Content-Type", 12, req);
  if (ret != 0) return -1;
  if (hdr.value_len <= 30) return -1;
  if (memcmp(hdr.value, "multipart/form-data; ", 21)) return -1;
  if (memcmp(hdr.value + 21, "boundary=", 9)) return -1;
  *len = hdr.value_len - 30;
  if (*len > 70) return -1;
  memcpy(boundary, hdr.value + 30, *len);
  return 0;
}

static uint8_t *kr_memmem(uint8_t *mem, size_t mem_len,
 uint8_t *sub, size_t sub_len) {
  int pos;
  int sub_pos;
  if (!mem || !sub) return NULL;
  if (!mem_len || !sub_len) return NULL;
  for (pos = 0 ; pos < mem_len; pos++) {
    if (*(mem + pos) != *sub) continue;
    if ( (mem_len - pos) < sub_len ) return NULL;
    for (sub_pos = 0; sub_pos < sub_len; sub_pos++) {
      if ( *(mem + pos + sub_pos) != *(sub + sub_pos) ) break;
    }
    if (sub_pos == sub_len) return mem + pos;
  }
  return NULL;
}

static ssize_t get_multipart_data(kr_web_client *client,
 uint8_t *buf, size_t len) {
  kr_web_client_info *info;
  int trailer;
  int ret;
  size_t pos;
  kr_iob *in;
  uint8_t start;
  in = client->in;
  start = 0;
  uint8_t *bnd;
  info = client_info(client);
  if (in->len < (client->boundary_len + 4)) return 0;
/*  if (!memcmp(in->rd_buf + in->len - client->boundary_len - 4,
   client->boundary, client->boundary_len)) {
    if (!memcmp(in->rd_buf + in->len - 4, "--\r\n", 4)) {
      printk("multipart: THE END");
      //kr_io2_pulled(in, client->boundary_len + 2);
      printk("%.*s", client->boundary_len + 4,
        in->rd_buf + in->len - client->boundary_len - 4);
      ret = kr_io2_unpack(in, buf, in->len - client->boundary_len - 6);
      //printk("in->len %d %d", in->len, client->boundary_len + 4);
      kr_io2_pulled(in, in->len);
      //ret = kr_io2_unpack(in, buf, len);
      return ret;
    }
  }*/

  if (info->http.input.multipart == KR_MULTIPART_INIT) {
    if (!memcmp(in->rd_buf + 2, client->boundary, client->boundary_len)) {
      start = 2;
      info->http.input.multipart = KR_MULTIPART_INFO;
      printk("multipart: starting..");
      if (start == 0) return -1;
      printk("multipart: found boundary!");
      pos = client->boundary_len + start + 2;
      trailer = find_trailer(in->rd_buf + pos,
       in->len - pos);
      if (trailer < 0) return 0;
      trailer += pos;
      printk("multipart inner header: %.*s", trailer - pos,
       in->rd_buf + pos);
      pos = trailer + 2;
      trailer = find_trailer(in->rd_buf + pos,
       in->len - pos);
      if (trailer < 0) return 0;
      trailer += pos;
      printk("multipart inner header: %.*s", trailer - pos,
       in->rd_buf + pos);
      kr_io2_pulled(in, trailer + 4);
    } else {
      return -1;
    }
  }
  bnd = kr_memmem(in->rd_buf, in->len, client->boundary, client->boundary_len);
  if (bnd) {
    printk("multipart: end of file data.");
    ret = kr_io2_unpack(in, buf, bnd - in->rd_buf - 4);
    kr_io2_pulled(in, in->len);
    info->http.input.multipart = KR_MULTIPART_DONE;
    info->http.input.state = KR_HTTP_IN_HEADER;
    info->http.input.pos = 0;
  } else {
    info->http.input.multipart = KR_MULTIPART_DATA;
    ret = kr_io2_unpack(in, buf, len);
  }
  return ret;
}

static ssize_t client_parse_http(kr_web_path *path) {
  ssize_t ret;
  kr_web_client_info *info;
  kr_web_client *client;
  kr_http_request *req;
  kr_http_response *res;
  kr_http_method method;
  kr_http_header hdr;
  info = &path->info.client;
  client = &path->client;
  ret = 0;
  method = 0;
  req = &client->http.request;
  res = &client->http.response;
  if (info->http.input.state == KR_HTTP_IN_HEADER) {
    if (info->type == KR_WEB_CLIENT_ACCEPT) {
      ret = kr_http_request_parse(req, (char *)client->in->buffer,
       client->in->len);
      if (ret > 0) {
        printkd("Web Client %zu: Got Request %zu", client->num,
         ++info->http.input.num);
      } else {
        printke("Web Client %zu: Got Incomplete Header", client->num);
      }
    } else {
      ret = kr_http_response_parse(res, (char *)client->in->buffer,
       client->in->len);
      if (ret > 0) {
        printk("Web Client %zu: Got Response %zu", client->num,
         ++info->http.input.num);
        info->http.input.state = KR_HTTP_IN_CONTENT;
        info->http.input.pos = 0;
        info->http.input.chunked = 0;
      } else {
        printk("Web Client %zu: Got Incomplete Header", client->num);
      }
    }
    if (ret < 1) {
      return ret;
    } else {
      kr_io2_pulled(client->in, ret);
      if ((info->type == KR_WEB_CLIENT_ACCEPT)
       && (kr_http_upgrade_wanted(req))) {
        return client_upgrade(path);
      }
      if (1) {
        if (info->type == KR_WEB_CLIENT_ACCEPT) {
          method = req->req_line.method;
        }
        if (((info->type == KR_WEB_CLIENT_ACCEPT) && ((method == KR_HTTP_PUT)
         || (method == KR_HTTP_SOURCE) || (method == KR_HTTP_POST)
         || (method == KR_HTTP_PATCH)))
         || (info->type == KR_WEB_CLIENT_CONNECT)) {
          info->http.input.state = KR_HTTP_IN_CONTENT;
          info->http.input.pos = 0;
          info->http.input.chunked = 0;
          if (info->type == KR_WEB_CLIENT_ACCEPT) {
            ret = kr_http_get_header(&hdr, "Content-Length", 14, req);
          } else if (info->type == KR_WEB_CLIENT_CONNECT) {
            ret = kr_http_response_get_header(&hdr, "Content-Length", 14, res);
          }
          if (ret == 0) {
            ret = kr_strtou64(&info->http.input.sz, hdr.value, hdr.value_len);
            if (ret != 0) {
              printke("Web Client %zu: content length is not a valid number!",
                client->num);
              return -1;
            }
            if (info->http.input.sz > 0) {
              printkd("Web Client %zu: Got content length: %zu", client->num,
               info->http.input.sz);
            }
            if (!get_multipart_boundary(req, client->boundary,
             &client->boundary_len)) {
              info->http.input.multipart = KR_MULTIPART_INIT;
              printk("multipart!");
            }
          } else {
            if (info->type == KR_WEB_CLIENT_ACCEPT) {
              ret = kr_http_get_header(&hdr, "Transfer-Encoding", 17, req);
            } else if (info->type == KR_WEB_CLIENT_CONNECT) {
              ret = kr_http_response_get_header(&hdr, "Transfer-Encoding", 17, res);
            }
            if (ret != 0) {
              printk("Web Client %zu: No content length in request header..",
                client->num);
              ret = path_event(path, KR_WEB_HEADERS);
              return ret;
            }
            if (hdr.value_len == 7 &&
              !memcmp(hdr.value, "chunked", 7)) {
              printk("Web Client %zu: Got chunked content", client->num);
              info->http.input.chunked = 1;
            }
          }
        }
      }
      ret = path_event(path, KR_WEB_HEADERS);
      if (ret != 0) return ret;
    }
  }
  if ((info->http.input.state == KR_HTTP_IN_CONTENT)
   && client->in->len > 0) {
    //printk("Got %d bytes", client->in->len);
    ret = path_event(path, KR_WEB_DATA);
  }
  return ret;
}

static ssize_t client_parse_ws_header(kr_web_path *path) {
  kr_web_client *client;
  ssize_t ret;
  kr_iob *in;
  kr_ws_frame *frm;
  client = &path->client;
  ret = -1;
  in = client->in;
  frm = &client->ws;
  if (frm->pos == frm->len) {
    memset(frm, 0, sizeof(*frm));
    ret = kr_ws_unpack_header(frm, in->rd_buf, in->len);
    if (ret == -1) return 0;
    kr_io2_pulled(in, ret);
    return ret;
  }
  return 0;
}

static ssize_t client_have_ws_frame(kr_web_path *path) {
  kr_web_client_info *info;
  kr_web_client *client;
  kr_iob *in;
  kr_ws_frame *frm;
  client = &path->client;
  info = &path->info.client;
  in = client->in;
  frm = &client->ws;
  if (in->len < frm->len) {
    printke("we dont have all the data for websocket frame right now");
    return 0;
  }
  info->ws.input.frame++;
  printkd("Websocket frame: #%zu %zu byte payload", info->ws.input.frame, frm->len);
  info->ws.input.pos = 0;
  info->ws.input.sz = frm->len;
  return 1;
}

static ssize_t client_handle_control_frame(kr_web_path *path) {
  kr_web_client *client;
  ssize_t ret;
  uint8_t buf[KR_WS_CONTROL_FRAME_MAX];
  kr_ws_frame *frm;
  kr_iob *in;
  kr_iob *out;
  kr_web_client_info *info;
  info = &path->info.client;
  client = &path->client;
  in = client->in;
  out = client->out;
  ret = -1;
  frm = &client->ws;
  if ((frm->opcode == KR_WS_CLOSE) || (frm->len > KR_WS_CONTROL_FRAME_MAX)) {
    /* screw it, pack close frame */
    printk("WebSocket: Close frame");
    ret = kr_websocket_pack_ctl_header(out->buf, KR_WS_CLOSE, out->space, 0);
    client_pushed(client, ret);
    client_packed(client);
    return -1;
  }
  ret = kr_io2_unpack(in, buf, frm->len);
  kr_ws_unmask(frm, buf, frm->len);
  info->ws.input.pos += ret;
  if (frm->opcode == KR_WS_PING) {
    printk("WebSocket: Ping frame");
    /* pack a pong */
    ret = kr_websocket_pack_ctl_header(out->buf, KR_WS_PONG, out->space,
     frm->len);
    if ((ret == -1) || (out->space < (ret + frm->len))) {
      printke("Web path pack: not enough space for ctl header in client output buffer");
      return -1;
    }
    client_pushed(client, ret);
    client_pack(client, buf, frm->len);
    client_packed(client);
    return 0;
  }
  if (frm->opcode == KR_WS_PONG) {
    return 0;
  }
  return -1;
}

static ssize_t client_parse_websocket(kr_web_path *path) {
  kr_web_client *client;
  ssize_t ret;
  kr_ws_frame *frm;
  client = &path->client;
  ret = -1;
  frm = &client->ws;
  for (;;) {
    if (frm->pos == frm->len) {
      ret = client_parse_ws_header(path);
      if (ret == 0) return ret;
    }
    ret = client_have_ws_frame(path);
    if ((ret == 1) && (frm->type == KR_WS_CONTROL)) {
      ret = client_handle_control_frame(path);
      if (ret != 0) return -1;
      continue;
    }
    break;
  }
  if (ret == 1) {
    path_event(path, KR_WEB_DATA);
  }
  return 0;
}

static void client_print(kr_web_path *path) {
  int ret;
  char text[8 * 8192];
  kr_web_client_info *info;
  info = &path->info.client;
  memset(text, 0, sizeof(text));
  ret = kr_web_client_info_to_text(text, info, sizeof(text));
  if (ret < 1) {
    printke("Web Client: Could not print client info :/");
    return;
  }
  printk("Client info:\n%s\n", text);
  memset(text, 0, sizeof(text));
  ret = kr_web_client_info_pack_json(text, info, sizeof(text));
  if (ret < 1) {
    printke("Web Client: Could not print client info :/");
    return;
  }
  printk("Client info:\n%s\n", text);
}

static ssize_t client_parse(kr_web_path *path) {
  int ret;
  ret = 0;
  kr_web_client_info *info;
  info = &path->info.client;
  if (info->protocol == KR_PROTOCOL_WEBSOCKET) {
    ret = client_parse_websocket(path);
  } else if (info->protocol == KR_PROTOCOL_HTTP) {
    ret = client_parse_http(path);
  } else if (info->protocol == KR_PROTOCOL_TCP) {
    ret = path_event(path, KR_WEB_DATA);
  }
  if (0) client_print(path);
  return ret;
}
#define container_of(ptr, type, member) ({ \
                    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
                    (type *)( (char *)__mptr - offsetof(type,member) );})

static kr_web_path *client_path(kr_web_client *client) {
  return container_of(client, kr_web_path, client);
}

static kr_web_client_info *client_info(kr_web_client *client) {
  kr_web_path *path;
  path = container_of(client, kr_web_path, client);
  return &path->info.client;
}

static ssize_t client_pack(kr_web_client *client, uint8_t *buf, size_t sz) {
  if (sz == 0) return -1;
  kr_io2_pack(client->out, buf, sz);
  return sz;
}

static ssize_t client_pushed(kr_web_client *client, size_t sz) {
  if (sz == 0) return -1;
  kr_io2_advance(client->out, sz);
  return sz;
}

static ssize_t client_packed(kr_web_client *client) {
  int ret;
  kr_event event;
  kr_web_path *path;
  path = client_path(client);
  memset(&event, 0, sizeof(event));
  event.fd = client->sd;
  event.events = EPOLLIN | EPOLLOUT;
  event.user = path;
  event.handler = client_event;
  ret = kr_loop_mod(path_loop(path), &event);
  return ret;
}

static ssize_t client_write(kr_web_path *path) {
  ssize_t bytes;
  kr_web_client *client;
  kr_web_client_info *info;
  client = &path->client;
  info = &path->info.client;
  bytes = 0;
  if (!client->out) return -1;
  kr_io2_output(client->out, &bytes);
  if (bytes > 0) info->bytes_out += bytes;
  return bytes;
}

static ssize_t client_read(kr_web_path *path) {
  ssize_t ret;
  kr_web_client *client;
  kr_web_client_info *info;
  client = &path->client;
  info = &path->info.client;
  ret = 0;
  ret = kr_io2_read(client->in);
  if (ret > 0) info->bytes_in += ret;
  return ret;
}

static int client_event(kr_event *event) {
  int ret;
  kr_web_path *path;
  path = (kr_web_path *)event->user;
  ret = 0;
  if (path->client.tls) {
    kr_tls_client_io_shake(path->client.tls);
  }
  if (event->events & EPOLLHUP || event->events & EPOLLERR) {
    printke("WebClient: event HUP or ERR");
    kr_web_path_remove(path);
    return 0;
  }
  if (event->events & EPOLLIN) {
    /*printk("WebClient: epollin");*/
    if (ret < 0) {
      kr_web_path_remove(path);
      return 0;
    }
    ret = client_read(path);
    if (ret < 1) {
      kr_web_path_remove(path);
      return 0;
    } else {
      ret = client_parse(path);
      if (ret < 0) {
        kr_web_path_remove(path);
        return 0;
      }
    }
  }
  if (event->events & EPOLLOUT) {
    /*printk("WebClient: epollout");*/
    if ((path->info.client.type == KR_WEB_CLIENT_CONNECT)
     && (path->info.client.state == KR_WEB_CONNECTING)) {
      path->info.client.state = KR_WEB_ACTIVE;
      ret = path_event(path, KR_WEB_OPEN);
      if (ret != 0) {
        kr_web_path_remove(path);
        return 0;
      }
    }
    ret = client_write(path);
    if (ret < 0) {
      kr_web_path_remove(path);
      return 0;
    }
    if (path->client.more_data) {
      path_event(path, KR_WEB_DATA);
      return 0;
    }
    if (!(kr_io2_want_out(path->client.out))) {
      /*printk("WebClient: Output buffer drained");*/
      event->events = EPOLLIN;
      event->user = path;
      event->handler = client_event;
      ret = kr_loop_mod(path_loop(path), event);
      if (ret) {
        printke("WebClient: loop mod ret %d", ret);
      }
    }
  }
  return 0;
}

static int client_connect(kr_web_path *path) {
  kr_web_client *client;
  kr_web_client_info *info;
  kr_web_remote_info *remote;
  client = &path->client;
  info = &path->info.client;
  remote = &info->remote;
  int ret;
  int flags;
  char port_string[6];
  struct in6_addr serveraddr;
  struct addrinfo hints;
  struct addrinfo *res;
  res = NULL;
  if ((remote->host[0] == 0) || ((remote->port < 0) || (remote->port > 65535))) {
    printke("client_connect() invalid remote info");
    return -1;
  }
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_NUMERICSERV;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  ret = inet_pton(AF_INET, remote->host, &serveraddr);
  if (ret == 1) {
    hints.ai_family = AF_INET;
    hints.ai_flags |= AI_NUMERICHOST;
  } else {
    ret = inet_pton(AF_INET6, remote->host, &serveraddr);
    if (ret == 1) {
      hints.ai_family = AF_INET6;
      hints.ai_flags |= AI_NUMERICHOST;
    }
  }
  snprintf(port_string, 6, "%d", remote->port);
  ret = getaddrinfo(remote->host, port_string, &hints, &res);
  if (ret != 0) {
    printke("getaddrinfo ret %d", ret);
    return -1;
  }
  flags = res->ai_socktype;
  flags |= SOCK_NONBLOCK | SOCK_CLOEXEC;
  client->sd = socket(res->ai_family, flags, res->ai_protocol);
  if (client->sd == -1) {
    printke("client_connect() socket() fail");
    return -1;
  } else {
    ret = connect(client->sd, res->ai_addr, res->ai_addrlen);
    if (ret == -1 && errno == EINPROGRESS) {
      /*printk("connect in progress");*/
      ret = 1;
    }
    if (ret != 0 && errno != EINPROGRESS) {
      printke("Web connect client: connect() %s", strerror(errno));
      return -1;
    }
  }
  if (res != NULL) {
    freeaddrinfo(res);
  }
  client_nodelay(client);
  return ret;
}

static void client_init(kr_web_path *path) {
  kr_web_client *client;
  kr_web_client_info *info;
  client = &path->client;
  info = &path->info.client;
  memset(info, 0, sizeof(*info));
  info->protocol = KR_PROTOCOL_HTTP;
  info->created_on = time(NULL);
  client->in = kr_io2_create_size(65536);
  client->out = kr_io2_create_size(65536);
}

int client_path_close(kr_web_path *path) {
  int ret;
  int len;
  uint8_t buf[2];
  kr_web_client *client;
  kr_web_client_info *info;
  client = &path->client;
  info = &path->info.client;
  printkd("WebClient: Closed. Bytes IN: %zu OUT: %zu", info->bytes_in,
   info->bytes_out);
  ret = kr_loop_del(path_loop(path), client->sd);
  if (info->protocol == KR_PROTOCOL_WEBSOCKET) {
    len = kr_websocket_pack_ctl_header(buf, KR_WS_CLOSE, sizeof(buf), 0);
    if (len) ret = write(client->sd, buf, len);
  }
  if (client->tls) {
    kr_tls_client_io_shutdown(client->tls);
    ret = kr_tls_client_clear(client->tls);
    client->tls = NULL;
  }
  close(client->sd);
  kr_io2_destroy(&client->in);
  kr_io2_destroy(&client->out);
  client->sd = -1;
  return ret;
}

int client_path_accept(kr_web_path *path) {
  int ret;
  kr_event harness;
  kr_web_client_info *info;
  info = &path->info.client;
  client_init(path);
  client_nodelay(&path->client);
  kr_io2_set_fd(path->client.in, path->client.sd);
  kr_io2_set_fd(path->client.out, path->client.sd);
  if (path->client.tls) {
    kr_io2_set_io(path->client.in, (kr_io_cb *)kr_tls_client_read,
     (kr_io_cb *)kr_tls_client_write, path->client.tls);
    kr_io2_set_io(path->client.out, (kr_io_cb *)kr_tls_client_read,
     (kr_io_cb *)kr_tls_client_write, path->client.tls);
  }
  info->type = KR_WEB_CLIENT_ACCEPT;
  info->state = KR_WEB_ACTIVE;
  harness.fd = path->client.sd;
  harness.user = path;
  harness.handler = client_event;
  harness.events = EPOLLIN;
  ret = kr_loop_add(path_loop(path), &harness);
  if (ret != 0) {
    printke("Web (accept): kr_loop_add failed %d", ret);
  }
  return ret;
}

int client_path_connect(kr_web_path *path, kr_web_client_setup_info *setup) {
  int ret;
  kr_event harness;
  kr_web_client *client;
  kr_web_client_info *info;
  client = &path->client;
  client_init(path);
  info = &path->info.client;
  info->type = KR_WEB_CLIENT_CONNECT;
  info->state = KR_WEB_CONNECTING;
  if (setup->protocol == KR_PROTOCOL_TCP) {
    info->protocol = setup->protocol;
  }
  info->remote = setup->remote;
  ret = client_connect(path);
  if (ret < 0) return -1;
  if (ret == 0) info->state = KR_WEB_ACTIVE;
  kr_io2_set_fd(client->in, client->sd);
  kr_io2_set_fd(client->out, client->sd);
  if (path->client.tls) {
    kr_io2_set_io(path->client.in, (kr_io_cb *)kr_tls_client_read,
     (kr_io_cb *)kr_tls_client_write, path->client.tls);
    kr_io2_set_io(path->client.out, (kr_io_cb *)kr_tls_client_read,
     (kr_io_cb *)kr_tls_client_write, path->client.tls);
  }
  harness.fd = client->sd;
  harness.user = path;
  harness.handler = client_event;
  harness.events = EPOLLOUT;
  ret = kr_loop_add(path_loop(path), &harness);
  if (ret != 0) {
    printke("Web (connect): kr_loop_add failed %d", ret);
  }
  if (ret == 0) {
    if (info->state == KR_WEB_ACTIVE) {
      path_event(path, KR_WEB_OPEN);
    }
  }
  return ret;
}

kr_web_path *kr_web_client_get_path(kr_web_client *client) {
  if (!client) return NULL;
  return client_path(client);
}

ssize_t kr_web_client_unpack(kr_web_client *client, uint8_t *buf, size_t len) {
  ssize_t ret;
  kr_iob *in;
  kr_ws_frame *frm;
  kr_web_client_info *info;
  kr_web_path *path;
  ret = -1;
  in = client->in;
  info = client_info(client);
  path = client_path(client);
  switch (info->protocol) {
    case KR_PROTOCOL_TCP:
      ret = kr_io2_unpack(in, buf, len);
      break;
    case KR_PROTOCOL_HTTP:
      if (!info->http.input.chunked) {
        if (info->http.input.multipart) {
          /* printk("want multipart data"); */
          ret = get_multipart_data(client, buf, len);
        } else {
          if (info->http.input.sz && (len > info->http.input.sz - info->http.input.pos)) {
            len = info->http.input.sz - info->http.input.pos;
          }
          ret = kr_io2_unpack(in, buf, len);
          if (ret > 0) {
            info->http.input.pos += ret;
            if (info->http.input.pos == info->http.input.sz) {
              info->http.input.state = KR_HTTP_IN_HEADER;
              info->http.input.pos = 0;
            }
          }
        }
      } else {
        ret = dechunker(&client->ct, in, buf, len);
        if (client->ct.state == KR_CHUNKED_DONE)
          memset(&client->ct, 0, sizeof(client->ct));
      }
      break;
    case KR_PROTOCOL_WEBSOCKET:
      frm = &client->ws;
      if (frm->pos == frm->len) {
        ret = client_parse_ws_header(path);
        if (ret == 0) return ret;
      }
      if (info->ws.input.pos == info->ws.input.sz) {
        ret = client_have_ws_frame(path);
        if (ret == 0) return ret;
      }
      /* FIXME control frames are not handled here */
      if (frm->len > len) {
        printke("Not enough room in pack unpath output buffer");
        return -1;
      }
      ret = kr_io2_unpack(in, buf, frm->len);
      kr_ws_unmask(frm, buf, frm->len);
      info->ws.input.pos += ret;
      break;
    default:
      printke("Web unpack: unhandled protocol type");
  }
  return ret;
}

ssize_t kr_web_client_pack_chunking_trailer(kr_web_client *client) {
  kr_iob *out;
  out = client->out;
  return kr_io2_pack(out, KR_CHUNKING_TRAILER, KR_CHUNKING_TRAILER_LEN);
}

ssize_t kr_web_client_pack_chunk(kr_web_client *client,
 uint8_t *buf, size_t sz) {
  ssize_t ret;
  kr_iob *out;
  kr_web_client_info *info;
  out = client->out;
  ret = -1;
  info = client_info(client);
  if (!sz) return -1;
  switch (info->protocol) {
    case KR_PROTOCOL_HTTP:
      ret = chunker(&client->ct, out, buf, sz);
      if (ret < 0) break;
      client_packed(client);
      break;
    default:
      printke("Web Path Pack: unhandled protocol type");
  }
  return ret;
}

int kr_web_client_want_space(kr_web_client *client) {
  if (!client) return -1;
  client->more_data = 1;
  return 0;
}

ssize_t kr_web_client_space(kr_web_client *client) {
  ssize_t ret;
  kr_iob *out;
  if (!client) return -1;
  //if (!client->out) return -2;
  out = client->out;
  ret = out->space;
  return ret;
}

ssize_t kr_web_client_pack(kr_web_client *client, uint8_t *buf, size_t sz) {
  ssize_t ret;
  kr_iob *out;
  kr_web_client_info *info;
  if (!client || !buf || !sz) return -1;
  //if (!client->out) return -2;
  out = client->out;
  ret = -1;
  info = client_info(client);
  switch (info->protocol) {
    case KR_PROTOCOL_TCP:
    case KR_PROTOCOL_HTTP:
      if (out->space < sz) {
        client->more_data = 1;
        sz = out->space;
      } else {
        client->more_data = 0;
      }
      client_pack(client, buf, sz);
      client_packed(client);
      ret = sz;
      break;
    case KR_PROTOCOL_WEBSOCKET:
      if (info->ws.type == KR_WEBSOCKET_TEXT) {
        ret = kr_websocket_pack_header(out->buf, out->space, sz);
      } else if (info->ws.type == KR_WEBSOCKET_BINARY) {
        ret = kr_websocket_pack_binary_header(out->buf, out->space, sz);
      }
      if ((ret == -1) || (out->space < (ret + sz))) {
        printke("Web path pack: not enough space for ws text or binary header in client output buffer");
        return -1;
      }
      client_pushed(client, ret);
      client_pack(client, buf, sz);
      client_packed(client);
      ret = ret + sz;
      break;
    default:
      printke("Web Path Pack: unhandled protocol type");
  }
  return ret;
}

ssize_t kr_web_client_pack_response(kr_web_client *c, kr_http_response *res) {
  ssize_t ret;
  size_t max;
  char *str;
  if (!c || !res) return -1;
  if (!c->out) return -2;
  kr_web_client_info *info;
  info = client_info(c);
  if (info->protocol != KR_PROTOCOL_HTTP) {
    printke("Web Client: Not packing HTTP Response for non-http client");
    return -1;
  }
  str = (char *)c->out->buf;
  max = c->out->space;
  ret = kr_http_response_string(str, res, max);
  if (ret < 0) return ret;
  client_pushed(c, ret);
  client_packed(c);
  return ret;
}

ssize_t kr_web_client_pack_request(kr_web_client *c, kr_http_request *req) {
  ssize_t ret;
  size_t max;
  char *str;
  if (!c || !req) return -1;
  if (!c->out) return -2;
  kr_web_client_info *info;
  info = client_info(c);
  if (info->protocol != KR_PROTOCOL_HTTP) {
    printke("Web Client: Not packing HTTP Request for non-http client");
    return -1;
  }
  str = (char *)c->out->buf;
  max = c->out->space;
  ret = kr_http_request_string(str, req, max);
  if (ret < 0) return ret;
  client_pushed(c, ret);
  client_packed(c);
  return ret;
}

kr_http_response *kr_web_client_get_response(kr_web_client *client) {
  kr_web_path *path;
  if (!client) return NULL;
  path = client_path(client);
  if (path->info.client.type != KR_WEB_CLIENT_CONNECT) return NULL;
  return &client->http.response;
}

kr_http_request *kr_web_client_get_request(kr_web_client *client) {
  kr_web_path *path;
  if (!client) return NULL;
  path = client_path(client);
  if (path->info.client.type != KR_WEB_CLIENT_ACCEPT) return NULL;
  return &client->http.request;
}

kr_web_client_info *kr_web_client_get_info(kr_web_client *client) {
  if (!client) return NULL;
  return client_info(client);
}

kr_web_client *kr_web_path_get_client(kr_web_path *path) {
  if (!path) return NULL;
  if (path->info.type != KR_WEB_CLIENT) return NULL;
  return &path->client;
}
