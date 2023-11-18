#include "common.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <krad/util/strton.h>

int kr_sysname_valid(char *sysname) {
  int i = 0;
  char j;
  char requirements[512];
  sprintf(requirements, "sysname's must be atleast %d characters long, only lowercase letters and numbers, "
   "begin with a letter, and no longer than %d characters.",
   KRAD_SYSNAME_MIN, KRAD_SYSNAME_MAX);

  if ((sysname == NULL) || (strlen(sysname) < KRAD_SYSNAME_MIN)) {
    fprintf(stderr, "sysname %s is invalid, too short!\n", sysname);
    fprintf(stderr, "%s\n", requirements);
    return 0;
  }
  if (strlen(sysname) > KRAD_SYSNAME_MAX) {
    fprintf(stderr, "sysname %s is invalid, too long!\n", sysname);
    fprintf(stderr, "%s\n", requirements);
    return 0;
  }
  j = sysname[i];
  if (!((isalpha(j)) && (islower(j)))) {
    fprintf(stderr, "sysname %s is invalid, must start with a lowercase letter!\n", sysname);
    fprintf(stderr, "%s\n", requirements);
    return 0;
  }
  i++;
  while (sysname[i]) {
    j = sysname[i];
      if (!isalnum(j)) {
        fprintf(stderr, "sysname %s is invalid, alphanumeric only!\n", sysname);
        fprintf(stderr, "%s\n", requirements);
        return 0;
      }
      if (isalpha(j)) {
        if (!islower(j)) {
          fprintf(stderr, "Sysname %s is invalid lowercase letters only!\n", sysname);
          fprintf(stderr, "%s\n", requirements);
          return 0;
        }
      }
    i++;
  }
  return 1;
}

int kr_valid_host_and_port(char *string) {
  int res;
  int port;
  char host[128];
  char *str;
  if (strchr(string, ':') != NULL) {
    str = strrchr(string, ':') + 1;
    res = kr_strtoi32(&port, str, strlen(str));
    if (res != 0) return -1;
    memset(host, '\0', 128);
    memcpy(host, string, strlen(string) - strlen(strrchr(string, ':')));
    if (((port >= 0) && (port <= 65535)) && (strlen(host) > 1)) {
      return 1;
    } else {
      //printke("INVALID host %s port %d", host, port);
    }
  }
  return 0;
}
