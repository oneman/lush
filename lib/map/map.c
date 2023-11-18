#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <krad/app/debug.h>
#include <krad/mem/pool.h>
#include <krad/map/map.h>
#include <krad/hash/murmur3.h>

#define MAX_BUCKET_ITEMS 12
#define MAP_64B 64

typedef struct {
  const char *key;
  void *val;
} kv_item;

typedef struct {
  kv_item items[MAX_BUCKET_ITEMS];
} bucket;

struct kr_map {
  kr_pool *buckets;
  kr_pool *nitems;
  uint32_t sz;
  uint32_t seed;
};

static uint32_t next_pow2(uint32_t v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}

static uint32_t map_hash_index(kr_map *map,
 const char *key, size_t len) {
  uint32_t hash;
  uint32_t index;
  hash = murmur3_32(key, len, map->seed);
  index = hash & (map->sz - 1);
  return index;
}

static void *bucket_fetch_val(bucket *buck, uint8_t n,
 const char *key, size_t len) {
  int i;
  kv_item *item;
  for (i = 0; i < n; i++) {
    item = buck->items + i;
    if (!memcmp(key, item->key, len)) {
      return item->val;
    }
  }
  return NULL;
}

uint32_t kr_map_size(kr_map *map) {
  return map->sz;
}

void kr_map_stats(kr_map *map) {
  uint8_t *nitems;
  int i;
  int j;
  uint32_t collisions;
  uint32_t max_collisions;
  uint32_t keys;
  i = 0;
  collisions = 0;
  max_collisions = 0;
  keys = 0;
  while ( (nitems = kr_pool_iterate_active(map->nitems, &i)) ) {
    for (j = 0; j < MAP_64B; ++j) {
      keys += *nitems;
      if (*nitems > 1) {
        collisions += (*nitems - 1);
        if (*nitems > max_collisions)
          max_collisions = *nitems - 1;
      }
      ++nitems;
    }
  }
  //kr_pool_debug(map->buckets);
  printk("map stats:");
  printk("  buckets: %ju", map->sz);
  printk("  keys/values: %u", keys);
  printk("  load factor: %0.2f", (float)keys/(float)map->sz);
  printk("  collisions: %u", collisions);
  printk("  max collisions per bucket: %u", max_collisions);
  printk("  max allowed collisions per bucket: %u", MAX_BUCKET_ITEMS);
  printk("  bucket size: %juB", sizeof(bucket));
  printk("  item size: %juB", sizeof(kv_item));
  printk("  total allocated memory: %dMB",
   (kr_pool_size(map->buckets) + kr_pool_size(map->nitems)) / (1024 * 1024) );
}

int kr_map_store(kr_map *map, const char *key,
 size_t len, void *value) {
  bucket *buck;
  kv_item *item;
  uint8_t *nitems;
  uint32_t index;
  if (!map) return -1;
  if (!key || !value) return -1;
  if (len == 0) return -1;
  index = map_hash_index(map, key, len);
  buck = kr_pool_slice_num(map->buckets, index);
  if (buck == NULL) return -1;
  nitems = kr_pool_slice_num(map->nitems, index / MAP_64B);
  if (nitems == NULL) return -1;
  nitems = nitems + index % MAP_64B;
  if (*nitems == MAX_BUCKET_ITEMS) {
    printke("too many collisions!!!");
    return -1;
  }
  if (*nitems == 0) {
    item = buck->items;
  } else {
    /* Collision */
    /* check for duplicates */
    if (bucket_fetch_val(buck, *nitems, key, len)) {
      /* key already here */
      return 0;
    }
    item = buck->items + *nitems;
  }
  item->key = key;
  item->val = value;
  ++(*nitems);
  return 0;
}

void *kr_map_fetch(kr_map *map, const char *key, size_t len) {
  bucket *buck;
  kv_item *item;
  uint32_t index;
  uint8_t *nitems;
  index = map_hash_index(map, key, len);
  buck = kr_pool_slice_num(map->buckets, index);
  if (buck == NULL) return NULL;
  nitems = kr_pool_slice_num(map->nitems, index / MAP_64B);
  if (nitems == NULL) return NULL;
  nitems = nitems + index % MAP_64B;
  if (*nitems == 1) {
    item = buck->items;
    return item->val;
  } else {
    return bucket_fetch_val(buck, *nitems, key, len);
  }
  return NULL;
}

int kr_map_destroy(kr_map *map) {
  if (!map) return -1;
  kr_pool_destroy(map->nitems);
  kr_pool_destroy(map->buckets);
  return 0;
}

kr_map *kr_map_create(uint32_t size) {
  kr_map map;
  kr_pool_setup psetup;
  struct timeval tv;
  if (size == 0) return NULL;
  memset(&map, 0, sizeof(map));
  map.sz = next_pow2(size);
  if (map.sz < MAP_64B) map.sz = MAP_64B;
  memset(&psetup, 0, sizeof(psetup));
  psetup.slices = map.sz / MAP_64B;
  psetup.size = MAP_64B;
  map.nitems = kr_pool_create(&psetup);
  if (map.nitems == NULL) return NULL;
  memset(&psetup, 0, sizeof(psetup));
  psetup.slices = map.sz;
  psetup.size = sizeof(bucket);
  psetup.overlay_sz = sizeof(map);
  map.buckets = kr_pool_create(&psetup);
  if (map.buckets == NULL) return NULL;
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);
  map.seed = rand();
  kr_pool_overlay_set(map.buckets, &map);
  return kr_pool_overlay_get(map.buckets);
}
