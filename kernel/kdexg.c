#define JSMN_IMPLEMENTATION
#include "jsmn.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init kdexg_init(void) {
  return 0;
}

static void __exit kdexg_cleanup(void) {
  
}

module_init(kdexg_init);
module_exit(kdexg_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Floyd Göttsch");
MODULE_DESCRIPTION("Kernel Module to detect and block suspicious file extractions");
