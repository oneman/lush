#include <stdlib.h>
#include <poll.h>
#include <krad/io/buffer.h>
#include <krad/app/client.h>
#include <krad/mem/mem.h>
#include <krad/app/debug.h>
#include "client_internal.h"

#define N_ONCB 16

#define DISCONNECT_EVENT 0
#define DESTROY_EVENT 1
#define CLIENT_EVENT_TYPES 2

struct kr_radio_client {
  kr_app_client *app;
  struct {
    void *user;
    kr_destroy_cb *cb;
  } on[CLIENT_EVENT_TYPES][N_ONCB];
  char *name;
  int autosync;
  int subscriber;
  int last_delivery_was_final;
  kr_ebml2_t *ebml2;
  kr_io2_t *io;
  kr_ebml2_t *ebml_in;
  kr_io2_t *io_in;
};

static int run_on(kr_radio_client *client, int event);
static int add_on(kr_radio_client *client, int event, kr_destroy_cb *cb, void *user);
static int rm_on(kr_radio_client *client, int event, void *user);

static int run_on(kr_radio_client *client, int event) {
  int i;
  for (i = 0; i < N_ONCB; i++) {
    if (client->on[event][i].user != NULL) {
      client->on[event][i].cb(client->on[event][i].user);
      client->on[event][i].user = NULL;
    }
  }
  return 0;
}

static int add_on(kr_radio_client *client, int event, kr_destroy_cb *cb, void *user) {
  int i;
  for (i = 0; i < N_ONCB; i++) {
    if (client->on[event][i].user == NULL) {
      client->on[event][i].user = user;
      client->on[event][i].cb = cb;
      return 0;
    }
  }
  return -1;
}

static int rm_on(kr_radio_client *client, int event, void *user) {
  int i;
  for (i = 0; i < N_ONCB; i++) {
    if (client->on[event][i].user == user) {
      client->on[event][i].user = NULL;
      return 0;
    }
  }
  return -1;
}

int kr_client_on_disconnect_rm(kr_radio_client *client, void *user) {
  if ((client == NULL) || (user == NULL)) return -1;
  return rm_on(client, DISCONNECT_EVENT, user);
}

int kr_client_on_disconnect(kr_radio_client *client, kr_destroy_cb *cb, void *user) {
  if ((client == NULL) || (cb == NULL) || (user == NULL)) return -1;
  return add_on(client, DISCONNECT_EVENT, cb, user);
}

int kr_client_on_destroy_rm(kr_radio_client *client, void *user) {
  if ((client == NULL) || (user == NULL)) return -1;
  return rm_on(client, DESTROY_EVENT, user);
}

int kr_client_on_destroy(kr_radio_client *client, kr_destroy_cb *cb, void *user) {
  if ((client == NULL) || (cb == NULL) || (user == NULL)) return -1;
  return add_on(client, DESTROY_EVENT, cb, user);
}

int kr_client_sync(kr_client *client) {
  int32_t ret;
  ssize_t bytes;
  ret = kr_io2_output(client->io, &bytes);
  if (ret != 0) {
    printke("oh i am so sad I could not do it all!");
  }
  kr_ebml2_set_buffer(client->ebml2, client->io->buf, client->io->space);
  return 0;
}

int kr_poll_out(kr_client *client, uint32_t timeout_ms) {
  int ret;
  struct pollfd pollfds[1];
  pollfds[0].fd = kr_app_client_fd(client->app);
  pollfds[0].events = POLLOUT;
  ret = poll(pollfds, 1, timeout_ms);
  if (pollfds[0].revents & POLLHUP) {
    ret = -1;
  }
  return ret;
}

int kr_client_want_out (kr_client *client) {
  return kr_io2_want_out(client->io);
}

#include <sys/socket.h>
#include <sys/un.h>

int kr_crate_send(kr_client *client, kr_crate *crate) {
  int ret;
  if (!client) return -1;
  if (!crate) return -2;
  if (crate->nfds > 0 && crate->method == KR_PATCH) {
    kr_app_client_send_fds(client->app, crate->fd, crate->nfds);
    crate->nfds = 0;
  }
  ret = kr_crate_pack_ebml(client->ebml2, crate);
  if (ret < 1) {
    printke("Client: Error packing EBML crate");
    return -3;
  }
  kr_client_push(client);
  return 0;
}

int kr_get(kr_client *client, char *address) {
  int ret;
  kr_crate crate;
  if (client == NULL) return -1;
  if (address == NULL) return -2;
  memset(&crate, 0, sizeof(kr_crate));
  crate.method = KR_GET;
  strncpy(crate.address, address, sizeof(crate.address) - 1);
  /* need a no payload type or something */
  ret = kr_crate_send(client, &crate);
  return ret;
}

int kr_delete(kr_client *client, char *address) {
  int ret;
  kr_crate crate;
  if (client == NULL) return -1;
  if (address == NULL) return -2;
  memset(&crate, 0, sizeof(kr_crate));
  crate.method = KR_DELETE;
  strncpy(crate.address, address, sizeof(crate.address) - 1);
  /* need a no payload type or something */
  ret = kr_crate_send(client, &crate);
  return ret;
}

int kr_client_push(kr_client *client) {
  kr_io2_advance(client->io, client->ebml2->pos);
  if (kr_client_want_out(client)) {
    if ((client->autosync == 1) && (kr_poll_out(client, 0) > 0)) {
      kr_client_sync(client);
    }
  }
  return 0;
}

kr_client *kr_client_create(char *client_name) {
  kr_client *client;
  int len;
  if (client_name == NULL) {
    return NULL;
  }
  len = strlen(client_name);
  if ((len == 0) || (len > 255)) {
    return NULL;
  }
  client = kr_allocz(1, sizeof(kr_client));
  client->name = strdup(client_name);
  client->autosync = 1;
  client->app = kr_app_client_create();
  return client;
}

int kr_connect_remote(kr_client *client, char *host, int port) {
  char address[640];
  int len;
  if ((client == NULL) || (host == NULL) ||
      ((port < 1) || (port > 65535))) {
    return 0;
  }
  len = strlen(host);
  if ((len == 0) || (len > 512)) {
    return 0;
  }
  snprintf(address, sizeof(address), "%s:%d", host, port);
  return kr_connect(client, address);
}

int kr_check_connection(kr_client *client) {
  int ret;
  char doctype[32];
  uint32_t version;
  uint32_t read_version;
  kr_ebml_pack_header(client->ebml2, KRAD_APP_CLIENT_DOCTYPE,
   KRAD_APP_DOCTYPE_VERSION, KRAD_APP_DOCTYPE_READ_VERSION);
  kr_client_push(client);
  kr_poll(client, -1);
  kr_io2_read(client->io_in);
  kr_ebml2_set_buffer(client->ebml_in, client->io_in->rd_buf,
   client->io_in->len);
  ret = kr_ebml2_unpack_header(client->ebml_in, doctype, sizeof(doctype),
   &version, &read_version);
  if (ret > 0) {
    if ((version == KRAD_APP_DOCTYPE_VERSION)
     && (read_version == KRAD_APP_DOCTYPE_READ_VERSION)
     && (strlen(KRAD_APP_SERVER_DOCTYPE) == strlen(doctype))
     && (strncmp(doctype, KRAD_APP_SERVER_DOCTYPE,
      strlen(KRAD_APP_SERVER_DOCTYPE)) == 0)) {
        printk("Local valid checked out %d - %d", ret, client->io_in->len);
        kr_io2_pulled(client->io_in, client->io_in->len);
        kr_ebml2_set_buffer(client->ebml_in, client->io_in->rd_buf,
         client->io_in->len);
        return ret;
    } else {
      printk("RadioClient: %u %u %s\n", version, read_version, doctype);
    }
  } else {
    printk("RadioClient: %d\n", ret);
  }
  return 0;
}

int kr_connect(kr_client *client, char *address) {
  int fd;
  if (client == NULL) {
    return 0;
  }
  if (kr_connected(client)) {
    kr_disconnect(client);
  }
  kr_app_connect(client->app, address);
  fd = kr_app_client_fd(client->app);
  if (fd != -1) {
    client->io = kr_io2_create();
    client->ebml2 = kr_ebml2_create();
    kr_io2_set_fd(client->io, fd);
    kr_ebml2_set_buffer(client->ebml2, client->io->buf, client->io->space);
    client->io_in = kr_io2_create();
    client->ebml_in = kr_ebml2_create();
    kr_io2_set_fd(client->io_in, fd);
    kr_ebml2_set_buffer(client->ebml_in, client->io_in->buf,
     client->io_in->space);
    if (kr_check_connection(client) > 0) {
      return 1;
    } else {
      kr_disconnect(client);
    }
  }
  return 0;
}

int kr_connected(kr_client *client) {
  if (client == NULL) return -1;
  if (kr_app_client_fd(client->app) != -1) return 1;
  return 0;
}

int kr_disconnect(kr_client *client) {
  if (client != NULL) {
    run_on(client, DISCONNECT_EVENT);
    if (kr_connected(client)) {
      kr_app_disconnect(client->app);
      if (client->io != NULL) {
        kr_io2_destroy(&client->io);
      }
      if (client->ebml2 != NULL) {
        kr_ebml2_destroy(&client->ebml2);
      }

      if (client->io_in != NULL) {
        kr_io2_destroy(&client->io_in);
      }
      if (client->ebml_in != NULL) {
        kr_ebml2_destroy(&client->ebml_in);
      }
      return 1;
    }
    return -2;
  }
  return -1;
}

int kr_client_destroy(kr_client **client) {
  if (*client != NULL) {
    if (kr_connected(*client)) {
      kr_disconnect(*client);
    }
    run_on(*client, DESTROY_EVENT);
    kr_app_client_destroy((*client)->app);
    (*client)->app = NULL;
    if ((*client)->name != NULL) {
      free ((*client)->name);
      (*client)->name = NULL;
    }
    free(*client);
    *client = NULL;
    return 1;
  }
  return -1;
}

int kr_client_local(kr_client *client) {
  if (client != NULL) {
    if (kr_connected(client)) {
      return kr_app_local(client->app);
    }
  }
  return -1;
}

int kr_client_fd(kr_client *client) {
  if (client != NULL) {
    if (kr_connected(client)) {
      return kr_app_client_fd(client->app);
    }
  }
  return -1;
}

void kr_subscribe_all(kr_client *client) {
}

void kr_subscribe(kr_client *client, uint32_t broadcast_id) {
}

int kr_poll(kr_client *client, uint32_t timeout_ms) {
  int ret;
  struct pollfd pollfds[1];
  pollfds[0].fd = kr_app_client_fd(client->app);
  if ((kr_client_want_out (client)) && (client->autosync == 1)) {
    pollfds[0].events = POLLIN | POLLOUT;
  } else {
    pollfds[0].events = POLLIN;
  }
  ret = poll (pollfds, 1, timeout_ms);
  if (pollfds[0].revents & POLLOUT) {
    kr_client_sync (client);
  }
  if (pollfds[0].revents & POLLHUP) {
    ret = -1;
  }
  return ret;
}

void kr_delivery_recv(kr_client *client) {
  kr_io2_read(client->io_in);
}

int kr_streamer45(kr_client *client) {
  kr_ebml2_t ebml;
  kr_crate crate;
  int ret;
  if (!(kr_io2_has_in(client->io_in))) {
    return 0;
  }
  memset(&crate, 0, sizeof(kr_crate));
  kr_ebml2_set_buffer(&ebml, client->io_in->rd_buf, client->io_in->len);
  ret = kr_crate_unpack_ebml(&crate, &ebml);
  if (ret > 0) {
    char string[8192];
    ret = kr_crate_to_string(string, &crate, sizeof(string));
    if (ret > 0) {
      printk("%s\n", string);
    }
    kr_io2_pulled(client->io_in, ebml.pos);
    return 1;
  }
  return 0;
}

int kr_crate_recv(kr_crate *crate, kr_client *client) {
  kr_ebml2_t ebml;
  int ret;
  if (!crate || !client) return -1;
  if (!(kr_io2_has_in(client->io_in))) {
    return 0;
  }
  memset(crate, 0, sizeof(kr_crate));
  kr_ebml2_set_buffer(&ebml, client->io_in->rd_buf, client->io_in->len);
  ret = kr_crate_unpack_ebml(crate, &ebml);
  if (ret > 0) {
    kr_io2_pulled(client->io_in, ebml.pos);
    return 1;
  }
  return 0;
}
