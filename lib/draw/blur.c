#include <math.h>
#include "blur.h"

#define ARRAY_LENGTH(a) (sizeof (a) / sizeof (a)[0])

int kr_image_blur(kr_image *image) {
	int32_t width, height, stride, x, y, z, w;
	uint8_t *src;
	uint32_t *s, *d, a, p;
	int i, j, k, size, half;
	uint32_t kernel[8];
	double f;
	size = ARRAY_LENGTH(kernel);
	width = image->info.w;
	height = image->info.h;
	stride = image->ps[0];
	src = image->px[0];
	half = 4;
	a = 0;
	for (i = 0; i < size; i++) {
		f = (i - half);
		kernel[i] = exp(- f * f / ARRAY_LENGTH(kernel)) * 10000;
		a += kernel[i];
	}
	for (i = 0; i < height; i++) {
		s = (uint32_t *) (src + i * stride);
		d = (uint32_t *) (src + i * stride);
		for (j = 0; j < width; j++) {
			x = 0;
			y = 0;
			z = 0;
			w = 0;
			for (k = 0; k < size; k++) {
				p = s[j - half + k];
				x += (p >> 24) * kernel[k];
				y += ((p >> 16) & 0xff) * kernel[k];
				z += ((p >> 8) & 0xff) * kernel[k];
				w += (p & 0xff) * kernel[k];
			}
			d[j] = (x / a << 24) | (y / a << 16) | (z / a << 8) | w / a;
		}
	}
	return 0;
}
