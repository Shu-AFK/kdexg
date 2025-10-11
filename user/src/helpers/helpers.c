#include "helpers.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

int parse_args(int argc, char **argv, bool *add, bool *rem, char **path, char **mode) {
  int opt; 
  int len = 0;

  static struct option long_options[] = {
    { "help",   no_argument,       0, 'h' },
    { "add",    no_argument,       0, 'a' },
    { "remove", no_argument,       0, 'r' },
    { "path",   required_argument, 0, 'p' },
    { "mode",   required_argument, 0, 'm' },
    { 0,        0,                 0,  0  }
  };

  while ((opt = getopt_long(argc, argv, "harp:m:", long_options, NULL)) != -1) {
    switch (opt) {
      case 'h':
        print_usage();
        return 0;

      case 'a':
        *add = true;
        break;

      case 'r':
        *rem = true;
        break;

      case 'p':
        len = strlen(optarg) + 1;
        *path = (char *) malloc(len);
        if(*path) strncpy(*path, optarg, len);
        else return -1;
        break;

      case 'm':
        len = strlen(optarg) + 1;
        if (len > 16) len = 16;
        *mode = malloc(len);
        if (*mode) {
          strncpy(*mode, optarg, len - 1);
          (*mode)[len - 1] = '\0';
        } else {
          return -1;
        }
        break;
    }
  }

  return 0;
}

void print_usage(void) {
    printf("Usage: kdexgctl [OPTIONS]\n\n");
    printf("Options:\n");
    printf("  -h, --help               Show this help message and exit\n");
    printf("  -a, --add                Add a new policy entry\n");
    printf("  -r, --remove             Remove an existing policy entry\n");
    printf("  -p, --path <path>        Specify the target file or directory path\n");
    printf("  -m, --mode <mode>        Specify access mode (e.g., read, write, block)\n\n");
    printf("Examples:\n");
    printf("  kdexgctl --add --path /home/user/docs --mode read\n");
    printf("  kdexgctl --remove --path /tmp/secrets --mode block\n\n");
}

int ensure_directory_exists(const char *path) {
  if (mkdir(path, 0755) == -1) {
    if (errno == EEXIST) {
      return 0;
    } else {
      perror("mkdir");
      return -1;
    }
  }

  return 0;
}
