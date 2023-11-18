#if !defined(_image_jpeg_H)
# define _image_jpeg_H (1)
int kr_image_to_jpeg(kr_coded_image *coded, kr_image *image);
int kr_image_from_jpeg(kr_image *image, kr_coded_image *coded);
#endif
