#define _GNU_SOURCE
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

#define ssl_errno_s "we"
#define SSL_SESSION_ID "kr_ssl_sessionid"

typedef struct kr_domain kr_domain;

struct kr_domain {
  char *name;
  char *certfile;
  char *certkey;
  char *cafile;
  SSL_CTX *ssl_ctx;
};

static void ssl_init() {
  SSL_library_init();
  SSL_load_error_strings();
}

/*static void ssl_teardown() {
  ERR_remove_state(0);
  ENGINE_cleanup();
  CONF_modules_unload(1);
  ERR_free_strings();
  EVP_cleanup();
  sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
  CRYPTO_cleanup_all_ex_data();
}*/

void ssl_domain_setup(kr_domain *dom) {
/*  STACK_OF(X509_NAME) *certs; */
#if !defined(OPENSSL_NO_EC)
/*  EC_KEY *ecdh;*/
#endif
  printf("kr domain sslstart: (%s)\n", dom->name);
  dom->ssl_ctx = SSL_CTX_new(SSLv23_server_method());
  if (dom->ssl_ctx == NULL) {
    printf(" fatal: kr domain_sslstart(): SSL_ctx_new(): %s\n", "we");
    return;
  }
  if (!SSL_CTX_use_certificate_chain_file(dom->ssl_ctx, dom->certfile)) {
    printf("SSL_CTX_use_certificate_chain_file(%s): %s\n", dom->certfile, ssl_errno_s);
    return;
  }
  if (!SSL_CTX_use_PrivateKey_file(dom->ssl_ctx, dom->certkey, SSL_FILETYPE_PEM)) {
    printf("SSL_CTX_use_PrivateKey_file(%s): %s\n", dom->certkey, ssl_errno_s);
    return;
  }
  if (!SSL_CTX_check_private_key(dom->ssl_ctx)) {
    printf("Public/Private key for %s do not match\n", dom->name);
    return;
  }
/*
  if (ssl_dhparam != NULL) {
    SSL_CTX_set_tmp_dh(dom->ssl_ctx, ssl_dhparam);
    SSL_CTX_set_options(dom->ssl_ctx, SSL_OP_SINGLE_DH_USE);
#if !defined(OPENSSL_NO_EC)
  if ((ecdh = EC_KEY_new_by_curve_name(NID_secp384r1)) != NULL) {
    SSL_CTX_set_tmp_ecdh(dom->ssl_ctx, ecdh);
    EC_KEY_free(ecdh);
  }
#endif
  }
  if (ssl_no_compression) {
    SSL_CTX_set_options(dom->ssl_ctx, SSL_OP_NO_COMPRESSION);
  }
  if (dom->cafile != NULL) {
    if ((certs = SSL_load_client_CA_file(dom->cafile)) == NULL) {
      printf("SSL_load_client_CA_file(%s): %s\n", dom->cafile, ssl_errno_s);
      return;
    }
    SSL_CTX_load_verify_locations(dom->ssl_ctx, dom->cafile, NULL);
    SSL_CTX_set_verify_depth(dom->ssl_ctx, 1);
    SSL_CTX_set_client_CA_list(dom->ssl_ctx, certs);
    SSL_CTX_set_verify(dom->ssl_ctx, SSL_VERIFY_PEER |
    SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
  }
*/
  SSL_CTX_set_session_id_context(dom->ssl_ctx,
   (unsigned char *)SSL_SESSION_ID, strlen(SSL_SESSION_ID));
  SSL_CTX_set_mode(dom->ssl_ctx, SSL_MODE_RELEASE_BUFFERS);
  SSL_CTX_set_mode(dom->ssl_ctx, SSL_MODE_ENABLE_PARTIAL_WRITE);
  SSL_CTX_set_options(dom->ssl_ctx, SSL_OP_NO_SSLv2);
  SSL_CTX_set_options(dom->ssl_ctx, SSL_OP_CIPHER_SERVER_PREFERENCE);
/*
  SSL_CTX_set_cipher_list(dom->ssl_ctx, kr_ssl_cipher_list);
  SSL_CTX_set_tlsext_servername_callback(dom->ssl_ctx, kr_ssl_sni_cb);
  SSL_CTX_set_next_protos_advertised_cb(dom->ssl_ctx, kr_ssl_npn_cb, NULL);
*/
}

static int test() {
  kr_domain domain;
  memset(&domain, 0, sizeof(domain));
  ssl_init();
  domain.name = "kradradio.com";
  domain.certfile = "/home/oneman/kode/krad_radio/tests/cert/server.crt";
  domain.certkey = "/home/oneman/kode/krad_radio/tests/cert/server.key";
  ssl_domain_setup(&domain);
  SSL_CTX_free(domain.ssl_ctx);
  return 0;
}

int main(int argc, char *argv[]) {
  int ret;
  ret = test();
  if (ret) {
    fprintf(stderr, "Fail.\n");
  } else {
    printf("It worked!\n");
  }
  return ret;
}
