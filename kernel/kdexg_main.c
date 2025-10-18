#include "communication.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init kdexg_init(void) {
  printk(KERN_INFO "Kdexg init\n");
  return init_comm();
}

static void __exit kdexg_cleanup(void) {
  printk(KERN_INFO "kdexg exit\n");
  exit_comm();
}

module_init(kdexg_init);
module_exit(kdexg_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Floyd Göttsch");
MODULE_DESCRIPTION("kDEXG - Data Exfiltration Guard kernel module");
MODULE_VERSION("0.1");
