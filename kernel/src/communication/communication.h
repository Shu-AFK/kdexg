#ifndef KDEXG_COMMUNICATION_H
#define KDEXG_COMMUNICATION_H

static int init_comm();
static void exit_comm();

static int handle_policy_update(struct sk_buff *skb, struct genl_info *info);
static int handle_policy_init(struct sk_buff *skb, struct genl_info *info);

#endif
