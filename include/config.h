#ifndef CLEX_H
#define CLEX_H

typedef struct {
    char* key;
    char* value;
} KeyValue;

typedef struct {
    KeyValue* keys;
    int keys_count;
} Config;

int get_config(const char* file, Config* config);

void free_config(Config* config);

char* config_get_str(Config* config, const char* key);
int config_get_int(Config* config, const char* key);
int config_get_bool(Config* config, const char* key);

#endif