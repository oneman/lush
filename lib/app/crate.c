#include <krad/app/debug.h>
#include <krad/json/parser.h>
#include <krad/util/strton.h>
#include <krad/gen/metainfo.h>
#include <krad/gen/info_types.h>
#include <krad/util/util.h>
#include "crate.h"

static const char *kr_method_str[] = {
  [0] = "NOOP",
  [KR_GET] = "GET",
  [KR_PUT] = "PUT",
  [KR_POST] = "POST",
  [KR_PATCH] = "PATCH",
  [KR_DELETE] = "DELETE"
};

static int has_payload(kr_crate *crate) {
  if (crate->disposition == KR_RESPONSE) {
    return kr_response_crate_has_payload[crate->response];
  }
  return kr_request_crate_has_payload[crate->method];
}

const char *kr_method_to_str(kr_method m) {
  if (m > KR_DELETE) m = 0;
  return kr_method_str[m];
}

int kr_crate_reset(kr_crate *crate) {
  if (!crate) return -1;
  crate->method = KR_NOOP;
  crate->disposition = KR_REQUEST;
  crate->nfds = 0;
  return 0;
}

static int str_to_kr_method(char *str) {
  if (!strncmp(str,"GET",3) || !strncmp(str,"get",3)) return KR_GET;
  if (!strncmp(str,"PUT",3) || !strncmp(str,"put",3)) return KR_PUT;
  if (!strncmp(str,"POST",4) || !strncmp(str,"post",4)) return KR_POST;
  if (!strncmp(str,"PATCH",5) || !strncmp(str,"patch",5)) return KR_PATCH;
  if (!strncmp(str,"DELETE",6) || !strncmp(str,"delete",6)) return KR_DELETE;
  return 0;
}

int kr_crate_pack_ebml(kr_ebml *ebml, kr_crate *crate) {
  uint8_t *ebml_crate;
  int ret;
  int val;
  val = 0;
  if ((crate == NULL) || (ebml == NULL)) return -1;
  kr_ebml2_start_element(ebml, KR_EID_CRATE, &ebml_crate);
  val = crate->method;
  ret = kr_ebml_pack_int32(ebml, 0xe1, val);
  if (ret != 0) return ret;
  val = crate->type;
  ret = kr_ebml_pack_int32(ebml, 0xe1, val);
  if (ret != 0) return ret;
  ret = kr_ebml_pack_string(ebml, 0xe1, crate->address);
  if (ret != 0) return ret;
  if (has_payload(crate)) {
    if (crate->method == KR_PATCH) {
      ret = kr_ebml_pack_data(ebml, 0xe1, &crate->patchset, sizeof(crate->patchset));
    } else {
      ret = info_functions[crate->type].pack_ebml(ebml, &crate->info.buf);
    }
  }
  kr_ebml2_finish_element(ebml, ebml_crate);
  printkd("EBML PACKED: %d", ebml->pos);
  return ebml->pos;
}

int kr_crate_unpack_ebml(kr_crate *crate, kr_ebml *ebml) {
  uint64_t size;
  uint32_t element;
  int ret;
  int val;
  val = 0;
  if (!crate || !ebml) return -1;
  ret = kr_ebml2_unpack_id(ebml, &element, &size);
  if (ret < 0) {
    printke("App Server: Full crate EBML ID Not found");
    return 0;
  }
  printkd("EBML UNPACK size: %llu", size);
  size += ebml->pos;
  if (ebml->len < size) {
    printke("App Server: Crate not full...");
    return 0;
  }
  if (element == KR_EID_VOID) {
    return size;
  }
  if (element != KR_EID_CRATE) {
    printke("Crate: EBML Crate fubar %d", size);
    return -1;
  }
  ret = kr_ebml2_unpack_element_int32(ebml, NULL, &val);
  if (ret != 0) return ret;
  crate->method = val;
  ret = kr_ebml2_unpack_element_int32(ebml, NULL, &val);
  if (ret != 0) return ret;
  crate->type = val;
  ret = kr_ebml2_unpack_element_string(ebml, NULL, crate->address,
   sizeof(crate->address));
  if (ret != 0) return ret;
  crate->addr_len = strlen(crate->address);
  if (has_payload(crate)) {
    if (crate->method == KR_PATCH) {
      ret = kr_ebml2_unpack_element_data(ebml, NULL, &crate->patchset, sizeof(crate->patchset));
    } else {
      ret = info_functions[crate->type].unpack_ebml(ebml, &crate->info.buf);
      if (ret != 0) return ret;
    }
  }
  return size;
}

ssize_t kr_crate_info_pack_json(char *json, kr_crate *crate, size_t len) {
  int ret;
  if (crate->type < 1) {
    printke("Crate: attempted to pack a crate of unknown type");
    return -1;
  }
  ret = info_functions[crate->type].pack_json(json, &crate->info.buf, len);
  if ( (ret + 1) >= len) return 0;
  return ret;
}

ssize_t kr_crate_pack_json(char *json, kr_crate *crate, size_t len) {
  ssize_t ret;
  ssize_t pos;
  pos = 0;
  if ((crate == NULL) || (json == NULL)) return -1;
  if (len == 0) return -1;
  if (!has_payload(crate)) {
    ret = snprintf(json, len, "{\"meth\":\"%s\",\"path\":\"%.*s\"}", kr_method_str[crate->method],
     crate->addr_len, crate->address);
    if (ret >= len) return 0;
    pos = ret;
  } else {
    ret = snprintf(json, len, "{\"meth\":\"%s\",\"path\":\"%.*s\",\"meta\":{},\"data\":",
     kr_method_str[crate->method], crate->addr_len, crate->address);
    if (ret >= len) return 0;
    pos += ret;
    ret = kr_crate_info_pack_json(json + pos, crate, len - pos);
    if (ret < 0) return ret;
    if (ret + 1 >= len) return 0;
    pos += ret;
    json[pos] = '}';
    pos++;
  }
  return pos;
}

ssize_t kr_crate_to_string(char *string, kr_crate *crate, size_t size) {
  ssize_t res;
  if (crate == NULL || string == NULL || size == 0) return 0;
  res = 0;
  res += snprintf(&string[res], size, "Method: %s\n", kr_method_str[crate->method]);
  if (res >= size) return size;
  res += snprintf(&string[res], size, "Address: %s\n", crate->address);
  if (res >= size) return size;
  if (has_payload(crate)) {
    res += snprintf(&string[res], size, "Type: %s\n", kr_info_type_to_str(crate->type));
    if (res >= size) return size;
    res += info_functions[crate->type].to_text(&string[res], crate->info.buf, size - res);
    if (res >= size) return size;
  }
  return res;
}

ssize_t kr_crate_addr_meth_unpack_json(kr_crate *crate,
 char *json, size_t len) {
  ssize_t ret;
  kr_jps *jps;
  if (!crate || !json || !len) return -1;
  if (crate->method && !crate->api) return -2;
  kr_jps_alloca(jps);
  jps_init(jps);
  json[len] = '\0';
  ret = jps_parse(jps, json);
  if (ret != 0) return -3;
  if (jps_token_count(jps) < 2) return -4;
  if (jps_curr_type(jps) != JS_OBJECT) return -5;
  if (jps_curr_size(jps) <= 0 || jps_curr_size(jps) > 8) return -6;
  jps_next(jps);
  if (jps_curr_type(jps) != JS_STRING) return -7;
  if (jps_curr_len(jps) != 4 || memcmp(jps_curr_start(jps), "meth", 4)) return -8;
  jps_next(jps);
  crate->method = str_to_kr_method(jps_curr_start(jps));
  if (crate->method == 0) return -8;
  jps_next(jps);
  if (jps_curr_type(jps) != JS_STRING) return -7;
  if (jps_curr_len(jps) != 4 || memcmp(jps_curr_start(jps), "path", 4)) return -8;
  jps_next(jps);
  if (jps_curr_type(jps) != JS_STRING) return -1;
  strncpy(crate->address, jps_curr_start(jps), sizeof(crate->address) - 1);
  crate->addr_len = strlen(crate->address);
  jps_rewind(jps);
  return ret;
}

ssize_t kr_crate_unpack_json(kr_crate *crate, char *json, size_t len) {
  ssize_t ret;
  kr_jps *jps;
  char *type;
  if ((crate == NULL) || (json == NULL)) return -1;
  if (len == 0) return -2;
  if (!crate->type || !crate->method || !crate->addr_len) return -3;
  if (has_payload(crate) == KR_CRATE_OPT_PAYLOAD) {
    if (len == 2 && !memcmp(json, "{}", len)) {
      crate->type = KR_NO_INFO;
      return len;
    }
  }
  kr_jps_alloca(jps);
  jps_init(jps);
  json[len] = '\0';
  ret = jps_parse(jps, json);
  if (ret != 0) return -4;
  if (jps_token_count(jps) < 1) return -5;
  printkd("JSON crate unpack:\n\n  %s\n", jps_curr_start(jps));
  /*if (jps_curr_size(jps) <= 0 || jps_curr_size(jps) > 4) return -6;*/
  if (jps_curr_size(jps) <= 0 || jps_curr_size(jps) > 8) return -6;
  if (has_payload(crate)) {
    if (jps_token_count(jps) < 5) return -7;
    type = kr_info_type_to_str(crate->type);
    if (type == NULL) return -8;
    if (crate->method == KR_PATCH) {
      printkd("JSON PATCH: %*s type is %s",
       crate->addr_len, crate->address, type);
      ret = jps_find_key(jps, "data");
      //if (ret != 0) return -9;
      memset(&crate->patchset, 0, sizeof(crate->patchset));
      crate->patchset.type = crate->type;
      ret = kr_json_to_patchset(&crate->patchset,
       jps_curr_start(jps), jps_curr_len(jps));
      if (ret < 0) return -13;
      printkd("JSON PATCH: len is %d", ret);
    } else {
      printkd("JSON PUT: %*s type is %s",
       crate->addr_len, crate->address, type);
      ret = jps_find_key(jps, "data");
      //if (ret != 0) return -10;
      if (jps_curr_type(jps) != JS_OBJECT) return -11;
      info_functions[crate->type].init(&crate->info.buf);
      ret = info_functions[crate->type].unpack_json(jps_curr_start(jps),
        &crate->info.buf);
      if (ret < 0) return -12;
    }
  }
  ret = jps_curr_end(jps) - json;
  jps_rewind(jps);
  return ret;
}

int kr_crate_pack(kr_packing_plan *plan, kr_crate *crate) {
  kr_ebml ebml;
  if ((crate == NULL) || (plan == NULL)) return -1;
  if ((plan->buf == NULL) || (plan->len < 1)) return -2;
  switch (plan->format) {
    case KR_FORMAT_JSON:
      return kr_crate_pack_json(plan->json, crate, plan->sz);
    case KR_FORMAT_JSON_PAYLOAD:
      return kr_crate_info_pack_json(plan->json, crate, plan->sz);
    case KR_FORMAT_EBML:
      kr_ebml2_set_buffer(&ebml, plan->buf, plan->sz);
      return kr_crate_pack_ebml(&ebml, crate);
    case KR_FORMAT_CBOR:
      printke("Crate: Unimplemented info format");
      return -7;
    default:
      printke("Crate: Unknown info format");
      return -8;
  }
  return -9;
}

int kr_crate_unpack(kr_crate *crate, kr_packing_plan *plan) {
  kr_ebml ebml;
  if (!crate || !plan) return -1;
  if (!plan->buf || plan->len < 1) return -2;
  switch (plan->format) {
    case KR_FORMAT_JSON_PAYLOAD:
    case KR_FORMAT_JSON:
      return kr_crate_unpack_json(crate, plan->json, plan->sz);
    case KR_FORMAT_EBML:
      kr_ebml2_set_buffer(&ebml, plan->buf, plan->sz);
      return kr_crate_unpack_ebml(crate, &ebml);
    case KR_FORMAT_CBOR:
      printke("Crate: Unimplemented info format");
      return -7;
    default:
      printke("Crate: Unknown info format");
      return -8;
  }
  return -9;
}

int kr_crate_attach_fd(kr_crate *crate, int fd) {
  if (!crate) return -1;
  if (fd < 0) return -2;
  if (crate->nfds >= KR_CRATE_NFDS) return -3;
  crate->fd[crate->nfds] = fd;
  crate->nfds++;
  return 0;
}
