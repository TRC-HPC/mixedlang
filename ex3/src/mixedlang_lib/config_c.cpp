#include "config.h"

using namespace demo::config;

extern "C" {

    char* get_config() {
        Config cfg;
        const std::string cfg_str(cfg.get_config());
        const size_t buf_len = cfg_str.length() + 1;
        char* c_cfg = static_cast<char*>(malloc(buf_len));
        cfg_str.copy(c_cfg, cfg_str.length());
        return c_cfg;
    }

}
