#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <uuid/uuid.h>
#include <krad/app/debug.h>
#include <krad/app/path.h>
#include <krad/mem/pool.h>
#include <krad/app/patch.h>
#include <krad/ebml/ebml.h>
#include <krad/util/util.h>
#include <krad/gen/metainfo.h>
#include <krad/feat/types.h>

typedef struct {
  kr_pool *pool;
} kr_features;

static int kr_feature_aspect_add(kr_feature *feat, int type,
 char *value) {
  kr_feature_aspect *aspect;
  uuid_t ui;
  int count;
  uuid_generate(ui);
  count = feat->aspect_count++;
  aspect = &feat->aspects[count];
  uuid_unparse_lower(ui, aspect->id);
  aspect->type = type;
  if (value)
    strncpy(aspect->value, value, sizeof(aspect->value));
  return 0;
}

static int kr_feature_aspect_lookup(kr_feature *feat,
 char *type, char *value) {
  int i;
  int type_val;
  type_val = str_to_kr_feature_aspect_type(type);
  if (type_val < 0) return 0;
  for (i = 0; i < feat->aspect_count; i++) {
    if ( (feat->aspects[i].type == type_val) &&
      !strncmp(feat->aspects[i].value, value,
       sizeof(feat->aspects[i].value))) {
      return 1;
    }
  }
  return 0;
}

static kr_feature *kr_feature_lookup(kr_features *features,
 char *type, char *value) {
  static int i = 0;
  kr_feature *feature;
  if (features == NULL) return NULL;
  while ((feature = kr_pool_iterate_active(features->pool, &i))) {
    if (kr_feature_aspect_lookup(feature, type, value)) {
      return feature;
    }
  }
  i = 0;
  return NULL;
}

static kr_features *setup() {
  kr_features *features;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(*features);
  pool_setup.size = sizeof(kr_feature);
  pool_setup.slices = 64;
  pool = kr_pool_create(&pool_setup);
  if (!pool) exit(1);
  features = kr_pool_overlay_get(pool);
  memset(features, 0, sizeof(*features));
  features->pool = pool;
  return features;
}

static int parse_tags(kr_feature *feat, char *line) {
  if (line[0] == '#') {
    kr_feature_aspect_add(feat, KR_FEATURE_TAG,
     line + 1);
    return 1;
  }
  return 0;
}

static int parse_headers(kr_feature *feat, char *line) {
  if (strstr(line, ".h")) {
    kr_feature_aspect_add(feat, KR_FEATURE_HEADER,
     line);
    return 1;
  }
  return 0;
}

static int parse_functions(kr_feature *feat, char *line) {
  char *openp;
  char *closep;
  openp = strchr(line, '(');
  if (!openp) return 0;
  closep = strchr(openp, ')');
  if (closep) {
    kr_feature_aspect_add(feat, KR_FEATURE_FUNCTION,
     line);
    return 1;
  }
  return 0;
}

static int parse_comments(kr_feature *feat, char *line) {
  if (isspace(line[0])) {
    kr_feature_aspect_add(feat, KR_FEATURE_COMMENTS,
     line + 1);
    return 1;
  }
  return 0;
}

static void parse_name(kr_feature *feat, char *line) {
  uuid_t ui;
  uuid_generate(ui);
  uuid_unparse_lower(ui, feat->id);
  strncpy(feat->name, line, sizeof(feat->name));
}

static int parse(kr_features *features, char *filename) {
  char *line;
  FILE *file;
  int res;
  size_t nbytes;
  ssize_t readb;
  kr_feature *feature;
  if (!filename) return -1;
  nbytes = 0;
  line = NULL;
  file = fopen(filename, "r");
  if (!file) return -1;
  printk("parsing %s...", filename);
  feature = kr_pool_slice(features->pool);
  while ( (readb = getline (&line, &nbytes, file)) > 0 ) {
    if (line[0] == '\n') {
      feature = kr_pool_slice(features->pool);
      if (feature == NULL) break;
      continue;
    }
    if (line[readb - 1] == '\n')
      line[readb - 1] = '\0';
    res = 0;
    res += parse_tags(feature, line);
    res += parse_headers(feature, line);
    res += parse_functions(feature, line);
    res += parse_comments(feature, line);
    if (res == 0)
      parse_name(feature, line);
  }
  printk("%d feature(s) parsed!",
   kr_pool_active(features->pool));
  free(line);
  fclose(file);
  return 0;
}

static int print_features(kr_features *features) {
  int i;
  int res;
  kr_feature *feature;
  char text[4096];
  if (features == NULL) return -1;
  i = 0;
  while ((feature = kr_pool_iterate_active(features->pool, &i))) {
    res = kr_feature_to_text(text, feature, sizeof(text));
    if (res >= sizeof(text)) return -1;
    printk("\nfeat[%d]: \n%s", i, text);
  }
  return i;
}

static int test_lookup(kr_features *features) {
  kr_feature *feat;
  printk("looking up feature_tag:muxing");
  while ( (feat = kr_feature_lookup(features, "feature_tag", "muxing"))) {
    printk("  found %s", feat->id);
  }
  printk("looking up feature_function:encode()");
  while ( (feat = kr_feature_lookup(features, "feature_function", "encode()"))) {
    printk("  found %s", feat->id);
  }
  printk("looking up feature_tag:audio");
  while ( (feat = kr_feature_lookup(features, "feature_tag", "audio"))) {
    printk("  found %s", feat->id);
  }
  return 0;
}

int main (int argc, char *argv[]) {
  static kr_features *features;
  features = setup();
  if (argc == 2) {
    if (parse(features, argv[1]) != 0) {
      printk("Failed parsing.");
      return 1;
    }
  }
  print_features(features);
  test_lookup(features);
  kr_pool_destroy(features->pool);
  return 0;
}
