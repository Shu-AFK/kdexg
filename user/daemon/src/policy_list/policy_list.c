#include "policy_list.h"

#include "utlist.h"

#include <stdlib.h>

struct policy *list = NULL, *element, tmp;

void free_policy_list() {
  LL_FOREACH(list, element) {
    free(element);
  }
}

int init_policy_list() {
  return 0;
}
