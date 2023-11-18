#include <krad/image/image.h>
#include <krad/image/coder.h>

int kr_image_to_png_filename(char *filename, kr_image *image);
int kr_image_from_png_filename(kr_image *image, char *filename);
int kr_image_to_jpeg_filename(char *filename, kr_image *image);
int kr_image_from_jpeg_filename(kr_image *image, char *filename);
int kr_coded_image_from_file(kr_coded_image *cimage, int fd, char *path);
int kr_coded_image_to_file(int fd, kr_coded_image *coded_image);
int kr_coded_image_load_file(kr_coded_image *cimage, char *filename);
int kr_coded_image_save_file(char *filename, kr_coded_image *cimage);
int kr_image_load_file(kr_image *img, char *filename);
int kr_image_save_file(char *filename, kr_image *img);
