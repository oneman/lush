#if !defined(_image_coder_H)
# define _image_coder_H (1)

#include <krad/image/image.h>

int kr_image_decode(kr_image *image, kr_coded_image *coded_image);
int kr_image_encode(kr_coded_image *coded_image, kr_image *image);

int kr_image_pixels_alloc(kr_image *image);
int kr_image_pixels_free(kr_image *image);
int kr_image_pixels_clear(kr_image *image);
int kr_image_same_pixels(kr_image *img1, kr_image *img2);
int kr_coded_image_data_alloc(kr_coded_image *coded_image, size_t size);
int kr_coded_image_data_free(kr_coded_image *coded_image);
int kr_coded_image_data_clear(kr_coded_image *coded_image);
#endif
