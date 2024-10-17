#include <stdio.h>
#include <stdlib.h>
#include "../include/config.h"

int main(int argc, const char** argv) {
    if(argc < 2) {
        printf("Usage:\n    %s <path to conf file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Config config = {0};
    int ret = get_config(argv[1], &config);
    if(ret < 0) {
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < config.keys_count; i++)
    {
        printf("%s      %s\n", config.keys[i].key, config.keys[i].value);
    }
    free_config(&config);
}