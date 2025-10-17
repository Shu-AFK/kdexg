#ifndef KDEXG_DEFINES_H
#define KDEXG_DEFINES_H

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

#endif
