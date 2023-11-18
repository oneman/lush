#if !defined(_radio_client_internal_H)
# define _radio_client_internal_H (1)

#include "client.h"

typedef int (kr_destroy_cb)(void *);

int kr_client_on_disconnect_rm(kr_radio_client *client, void *user);
int kr_client_on_disconnect(kr_radio_client *client, kr_destroy_cb *cb, void *user);
int kr_client_on_destroy_rm(kr_radio_client *client, void *user);
int kr_client_on_destroy(kr_radio_client *client, kr_destroy_cb *cb, void *user);

#endif
