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

    
    char* strval = config_get_str(&config, "KEY4");
    int intval = config_get_int(&config, "KEY2");
    int boolval = config_get_bool(&config, "KEY3");

    printf("strval: %s\n", strval);
    printf("intval: %d\n", intval);
    if(boolval) {
        printf("boolval: %d\n", boolval);
    }
    free_config(&config);
    free(strval);
}