#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <krad/app/debug.h>

typedef struct {
  double a0;
  double b1;
  double z1;
} kr_onepole;

static void onepole_set_fc(kr_onepole *p, double fc) {
  p->b1 = exp(-2.0 * M_PI * fc);
  p->a0 = 1.0 - p->b1;
}

size_t kr_onepole_sizeof() {
  return sizeof(kr_onepole);
}

float kr_onepole_process(kr_onepole *p, float in) {
  float s;
  s = in * p->a0 + p->z1 * p->b1;
  p->z1 = s;
  return s;
}

int kr_onepole_init(kr_onepole *p, float rate) {
  if (!p || rate < 0.0f) return 1;
  p->z1 = 0;
  onepole_set_fc(p, rate);
  return 0;
}

static void test_rate(kr_onepole *p, float rate) {
  int i;
  float s;
  float res;
  float fc;
  float percent;
  fc = (1000.0 / 48000.0) / rate;
  kr_onepole_init(p, fc);
  printf("onepole setup at %f -> %f\n", rate, fc);
  for (s = 0.10000; s < 0.30000; s += 0.1) {
    i = 1;
    res = 0;
    percent = 0;
    while (percent < 99.5f) {
      res = kr_onepole_process(p, s);
      percent = ((double)res/s) * 100;
      i++;
      //if (i > 300) break;
    }
    printf("%g %6d: %f -> %f [%g%%]\n", rate, i, s, res, percent);
  }
}

static int onepole_test() {
  kr_onepole *p;
  p = malloc(kr_onepole_sizeof());
  test_rate(p, 480);
  //test_rate(p, 48000);
  //test_rate(p, 44100);
  test_rate(p, 990);
  test_rate(p, 100);
  test_rate(p, 150);
  test_rate(p, 170);
  test_rate(p, 185);
  test_rate(p, 200);
  test_rate(p, 55);
  free(p);
  return 0;
}

int main(int argc, char const *argv[]) {
  onepole_test();
  return 0;
}
