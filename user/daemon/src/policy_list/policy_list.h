#ifndef KDEXG_DAEMON_LIST_H
#define KDEXG_DAEMON_LIST_H

#include "json.h"

struct policy {
  PolicyEntry entry;
  struct policy *next;
};

void free_policy_list();
int init_policy_list();

#endif
