#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <uuid/uuid.h>
#include <krad/app/debug.h>
#include <krad/time/timer.h>
#include <krad/mem/pool.h>
#include <krad/map/map.h>

char **gen_values(int n) {
  int i;
  int j;
  char **values;
  double scale;
  srand(time(NULL));
  values = calloc(n, sizeof(char *));
  scale = (double)(122-97) / RAND_MAX;
  for (i = 0; i < n; i++) {
    values[i] = malloc(8);
    for (j = 0; j < 7; j++) {
      values[i][j] = 97 + floor(rand() * scale);
    }
    values[i][7] = '\0';
  }
  return values;
}

char **gen_keys(int n) {
  int i;
  uuid_t id;
  char **keys;
  keys = calloc(n, sizeof(char *));
  for (i = 0; i < n; i++) {
    uuid_generate(id);
    keys[i] = malloc(40);
    uuid_unparse(id, keys[i]);
  }
  return keys;
}

int free_strings(char **strings, int n) {
  int i;
  for (i = 0; i < n; i++) {
    free(strings[i]);
  }
  free(strings);
  return 0;
}

void print_key_value(char **keys, char **values, int n) {
  int i;
  for (i = 0; i < n; i++) {
    printk("  %s --> %s", keys[i], values[i]);
  }
}

void random_key_indexes_gen(uint *indexes, int n) {
  int i;
  for (i = 0; i < n; i++) {
    indexes[i] = rand() % n;
  }
}

int run_array_test(char **keys, char **values,
 uint *indexes, int nvalues) {
  int i;
  int j;
  int n;
  char *key;
  kr_timer *timer;
  timer = kr_timer_create();
  kr_timer_start(timer);
  n = 0;
  for (i = 0; i < nvalues; i++) {
    key = keys[indexes[i]];
    for (j = 0; j < nvalues; j++) {
      if (!memcmp(keys[j], key, 36)) {
        // found key/val
        ++n;
        break;
      }
    }
  }
  kr_timer_finish(timer);
  printk("trying to fetch %d random key/value pairs.", nvalues);
  printk("  %d values fetched in %"PRIu64" ms.", n,
   kr_timer_duration_ms(timer));
  if (kr_timer_duration_ms(timer) == 0) {
    printk("  rate of %d++ keys/ms", n);
  } else {
    printk("  rate of %0.2f keys/ms",
     (float)n / (float)kr_timer_duration_ms(timer));
  }
  kr_timer_destroy(timer);
  return 0;
}

int run_map_test(char **values, char **keys,
 uint *indexes, int nvalues, size_t sz) {
  kr_map *map;
  kr_timer *timer;
  char *val;
  int i;
  int res;
  timer = kr_timer_create();
  kr_timer_start(timer);
  map = kr_map_create(sz);
  if (!map) return -1;
  kr_timer_finish(timer);
  printk("map of %d created in %"PRIu64" ms.", kr_map_size(map),
   kr_timer_duration_ms(timer));
  kr_map_stats(map);
  printk("trying to store %d key/value pairs.", nvalues);
  kr_timer_start(timer);
  for (i = 0; i < nvalues; i++) {
    res = kr_map_store(map, keys[i], 36, values[i]);
    if (res != 0) return -1;
  }
  kr_timer_finish(timer);
  printk("  keys/values stored in %"PRIu64" ms.",
   kr_timer_duration_ms(timer));
  printk("trying to fetch %d random key/value pairs.", nvalues);
  kr_timer_start(timer);
  for (i = 0; i < nvalues; i++) {
    val = kr_map_fetch(map, keys[indexes[i]], 36);
    if (!val) return -1;
  }
  kr_timer_finish(timer);
  printk("  %d values fetched in %"PRIu64" ms.", nvalues,
   kr_timer_duration_ms(timer));
  if (kr_timer_duration_ms(timer) == 0) {
    printk("  rate of %d++ keys/ms", nvalues);
  } else {
    printk("  rate of %0.2f keys/ms",
     (float)nvalues / (float)kr_timer_duration_ms(timer));
  }
  kr_map_stats(map);
  kr_map_destroy(map);
  printk("map destroyed.");
  kr_timer_destroy(timer);
  return 0;
}

int map_test() {
  kr_timer *timer;
  int res;
  size_t sz;
  sz = 1000000;
  char **values;
  char **keys;
  uint *indexes;
  int nvalues;
  nvalues = 800000;
  timer = kr_timer_create();
  indexes = malloc(nvalues * sizeof(uint));
  printk("generating random key/value pairs..");
  kr_timer_start(timer);
  values = gen_values(nvalues);
  keys = gen_keys(nvalues);
  if (!values || !keys) return -1;
  kr_timer_finish(timer);
  printk("  %d random key/value pairs generated in %"PRIu64" ms.",
   nvalues, kr_timer_duration_ms(timer));
  kr_timer_start(timer);
  random_key_indexes_gen(indexes, nvalues);
  kr_timer_finish(timer);
  printk("  %d random key indexes generated in %"PRIu64" ms.",
   nvalues, kr_timer_duration_ms(timer));
  res = run_map_test(values, keys, indexes, nvalues, sz);
  if (res != 0) return -1;
  //run_array_test(keys, values, indexes, nvalues);
  kr_timer_destroy(timer);
  free_strings(values, nvalues);
  free_strings(keys, nvalues);
  free(indexes);
  return 0;
}

int main(int argc, char const *argv[]) {
  int ret;
  if (argc != 1) return -1;
  ret = map_test();
  if (ret != 0) {
    fprintf(stderr, "Failure.");
    return -1;
  }
  return 0;
}
