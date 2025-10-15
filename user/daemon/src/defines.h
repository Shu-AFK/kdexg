#ifndef KDEXGDAEMON_DEFINES_H
#define KDEXGDAEMON_DEFINES_H

#define FAMILY "KDEXG_FAMILY"

enum operations {
  POLICY_UPDATE,
  POLICY_INIT,
};

enum msg_attributes {
  PE_PATH,
  PE_MODE,
  PE_COUNT,
#define PE_MAX (PE_COUNT - 1)
};

#define POLICY_DIRECTORY_PATH "/etc/kdexg/"
#define POLICY_FILE_PATH      "/etc/kdexg/policy.json"

#endif
