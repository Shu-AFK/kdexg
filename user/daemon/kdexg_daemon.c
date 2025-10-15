#include "watcher.h"
#include "communication.h"

#include "json.h"

#include <stdio.h>

int main(void) {
  if (init_policy_json() == -1) {
    printf("[ERR] failed to init policy json\n");
    return -1;
  }

  if (watcher(&comm_handler) == -1) {
    printf("[ERR] failed to add watcher\n");
    return -1;
  }

  return 0;
}
