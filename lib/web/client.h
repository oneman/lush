#include <sys/types.h>

#if !defined(_web_client_H)
# define _web_client_H (1)

typedef struct {
  char *key;
  size_t key_len;
  char *value;
  size_t value_len;
} kr_web_key_value;

#include <krad/web/http/http.h>
#include <krad/web/protocol/websocket.h>

typedef struct kr_web_client kr_web_client;

ssize_t kr_web_client_unpack(kr_web_client *client, uint8_t *buf, size_t len);
ssize_t kr_web_client_pack_chunking_trailer(kr_web_client *client);
ssize_t kr_web_client_pack_chunk(kr_web_client *client, uint8_t *buf, size_t sz);
int kr_web_client_want_space(kr_web_client *client);
ssize_t kr_web_client_space(kr_web_client *client);
ssize_t kr_web_client_pack(kr_web_client *client, uint8_t *buf, size_t sz);
ssize_t kr_web_client_pack_response(kr_web_client *c, kr_http_response *r);
ssize_t kr_web_client_pack_request(kr_web_client *c, kr_http_request *r);
kr_http_response *kr_web_client_get_response(kr_web_client *client);
kr_http_request *kr_web_client_get_request(kr_web_client *client);
kr_web_client_info *kr_web_client_get_info(kr_web_client *client);
kr_web_client *kr_web_path_get_client(kr_web_path *path);
kr_web_path *kr_web_client_get_path(kr_web_client *client);

#endif
