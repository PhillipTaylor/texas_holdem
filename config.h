
#include <string.h>
#include "linkedlist.h"
#include <stdbool.h>

bool config_load(char *s);

bool config_get_string(char *key, char **output);
bool config_get_int(char *key, int **output);
bool config_get_bool(char *key, bool **output);

