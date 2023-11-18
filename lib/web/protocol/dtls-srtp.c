#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/dh.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/engine.h>
#include <krad/app/debug.h>
#include "dtls-srtp.h"


/*
static int bio_destroy(BIO *b) {
  if (b == NULL) return 0;
  //b->ptr   = NULL;
  //b->init  = 0;
  //b->flags = 0;
  return 1;
}

static int bio_create(BIO *b) {
  //b->init = 1;
  //b->num = 0;
  //b->ptr = NULL;
  //b->flags = 0;
  return 1;
}

static int bio_read(BIO *b, char *buf, int len) {
  int r;
  int avail;
  kr_dtls_conn *conn;
  conn = (kr_dtls_conn *)b;
  //conn = (kr_dtls_conn *)b->ptr;
  //printk("DTLS: bio read called with %d bytes", len);
  //usleep(5000);
  avail = conn->in_len - conn->in_pos;
  if (avail == 0) {
    errno = EAGAIN;
    return -1;
  }
  if (len > avail) {
    r = avail;
  } else {
    r = len;
  }
  memcpy(buf, conn->in_buf + conn->in_pos, r);
  conn->in_pos += r;
  return r;
}

static int bio_write(BIO *b, const char *buf, int len) {
  int w;
  int avail;
  kr_dtls_conn *conn;
  conn = (kr_dtls_conn *)b;
  //conn = (kr_dtls_conn *)b->ptr;
  avail = conn->out_len - conn->out_pos;
  if (avail == 0) {
    errno = EAGAIN;
    return -1;
  }
  if (len > avail) {
    w = avail;
  } else {
    w = len;
  }
  memcpy(conn->out_buf + conn->out_pos, buf, w);
  conn->out_pos += w;
  return w;
}

static long bio_ctrl(BIO *b, int cmd, long num, void *ptr) {
  (void)ptr;
  kr_dtls_conn *conn;
  //conn = (kr_dtls_conn *)b->ptr;
  conn = (kr_dtls_conn *)b;
  if (conn == NULL) {
    printke("DTLS: bio ctrl");
    exit(1);
  }
  switch (cmd) {
    case BIO_CTRL_FLUSH:
      return 1;
    case BIO_CTRL_WPENDING:
      return 0;
    case BIO_CTRL_DGRAM_QUERY_MTU:
      return 1472;
    case BIO_CTRL_GET:
      return BIO_TYPE_SOURCE_SINK;
    default:
      break;
  }
  return 0;
}

*/

size_t kr_dtls_size() {
  return sizeof(kr_dtls);
}

size_t kr_dtls_conn_size() {
  return sizeof(kr_dtls_conn);
}

int kr_dtls_clear(kr_dtls *dtls) {
  if (dtls == NULL) return -1;
  SSL_CTX_free(dtls->ctx);
  memset(dtls, 0, sizeof(*dtls));
  return 0;
}

int dtls_verify(int ok, X509_STORE_CTX *ctx) {
  //SSL_CTX_get_app_data();
  printk("DTLS: verify callback");
  return 1;
}

int kr_dtls_init(kr_dtls *dtls) {
  int ret;
  if (dtls == NULL) return -1;
  memset(dtls, 0, sizeof(*dtls));
  //dtls->ctx = SSL_CTX_new(DTLSv1_method());
  if (dtls->ctx == NULL) return -2;
  //ret = SSL_CTX_set_cipher_list(dtls->ctx, "DHE-RSA-AES128-GCM-SHA:DHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256");
  ret = SSL_CTX_set_cipher_list(dtls->ctx, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
  if (ret == 0) {
    printke("DTLS: Cipher list");
    kr_dtls_clear(dtls);
    return -1;
  }
  SSL_CTX_set_verify(dtls->ctx, SSL_VERIFY_PEER |SSL_VERIFY_FAIL_IF_NO_PEER_CERT,
   dtls_verify);
  SSL_CTX_set_verify_depth(dtls->ctx, 4);
  ret = SSL_CTX_set_tlsext_use_srtp(dtls->ctx, "SRTP_AES128_CM_SHA1_80");
  //printk("srtp ret %d", ret);
  /*SSL_CTX_set_session_cache_mode(dtls->ctx, SSL_SESS_CACHE_OFF);
    SSL_CTX_set_read_ahead(dtls->ctx, 1);*/
  return 0;
}

int kr_dtls_conn_init(kr_dtls *dtls, kr_dtls_conn *conn) {
  if (dtls == NULL) return -1;
  if (dtls->ctx == NULL) return -2;
  if (conn == NULL) return -3;
  memset(conn, 0, sizeof(*conn));
  conn->ssl = SSL_new(dtls->ctx);
	if (conn->ssl == NULL) return -4;
  
  /*	
     conn->in = BIO_new(&kr_bio);
	if (conn->in == NULL) {
    SSL_free(conn->ssl);
		return -5;
	}
  conn->out = BIO_new(&kr_bio);
	if (conn->out == NULL) {
    BIO_free(conn->in);
    SSL_free(conn->ssl);
		return -6;
	}
	conn->in->ptr = conn;
	conn->out->ptr = conn;

  */

  /*
  BIO_set_nbio(conn->in, 1);
  BIO_set_nbio(conn->out, 1);
  BIO_should_retry(conn->in);
  BIO_should_retry(conn->out);
  */
	SSL_set_bio(conn->ssl, conn->in, conn->out);
  return 0;
}

int kr_dtls_conn_clear(kr_dtls_conn *conn) {
  if (conn == NULL) return -1;
  SSL_free(conn->ssl);
  memset(conn, 0, sizeof(*conn));
  return 0;
}

//static void dtls_print_state(char *name, kr_dtls_conn *conn) {
  /*printk("DTLS: State is %s", SSL_state_string(conn->ssl));*/
  //printk("DTLS: %s state is %s", name, SSL_state_string_long(conn->ssl));
//}

#define FPSZ 512

// static void dtls_print_fingerprint(char *str, X509 *cert) {
//   int i;
//   int pos;
//   int ret;
//   uint8_t fingerprint[FPSZ];
//   char fingerprint_string[FPSZ];
//   uint32_t fingerprint_len;
//   pos = 0;
//   memset(fingerprint, 0, FPSZ);
//   memset(fingerprint_string, 0, FPSZ);
//   fingerprint_len = FPSZ;
//   ret = X509_digest(cert, EVP_sha256(), fingerprint, &fingerprint_len);
//   if (ret != 1) {
//     printke("X509_digest");
//     exit(1);
//   }
//   for (i = 0; i < fingerprint_len; i++) {
//     if (i > 0) {
//       pos += snprintf(fingerprint_string + pos, FPSZ - pos, ":");
//     }
//     pos += snprintf(fingerprint_string + pos, FPSZ - pos, "%02X", fingerprint[i]);
//   }
//   /*printk("x509digest ret %d fingerprint %d:\n%s", ret, fingerprint_len, fingerprint_string);*/
//   printk("%s", str);
//   printk("a=fingerprint:sha-256 %s", fingerprint_string);
// }

static void dtls_print_error(kr_dtls_conn *conn, int ret) {
  uint32_t err;
  err = SSL_get_error(conn->ssl, ret);
  switch (err) {
    case SSL_ERROR_ZERO_RETURN:
      printke("DTLS: zero return");
      break;
    case SSL_ERROR_WANT_READ:
      printke("DTLS: openssl want read");
      break;
    case SSL_ERROR_WANT_WRITE:
      printke("DTLS: openssl want write");
      break;
    case SSL_ERROR_WANT_CONNECT:
      printke("DTLS: openssl want connect");
      break;
    case SSL_ERROR_WANT_ACCEPT:
      printke("DTLS: openssl want accept");
      break;
    case SSL_ERROR_WANT_X509_LOOKUP:
      printke("DTLS: openssl want x509 lookup");
      break;
    case SSL_ERROR_SYSCALL:
      /*printke("DTLS: openssl syscall");*/
      break;
    case SSL_ERROR_SSL:
      printke("DTLS: openssl protocol: %s", ERR_error_string(ERR_get_error(), NULL));
      break;
    default:
      printke("DTLS: unknown openssl error");
      break;
  }
}

int kr_dtls_decrypt(kr_dtls_conn *conn, uint8_t *out, int max, uint8_t *in,
 int len) {
  int ret;
  if (conn == NULL) return -1;
  if (in == NULL) return -2;
  if (len < 1) return -3;
  if (out == NULL) return -4;
  if (max < 1) return -5;
  conn->in_buf = in;
  conn->in_len = len;
  conn->in_pos = 0;
	ret = SSL_read(conn->ssl, out, max);
  /*printk("DTLS: openssl read with %d len returns %d", len, ret);*/
	if (ret < 0) {
    dtls_print_error(conn, ret);
    ret = 0;
  }
  return ret;
}

int kr_dtls_return_len(kr_dtls_conn *conn) {
  return conn->out_pos;
}

int kr_dtls_set_return(kr_dtls_conn *conn, uint8_t *out, int len) {
  if (conn == NULL) return -1;
  if (out == NULL) return -2;
  if (len < 1) return -3;
  conn->out_buf = out;
  conn->out_len = len;
  conn->out_pos = 0;
  return 0;
}

int kr_dtls_insert(kr_dtls_conn *conn, uint8_t *in, int len) {
  if (conn == NULL) return -1;
  if (in == NULL) return -2;
  if (len < 1) return -3;
  conn->in_buf = in;
  conn->in_len = len;
  conn->in_pos = 0;
  return 0;
}

int kr_dtls_encrypt(kr_dtls_conn *conn, uint8_t *out, int max, uint8_t *in,
 int len) {
  int ret;
  if (conn == NULL) return -1;
  if (in == NULL) return -2;
  if (len < 1) return -3;
  if (out == NULL) return -4;
  if (max < 1) return -5;
  conn->out_buf = out;
  conn->out_len = max;
  conn->out_pos = 0;
	ret = SSL_write(conn->ssl, in, len);
  /*printk("DTLS: openssl write with %d len returns %d", len, ret);*/
	if (ret < 0) {
    dtls_print_error(conn, ret);
    ret = 0;
  }
  return ret;
}
