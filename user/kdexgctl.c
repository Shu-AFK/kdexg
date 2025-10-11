#include "json.h"
#include "helpers.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc == 1) {
    print_usage();
    return 0;
  }

  bool add, remove = false;
  char *path, *mode = NULL;

  if (parse_args(argc, argv, &add, &remove, &path, &mode) == -1) {
    printf("Failed to parse command line args\n");
    goto err;
  }

  if ((!add && !remove) || (add && remove)) {
    printf("Please either add or remove a policy.\n");
    print_usage();
    goto err;
  }

  if (init_policy_json() != 0) {
    printf("Failed to init json file.\n");
    goto err;
  }

  if(add) {
    if (!mode || !path) {
      printf("Please specify a path and a mode for adding a policy.\n");
      print_usage();
      goto err;
    }

    PolicyEntry entry = {
      .path = path,
      .mode = mode
    };

    if (write_entry_to_file(&entry) != 0) {
      printf("Write entry to file failed.\n");
      goto err;
    }
  } else {
    if (!mode || !path) {
      printf("Please specify a path and a mode for adding a policy.\n");
      print_usage();
      goto err;
    }

    if(remove_policy_entry(path, mode) != 0) {
      printf("Remove policy entry failed.\n");
      goto err;
    }
  }

  if (path) free(path);
  if (mode) free(mode);

  return 0;

err:
  if (path) free(path);
  if (mode) free(mode);

  return -1;
}
