#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <turbojpeg.h>
#include <krad/image/image.h>
#include <krad/image/coder.h>
#include <krad/image/jpeg.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>

int kr_image_to_jpeg(kr_coded_image *coded, kr_image *image) {
  int pixel_format;
  int quality;
  int subsamp;
  int flags;
  int ret;
  tjhandle jpeg_enc;
  flags = TJFLAG_NOREALLOC;
  pixel_format = TJPF_BGRX;
  quality = 90;
  subsamp = TJSAMP_444;
  jpeg_enc = tjInitCompress();
  ret = tjCompress2(jpeg_enc, image->px[0], image->info.w, image->ps[0], image->info.h,
    pixel_format, &coded->data, &coded->info.sz, subsamp, quality, flags);
  if (ret != 0) {
    printke("JPEG: encoding error: %s", tjGetErrorStr());
  }
  tjDestroy(jpeg_enc);
  return ret;
}

int kr_image_from_jpeg(kr_image *image, kr_coded_image *coded) {
  int subsamp;
  int ret;
  int w;
  int h;
  tjhandle jpeg_dec;
  if (coded == NULL || image == NULL) return 1;
  jpeg_dec = tjInitDecompress();
  ret = tjDecompressHeader2(jpeg_dec, coded->data, coded->info.sz, &w, &h, &subsamp);
  if (ret != 0) {
    printke("JPEG: header decoding error: %s", tjGetErrorStr());
  } else {
    image->info.w = w;
    image->info.h = h;
    image->info.fmt = KR_PIXELS_ARGB;
    image->ps[0] = w * 4;
    if (image->px[0] == NULL) kr_image_pixels_alloc(image);
    ret = tjDecompress2(jpeg_dec, coded->data, coded->info.sz, image->px[0], image->info.w,
      image->ps[0], image->info.h, TJPF_BGRX, 0);
    if (ret != 0) {
      printke("JPEG: decoding error: %s", tjGetErrorStr());
    }
  }
  tjDestroy(jpeg_dec);
  return ret;
}
