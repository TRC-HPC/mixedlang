#include "logger_c.h"
#include "config_c.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    LOG(INFO, "Demo C app to call C++ library.");

    printf("Default log level: %s\n", get_level_as_string());
    set_level(9); // set an invalid level
    printf("Log level: %s\n", get_level_as_string());
    set_level_from_string("INVALID");
    printf("Log level: %s\n", get_level_as_string());
    set_level_from_string("DEBUG");

    char* cfg = get_config();
    LOG(DEBUG, "Config: %s", cfg);
    free(cfg);

    return 0;
}
