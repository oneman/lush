#include <linux/init.h> /* Needed for the macros */
#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */

MODULE_LICENSE("No");
MODULE_AUTHOR("drr");
MODULE_DESCRIPTION("A systems module");

static int __init sysop_init(void) {
  pr_info("System Operation Activity\n");
  return 0;
}

static void __exit sysop_exit(void) {
  pr_info("System Processing Complete\n");
}

module_init(sysop_init);
module_exit(sysop_exit);
