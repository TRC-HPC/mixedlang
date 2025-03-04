#include "config.h"
#include "logger.h"

namespace demo {
    namespace config {

        std::string Config::get_config() const {
            LOG_INFO << "Generating the config...";
            return "This is the dummy configuration.";
        }

    } // namespace config
} // namespace demo
