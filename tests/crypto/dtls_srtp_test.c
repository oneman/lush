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
#include <krad/web/protocol/dtls-srtp.h>

#define TEST_SZ 8192

static void ssl_init() {
  SSL_library_init();
  SSL_load_error_strings();
}

static void ssl_teardown() {
  ERR_remove_state(0);
  ENGINE_cleanup();
  CONF_modules_unload(1);
  ERR_free_strings();
  EVP_cleanup();
  sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
  CRYPTO_cleanup_all_ex_data();
}

static int test_dtls() {
  int r;
  int ret;
  kr_dtls *dtls1;
  kr_dtls *dtls2;
  kr_dtls_conn *peer1;
  kr_dtls_conn *peer2;
  uint8_t *xfer_ret;
  uint8_t *xfer;
  int xfer_pos;
  char *out;
  int out_pos;
  int xfer_ret_pos;
  int encrypted;
  int checked_fingerprint;
  X509 *cert;
  xfer_ret_pos = 0;
  xfer_pos = 0;
  out_pos = 0;
  encrypted = 0;
  checked_fingerprint = 0;
  char *string1 = "Krad DTLS Works";
  ssl_init();
  dtls1 = calloc(1, kr_dtls_size());
  dtls2 = calloc(1, kr_dtls_size());
  peer1 = calloc(1, kr_dtls_conn_size());
  peer2 = calloc(1, kr_dtls_conn_size());
  xfer_ret = calloc(1, TEST_SZ);
  xfer = calloc(1, TEST_SZ);
  out = calloc(1, TEST_SZ);
  ret = kr_dtls_init(dtls1);
  if (ret != 0) return ret;
  ret = kr_dtls_init(dtls2);
  if (ret != 0) return ret;

	if (!SSL_CTX_use_certificate_file(dtls1->ctx, "client-cert.pem", SSL_FILETYPE_PEM))
		printf("\nERROR: no certificate found!");

	if (!SSL_CTX_use_PrivateKey_file(dtls1->ctx, "client-key.pem", SSL_FILETYPE_PEM))
		printf("\nERROR: no private key found!");

	if (!SSL_CTX_check_private_key (dtls1->ctx))
		printf("\nERROR: invalid private key!");

	if (!SSL_CTX_use_certificate_file(dtls2->ctx, "server-cert.pem", SSL_FILETYPE_PEM))
		printf("\nERROR: no certificate found!");

	if (!SSL_CTX_use_PrivateKey_file(dtls2->ctx, "server-key.pem", SSL_FILETYPE_PEM))
		printf("\nERROR: no private key found!");

	if (!SSL_CTX_check_private_key (dtls2->ctx))
		printf("\nERROR: invalid private key!");


  ret = kr_dtls_conn_init(dtls1, peer1);
  if (ret != 0) return ret;
  ret = kr_dtls_conn_init(dtls2, peer2);
  if (ret != 0) return ret;

  //dtls_print_state("peer1", peer1);
  //dtls_print_state("peer2", peer2);

  cert = SSL_get_certificate(peer1->ssl);
  //dtls_print_fingerprint("peer1", cert);
  cert = SSL_get_certificate(peer2->ssl);
  //dtls_print_fingerprint("peer2", cert);

  //SSL_set_verify(peer1->ssl, 0, 0);
  //SSL_set_verify(peer2->ssl, 0, 0);

  SSL_set_connect_state(peer1->ssl);
  r = kr_dtls_encrypt(peer1, (uint8_t *)xfer, TEST_SZ - xfer_pos, (uint8_t *)string1, strlen(string1) + 1);
  ret = kr_dtls_return_len(peer1);
  if (ret > 0) xfer_pos = ret;
  printk("Encrypted %d bytes, Return bytes %d", r, ret);

  //dtls_print_state("peer1", peer1);
  //dtls_print_state("peer2", peer2);

  SSL_set_accept_state(peer2->ssl);
  kr_dtls_set_return(peer2, xfer_ret, TEST_SZ - xfer_ret_pos);
  r = kr_dtls_decrypt(peer2, (uint8_t *)out, TEST_SZ - out_pos, (uint8_t *)xfer, xfer_pos);
  ret = kr_dtls_return_len(peer2);
  if (ret > 0) xfer_ret_pos = ret;
  printk("Decrypted %d bytes, Return bytes %d", r, ret);

  //dtls_print_state("peer1", peer1);
  //dtls_print_state("peer2", peer2);

 while (encrypted == 0) {
  kr_dtls_insert(peer1, xfer_ret, xfer_ret_pos);
  r = kr_dtls_encrypt(peer1, (uint8_t *)xfer, TEST_SZ - xfer_pos, (uint8_t *)string1, strlen(string1) + 1);
  if (r > 0) {
    encrypted = 1;
  }
  ret = kr_dtls_return_len(peer1);
  if (ret > 0) xfer_pos = ret;
  printk("Encrypted %d bytes, Return bytes %d", r, ret);
  //dtls_print_state("peer1", peer1);
  //dtls_print_state("peer2", peer2);

  memset(out, 0, TEST_SZ);
  //printk("%s--%d", xfer, xfer_pos);
  kr_dtls_set_return(peer2, xfer_ret, TEST_SZ - xfer_ret_pos);
  r = kr_dtls_decrypt(peer2, (uint8_t *)out, TEST_SZ - out_pos, (uint8_t *)xfer, xfer_pos);
  ret = kr_dtls_return_len(peer2);
  if (r > 0) {
    out_pos = r;
  }
  xfer_ret_pos = ret;
  printk("Decrypted %d bytes, Return bytes %d", r, ret);
 }

  if (checked_fingerprint == 0) {
    cert = SSL_get_peer_certificate(peer1->ssl);
    if (cert != NULL) {
		  printk("peer1 cipher: %s", SSL_CIPHER_get_name(SSL_get_current_cipher(peer1->ssl)));
      //dtls_print_fingerprint("peer2 from peer1", cert);
      cert = SSL_get_peer_certificate(peer2->ssl);
      if (cert != NULL) {
		    printk("peer2 cipher: %s", SSL_CIPHER_get_name(SSL_get_current_cipher(peer2->ssl)));
        //dtls_print_fingerprint("peer1 from peer2", cert);
        checked_fingerprint = 1;
      }
    }
  }

  printk("Output len is: %d", out_pos);
  printk("%s", out);

  ret = kr_dtls_conn_clear(peer1);
  if (ret != 0) return ret;
  ret = kr_dtls_conn_clear(peer2);
  if (ret != 0) return ret;
  ret = kr_dtls_clear(dtls1);
  if (ret != 0) return ret;
  ret = kr_dtls_clear(dtls2);
  if (ret != 0) return ret;
  free(peer1);
  free(peer2);
  free(dtls1);
  free(dtls2);
  free(xfer);
  free(xfer_ret);
  free(out);
  ssl_teardown();
  return 0;
}

int main(int argc, char *argv[]) {
  int ret;
  ret = test_dtls();
  if (ret) {
    fprintf(stderr, "Fail.\n");
  } else {
    printf("It worked!\n");
  }
  return ret;
}
