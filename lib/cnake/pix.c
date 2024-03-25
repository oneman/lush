typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} rgba32;

static char *letter_color[26] = {"amethyst", "blue", "caramel", "damson",
  "ebony", "forest", "green", "honey", "iron", "jade", "kingcrab", "lavender",
  "magenta", "navy", "orange", "pink", "quagmire", "red", "sea", "turquoise",
  "ultraviolet", "violet", "wine", "xanthin", "yellow", "zinnia"
};

typedef enum { amethyst, blue, caramel, damson, ebony, forest, green, honey,
  iron, jade, kingcrab, lavender, magenta, navy, orange, pink, quagmire, red,
  sea, turquoise, ultraviolet, violet, wine, xanthin, yellow, zinnia
} ncolor;

static const rgba32 colors26[26] = {
  {170,11,254,0}, {50,131,255}, {133,102,12}, {119,40,182}, {86,86,86},
  {28,131,86}, {22,254,50}, {247,225,160}, {226,226,226}, {28,191,78},
  {197,68,28}, {222,160,253}, {252,0,249}, {50,90,156}, {255,175,22},
  {247,160,158}, {144,172,27}, {245,33,45}, {28,255,207}, {46,217,255},
  {177,12,161}, {191,115,165}, {251,27,189}, {176,0,103},{250,227,38},
  {249,0,134}
};

typedef struct {
  rgba32 *src;
  int w;
  int h;
  int pr[4096 * 26];
  int nr;
  int nrj;
} pxcop;

void pxcop_reboot(pxcop *pc) {
  memset(pc, 0, sizeof(*pc));
}

int active_regions(pxcop *pc) {
  return pc->nr - pc->nrj;
}

int pixel_region(pxcop *pc, int x, int y, int w) {
  int n = (y * w) + x;
  int r = pc->pr[n];
  //printf("n=%d\n", n);
  //printf("get region: %d,%d: %d\n", x, y, r);
  return r;
}

int pixel_region_new(pxcop *pc, int x, int y, int w) {
  int n = (y * w) + x;
  int r = ++pc->nr;
  pc->pr[n] = r;
  //printf("n=%d\n", n);
  //printf("new region: %d,%d: %d\n", x, y, r);
  return r;
}

int pixel_region_join(pxcop *pc, int r, int x, int y, int w) {
  int n = (y * w) + x;
  int old_r = pc->pr[n];
  pc->pr[n] = r;
  //printf("n=%d\n", n);
  //printf(" join region: %d,%d: %d -> %d\n", x, y, old_r, r);
  if ((old_r) && (old_r != r)) {
    pc->nrj++;
    for (int i = 0; i < n; i++) {
      if (pc->pr[i] == old_r) {
        pc->pr[i] = r;
      //  printf(" %d moved to: %d\n", i, r);
      }
    }
  }
  return r;
}

int rgbcmp(rgba32 *p1, rgba32 *p2) {
  if ((p1->r == p2->r) &&
      (p1->g == p2->g) &&
      (p1->b == p2->b)) {
    return 0;
  }
  return 1;
}

int samergb(rgba32 *p1, rgba32 *p2) {
  if (rgbcmp(p1, p2) == 0) return 1;
  return 0;
}

int argbcmp(rgba32 *a, rgba32 *b) {
  if ((a->a == b->a) &&
      (a->r == b->r) &&
      (a->g == b->g) &&
      (a->b == b->b)) {
    return 0;
  }
  return 1;
}

int pixcmp(rgba32 *a, rgba32 *b) {
  if ((a->a == b->a) &&
      (a->r == b->r) &&
      (a->g == b->g) &&
      (a->b == b->b)) {
    return 0;
  }
  return 1;
}

void print_pixel_regions(pxcop *pc, uint32_t *px, int w, int h) {
  int x;
  int y;
  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      printf("%4d ", pixel_region(pc, x, y, w));
    }
    printf("\n");
  }
  printf("\n");
}

void px_scan(pxcop *pc, uint32_t *px, int w, int h) {
  printf("pixel scan: %d x %d - %d bytes per pixel\n", w, h, 4);
  int x;
  int y;
  int r;
  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      rgba32 *upleft = NULL;
      rgba32 *up = NULL;
      rgba32 *left = NULL;
      rgba32 *upright = NULL;
      rgba32 *cur = &px[(y * w) + x];
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
          r = pixel_region(pc, x - 1, y - 1, w);
          pixel_region_join(pc, r, x, y, w);
        }
      }
      if (up) {
        if (samergb(cur, up)) {
          r = pixel_region(pc, x, y - 1, w);
          pixel_region_join(pc, r, x, y, w);
        }
      }
      if (left) {
        if (samergb(cur, left)) {
          r = pixel_region(pc, x - 1, y, w);
          pixel_region_join(pc, r, x, y, w);
        }
      }
      if (upright) {
        if (samergb(cur, upright)) {
          r = pixel_region(pc, x + 1, y - 1, w);
          pixel_region_join(pc, r, x, y, w);
        }
      }
      if (!pixel_region(pc, x, y, w)) {
        pixel_region_new(pc, x, y, w);
      }
    }
    printf("\n");
  }
  printf("\n");
  printf("%d active regions %d created, %d joined\n",
          active_regions(pc), pc->nr, pc->nrj);
  print_pixel_regions(pc, px, w, h);
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
  pxcop *pc = malloc(sizeof(*pc));
  pxcop_reboot(pc);
  px_scan(pc, argb_px, w, h);
  free(pc);
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}

void cpixel() {
  pixel_file_scan("/home/demo/src/lush/doc/png/26mhz_osc_xtal.png");
  exit(0);
}
