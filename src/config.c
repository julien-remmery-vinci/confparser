#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/config.h"
#include "../lib/clogger/include/logger.h"

#define FILE_END 1
#define SUCCESS 0
#define ERROR -1
#define MISSING_KEY -2
#define MISSING_VALUE -3

#define TRUE 1
#define FALSE 0

Logger warn_log = {
    .level = LOG_WARN
};

char* next_token(FILE* file) {
    char* token = NULL;
    int size = 0;
    char c;

    fpos_t pos;
    
    while((c = getc(file)) != EOF) {
        switch(c) {
            case ' ':
                if (token != NULL) {
                    return token;
                }
                continue;
            case '#':
                if(token != NULL) {
                    fsetpos(file, &pos);
                    return token;
                } else {
                    while ((c = getc(file)) != EOF && c != '\n') {
                        fgetpos(file, &pos);
                    }
                    fsetpos(file, &pos);
                    break;
                }
            case '\n':
                if(token != NULL) {
                    fsetpos(file, &pos);
                    return token;
                }
                return strdup("\n");
            case '\"':
                while ((c = getc(file)) != EOF && (c != '\"')) {
                    if (token == NULL) {
                        token = (char*)malloc(2 * sizeof(char));
                        if (!token) {
                            perror("Memory allocation failed");
                            return NULL;
                        }
                        token[0] = '\0';
                    } else {
                        token = (char*)realloc(token, (size + 2) * sizeof(char));
                        if (!token) {
                            perror("Memory reallocation failed");
                            return NULL;
                        }
                    }
                    strncat(token, &c, 1);
                    size++;
                }
                return token;
            case '\'':
                while ((c = getc(file)) != EOF && (c != '\'')) {
                    if (token == NULL) {
                        token = (char*)malloc(2 * sizeof(char));
                        if (!token) {
                            perror("Memory allocation failed");
                            return NULL;
                        }
                        token[0] = '\0';
                    } else {
                        token = (char*)realloc(token, (size + 2) * sizeof(char));
                        if (!token) {
                            perror("Memory reallocation failed");
                            return NULL;
                        }
                    }
                    strncat(token, &c, 1);
                    size++;
                }
                return token;
            default:
                if(token == NULL) {
                    token = (char*)malloc(2*sizeof(char));
                    if (!token) {
                        perror("Memory allocation failed");
                        return NULL;
                    }
                    token[0] = '\0';
                } else {
                    token = (char*)realloc(token, (size + 2)*sizeof(char));
                    if (!token) {
                        perror("Memory reallocation failed");
                        return NULL;
                    }
                }
                strncat(token, &c, 1);
                size++;
                fgetpos(file, &pos);
                break;
        }
    }
    return token;
}

int get_next_key_value(FILE* file, KeyValue* kv) {
    char* key;
    do { // keep going while file contains '\n' -> maybe another key exists
        key = next_token(file);
        if(key == NULL) {
            return MISSING_KEY;
        }
    } while(strcmp(key, "\n") == 0);

    char* value = next_token(file);
    if(value == NULL || strcmp(value, "\n") == 0) {
        logger(&warn_log, "Missing value for key %s, ignoring it\n", key);
        free(key);
        return MISSING_VALUE;
    }
    kv->key = key;
    kv->value = value;

    char* next = next_token(file);

    if(next == NULL) {
        return FILE_END;
    }

    free(next);

    return SUCCESS;
}

int add_key_value(Config* config, KeyValue kv) {
    if(config->keys_count == 0) {
        config->keys = malloc(sizeof(KeyValue));
    } else {
        config->keys = realloc(config->keys, (config->keys_count + 1) * sizeof(KeyValue));
    }
    if (config->keys == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return ERROR;
    }
    config->keys[config->keys_count++] = kv;
    return SUCCESS;
}

int build_config(FILE* file, Config* config) {
    int ret;
    do {
        KeyValue kv = {0};
        ret = get_next_key_value(file, &kv);
        if(ret == FILE_END || ret == SUCCESS) {
            add_key_value(config, kv); 
            // TODO : Add error handling (weird infinite loop effect when add ret = in front)
        }
    } while(ret != FILE_END && ret != ERROR && ret != MISSING_KEY);
    if(ret == FILE_END || ret == MISSING_KEY) { // No more keys
        return SUCCESS;
    }
    return ERROR; // Error
}

int get_config(const char* filepath, Config* config) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        perror("fopen");
        return ERROR;
    }
    int ret = build_config(file, config);
    fclose(file);
    return ret;
}

void free_config(Config* config) {
    for (int i = 0; i < config->keys_count; i++) {
        free(config->keys[i].key);
        free(config->keys[i].value);
    }
    free(config->keys);
    config->keys = NULL;
    config->keys_count = 0;
}

char* config_get_str(Config* config, const char* key) {
    for (int i = 0; i < config->keys_count; i++)
    {
        if(strcmp(key, config->keys[i].key) == 0) {
            return strdup(config->keys[i].value);
        }
    }
    return NULL;
}

int config_get_int(Config* config, const char* key) {
    for (int i = 0; i < config->keys_count; i++)
    {
        if(strcmp(key, config->keys[i].key) == 0) {
            return atoi(config->keys[i].value);
        }
    }
}

int config_get_bool(Config* config, const char* key) {
    for (int i = 0; i < config->keys_count; i++)
    {
        if(strcmp(key, config->keys[i].key) == 0) {
            return strcmp(config->keys[i].value, "true") == 0 ? TRUE :
                strcmp(config->keys[i].value, "false") == 0 ? FALSE : ERROR;
        }
    }
    return ERROR;
}