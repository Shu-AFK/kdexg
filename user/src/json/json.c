#include "json.h"

#include "helpers.h"

#include "cJSON.h"
#include "defines.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>

int init_policy_json() {
  if(access(POLICY_FILE_PATH, F_OK) == 0) {
    return 0;
  }

  if (ensure_directory_exists(POLICY_DIRECTORY_PATH) != 0) return -1;

  cJSON *root = cJSON_CreateObject();
  cJSON *array = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "policies", array);

  char *json_str = cJSON_Print(root);
  FILE *policy_file = fopen(POLICY_FILE_PATH, "w");
  if (!policy_file) {
    perror("fopen");
    cJSON_free(json_str);
    cJSON_Delete(root);
    return -1;
  }

  fputs(json_str, policy_file);
  fclose(policy_file);

  cJSON_free(json_str);
  cJSON_Delete(root);

  return 0;
}

int get_entries_from_file(PolicyEntry **entryList, int *count_out) {
  FILE *policy_file;
  policy_file = fopen(POLICY_FILE_PATH, "r");

  if (!policy_file)
    return -1;

  // Get File length
  fseek(policy_file, 0, SEEK_END);
  size_t len = ftell(policy_file);
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
  if(!root) return -1;

  cJSON *array = cJSON_GetObjectItem(root, "policies");
  if (!cJSON_IsArray(array)) {
    cJSON_Delete(root);
    return -1;
  }

  int count = cJSON_GetArraySize(array);
  PolicyEntry *entries = (PolicyEntry *) calloc(count, sizeof(PolicyEntry));
  if (!entries) {
    cJSON_Delete(root);
    return -1;
  } 

  for (int i = 0; i < count; i++) {
    cJSON *item = cJSON_GetArrayItem(array, i);
    cJSON *path = cJSON_GetObjectItem(item, "path");
    cJSON *mode = cJSON_GetObjectItem(item, "mode");

    if (cJSON_IsString(path)) entries[i].path = strdup(path->valuestring);
    if (cJSON_IsString(mode)) entries[i].mode = strdup(mode->valuestring); 
  }

  cJSON_Delete(root);
  *count_out = count;
  *entryList = entries;

  return 0;
}

int write_entry_to_file(const PolicyEntry *entry) {
  int len;
  PolicyEntry *entries;
  if (get_entries_from_file(&entries, &len) == -1) return -1;

  PolicyEntry *tmp = realloc(entries, (len + 1) * sizeof(PolicyEntry));
  if (!tmp) {
    free(entries);
    return -1;
  }

  entries = tmp;
  entries[len].path = strdup(entry->path);
  entries[len].mode = strdup(entry->mode);
  len++;

  FILE *policy_file = fopen(POLICY_FILE_PATH, "w");
  if (!policy_file) {
    for (int i = 0; i < len; i++) {
      free(entries[i].path);
      free(entries[i].mode);
    }

    free(entries);
    return -1;
  }

  cJSON *root = cJSON_CreateObject();
  cJSON *array = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "policies", array);

  for (int i = 0; i < len; i++) {
    cJSON *item = cJSON_CreateObject();
    cJSON_AddStringToObject(item, "path", entries[i].path);
    cJSON_AddStringToObject(item, "mode", entries[i].mode);
    cJSON_AddItemToArray(array, item);
  }

  char *json_str = cJSON_Print(root);
  fputs(json_str, policy_file);

  fclose(policy_file);
  cJSON_Delete(root);
  cJSON_free(json_str);

  for (int i = 0; i < len; i++) { 
    free(entries[i].path);
    free(entries[i].mode);
  }
  free(entries);

  return 0;
}

int remove_policy_entry(const char *path, const char *mode) {
  return 0;
}
