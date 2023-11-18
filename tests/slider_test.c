#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <krad/app/debug.h>
#include <krad/audio/audio.h>
#include <krad/mem/mem.h>

typedef struct {
  int pos;
  int len;
} kr_slider;

typedef struct {
  float min;
  float max;
  struct {
    float len;
    float percent;
  } linear;
} kr_range;

static float kr_slider_to_percent(kr_slider *s) {
  float val;
  if (!s) return -1.0f;
  val = (float)s->pos / s->len;
  return val;  
}

static int kr_range_valid(kr_range *r) {
  float len;
  if (!r) return 0;
  len = fabs(r->min) + fabs(r->max);
  if (r->min >= r->max) return 0;
  if (r->linear.len > len) return 0;
  if (r->linear.percent < 0) return 0;
  if (r->linear.len > 0 && r->linear.percent < 0.01f) return 0;
  if (r->linear.percent > 1.0f) return 0;
  return 1;
}

static int kr_range_desc(kr_range *r) {
  if (!r) return -1;
  if (!kr_range_valid(r)) {
    printk("Range invalid");
    return -1;
  }
  printk("Range: %f - %f", r->min, r->max);
  if (r->linear.len > 0) {
    printk("Top %f%% linear is %f, %f - %f", r->linear.percent * 100.0f,
     r->linear.len, r->max - r->linear.len, r->max);
  }
  return 0;
}

static float kr_range_value_at(kr_range *r, float percent) {
  float len;
  float res;
  float p;
  if (!r) return -INFINITY;
  if (percent < 0 || percent > 1.0f) return -INFINITY;
  if (percent < 1.0f - r->linear.percent) {
    len = fabs(r->min) + fabs(r->max);
    len -= r->linear.len;
    percent = percent/(1.0f - r->linear.percent);
    percent = 1.0f - pow(1.0f - percent, 3);
    res = percent * len;
    res += r->min;
  } else {
    p = percent - (1.0f - r->linear.percent);
    percent = p/r->linear.percent;
    res = (r->max - r->linear.len) + (percent * r->linear.len);
  }
  return res;
}

static float kr_slider_range_value(kr_slider *s, kr_range *r) {
  float pos;
  float val;
  pos = kr_slider_to_percent(s);
  val = kr_range_value_at(r, pos);
  return val;
}

static void slider_range_test(kr_slider *s, kr_range *r) {
  float val;
  float pos;
  for (s->pos = 0; s->pos <= s->len; s->pos++) {
    pos = kr_slider_to_percent(s);
    val = kr_slider_range_value(s, r);
    printk("Slider %d/%d -> %f of [%g..%g] -> %f", s->pos, s->len,
     pos, r->min, r->max, val);
  }
}

static int slider_test() {
  kr_slider slider;
  kr_range range;
  slider.pos = 0;
  slider.len = 220;
  range.min = -120.0f;
  range.max = 6.0f;
  range.linear.len = 18.0f;
  range.linear.percent = 0.8f;
  if (!kr_range_valid(&range)) {
    printk("Invalid Range! %g - %g", range.min, range.max);
    return -1;
  }
  kr_range_desc(&range);
  slider_range_test(&slider, &range);
  return 0;
}

static void print_usage() {
  printk("");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 1) {
    print_usage();
    return 1;
  }
  ret = slider_test();
  if (ret) {
    printk("Failure!");
  }
  return ret;
}
