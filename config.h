
#include <string.h>
#include "linkedlist.h"
#include <stdbool.h>

bool config_load(char *s);

bool config_get_string(char *key, char **output);
bool config_get_int(char *key, int **output);
bool config_get_bool(char *key, bool **output);

void config_set_string(char *key, char *value);
void config_set_int(char *key, int *value);
void config_set_bool(char *key, bool *value);

void config_del_value(char *key);




