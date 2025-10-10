#include "json.h"

#include "cJSON.h"
#include "defines.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *policy_entry_to_json(const PolicyEntry *entry) {
  cJSON *root = cJSON_CreateObject();

  cJSON_AddStringToObject(root, "path", entry->path);
  cJSON_AddStringToObject(root, "mode", entry->mode);

  char *json_str = cJSON_PrintUnformatted(root);
  cJSON_Delete(root);
  return json_str;
}

int get_entries_from_file(PolicyEntry *entryList, int *count_out) {
  FILE *policy_file;
  policy_file = fopen("POLICY_FILE_PATH", "r");

  if (!policy_file)
    return -1;

  // Get File length
  fseek(policy_file, 0, SEEK_END);
  long len = ftell(policy_file);
  rewind(policy_file);

  char *data = malloc(len + 1);
  
  if (fread(data, 1, len, policy_file) != len) {
    fclose(policy_file);
    return -1;
  }

  data[len] = '\0';
  fclose(policy_file);

  cJSON *root = cJSON_Parse(data);
  free(data);
  if(!cJSON_IsArray(root)) {
    cJSON_Delete(root);
    return -1;
  }

  int count = cJSON_GetArraySize(root);
  PolicyEntry *entries = calloc(count, sizeof(PolicyEntry));

  for (int i = 0; i < count; i++) {
    cJSON *item = cJSON_GetArrayItem(root, i);
    cJSON *path = cJSON_GetObjectItem(item, "path");
    cJSON *mode = cJSON_GetObjectItem(item, "mode");

    if (cJSON_IsString(path)) entries[i].path = strdup(path->valuestring);
    if (cJSON_IsString(mode)) snprintf(entries[i].mode, sizeof(entries[i].mode), "%s", mode->valuestring); 
  }

  cJSON_Delete(root);
  *count_out = count;

  return 0;
}

int write_entry_to_file(const PolicyEntry *entry) {
  return 0; 
}

int remove_policy_entry(const char *path, const char *mode) {
  return 0;
}
