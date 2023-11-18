#include <alloca.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <krad/radio/client.h>
#include <krad/io/file.h>
#include <krad/json/jsmn/jsmn.h>

void krad_radio_command_help() {
  printf("kr [STATION_NAME] [COMMAND | ADDRESS | OPTIONS]");
  printf("\n");
}

int json_array_to_crates(char *json, kr_crate *crates, int n) {
  jsmn_parser parser;
  jsmn_parser aux_parser;
  jsmntok_t tokens[512];
  jsmntok_t aux_tokens[512];
  jsmnerr_t err;
  int i;
  int pos;
  int res;
  int ntokens;
  jsmn_init(&parser);
  err = jsmn_parse(&parser,json,tokens,512);
  if (err != JSMN_SUCCESS) return 0;
  for (i = 0, pos = 1; i < n; i++) {
    json[tokens[pos].end] = '\0';
    jsmn_init(&aux_parser);
    err = jsmn_parse(&aux_parser,&json[tokens[pos].start],aux_tokens,512);
    if (err != JSMN_SUCCESS) return 0;
    ntokens = aux_parser.toknext;
    res = kr_crate_unpack_json(&crates[i],
      &json[tokens[pos].start], tokens[pos].end - tokens[pos].start);
    if (res < 0) return 0;
    pos+=ntokens;
  }
  return n;
}

int json_array_len(char *json) {
  jsmn_parser parser;
  jsmntok_t tokens[512];
  jsmnerr_t err;
  int ntokens;
  jsmn_init(&parser);
  err = jsmn_parse(&parser,json,tokens,512);
  ntokens = parser.toknext;
  if (err != JSMN_SUCCESS || ntokens < 3) return 0;
  if (tokens[0].type != JSMN_ARRAY) return 0;
  return tokens[0].size;
}

int kr_crates_send(kr_client *client, kr_crate *crates, int n) {
  int i;
  int res;
  for (i = 0; i < n; i++) {
    res = kr_crate_send(client, &crates[i]);
    printf("res[%d]: %d\n",i,res);
    if (res != 0) return 1;
  }
  return 0;
}

int kr_send_crates_from_file(kr_client *client, char *filename) {
  int fd;
  kr_crate *crates;
  char json[4096];
  int res;
  int len;
  if (client == NULL || filename == NULL) return -1;
  fd = open(filename, O_RDONLY);
  if (fd < 0) return -1;
  memset(json, 0, sizeof(json));
  res = read(fd, json, sizeof(json));
  if (res <= 0) return -1;
  len = json_array_len(json);
  if (len <= 0) return -1;
  crates = alloca(sizeof(kr_crate) * len);
  memset(crates, 0 , sizeof(kr_crate) * len);
  res = json_array_to_crates(json, crates, len);
  if (res != len) return -1;
  printf("Ok!, converted %d crates!\n",res);
  res = kr_crates_send(client, crates, len);
  if (res != 0) return -1;
  printf("Success! All %d crates correctly sent.\n",len);
  close(fd);
  return 0;
}

int kr_cmd_handle(kr_client *client, char *argv[], int argc) {
  int ret;
  ret = -1;
  if ((client == NULL) || (argc < 1)) return -1;
  if (argc == 1) {
    ret = kr_get(client, argv[0]);
    kr_poll(client, 250);
    kr_delivery_recv(client);
    while (kr_streamer45(client));
  }
  if (argc == 2) {
    if ((strlen(argv[0]) == 2) && (strncmp(argv[0], "rm", 2) == 0)) {
      ret = kr_delete(client, argv[1]);
      printf("sending delete %s\n", argv[1]);
      kr_poll(client, 250);
      kr_delivery_recv(client);
      while (kr_streamer45(client));
    } else if (!strncmp(argv[0],"crates",6)) {
      kr_send_crates_from_file(client, argv[1]);
    }
  }
  return ret;
}

int main(int argc, char *argv[]) {
  kr_client *client;
  char *sysname;
  int ret;
  sysname = NULL;
  client = NULL;
  ret = 0;
  if ((argc == 1) || (argc == 2)) {
    if (argc == 2) {
      if (((strlen(argv[1]) == 2) && (strncmp(argv[1], "vn", 2) == 0)) ||
          ((strlen(argv[1]) == 3) && (strncmp(argv[1], "-vn", 3) == 0)) ||
          ((strlen(argv[1]) == 4) && (strncmp(argv[1], "--vn", 4) == 0))) {
          printf("%d\n", KR_VERSION);
          return 0;
      }
      if (((strlen(argv[1]) == 2) && (strncmp(argv[1], "gv", 2) == 0)) ||
          ((strlen(argv[1]) == 2) && (strncmp(argv[1], "vg", 2) == 0)) ||
          ((strlen(argv[1]) == 3) && (strncmp(argv[1], "-vg", 3) == 0)) ||
          ((strlen(argv[1]) == 3) && (strncmp(argv[1], "git", 3) == 0)) ||
          ((strlen(argv[1]) == 4) && (strncmp(argv[1], "--vg", 4) == 0)) ||
          ((strlen(argv[1]) == 6) && (strncmp(argv[1], "gitver", 6) == 0)) ||
          ((strlen(argv[1]) == 10) && (strncmp(argv[1], "gitversion", 10) == 0))) {
          printf("%s\n", KR_GIT_VERSION);
          return 0;
      }
      if (((strlen(argv[1]) == 1) && (strncmp(argv[1], "v", 1) == 0)) ||
          ((strlen(argv[1]) == 2) && (strncmp(argv[1], "-v", 2) == 0)) ||
          ((strlen(argv[1]) >= 3) && (strncmp(argv[1], "--v", 3) == 0))) {
          printf(KR_VERSION_STR_FULL "\n");
          return 0;
      }
      if ((strlen(argv[1]) == 2) && (strncmp(argv[1], "ls", 2) == 0)) {
        if (printf("%s", krad_radio_running_stations())) {
          printf("\n");
        }
        return 0;
      }
    }
    krad_radio_command_help();
    return 0;
  }
  if (kr_valid_host_and_port(argv[1])) {
    sysname = argv[1];
  } else {
    if (!kr_sysname_valid(argv[1])) {
      fprintf(stderr, "Invalid station sysname!\n");
      return 1;
    } else {
      sysname = argv[1];
    }
  }
  if ((strncmp(argv[2], "launch", 6) == 0) || (strncmp(argv[2], "load", 4) == 0)) {
    krad_radio_launch(sysname);
    return 0;
  }
  if ((strncmp(argv[2], "destroy", 7) == 0) || (strncmp(argv[2], "kill", 4) == 0)) {
    ret = krad_radio_destroy(sysname);
    if (ret == 1) {
      fprintf(stderr, "Daemon was killed\n");
    }
    if (ret == -1) {
      fprintf(stderr, "Daemon was not running\n");
    }
    return 0;
  }
  client = kr_client_create("krad command line client");
  if(client == NULL) {
    fprintf(stderr, "Could create client\n");
    return 1;
  }
  if (!kr_connect(client, sysname)) {
    fprintf(stderr, "Could not connect to %s krad radio daemon\n", sysname);
    kr_client_destroy(&client);
    return 1;
  }
  kr_cmd_handle(client, argv + 2, argc - 2);
  kr_client_destroy(&client);
  return 0;
}
