#if !defined(_web_protocol_tls_H)
# define _web_protocol_tls_H (1)

#include <inttypes.h>
#include <krad/app/debug.h>

typedef struct kr_tls_client_group kr_tls_client_group;
typedef struct kr_tls_domain kr_tls_domain;
typedef struct kr_tls_conn kr_tls_conn;

typedef enum {
  KR_TLS_FALSE,
  KR_TLS_OPENSSL
} kr_tls_lib;

typedef struct {
  kr_tls_lib lib;
} kr_tls_client_group_setup;

typedef struct {
  char *certfile;
  char *keyfile;
} kr_tls_domain_setup;

int kr_tls_global_init();

/*  *_io_* functions using openssl easy w/e   */
int kr_tls_client_clear(kr_tls_conn *conn);
int kr_tls_client_io_shutdown(kr_tls_conn *conn);
ssize_t kr_tls_client_write(kr_tls_conn *conn, uint8_t *buf, size_t sz);
ssize_t kr_tls_client_read(kr_tls_conn *conn, uint8_t *buf, size_t sz);
int kr_tls_client_io_shake(kr_tls_conn *conn);

/*
int kr_tls_client_init(kr_tls_client_group *cg, kr_tls_conn **conn);
int kr_tls_client_group_destroy(kr_tls_client_group *cg);
kr_tls_client_group *kr_tls_client_group_create(kr_tls_client_group_setup *s);
*/

int kr_tls_domain_client_io_init(kr_tls_conn *conn, int fd);
kr_tls_conn *kr_tls_domain_client_init(kr_tls_domain *dom);
int kr_tls_domain_destroy(kr_tls_domain *dom);
kr_tls_domain *kr_tls_domain_create(kr_tls_domain_setup *setup);

#endif
