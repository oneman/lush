static const char *names[] = {
  "idomeneo",  "subduct",  "italianating",  "dromos",  "donets",
  "crystallinity",  "arborescently",  "privately",  "expatriating",
  "granicus",  "anacoustic",  "washerwoman",  "wadsetting",  "saviorship",
  "justification",  "scrollwork",  "hypnotization",  "succorer",  "papula",
  "prediscrimination",  "bugbane",  "decca",  "christoff",  "steamless",
  "compromissary",  "aves",  "docket",  "crookback",  "tuberculinize",
  "lopstick",  "aircraftsman",  "chelicera",  "chunnel",  "spiderman",
  "nurl",  "prepriced",  "lawful",  "reillumination",  "nicaragua",
  "reaccuse",  "poussin",  "unshocked",  "gnaw",  "hammerfest",  "privates",
  "nonstimulant",  "unmeasurably",  "shemite",  "arctogaeic",  "school",
  "intertribal",  "meningococci",  "octavia",  "penultimate",  "netsuke",
  "abed",  "unsoiled",  "rickettsial",  "mousepox",  "bourse",  "unbating",
  "grackle",  "vulgarised",  "nonrevokable",  "leerily",  "poetizer",  "underscribe",
  "disposal",  "exstrophy",  "nymph",  "jangly",  "precollector",  "intersexually",
  "cogitation",  "hazer",  "essay",  "reincarnationist",  "lombardic",
  "unsplenetic",  "heroicity",  "phdre",  "cofinal",  "backbite",
  "staphylorrhaphy",  "alopecia",  "entertainment",  "fabre",
  "precommunicated",  "bastogne",  "starriness",  "multure",
  "polycarpous",  "peronismo",  "homeotherm",  "thessalonki",
  "accessibility",  "decidable",  "delitescency",  "biding",  "exilarch"
};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <krad/app/debug.h>

char *test_dir() {
  static int setup = 0;
  static char thedir[512];
  if (setup == 0) {
    if (getenv("KRAD_TEST_DATA") == NULL) {
      printke("Test Data Error! Can not find KRAD_TEST_DATA env var!");
      exit(1);
    }
    snprintf(thedir, sizeof(thedir), "%s", getenv("KRAD_TEST_DATA"));
    setup = 1;
  }
  return thedir;
}

char *test_output_dir() {
  static int setup = 0;
  static char thedir[512];
  if (setup == 0) {
    snprintf(thedir, sizeof(thedir), "%s/output", test_dir());
    setup = 1;
  }
  return thedir;
}

char *test_gif_dir() {
  static int setup = 0;
  static char thedir[512];
  if (setup == 0) {
    snprintf(thedir, sizeof(thedir), "%s/images/gif", test_dir());
    setup = 1;
  }
  return thedir;
}

char *test_png_dir() {
  static int setup = 0;
  static char thedir[512];
  if (setup == 0) {
    snprintf(thedir, sizeof(thedir), "%s/images/png", test_dir());
    setup = 1;
  }
  return thedir;
}

char *test_jpg_dir() {
  static int setup = 0;
  static char thedir[512];
  if (setup == 0) {
    snprintf(thedir, sizeof(thedir), "%s/images/jpg", test_dir());
    setup = 1;
  }
  return thedir;
}

char *test_font_dir() {
  static int setup = 0;
  static char thedir[512];
  if (setup == 0) {
    snprintf(thedir, sizeof(thedir), "%s/fonts", test_dir());
    setup = 1;
  }
  return thedir;
}

int test_time() {
  int seconds;
  struct timeval tv;
  gettimeofday (&tv, NULL);
  seconds = tv.tv_sec;
  return seconds;
}
