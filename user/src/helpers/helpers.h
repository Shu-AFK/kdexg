#ifndef KDEXGCTL_HELPERS_H
#define KDEXGCTL_HELPERS_H

int parse_args(int argc, char **argv, bool *add, bool *rem, char **path, char **mode);
void print_usage();

int ensure_directory_exists(const char *path);

#endif
