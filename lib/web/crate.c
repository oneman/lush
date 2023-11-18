#include <krad/app/debug.h>
#include <krad/app/crate.h>
#include "crate.h"

static const kr_http_response_type res_kr_http_res_type[] = {
  [KR_NONE] = KR_HTTP_ERROR,
  [KR_OK] = KR_HTTP_OK,
  [KR_INFORM] = KR_HTTP_OK,
  [KR_CREATED] = KR_HTTP_CREATED,
  [KR_METHOD_NOPE] = KR_HTTP_METHOD_NOPE,
  [KR_NOT_FOUND] = KR_HTTP_NOT_FOUND,
  [KR_BAD_REQUEST] = KR_HTTP_BAD_REQUEST,
  [KR_CONFLICT] = KR_HTTP_CONFLICT,
  [KR_ERROR] = KR_HTTP_ERROR
};

const kr_http_response_type kr_res_to_http_res_type(kr_response res) {
  if (res > KR_ERROR) res = 0;
  return res_kr_http_res_type[res];
}

int kr_http_request_to_crate(kr_crate *crate, kr_http_request *request) {
  size_t addr_len;
  if (crate == NULL) return -1;
  if (request == NULL) return -2;
  addr_len = request->req_line.path_len;
  if (addr_len > sizeof(crate->address)) return -3;
  switch(request->req_line.method) {
    case KR_HTTP_GET:
      crate->method = KR_GET;
      break;
    case KR_HTTP_PUT:
      crate->method = KR_PUT;
      break;
    case KR_HTTP_POST:
      crate->method = KR_POST;
      break;
    case KR_HTTP_PATCH:
      crate->method = KR_PATCH;
      break;
    case KR_HTTP_DELETE:
      crate->method = KR_DELETE;
      break;
    default:
      printk("InterWeb: couldn't find kr method from http method");
      return -1;
  }
  memcpy(crate->address, request->req_line.path, addr_len);
  crate->addr_len = addr_len;
  if (crate->addr_len == 4 && !memcmp(crate->address, "/api", 4)) {
    crate->api = 1;
  }
  return 0;
}
