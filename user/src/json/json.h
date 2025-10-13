#ifndef KDEXGCTL_JSON_H
#define KDEXGCTL_JSON_H

typedef struct {
  char *path;
  char *mode;
} PolicyEntry;

int init_policy_json();
int get_entries_from_file(PolicyEntry **entryList, int *count_out);
int write_entry_to_file(const PolicyEntry *entry);
int remove_policy_entry(const char *path);

void free_entries(PolicyEntry *entries, int len);

#endif
