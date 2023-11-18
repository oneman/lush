#include <krad/util/util.h>
#include <krad/app/debug.h>

int meta_loop(struct_cb *scb, member_cb *mcb) {
  int i;
  int j;
  int res;
  if (!scb || !mcb) return -1;
  for (i = 1; i < KR_META_INFO_LAST; i++) {
    if (meta_info[i].member_count < 1) continue;
    if (meta_info[i].member_count == 2) {
      if ((memcmp(meta_info[i].members[0].name, "type", 4) == 0)
       && (meta_info[i].members[1].type == T_AUNION)) {
        continue;
      }
    }
    //printk("mem count %d", meta_info[i].member_count);
    res = scb(&meta_info[i], 1);
    if (res != 0) return res;
    for (j = 0; j < meta_info[i].member_count; j++) {
      res = mcb(&meta_info[i].members[j]);
      if (res != 0) return res;
    }
    res = scb(&meta_info[i], 0);
  }
  return 0;
}
