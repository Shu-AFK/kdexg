// https://stackoverflow.com/questions/60821210/how-to-send-and-receive-a-struct-through-netlink
#include "communication.h"

#include "json.h"
#include "defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include <netlink/genl/ctrl.h>
#include <netlink/genl/genl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/inotify.h>

static struct nl_sock *sock;
static int genl_family;

int init_comm() {
  sock = nl_socket_alloc();
  if (sock == NULL) {
    printf("[ERR] unable to allocate socket\n");
    return -1;
  }

  if (genl_connect(sock) < 0) {
    perror("genl_connect");
    nl_socket_free(sock);
    return -1;
  }
  
  genl_family = genl_ctrl_resolve(sock, FAMILY);
  if (genl_family < 0) {
    printf("[ERR] unable to resolve family %s\n", FAMILY);
    return -1;
  }

  return 0;
}

void exit_comm() {
  nl_socket_free(sock);
}

static struct nl_msg *prepare_message(const PolicyEntry entry, uint32_t type) {
  struct nl_msg *msg;
  msg = nlmsg_alloc();
  if (msg == NULL) {
    printf("[ERR] unable to allocate message\n");
    return NULL;
  }
  
  struct stat sb;
  if (stat(entry.path, &sb) == -1) {
    perror("stat");
    nlmsg_free(msg);
    return NULL;
  }

  genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, genl_family, 0, 0, type, 1);
  nla_put_u64(msg, PO_DEV, (uint64_t) sb.st_dev);
  nla_put_u64(msg, PO_INO, (uint64_t) sb.st_ino);
  nla_put_u64(msg, PO_CTIME, (uint64_t) sb.st_ctime);

  if (strcmp(entry.mode, "BLOCK") == 0) nla_put_u32(msg, PO_ACTION, DEXG_ACTION_BLOCK);
  else if (strcmp(entry.mode, "LOG") == 0) nla_put_u32(msg, PO_ACTION, DEXG_ACTION_LOG);
  else {
    printf("[ERR] invalid entry mode %s\n", entry.mode);
    nlmsg_free(msg);
    return NULL;
  }

  return msg;
}

// TODO: Compare to internal list to determine type
int comm_handler(uint32_t mask) {
  PolicyEntry *entries;
  int len = 0;

  if (get_entries_from_file(&entries, &len) == -1) {
    printf("[ERR] Failed to get entries from file\n");
    return -1;
  }

  struct nl_msg *msg;
  for (int i = 0; i < len; i++) {
    // TODO: For testing 
    msg = prepare_message(entries[i], POLICY_ADD);
    if (msg == NULL) {
      free_entries(entries, len);
      return -1;
    }
    
    int ret = nl_send_auto(sock, msg);   
    if (ret < 0) printf("[WARN] nl_send_auto failed: %s\n", nl_geterror(ret));
    
    nlmsg_free(msg);
  }

  free_entries(entries, len);
  return 0;
}
