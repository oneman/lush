#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include "tags.h"

void kr_tags_destroy(kr_tags *tags) {
  int t;
  pthread_rwlock_wrlock(&tags->tags_rwlock);
  for (t = 0; t < KRAD_MAX_TAGS; t++) {
    if (tags->tags[t].name != NULL) {
      free(tags->tags[t].name);
      tags->tags[t].name = NULL;
      free(tags->tags[t].value);
      tags->tags[t].value = NULL;
    }
  }
  free(tags->item);
  pthread_rwlock_unlock(&tags->tags_rwlock);
  pthread_rwlock_destroy(&tags->tags_rwlock);
  free(tags->tags);
  free(tags);
}

kr_tags *kr_tags_create(char *item) {
  kr_tags *tags = kr_allocz(1, sizeof(kr_tags));
  tags->tags = kr_allocz(KRAD_MAX_TAGS, sizeof(kr_tag));
  if (tags->tags == NULL) {
    failfast ("Krad Tags: Out of memory");
  }
  tags->item = strdup(item);
  if (tags->item == NULL) {
    failfast ("Krad Tags: Out of memory");
  }
  pthread_rwlock_init(&tags->tags_rwlock, NULL);
  return tags;
}

void kr_tags_set_set_tag_callback(kr_tags *tags, void *callback_pointer,
 void (*set_tag_callback)( void *, char *, char *, char *, int)) {
  tags->callback_pointer = callback_pointer;
  tags->set_tag_callback = set_tag_callback;
}

char *kr_tags_get_tag(kr_tags *tags, char *name) {
  int t;
  for (t = 0; t < KRAD_MAX_TAGS; t++) {
    if (tags->tags[t].name != NULL) {
      if (strcmp(tags->tags[t].name, name) == 0) {
        return tags->tags[t].value;
      }
    }
  }
  return "";
}

void kr_tags_set_tag(kr_tags *tags, char *name, char *value) {
  kr_tags_set_tag_opt(tags, name, value, 0);
}

void kr_tags_set_tag_internal(kr_tags *tags, char *name, char *value) {
  kr_tags_set_tag_opt(tags, name, value, 1);
}

void kr_tags_set_tag_opt(kr_tags *tags, char *name, char *value, int internal) {
  //printk ("set tag %s %s", name, value);
  int t;
  if ((name == NULL) || (strlen(name) == 0)) {
    return;
  }
  pthread_rwlock_wrlock(&tags->tags_rwlock);
  for (t = 0; t < KRAD_MAX_TAGS; t++) {
    if (tags->tags[t].name != NULL) {
      if (strcmp(tags->tags[t].name, name) == 0) {
        free(tags->tags[t].value);
        tags->tags[t].value = NULL;
        if ((value != NULL) && (strlen(value))) {
          tags->tags[t].value = strdup(value);
          //UPDATED tag
          if (tags->set_tag_callback) {
            tags->set_tag_callback(tags->callback_pointer, tags->item,
             tags->tags[t].name, tags->tags[t].value, internal);
          }
        } else {
          //CLEARED tag
          if (tags->set_tag_callback) {
            tags->set_tag_callback(tags->callback_pointer, tags->item,
             tags->tags[t].name, "", internal);
          }
          free(tags->tags[t].name);
          tags->tags[t].name = NULL;
        }
        pthread_rwlock_unlock(&tags->tags_rwlock);
        return;
      }
    }
  }
  if ((value == NULL) || (strlen(value) == 0)) {
    //wanted to create a tag with name but no value
    pthread_rwlock_unlock(&tags->tags_rwlock);
    return;
  }
  for (t = 0; t < KRAD_MAX_TAGS; t++) {
    if (tags->tags[t].name == NULL) {
      // NEW tag
      tags->tags[t].name = strdup(name);
      tags->tags[t].value = strdup(value);
      if (tags->set_tag_callback) {
        tags->set_tag_callback(tags->callback_pointer, tags->item,
         tags->tags[t].name, tags->tags[t].value, internal);
      }
      pthread_rwlock_unlock(&tags->tags_rwlock);
      return;
    }
  }
}

int kr_tags_get_next_tag(kr_tags *tags, int *tagnum, char **name, char **value) {
  int t;
  for (t = *tagnum; t < KRAD_MAX_TAGS; t++) {
    if (tags->tags[t].name != NULL) {
      *name = tags->tags[t].name;
      *value = tags->tags[t].value;
      *tagnum = ++t;
      return 1;
    }
  }
  return 0;
}
