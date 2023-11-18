#ifndef KRAD_TAG_H
#define KRAD_TAG_H

#define KRAD_MAX_TAGS 20

typedef struct kr_tag kr_tag;
typedef struct kr_tags kr_tags;

struct kr_tag {
  char *name;
  char *value;
};

struct kr_tags {
  kr_tag *tags;
  char *item;
  pthread_rwlock_t tags_rwlock;
  void *callback_pointer;
  void (*set_tag_callback)( void *, char *, char *, char *, int);
};

void kr_tags_destroy(kr_tags *tags);
kr_tags *kr_tags_create();

void kr_tags_set_set_tag_callback(kr_tags *tags, void *pointer,
 void (*set_tag_callback)( void *, char *, char *, char *, int));

char *kr_tags_get_tag(kr_tags *tags, char *name);
void kr_tags_set_tag(kr_tags *tags, char *name, char *value);

void kr_tags_set_tag_internal(kr_tags *tags, char *name, char *value);
void kr_tags_set_tag_opt(kr_tags *tags, char *name, char *value, int internal);
int kr_tags_get_next_tag(kr_tags *tags, int *tagnum, char **name, char **value);

#endif
