#include "communication.h"

#include "defines.h"
#include "policy.h"

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <net/genetlink.h>

struct nla_policy const kdexg_pol[PE_COUNT] = {
  [PO_DEV] =    { .type = NLA_U64 },
  [PO_INO] =    { .type = NLA_U64 },
  [PO_CTIME] =  { .type = NLA_U64 },
  [PO_ACTION] = { .type = NLA_U32 },
};

static const struct genl_ops ops[] = {
  {
    .cmd = POLICY_ADD,
    .doit = handle_policy_add,
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 2, 0)
    .policy = kdexg_pol,  
#endif
  },
  {
    .cmd = POLICY_REMOVE,
    .doit = handle_policy_remove,
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 2, 0)
    .policy = kdexg_pol,  
#endif 
  },
  {
    .cmd = POLICY_CLEAR,
    .doit = handle_policy_clear,
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 2, 0)
    .policy = kdexg_pol,  
#endif 
  },
};

static struct genl_family kdexg_family = {
  .name = FAMILY,
  .version = 1,
  .maxattr = PE_MAX,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 2, 0)
  .policy = kdexg_pol,
#endif
  .module = THIS_MODULE,
  .ops = ops,
  .n_ops = ARRAY_SIZE(ops),
};

int init_comm(void) {
  return genl_register_family(&kdexg_family);
}

void exit_comm(void) {
  genl_unregister_family(&kdexg_family);
}

static int handle_policy_add(struct sk_buff *skb, struct genl_info *info) {
  printk(KERN_INFO "Policy Add");
  return 0;
}

static int handle_policy_remove(struct sk_buff *skb, struct genl_info *info) {
  printk(KERN_INFO "Policy Remove");
  return 0;
}

static int handle_policy_clear(struct sk_buff *skb, struct genl_info *info) {
  printk(KERN_INFO "Policy Clear");
  return 0;
}
