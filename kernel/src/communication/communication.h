#ifndef KDEXG_COMMUNICATION_H
#define KDEXG_COMMUNICATION_H

#include <net/genetlink.h>

int init_comm(void);
void exit_comm(void);

static int handle_policy_add(struct sk_buff *skb, struct genl_info *info);
static int handle_policy_remove(struct sk_buff *skb, struct genl_info *info);
static int handle_policy_clear(struct sk_buff *skb, struct genl_info *info);

#endif
