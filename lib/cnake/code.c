/*
 *
 *
 * 🝩 🜜 🜝 ☐ ⧉ ⦗ ⦘
 *
 *  ⧉  𝡴  𝡵  💻  💺  🗄 🗟 📒 📏 📐 📑 🖧  🖥 🕺 🗺 🎛 🎚
 *
 * 🚜 ⌖  ⌜ ⌝ ⌞ ⌟ ⌨  ⎔  ⏚ ⏱ ⏦   ⏠  ⏡  ⏟ ⏞
 *
 *
 */

#include "header.h"

typedef enum {
 💻, 💺, 📑, 🖧 , 🎛, 🎚
} activity;

typedef struct {
  int one;
} datatype;

#include "key.c"
#include "code2.c"
#include "win.c"

void cnake(void) {
  reconfigure();
  dump_config();
  exit(0);
  testext();
  exit(0);
  for (;;) {
    home_scan();
    printf("scan done\n");
    exit(0);
  }
}
