#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <krad/web/client.h>
#include <krad/app/debug.h>
#include <krad/util/strton.h>
#include "http.h"
#include "versions.h"

static size_t random_server_version(char **ver);

static char hex_decode_char(uint8_t c) {
  return isdigit(c) ? c - '0' : c - 'a' + 10;
}

static char hex_decode(uint8_t hi, uint8_t lo) {
  return (hex_decode_char(hi) << 4) + hex_decode_char(lo);
}

static bool is_hex_char(int c) {
  return (isdigit(c) || (c <= 'f' && c >= 'a'));
}

ssize_t kr_http_url_decode(char *out, size_t max, char *in, size_t len) {
  size_t in_pos;
  size_t out_pos;
  char c;
  char hi;
  char lo;
  in_pos = 0;
  out_pos = 0;
  while (in_pos < len) {
    if (out_pos >= max) return -1;
    c = in[in_pos++];
    if (c == '%') {
      if (!(in_pos + 1 < len)) return -1;
      hi = tolower(in[in_pos++]);
      lo = tolower(in[in_pos++]);
      if (!is_hex_char(hi) || !is_hex_char(lo)) return -1;
      out[out_pos++] = hex_decode(hi, lo);
    } else {
      if (c == '+') c = ' ';
      out[out_pos++] = c;
    }
  }
  return out_pos;
}

static const char *http_method_str[] = {
  [KR_HTTP_GET] = "GET",
  [KR_HTTP_PUT] = "PUT",
  [KR_HTTP_SOURCE] = "SOURCE",
  [KR_HTTP_POST] = "POST",
  [KR_HTTP_HEAD] = "HEAD",
  [KR_HTTP_PATCH] = "PATCH",
  [KR_HTTP_DELETE] = "DELETE",
  [KR_HTTP_OPTIONS] = "OPTIONS",
  [KR_HTTP_TRACE] = "TRACE"
};

static size_t random_server_version(char **ver) {
  int i;
  static const int n = sizeof(server_versions) / sizeof(server_versions[0]);
  i = rand() % (n - 1);
  *ver = server_versions[i];
  return strlen(*ver);
}

static kr_http_response_type response_code_to_type(int code) {
  switch (code) {
    case 100:
      return KR_HTTP_CONTINUE;
    case 200:
      return KR_HTTP_OK;
    case 201:
      return KR_HTTP_CREATED;
    case 206:
      return KR_HTTP_PARTIAL;
    case 301:
      return KR_HTTP_MOVED;
    case 302:
      return KR_HTTP_FOUND;
    case 400:
      return KR_HTTP_BAD_REQUEST;
    case 403:
      return KR_HTTP_FORBIDDEN;
    case 404:
      return KR_HTTP_NOT_FOUND;
    case 405:
      return KR_HTTP_METHOD_NOPE;
    case 409:
      return KR_HTTP_CONFLICT;
    case 416:
      return KR_HTTP_RANGE_NOPE;
    case 500:
      return KR_HTTP_ERROR;
    default:
      break;
  }
  return KR_HTTP_UNKNOWN;
}

static const char *httpmstr(kr_http_method method) {
  return http_method_str[method];
}

static void print_request_line(kr_http_request_line *r) {
  int i;
  const char *ver;
  kr_http_param *p;
  if (r->version == KR_HTTP_VERSION_1_1) {
    ver = "HTTP 1.1";
  } else {
    ver = "Not Exactly HTTP 1.1";
  }
  printk("%s %.*s %s", httpmstr(r->method), r->path_len, r->path, ver);
  for (i = 0; i < r->param_count; i++) {
    p = &r->param[i];
    printk(" Parameter '%.*s' : '%.*s'", p->key_len, p->key, p->value_len,
     p->value);
  }
}

static void print_header(kr_http_header *h) {
  printk(" %.*s: %.*s", h->key_len, h->key, h->value_len, h->value);
}

typedef struct {
  char *str;
  size_t sz;
} kr_line;

static int line_header(kr_http_header *header, kr_line *line) {
  char *end;
  int sub;
  if (!header) return -1;
  if (!line) return -2;
  sub = 1;
  header->key = line->str;
  end = memchr(header->key, ':', line->sz);
  if (!end) return -3;
  header->key_len = end - header->key;
  if (isspace(end[1])) sub = 2;
  if (line->sz - sub <= header->key_len) return -4;
  header->value = end + sub;
  header->value_len = line->sz - (header->key_len + sub);
  return 0;
}

static int parse_parameters(kr_http_param *param, size_t n,
 uint8_t *buf, size_t sz) {
  int i;
  int p;
  int t;
  int pos;
  int count;
  char *str;
  static const char token[2] = { '=', '&' };
  t = 0;
  p = 0;
  pos = 0;
  count = 1;
  str = (char *)buf;
  param[p].key = str;
  param[p].key_len = sz;
  if (sz == 0) return -1;
  for (i = 0; i < sz; i++) {
    if (p >= n) return -2;
    if (buf[i] == token[t]) {
      t++;
      if (t == 1) {
        param[p].key = str;
        param[p].key_len = pos;
      }
      if (t == 2) {
        param[p].value = str;
        param[p].value_len = pos;
        t = 0;
        p++;
        count = p + 1;
      }
      pos = 0;
      str = (char *)buf + i + 1;
    } else {
      pos++;
    }
  }
  if ((t == 1) || (i == sz)) {
    param[p].value = str;
    param[p].value_len = pos;
  }
  return count;
}

static int line_to_request_line(kr_http_request_line *request_line,
 kr_line *line) {
  int ret;
  char *str;
  char *end;
  int len;
  if (!request_line) return -1;
  if (!line) return -2;
  str = line->str;
  end = memchr(str, ' ', line->sz);
  if (!end) return -3;
  len = end - str;
  switch (len) {
    case 3:
      if (memcmp(str, "GET", len) == 0) request_line->method = KR_HTTP_GET;
      if (memcmp(str, "PUT", len) == 0) request_line->method = KR_HTTP_PUT;
      break;
    case 4:
      if (memcmp(str, "POST", len) == 0) request_line->method = KR_HTTP_POST;
      if (memcmp(str, "HEAD", len) == 0) request_line->method = KR_HTTP_HEAD;
      break;
    case 5:
      if (memcmp(str, "PATCH", len) == 0) request_line->method = KR_HTTP_PATCH;
      if (memcmp(str, "TRACE", len) == 0) request_line->method = KR_HTTP_TRACE;
      break;
    case 6:
      if (memcmp(str, "SOURCE", len) == 0) request_line->method =
       KR_HTTP_SOURCE;
      if (memcmp(str, "DELETE", len) == 0) request_line->method =
       KR_HTTP_DELETE;
      break;
    case 7:
      if (memcmp(str, "OPTIONS", len) == 0) request_line->method =
       KR_HTTP_OPTIONS;
      break;
    default:
      return -1;
      break;
  }
  request_line->path = end + 1;
  request_line->path_len = line->sz - (len + 1);
  /* get that http version */
  request_line->version = KR_HTTP_VERSION_UNKNOWN;
  str = memrchr(request_line->path, ' ', request_line->path_len);
  //printk("path is '%.*s' %d", request_line->path_len, request_line->path,
  // request_line->path_len);
  if (str) {
    str++;
    len = request_line->path_len - (str - request_line->path);
    //printk("str is '%.*s' %d", len, str, len);
    if (len >= 8) {
      if (memcmp(str, "HTTP/", 5) == 0) {
        request_line->path_len -= len + 1;
      }
      if (memcmp(str + 5, "1.1", 3) == 0) {
        request_line->version = KR_HTTP_VERSION_1_1;
      }
    }
  }
  /* split off them params */
  str = memchr(request_line->path, '?', request_line->path_len);
  if (str) {
    len = (request_line->path_len) - (str - request_line->path);
    request_line->path_len -= len;
    str++;
    len--;
    memset(request_line->param, 0, sizeof(request_line->param));
    request_line->param_count = 0;
    ret = parse_parameters(request_line->param, KR_HTTP_PARAMS_MAX,
      (uint8_t *)str, len);
    if (ret < 0) return ret;
    request_line->param_count = ret;
  }
  return 0;
}

static int get_line(kr_line *line, uint8_t *buf, size_t sz) {
  int pos;
  int found;
  uint8_t *end;
  pos = 0;
  found = 0;
  end = buf + sz;
  line->str = (char *)buf;
  while ((buf != end) && (!found)) {
    if (*buf == '\n') {
      found = 1;
      if ((pos > 0) && (buf[-1] == '\r')) {
        line->sz = pos - 1;
      } else {
        line->sz = pos;
      }
    } else {
      pos++;
    }
    buf++;
  }
  return found;
}

int kr_http_url_parse(kr_http_url *url, char *buf, size_t len) {
  int ret;
  char *req;
  char *slash;
  char *port;
  char *at;
  char *sep;
  size_t port_len;
  if (!url || !buf || !len) return -1;
  if (len < 9) return -2;
  ret = kr_http_url_decode(buf, len, buf, len);
  if (ret < 0) return -3;
  if (memcmp(buf, "http://", 7)) return -4;
  port_len = 0;
  buf += 7;
  len -= 7;
  at = memchr(buf, '@', len);
  if (at) {
    sep = memchr(buf, ':', at - buf);
    if (!sep) return -5;
    url->user_len = sep - buf;
    url->user = buf;
    url->pwd = sep + 1;
    url->pwd_len = at - sep - 1;
    buf += (url->pwd_len + url->user_len + 2);
    len -= (url->pwd_len + url->user_len + 2);
  }
  slash = memchr(buf, '/', len);
  if (slash) {
    port = memchr(buf, ':', slash - buf);
    if (port) {
      kr_strtou16(&url->port, port + 1, slash - port - 1);
      url->host = buf;
      url->host_len = port - buf;
      port_len = slash - port;
    } else  {
      url->host = buf;
      url->host_len = slash - buf;
      url->port = 80;
    }
  } else {
    port = memchr(buf, ':', len);
    if (port) {
      kr_strtou16(&url->port, port + 1, buf + len - port - 1);
      url->host = buf;
      url->host_len = port - buf;
      port_len = buf + len - port;
    } else {
      url->host = buf;
      url->host_len = len;
      url->port = 80;
    }
  }
  buf += url->host_len + port_len;
  len -= url->host_len + port_len;
  if (len == 0) return 0;
  url->path_len = 0;
  req = memchr(buf, '?', len);
  if (req) {
    url->path = buf;
    url->path_len = req - buf;
  } else {
    url->path = buf;
    url->path_len = len;
    return 0;
  }
  buf += url->path_len + 1;
  len -= url->path_len + 1;
  url->path_len += len + 1;
  ret = parse_parameters(url->param, KR_HTTP_PARAMS_MAX, (uint8_t *)buf, len);
  if (ret <= 0) return ret;
  url->param_count = ret;
  return 0;
}

int kr_http_req_indata(kr_http_request *req) {
  if ((req->req_line.method == KR_HTTP_PUT)
   || (req->req_line.method == KR_HTTP_POST)
   || (req->req_line.method == KR_HTTP_PATCH)) {
    return 1;
  }
  return 0;
}

int kr_http_upgrade_wanted(kr_http_request *req) {
  int i;
  int ret;
  kr_http_header header;
  ret = kr_http_get_header(&header, "Connection", 10, req);
  if (ret != 0) kr_http_get_header(&header, "connection", 10, req);
  if (ret != 0) return 0;
  if (header.value_len < 7) return 0;
  for (i = 0; i + 7 <= header.value_len; i++) {
    if (header.value[i] == 'U' || header.value[i] == 'u') {
      if (memcmp("pgrade", header.value + (i + 1), 6) == 0) return 1;
    }
  }
  return 0;
}

int kr_http_get_header(kr_http_header *header, char *name, size_t len,
 kr_http_request *req) {
  int i;
  for (i = 0; i < KR_HTTP_HEADERS_MAX; i++) {
    if (req->header[i].key_len == len) {
      if (strncasecmp(req->header[i].key, name, len) == 0) {
        header->key = req->header[i].key;
        header->key_len = req->header[i].key_len;
        header->value = req->header[i].value;
        header->value_len = req->header[i].value_len;
        return 0;
      }
    }
  }
  return -1;
}

int kr_http_response_get_header(kr_http_header *header, char *name, size_t len,
 kr_http_response *res) {
  int i;
  for (i = 0; i < KR_HTTP_HEADERS_MAX; i++) {
    if (res->headers[i].key_len == len) {
      if (strncasecmp(res->headers[i].key, name, len) == 0) {
        header->key = res->headers[i].key;
        header->key_len = res->headers[i].key_len;
        header->value = res->headers[i].value;
        header->value_len = res->headers[i].value_len;
        return 0;
      }
    }
  }
  return -1;
}

static int64_t parse_int(char *buf, size_t sz) {
  int64_t integer;
  char cpy[32];
  int i;
  if (sz > 20) return -1;
  for (i = 0; i < sz; i++) {
    if (!isdigit(buf[i])) return -1;
  }
  memcpy(cpy, buf, sz);
  buf[sz] = '\0';
  integer = strtoll(buf, NULL, 10);
  return integer;
}

static int parse_range(kr_http_range *range, char *buf, size_t sz) {
  int i;
  int64_t start;
  int64_t end;
  start = 0;
  if ((buf[0] == '-') || (buf[sz - 1] == '-')) {
    if (buf[0] == '-') {
      start = parse_int(buf + 1, sz - 1);
      if (start == -1) return 0;
      start = start * -1;
    } else if (buf[sz - 1] == '-') {
      start = parse_int(buf, sz - 1);
      if (start == -1) return 0;
    }
    range->type = KR_HTTP_RANGE_START_FROM;
    range->start = start;
    range->end = 0;
    printk("Parsed start from range: %zd", range->start);
    return 1;
  }
  for (i = 0; i < sz; i++) {
    if (buf[i] == '-') {
      start = parse_int(buf, i);
      if (start < 0) return 0;
      end = parse_int(buf + i + 1, sz - (i + 1));
      if (end < start) return 0;
      range->type = KR_HTTP_RANGE_SPEC;
      range->start = start;
      range->end = end;
      printk("Parsed full range: %zd - %zd", range->start, range->end);
      return 1;
    }
  }
  return 0;
}

static int parse_ranges(kr_http_range *rngs, int n, char *buf, size_t sz) {
  int i;
  int c;
  int r;
  int ret;
  int len;
  r = 0;
  c = 0;
  for (i = 0; i < sz; i++) {
    ret = 0;
    len = 0;
    if (buf[i] == ',') {
      len = i - c;
    } else if (i + 1 == sz) {
      len = sz - c;
    }
    if (len) {
      ret = parse_range(&rngs[r], buf + c, len);
      if (ret) {
        r++;
        if (r == n) return r;
        c = i + 1;
      } else {
        printke("Parsing range: %.*s", len, buf + c);
        return -1;
      }
    }
  }
  return r;
}

int kr_http_request_get_ranges(kr_http_request *req, kr_http_range *ranges,
 int nranges) {
  static const int pf = 6;
  int ret;
  kr_http_header hdr;
  if (!req || !ranges || nranges < 1) return -1;
  ret = kr_http_get_header(&hdr, "Range", 5, req);
  if (ret) return 0;
  if (hdr.value_len < 8 || (memcmp(hdr.value, "bytes=", pf) != 0)) return -1;
  printk("Got range request!: %.*s", hdr.value_len - pf, hdr.value + pf);
  ret = parse_ranges(ranges, nranges, hdr.value + pf, hdr.value_len - pf);
  return ret;
}

ssize_t kr_http_request_parse(kr_http_request *req, char *buf, size_t sz) {
  /* entry point function: return -1 on fatally screwed up stuff, or
   * no end of headers, return 0 on incomplete headers, return the byte
   * position of the first byte of payload data on success */
  int ret;
  ssize_t len;
  int cnt;
  char *start;
  kr_line line;
  cnt = 0;
  start = buf;
  memset(&line, 0, sizeof(line));
  for (;;) {
    if (get_line(&line, (uint8_t *)buf, sz)) {
      /*printk("line found. len: %d '%.*s'", line.sz, line.sz, line.str);*/
      if (buf[line.sz] == '\r') {
        buf += line.sz + 2;
      } else {
        buf += line.sz + 1;
      }
      if (line.sz == 0) {
        return buf - start;
      }
      if (cnt == 0) {
        ret = line_to_request_line(&req->req_line, &line);
        len = kr_http_url_decode(req->req_line.path,
         req->req_line.path_len, req->req_line.path, req->req_line.path_len);
        if (ret == 0 && len > 0) {
          req->req_line.path_len = len;
          if (0) print_request_line(&req->req_line);
        } else {
          printke("Interweb: parse request line failed %d", ret);
          return -1;
        }
      } else {
        if (cnt > KR_HTTP_HEADERS_MAX) {
          printke("Interweb: too many http headers to parse.");
          return -1;
        }
        ret = line_header(&req->header[cnt - 1], &line);
        if (ret == 0) {
          if (0) print_header(&req->header[cnt - 1]);
        } else {
          printke("Interweb: parse header line failed %d", ret);
          continue;
        }
      }
      req->count = cnt;
      cnt++;
    } else {
      if (buf - start == 4096) {
        return -1;
      } else {
        return 0;
      }
    }
  }
  return -1;
}

ssize_t kr_http_response_parse(kr_http_response *res, char *buf, size_t sz) {
  int ret;
  char *start;
  char code[4];
  int32_t type;
  kr_line line;
  memset(&line, 0, sizeof(line));
  start = buf;
  //printk("buf: %.*s", sz, buf);
  if (!get_line(&line, (uint8_t *)(buf) , sz)) return -1;
  if (line.sz == 0) return -1;
  if (memcmp(line.str, "HTTP/", 5)) return -1;
  if (memcmp(line.str + 5, "1.1", 3) &&
   memcmp(line.str + 5, "1.0", 3)) return -1;
  memcpy(code, line.str + 9, 3);
  code[3] = '\0';
  //printk("code %s", code);
  ret = kr_strtoi32(&type, code, 3);
  if (ret != 0) return -1;
  res->type = response_code_to_type(type);
  if (buf[line.sz] == '\r') buf += line.sz + 2;
  else buf += line.sz + 1;
  for (;;) {
    if (get_line(&line, (uint8_t *)(buf) , sz - (buf - start))) {
      if (buf[line.sz] == '\r') buf += line.sz + 2;
      else buf += line.sz + 1;
      if (line.sz == 0) return buf - start;
      //printk("LINE: %.*s", line.sz, line.str);
      ret = line_header(&res->headers[res->count++], &line);
      if (ret == 0) {
        if (0) print_header(&res->headers[res->count - 1]);
      } else {
        printke("Interweb: parse header line failed %d", ret);
        continue;
      }
    } else {
      if (buf - start == 4096) return -1;
      else return 0;
    }
  }
}

ssize_t kr_http_request_string(char *str, kr_http_request *req, size_t max) {
  int i;
  int res;
  const char *method;
  if (req == NULL || str == NULL) return -1;
  res = 0;
  if (req->req_line.method < KR_HTTP_METHOD_FIRST) return -1;
  if (req->req_line.method > KR_HTTP_METHOD_LAST) return -1;
  method = http_method_str[req->req_line.method];
  res += snprintf(str + res, max - res, "%s ", method);
  if (res >= max) return -1;
  res += snprintf(str + res, max - res, "%.*s",
   (int)req->req_line.path_len, req->req_line.path);
  if (res >= max) return -1;
  for (i = 0; i < req->req_line.param_count; i++) {
    if (i == 0) {
      res += snprintf(str + res, max - res, "?");
    } else {
      res += snprintf(str + res, max - res, "&");
    }
    if (res >= max) return -1;
    res += snprintf(str + res, max - res, "%.*s=%.*s",
     (int)req->req_line.param[i].key_len, req->req_line.param[i].key,
     (int)req->req_line.param[i].value_len, req->req_line.param[i].value);
    if (res >= max) return -1;
  }
  if (req->req_line.version != KR_HTTP_VERSION_1_1) return -1;
  res += snprintf(str + res, max - res, " HTTP/1.1\r\n");
  if (res >= max) return -1;
  for (i = 0; i < req->count; i++) {
    res += snprintf(str + res, max - res, "%.*s: %.*s\r\n",
     (int)req->header[i].key_len, req->header[i].key,
     (int)req->header[i].value_len, req->header[i].value);
    if (res >= max) return -1;
  }
  res += snprintf(str + res, max - res, "\r\n");
  if (res >= max) return -1;
  return res;
}

ssize_t kr_http_response_string(char *str, kr_http_response *res, size_t max) {
  int i;
  int p;
  int len;
  int rem;
  kr_http_header *hdr;
  len = 0;
  rem = max - 2;
  if (!str) return -1;
  if (!res) return -1;
  if (max < 2) return -1;
  switch (res->type) {
    case KR_HTTP_CONTINUE:
      p = snprintf(str, rem, "HTTP/1.1 100 Continue\r\n\r\n");
      if (p > rem) return -1;
      return p;
      break;
    case KR_HTTP_OK:
      p = snprintf(str, rem, "HTTP/1.1 200 OK\r\n");
      if (p > rem) return -1;
      str += p;
      len += p;
      rem -= p;
      break;
    case KR_HTTP_CREATED:
      p = snprintf(str, rem, "HTTP/1.1 201 Created\r\n");
      if (p > rem) return -1;
      str += p;
      len += p;
      rem -= p;
      break;
    case KR_HTTP_PARTIAL:
      p = snprintf(str, rem, "HTTP/1.1 206 Partial Content\r\n");
      if (p > rem) return -1;
      str += p;
      len += p;
      rem -= p;
      break;
    case KR_HTTP_MOVED:
      p = snprintf(str, rem, "HTTP/1.1 301 Moved Permanently\r\n");
      if (p > rem) return -1;
      str += p;
      len += p;
      rem -= p;
      break;
    case KR_HTTP_BAD_REQUEST:
      p = snprintf(str, rem, "HTTP/1.1 400 Bad Request\r\n");
      if (p > rem) return -1;
      str += p;
      len += p;
      rem -= p;
     break;
    case KR_HTTP_FORBIDDEN:
      p = snprintf(str, rem, "HTTP/1.1 403 Forbidden\r\n");
      if (p > rem) return -1;
      str += p;
      len += p;
      rem -= p;
     break;
    case KR_HTTP_NOT_FOUND:
      p = snprintf(str, rem, "HTTP/1.1 404 Not Found\r\n");
      if (p > rem) return -1;
      str += p;
      len += p;
      rem -= p;
     break;
    case KR_HTTP_METHOD_NOPE:
      p = snprintf(str, rem, "HTTP/1.1 405 Method Not Allowed\r\n");
      if (p > rem) return -1;
      str += p;
      len += p;
      rem -= p;
     break;
    case KR_HTTP_CONFLICT:
      p = snprintf(str, rem, "HTTP/1.1 409 Conflict\r\n");
      if (p > rem) return -1;
      str += p;
      len += p;
      rem -= p;
     break;
    case KR_HTTP_ERROR:
      p = snprintf(str, rem, "HTTP/1.1 500 Internal Server Error\r\n");
      if (p > rem) return -1;
      str += p;
      len += p;
      rem -= p;
     break;
   default:
    printke("HTTP: Don't know about type");
    return -1;
  }
  /*
  pos += sprintf(buffer + pos, "Connection: close\r\n");
  pos += sprintf(buffer + pos, "Server: Krad Radio %d\r\n", KR_VERSION);
  pos += sprintf(buffer + pos, "Content-Type: %s; charset=utf-8\r\n", mimetype);
  pos += sprintf(buffer + pos, "Content-Length: %d\r\n", len);
  */
  for (i = 0; i < res->count; i++) {
    hdr = &res->headers[i];
    p = snprintf(str, rem, "%.*s: %.*s\r\n", (int)hdr->key_len, hdr->key,
     (int)hdr->value_len, hdr->value);
    if (p > rem) return -1;
    str += p;
    len += p;
    rem -= p;
  }
  p = snprintf(str, rem, "\r\n");
  if (p > rem) return -1;
  str += p;
  len += p;
  rem -= p;
  return len;
}

ssize_t kr_http_response_add_header(kr_http_response *response,
 kr_http_header *header) {
  kr_http_header *hdr;
  if (response->count == KR_HTTP_HEADERS_MAX) {
    return -1;
  }
  hdr = &response->headers[response->count++];
  *hdr = *header;
  return 0;
}

ssize_t kr_http_request_add_header(kr_http_request *request,
 kr_http_header *header) {
  kr_http_header *hdr;
  if (request->count == KR_HTTP_HEADERS_MAX) {
    return -1;
  }
  hdr = &request->header[request->count++];
  *hdr = *header;
  return 0;
}

ssize_t kr_http_request_add_param(kr_http_request *r, kr_http_param *param) {
  kr_http_param *p;
  if (r->req_line.param_count == KR_HTTP_PARAMS_MAX) {
    return -1;
  }
  p = &r->req_line.param[r->req_line.param_count++];
  *p = *param;
  return 0;
}

int kr_http_request_init(kr_http_request *req) {
  if (!req) return -1;
  req->req_line.method = KR_HTTP_GET;
  req->req_line.path_len = 0;
  req->req_line.param_count = 0;
  req->req_line.version = KR_HTTP_VERSION_1_1;
  req->count = 0;
  return 0;
}

int kr_http_request_set_method(kr_http_request *req, kr_http_method meth) {
  if (!req) return -1;
  req->req_line.method = meth;
  return 0;
}

int kr_http_request_set_path(kr_http_request *req, char *path, size_t len) {
  if (!req || len < 1) return -1;
  req->req_line.path = path;
  req->req_line.path_len = len;
  return 0;
}

int kr_http_response_any_origin(kr_http_response *res) {
  static kr_http_header hdr;
  if (!hdr.key) {
    hdr.key = "Access-Control-Allow-Origin";
    hdr.key_len = strlen(hdr.key);
    hdr.value = "*";
    hdr.value_len = strlen(hdr.value);
  }
  return kr_http_response_add_header(res, &hdr);
}

int kr_http_response_zero_content(kr_http_response *res) {
  static kr_http_header hdr;
  if (!hdr.key) {
    hdr.key = "Content-Length";
    hdr.key_len = strlen(hdr.key);
    hdr.value = "0";
    hdr.value_len = strlen(hdr.value);
  }
  return kr_http_response_add_header(res, &hdr);
}

int kr_http_response_transfer_chunked(kr_http_response *res) {
  static kr_http_header hdr;
  if (!hdr.key) {
    hdr.key = "Transfer-Encoding";
    hdr.key_len = strlen(hdr.key);
    hdr.value = "chunked";
    hdr.value_len = strlen(hdr.value);
  }
  return kr_http_response_add_header(res, &hdr);
}

int kr_http_response_content_encoding(kr_http_response *res) {
  static kr_http_header hdr;
  if (!hdr.key) {
    hdr.key = "Content-Encoding";
    hdr.key_len = 16;
    hdr.value = "gzip";
    hdr.value_len = 4;
  }
  return kr_http_response_add_header(res, &hdr);
}

int kr_http_response_connection_close(kr_http_response *res) {
  static kr_http_header hdr;
  if (!hdr.key) {
    hdr.key = "Connection";
    hdr.key_len = strlen(hdr.key);
    hdr.value = "Close";
    hdr.value_len = strlen(hdr.value);
  }
  return kr_http_response_add_header(res, &hdr);
}

int kr_http_response_accept_ranges(kr_http_response *res) {
  static kr_http_header hdr;
  if (!hdr.key) {
    hdr.key = "Accept-Ranges";
    hdr.key_len = strlen(hdr.key);
    hdr.value = "bytes";
    hdr.value_len = strlen(hdr.value);
  }
  return kr_http_response_add_header(res, &hdr);
}

static int http_date_at(kr_http_date_header *dh, time_t *now) {
  static const clockid_t cid = CLOCK_REALTIME_COARSE;
  static const char *http_date_fmt = "%a, %d %b %Y %T GMT";
  int ret;
  struct tm gmt;
  struct timespec ts_now;
  time_t unsecs;
  if (now) {
    unsecs = *now;
  } else {
    ret = clock_gettime(cid, &ts_now);
    if (ret) return -1;
    unsecs = ts_now.tv_sec;
  }
  if (!gmtime_r(&unsecs, &gmt)) return -1;
  ret = strftime(dh->str, sizeof(dh->str), http_date_fmt, &gmt);
  if (ret < 29) return -1;
  dh->hdr.value = dh->str;
  dh->hdr.value_len = ret;
  return 0;
}

int kr_http_date_header_at(kr_http_date_header *dh, time_t *now) {
  int ret;
  dh->hdr.key = "Date";
  dh->hdr.key_len = 4;
  ret = http_date_at(dh, now);
  return ret;
}

int kr_http_lastmod_header_at(kr_http_date_header *dh, time_t *now) {
  int ret;
  dh->hdr.key = "Last-Modified";
  dh->hdr.key_len = 13;
  ret = http_date_at(dh, now);
  return ret;
}

int kr_http_lastmod_header_now(kr_http_date_header *dh) {
  return kr_http_lastmod_header_at(dh, NULL);
}

int kr_http_date_header_now(kr_http_date_header *dh) {
  return kr_http_date_header_at(dh, NULL);
}

int kr_http_length_header_init(kr_http_header *hdr, size_t len, uint8_t *buf,
 size_t max) {
  int c;
  c = snprintf((char *)buf, max, "%zu", len);
  if (c > max) return -1;
  hdr->key = "Content-Length";
  hdr->key_len = 14;
  hdr->value = (char *)buf;
  hdr->value_len = c;
  return c;
}

int kr_http_range_header_init(kr_http_header *hdr, size_t start, size_t end,
 size_t sz, uint8_t *buf, size_t max) {
  int c;
  c = snprintf((char *)buf, max, "bytes %zu-%zu/%zu ", start, end, sz);
  if (c > max) return -1;
  hdr->key = "Content-Range";
  hdr->key_len = 13;
  hdr->value = (char *)buf;
  hdr->value_len = c;
  return c;
}

int kr_http_response_add_version_rand(kr_http_response *res) {
  int ret;
  kr_http_header hdr;
  hdr.key = "Server";
  hdr.key_len = 6;
  hdr.value_len = random_server_version(&hdr.value);
  ret = kr_http_response_add_header(res, &hdr);
  kr_http_date_header dh;
  kr_http_date_header_now(&dh);
  return ret;
}

int kr_http_response_init(kr_http_response *res, kr_http_response_type type) {
  if (!res) return -1;
  res->type = type;
  res->count = 0;
  return 0;
}
