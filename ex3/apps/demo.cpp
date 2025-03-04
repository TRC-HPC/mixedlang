#include "logger.h"
#include "config.h"

using namespace demo::config;

int main(int argc, char *argv[]) {
    LOG_INFO << "Demo C++ app to call C++ library.";

    Config cfg;
    LOG_INFO << cfg.get_config();

    return 0;
}
