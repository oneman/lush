static int pr[4096 * 8];
static int nr = 0;
static int nrj = 0;

int active_regions() {
  return nr - nrj;
}

int pixel_region(int x, int y, int w) {
  int n = (y * w) + x;
  int r = pr[n];
  //printf("n=%d\n", n);
  //printf("get region: %d,%d: %d\n", x, y, r);
  return r;
}

int pixel_region_new(int x, int y, int w) {
  int n = (y * w) + x;
  int r = ++nr;
  pr[n] = r;
  //printf("n=%d\n", n);
  //printf("new region: %d,%d: %d\n", x, y, r);
  return r;
}

int pixel_region_join(int r, int x, int y, int w) {
  int n = (y * w) + x;
  int old_r = pr[n];
  pr[n] = r;
  //printf("n=%d\n", n);
  //printf(" join region: %d,%d: %d -> %d\n", x, y, old_r, r);
  if ((old_r) && (old_r != r)) {
    nrj++;
    for (int i = 0; i < n; i++) {
      if (pr[i] == old_r) {
        pr[i] = r;
      //  printf(" %d moved to: %d\n", i, r);
      }
    }
  }
  return r;
}

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} pixel_t;

typedef struct {
  int w;
  int h;
} area_t;

int rgbcmp(pixel_t *p1, pixel_t *p2) {
  if ((p1->r == p2->r) &&
      (p1->g == p2->g) &&
      (p1->b == p2->b)) {
    return 0;
  }
  return 1;
}

int samergb(pixel_t *p1, pixel_t *p2) {
  if (rgbcmp(p1, p2) == 0) return 1;
  return 0;
}

int argbcmp(pixel_t *a, pixel_t *b) {
  if ((a->a == b->a) &&
      (a->r == b->r) &&
      (a->g == b->g) &&
      (a->b == b->b)) {
    return 0;
  }
  return 1;
}

int pixcmp(pixel_t *a, pixel_t *b) {
  if ((a->a == b->a) &&
      (a->r == b->r) &&
      (a->g == b->g) &&
      (a->b == b->b)) {
    return 0;
  }
  return 1;
}

void print_pixel_regions(uint32_t *px, int w, int h) {
  int x;
  int y;
  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      printf("%4d ", pixel_region(x, y, w));
    }
    printf("\n");
  }
  printf("\n");
}

void pixel_scan(uint32_t *px, int w, int h) {
  printf("pixel scan: %d x %d - %d bytes per pixel\n", w, h, 4);
  int x;
  int y;
  int r;
  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      pixel_t *upleft = NULL;
      pixel_t *up = NULL;
      pixel_t *left = NULL;
      pixel_t *upright = NULL;
      pixel_t *cur = &px[(y * w) + x];
      if (x == 0) printf("%d", y);
      printf(" #%02X%02X%02X", cur->r, cur->g, cur->b);
      if (y > 0) {
        up = &px[((y - 1) * w) + x];
        if (x > 0) upleft = &px[((y - 1) * w) + (x - 1)];
        if (x < (w - 1)) upright = &px[((y - 1) * w) + (x + 1)];
      }
      if (x > 0) left = &px[(y * w) + (x - 1)];
      if (upleft) {
        if (samergb(cur, upleft)) {
          r = pixel_region(x - 1, y - 1, w);
          pixel_region_join(r, x, y, w);
        }
      }
      if (up) {
        if (samergb(cur, up)) {
          r = pixel_region(x, y - 1, w);
          pixel_region_join(r, x, y, w);
        }
      }
      if (left) {
        if (samergb(cur, left)) {
          r = pixel_region(x - 1, y, w);
          pixel_region_join(r, x, y, w);
        }
      }
      if (upright) {
        if (samergb(cur, upright)) {
          r = pixel_region(x + 1, y - 1, w);
          pixel_region_join(r, x, y, w);
        }
      }
      if (!pixel_region(x, y, w)) {
        pixel_region_new(x, y, w);
      }
    }
    printf("\n");
  }
  printf("\n");
  printf("%d active regions %d created, %d joined\n",
          active_regions(), nr, nrj);
  print_pixel_regions(px, w, h);
}

void pixel_file_scan(char *filename) {
  cairo_surface_t *surface;
  cairo_t *cr;
  int w;
  int h;
  uint32_t *argb_px;
  printf("pixel file scan:\n%s\n\n", filename);
  surface = cairo_image_surface_create_from_png(filename);
  w = cairo_image_surface_get_width(surface);
  h = cairo_image_surface_get_height(surface);
  cr = cairo_create(surface);
  argb_px = cairo_image_surface_get_data(surface);
  pixel_scan(argb_px, w, h);
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}
