#define JSMN_IMPLEMENTATION

#include "communication.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Floyd Göttsch");
MODULE_DESCRIPTION("Kernel Module to detect and block suspicious file extractions");

static int __init kdexg_init(void) {
  return init_comm();
}

static void __exit kdexg_cleanup(void) {
  exit_comm();
}

module_init(kdexg_init);
module_exit(kdexg_cleanup);
