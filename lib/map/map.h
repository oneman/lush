#if !defined(_map_map_H)
# define _map_map_H (1)
typedef struct kr_map kr_map;

void kr_map_stats(kr_map *map);
uint32_t kr_map_size(kr_map *map);
int kr_map_store(kr_map *map, const char *key,
 size_t len, void *value);
void *kr_map_fetch(kr_map *map, const char *key, size_t len);
kr_map *kr_map_create(uint32_t size);
int kr_map_destroy(kr_map *map);
#endif
