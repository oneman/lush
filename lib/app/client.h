#if !defined(_app_client_H)
# define _app_client_H (1)

#include <krad/radio/version.h>
#include "common.h"
#include "types.h"

#define KRAD_APP_CLIENT_DOCTYPE "kr_app_client"
#define KRAD_APP_SERVER_DOCTYPE "kr_app_server"
#define KRAD_APP_DOCTYPE_VERSION KR_VERSION
#define KRAD_APP_DOCTYPE_READ_VERSION KR_VERSION
#define EBML_ID_KRAD_APP_CMD 0x4444

typedef struct kr_app_client kr_app_client;

int kr_app_client_send_fds(kr_app_client *client, const int *fds, int nfds);
int kr_app_client_send_fd(kr_app_client *client, int fd);
int kr_app_client_fd(kr_app_client *client);
int kr_app_local(kr_app_client *client);
int kr_app_disconnect(kr_app_client *client);
int kr_app_connect(kr_app_client *client, char *address);
int kr_app_client_destroy(kr_app_client *client);
kr_app_client *kr_app_client_create();

#endif
