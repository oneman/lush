#if !defined(_web_dtls_H)
# define _web_dtls_H (1)

#include <openssl/ssl.h>

typedef struct kr_dtls kr_dtls;
typedef struct kr_dtls_conn kr_dtls_conn;

struct kr_dtls {
  SSL_CTX *ctx;
};

struct kr_dtls_conn {
  SSL *ssl;
  BIO *in;
  BIO *out;
  uint8_t *in_buf;
  int in_pos;
  int in_len;
  uint8_t *out_buf;
  int out_pos;
  int out_len;
};

size_t kr_dtls_size();
size_t kr_dtls_conn_size();
int kr_dtls_clear(kr_dtls *dtls);
int dtls_verify(int ok, X509_STORE_CTX *ctx);
int kr_dtls_init(kr_dtls *dtls);
int kr_dtls_conn_init(kr_dtls *dtls, kr_dtls_conn *conn);
int kr_dtls_conn_clear(kr_dtls_conn *conn);
int kr_dtls_decrypt(kr_dtls_conn *conn, uint8_t *out, int max, uint8_t *in,
 int len);
int kr_dtls_return_len(kr_dtls_conn *conn);
int kr_dtls_set_return(kr_dtls_conn *conn, uint8_t *out, int len);
int kr_dtls_insert(kr_dtls_conn *conn, uint8_t *in, int len);
int kr_dtls_encrypt(kr_dtls_conn *conn, uint8_t *out, int max, uint8_t *in,
 int len);


#endif
