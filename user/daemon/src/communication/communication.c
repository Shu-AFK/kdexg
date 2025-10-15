// https://stackoverflow.com/questions/60821210/how-to-send-and-receive-a-struct-through-netlink
#include "communication.h"

#include "json.h"
#include "defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <netlink/genl/ctrl.h>
#include <netlink/genl/genl.h>
#include <sys/types.h>
#include <sys/inotify.h>

static struct nl_sock *sock;
static int genl_family;

static struct nl_msg *prepare_message(const PolicyEntry entry, uint32_t mask) {
  struct nl_msg *msg;
  msg = nlmsg_alloc();
  int type;

  if (mask == IN_MODIFY) type = POLICY_UPDATE;
  else type = POLICY_INIT;

  genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, genl_family, 0, 0, type, 1);
  nla_put_string(msg, PE_PATH, entry.path);
  nla_put_string(msg, PE_MODE, entry.mode);

  return msg;
}

int comm_handler(uint32_t mask) {
  PolicyEntry *entries;
  int len = 0;

  if (get_entries_from_file(&entries, &len) == -1) {
    printf("[ERR] Failed to get entries from file\n");
    return -1;
  }

  sock = nl_socket_alloc();
  genl_connect(sock);
  genl_family = genl_ctrl_resolve(sock, FAMILY);

  struct nl_msg *msg;
  for (int i = 0; i < len; i++) {
    msg = prepare_message(entries[i], mask);
    nl_send_auto(sock, msg);

    nlmsg_free(msg);
  }

  nl_socket_free(sock);
  free_entries(entries, len);
  return 0;
}
