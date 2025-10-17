#ifndef KDEXGDAEMON_DEFINES_H
#define KDEXGDAEMON_DEFINES_H

enum dexg_policy_action {
  DEXG_ACTION_LOG = 1,
  DEXG_ACTION_BLOCK,
};

#define FAMILY "KDEXG_FAMILY"

enum operations {
  POLICY_ADD,
  POLICY_REMOVE,
  POLICY_CLEAR,
};

enum msg_attributes {
  PO_DEV = 1,
  PO_INO,
  PO_CTIME,
  PO_ACTION,
  PE_COUNT,
#define PE_MAX (PE_COUNT - 1)
};

#define POLICY_DIRECTORY_PATH "/etc/kdexg/"
#define POLICY_FILE_PATH      "/etc/kdexg/policy.json"

#endif
