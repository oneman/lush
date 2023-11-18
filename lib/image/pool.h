#if !defined(_image_pool_H)
# define _image_pool_H (1)

#include <krad/mem/pool.h>
#include <krad/image/image.h>

typedef struct kr_pool kr_image_pool;

kr_image_pool *kr_image_pool_create(kr_image *image, size_t len);
int kr_image_pool_getimage(kr_image_pool *image_pool, kr_image *image);
int kr_image_pool_destroy(kr_image_pool *image_pool);

#endif
