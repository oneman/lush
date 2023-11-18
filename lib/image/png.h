#if !defined(_image_png_H)
# define _image_png_H (1)
int kr_image_to_png(kr_coded_image *coded, kr_image *image);
int kr_image_from_png(kr_image *image, kr_coded_image *coded);
#endif
