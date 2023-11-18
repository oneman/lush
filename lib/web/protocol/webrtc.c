typedef struct kr_webrtc_user kr_webrtc_user;
typedef struct kr_webrtc_signal kr_webrtc_signal;

struct kr_webrtc_user {
  int active;
  char name[KR_WEBRTC_NAME_MAX];
};

enum kr_webrtc_signal_type {
  CALL = 1,
  ANSWER,
  HANGUP
};

struct kr_webrtc_signal {
  char from[KR_WEBRTC_NAME_MAX];
  char to[KR_WEBRTC_NAME_MAX];
  int32_t signal_type;
  char sdp[4096];
};

void kr_webrtc_register(kr_web_client *client, char *name);
void kr_webrtc_unregister(kr_web_client *client);
void kr_webrtc_list_users(kr_web_client *client);
void kr_webrtc_call(kr_web_client *client, char *to, char *from, char *sdp);
void kr_webrtc_answer(kr_web_client *client, char *to, char *from, char *sdp);
void kr_webrtc_candidate(kr_web_client *client, char *to, char *from,
 char *candidate);

void kr_webrtc_register(kr_web_client *client, char *name) {
  int i;
  char json[256];

  if (client->webrtc_user.active == 0) {
    /*Broadcast existing users to new user.*/
    for (i = 0; i < KR_WEB_CLIENTS_MAX; i++) {
      if (client->server->clients[i].webrtc_user.active == 1) {
        sprintf(json, "[{\"com\":\"rtc\",\"ctrl\":\"user_registered\",\"user\":\"%s\"}]",
         client->server->clients[i].webrtc_user.name);
        interweb_ws_pack(client, (uint8_t *)json, strlen(json));
      }
    }
    client->webrtc_user.active = 1;
    strncpy(client->webrtc_user.name, name, KR_WEBRTC_NAME_MAX);
    client->webrtc_user.name[KR_WEBRTC_NAME_MAX-1] = '\0';
    sprintf(json, "[{\"com\":\"rtc\",\"ctrl\":\"user_registered\",\"user\":\"%s\"}]", name);

    for (i = 0; i < KR_WEB_CLIENTS_MAX; i++) {
      if (client->server->clients[i].webrtc_user.active == 1) {
        interweb_ws_pack(&(client->server->clients[i]), (uint8_t *)json, strlen(json));
      }
    }
  }
}

void kr_webrtc_unregister(kr_web_client *client) {
  int i;
  char json[256];

  if (client->webrtc_user.active == 1) {
    for (i = 0; i < KR_WEB_CLIENTS_MAX; i++) {
      sprintf(json, "[{\"com\":\"rtc\",\"ctrl\":\"user_unregistered\",\"user\":\"%s\"}]",
       client->webrtc_user.name);
      if (client->server->clients[i].webrtc_user.active == 1) {
        interweb_ws_pack(&(client->server->clients[i]), (uint8_t *)json, strlen(json));
      }
    }
    client->webrtc_user.active = 0;
    client->webrtc_user.name[0] = '\0';
  }
}

void kr_webrtc_list_users(kr_web_client *client) {
  int i;
  char tmp[KR_WEBRTC_NAME_MAX+16];
  char json[KR_WEB_CLIENTS_MAX*(KR_WEBRTC_NAME_MAX+16)];
  int first = 1;

  for (i = 0; i < KR_WEB_CLIENTS_MAX; i++) {
    if (client->server->clients[i].webrtc_user.active == 1) {
      if (first == 1) {
        first = 0;
        sprintf(json, "[{\"com\":\"rtc\",\"names\":[\"");
      }
      sprintf(tmp, "%s\",\"", client->server->clients[i].webrtc_user.name);
      strcat(json, tmp);
    }
  }
  if (first == 0) {
    sprintf(tmp, "\"]}]");
    strcat(json, tmp);
  }
  interweb_ws_pack(client, (uint8_t *)json, strlen(json));
}

void kr_webrtc_call(kr_web_client *client, char *to, char *from, char *sdp) {
  int i;
  for (i = 0; i < KR_WEB_CLIENTS_MAX; i++) {
    if (strncmp(client->server->clients[i].webrtc_user.name, to,
     KR_WEBRTC_NAME_MAX) == 0) {
      char json[4096];
      snprintf(json, sizeof(json), "[{\"com\":\"rtc\",\"ctrl\":\"call\",\"user\":\"%s\",\"sdp\":\"%s\"}]", from, sdp);
      interweb_ws_pack(client, (uint8_t *)json, strlen(json));
      break;
    }
  }
}

void kr_webrtc_answer(kr_web_client *client, char *to, char *from, char *sdp) {
  int i;
  for (i = 0; i < KR_WEB_CLIENTS_MAX; i++) {
    if (strncmp(client->server->clients[i].webrtc_user.name, to,
     KR_WEBRTC_NAME_MAX) == 0) {
      char json[4096];
      snprintf(json, sizeof(json), "[{\"com\":\"rtc\",\"ctrl\":\"answer\","
       "\"user\":\"%s\",\"sdp\":\"%s\"}]", from, sdp);
      interweb_ws_pack(client, (uint8_t *)json, strlen(json));
      break;
    }
  }
}

void kr_webrtc_candidate(kr_web_client *client, char *to, char *from, char *candidate) {
  int i;
  for (i = 0; i < KR_WEB_CLIENTS_MAX; i++) {
    if (strncmp(client->server->clients[i].webrtc_user.name, to,
     KR_WEBRTC_NAME_MAX) == 0) {
      char json[4096];
      snprintf(json, sizeof(json), "[{\"com\":\"rtc\",\"ctrl\":\"candidate\","
       "\"user\":\"%s\",%s}]", from, candidate);
      interweb_ws_pack(client, (uint8_t *)json, strlen(json));
      break;
    }
  }
}
