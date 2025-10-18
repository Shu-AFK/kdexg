#include "watcher.h"
#include "communication.h"
#include "daemon_state.h"

#include "json.h"

#include <stdio.h>
#include <signal.h>

volatile sig_atomic_t stop = 0;

void handle_signal(int sig) {
  stop = 1;
}

int main(void) {
  signal(SIGINT, handle_signal);
  signal(SIGTERM, handle_signal);

  if (init_policy_json() == -1) {
    printf("[ERR] failed to init policy json\n");
    return -1;
  }

  if (init_comm() == -1) {
    printf("[ERR] failed to init nl communication\n");
    return -1;
  }

  if (watcher(&comm_handler) == -1) {
    printf("[ERR] failed to add watcher\n");
    exit_comm();
    return -1;
  }

  exit_comm();
  return 0;
}
