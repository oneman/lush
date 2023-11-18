#include <sys/types.h>
#include <sys/stat.h>
#include <sys/cdefs.h>
#include <sys/time.h>
#include <string.h>
#include <inttypes.h>

#if !defined(_hash_base64_H)
# define _hash_base64_H (1)

size_t kr_base64_encode(char *dest, char *src, int maxlen);
size_t kr_base64(uint8_t *dest, uint8_t *src, int len, int maxlen);

#endif
