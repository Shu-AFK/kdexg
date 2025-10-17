#ifndef KDEXG_POLICY_H
#define KDEXG_POLICY_H

#include <linux/types.h>
#include <linux/fs.h>

enum dexg_policy_action {
  DEXG_ACTION_LOG = 1,
  DEXG_ACTION_BLOCK,
};

struct dexg_policy_entry {
    dev_t dev;
    unsigned long ino;
    unsigned long ctime;
    unsigned int action;
};

int add_policy_entry(dev_t dev, unsigned long ino, unsigned long ctime, unsigned int action);
int remove_policy_entry(dev_t dev, unsigned long ino);
bool dexg_policy_lookup(dev_t dev, unsigned long ino, unsigned int *action_out);
void policy_clear(void);
void policy_dump(void); // For debugging


#endif
