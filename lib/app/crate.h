#if !defined(_app_crate_H)
# define _app_crate_H (1)

typedef struct kr_crate kr_crate;

#include "types.h"
#include <krad/ebml/ebml.h>
#include <krad/gen/info_functions.h>

#define KR_CRATE_NFDS 4

typedef enum {
  KR_NOOP,
  KR_GET,
  KR_PUT,
  KR_POST,
  KR_PATCH,
  KR_DELETE
} kr_method;

typedef enum {
  KR_NONE,
  KR_OK,
  KR_INFORM,
  KR_CREATED,
  KR_NOT_FOUND,
  KR_BAD_REQUEST,
  KR_METHOD_NOPE,
  KR_CONFLICT,
  KR_ERROR
} kr_response;

typedef enum {
  KR_REQUEST,
  KR_RESPONSE
} kr_disposition;

typedef enum {
  KR_CRATE_NO_PAYLOAD = 0,
  KR_CRATE_REQ_PAYLOAD,
  KR_CRATE_OPT_PAYLOAD
} kr_payload;

struct kr_crate {
  kr_disposition disposition;
  union {
    kr_method method;
    kr_response response;
  };
  kr_info_type type;
  int addr_len;
  char address[64];
  int fd[KR_CRATE_NFDS];
  int nfds;
  union {
    kr_info info;
    kr_patchset patchset;
  };
  void *headers;
  void *client;
  int api;
};

typedef enum {
  KR_FORMAT_JSON,
  KR_FORMAT_JSON_PAYLOAD,
  KR_FORMAT_EBML,
  KR_FORMAT_CBOR
} kr_info_format;

typedef struct {
  union {
    uint8_t *buf;
    char *json;
  };
  union {
    size_t len;
    size_t sz;
  };
  kr_info_format format;
} kr_packing_plan;

static const int kr_response_crate_has_payload[] = {
  [KR_NONE] = 0,
  [KR_OK] = 0,
  [KR_INFORM] = 1,
  [KR_CREATED] = 0,
  [KR_NOT_FOUND] = 0,
  [KR_BAD_REQUEST] = 0,
  [KR_METHOD_NOPE] = 0,
  [KR_CONFLICT] = 0,
  [KR_ERROR] = 0
};

static const int kr_request_crate_has_payload[] = {
  [KR_GET] = KR_CRATE_NO_PAYLOAD,
  [KR_PUT] = KR_CRATE_REQ_PAYLOAD,
  [KR_POST] = KR_CRATE_OPT_PAYLOAD,
  [KR_PATCH] = KR_CRATE_REQ_PAYLOAD,
  [KR_DELETE] = KR_CRATE_NO_PAYLOAD
};

typedef struct {
  void *user;
  uint8_t *input;
  size_t len;
  uint8_t *output;
  size_t max;
} kr_load_plan;

typedef ssize_t (kr_crate_unloader)(kr_crate *crate, kr_load_plan *plan);
typedef ssize_t (kr_crate_loader)(kr_load_plan *plan, kr_crate *crate);

const char *kr_method_to_str(kr_method m);
int kr_crate_reset(kr_crate *crate);
ssize_t kr_crate_to_string(char *string, kr_crate *crate, size_t size);
int kr_crate_pack_ebml(kr_ebml *ebml, kr_crate *crate);
int kr_crate_unpack_ebml(kr_crate *crate, kr_ebml *ebml);
ssize_t kr_crate_info_pack_json(char *json, kr_crate *crate, size_t len);
ssize_t kr_crate_pack_json(char *json, kr_crate *crate, size_t len);
ssize_t kr_crate_unpack_json(kr_crate *crate, char *json, size_t len);
int kr_crate_pack(kr_packing_plan *plan, kr_crate *crate);
int kr_crate_unpack(kr_crate *crate, kr_packing_plan *plan);
ssize_t kr_crate_addr_meth_unpack_json(kr_crate *crate,
 char *json, size_t len);
int kr_crate_attach_fd(kr_crate *crate, int fd);

#endif
