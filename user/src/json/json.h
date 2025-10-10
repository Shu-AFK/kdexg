#ifndef KDEXGCTL_JSON_H
#define KDEXGCTL_JSON_H

typedef struct {
  char *path;
  char mode[16];
} PolicyEntry;

char *policy_entry_to_json(const struct PolicyEntry *entry);
int get_entries_from_file(struct PolicyEntry *entryList, int *count_out);
int write_entry_to_file(const PolicyEntry *entry);
int remove_policy_entry(const char *path, const char *mode);

#endif
