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

#endif