#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <openssl/err.h>
#include <openssl/dh.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/engine.h>
#include <krad/app/debug.h>

#include "tls.h"

#define ssl_errno_s ERR_error_string(ERR_get_error(), NULL)

struct kr_tls_domain {
  SSL_CTX *ctx;
};

struct kr_tls_client_group {
  SSL_CTX *ctx;
};

struct kr_tls_conn {
  SSL *tls;
};

int kr_tls_global_init() {
  static int tls_initialized = 0;
  if (!tls_initialized) {
    SSL_library_init();
    SSL_load_error_strings();
    tls_initialized = 1;
  }
  return tls_initialized;
}

/*
static void openssl_teardown() {
  ERR_remove_state(0);
  ENGINE_cleanup();
  CONF_modules_unload(1);
  ERR_free_strings();
  EVP_cleanup();
  sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
  CRYPTO_cleanup_all_ex_data();
}
*/

ssize_t kr_tls_client_write(kr_tls_conn *conn, uint8_t *buf, size_t sz) {
  return SSL_write(conn->tls, (void *)buf, (int)sz);
}

ssize_t kr_tls_client_read(kr_tls_conn *conn, uint8_t *buf, size_t sz) {
  return SSL_read(conn->tls, (void *)buf, (int)sz);
}

int kr_tls_client_io_shake(kr_tls_conn *conn) {
  int ret;
  if (SSL_is_init_finished(conn->tls)) return 1;
  ret = SSL_do_handshake(conn->tls);
  if (ret <= 0) {
    ret = SSL_get_error(conn->tls, ret);
    switch (ret) {
      case SSL_ERROR_WANT_READ:
      case SSL_ERROR_WANT_WRITE:
        return 0;
      default:
        printke("TLS: SSL_do_handshake(): %s", ssl_errno_s);
        return -1;
    }
  }
  return ret;
}

int kr_tls_client_io_shutdown(kr_tls_conn *conn) {
  if (!conn) return -1;
  SSL_shutdown(conn->tls);
  return 0;
}

int kr_tls_domain_client_io_init(kr_tls_conn *conn, int fd) {
  if (!conn) return -1;
  SSL_set_fd(conn->tls, fd);
  SSL_set_accept_state(conn->tls);
  return 0;
}

int kr_tls_client_clear(kr_tls_conn *conn) {
  if (!conn) return -1;
  SSL_free(conn->tls);
  free(conn);
  return 0;
}

/*
int kr_tls_client_group_destroy(kr_tls_client_group *cg) {
  if (!cg) return -1;
  SSL_CTX_free(cg->ctx);
  free(cg);
  return 0;
}

kr_tls_client_group *kr_tls_client_group_create(kr_tls_client_group_setup *s) {
  kr_tls_client_group cg;
  kr_tls_global_init();
  cg.ctx = SSL_CTX_new(SSLv23_client_method());
  if (!cg.ctx) {
    printke("TLS: SSL_ctx_new(): %s", ssl_errno_s);
    exit(1);
  }
  return cg;
}

int kr_tls_client_init(kr_tls_client_group *cg, kr_tls_conn **conn) {
  if (!conn) return -1;
  *conn = (void *)SSL_new(cg->ctx);
  if (!*conn) return -1;
  return 0;
}
*/

kr_tls_conn *kr_tls_domain_client_init(kr_tls_domain *dom) {
  kr_tls_conn *conn;
  if (!dom) return NULL;
  conn = kr_alloc(sizeof(*conn));
  conn->tls = SSL_new(dom->ctx);
  return conn;
}

int kr_tls_domain_destroy(kr_tls_domain *dom) {
  if (!dom) return -1;
  SSL_CTX_free(dom->ctx);
  free(dom);
  return 0;
}

kr_tls_domain *kr_tls_domain_create(kr_tls_domain_setup *setup) {
  kr_tls_domain *dom;
  kr_tls_global_init();
  dom = kr_alloc(sizeof(*dom));
  dom->ctx = SSL_CTX_new(SSLv23_server_method());
  if (!dom->ctx) {
    printke("TLS: SSL_ctx_new(): %s", ssl_errno_s);
    exit(1);
  }
  if (!SSL_CTX_use_certificate_chain_file(dom->ctx, setup->certfile)) {
    printke("TLS: SSL_CTX_use_certificate_chain_file(%s): %s",
     setup->certfile, ssl_errno_s);
    exit(1);
  }
  if (!SSL_CTX_use_PrivateKey_file(dom->ctx, setup->keyfile, SSL_FILETYPE_PEM)) {
    printke("TLS: SSL_CTX_use_PrivateKey_file(%s): %s", setup->keyfile,
     ssl_errno_s);
    exit(1);
  }
  if (!SSL_CTX_check_private_key(dom->ctx)) {
    printke("TLS: Public/Private key do not match: %s", ssl_errno_s);
    exit(1);
  }
  return dom;
}
