// Fortran bindings for mixedlang lib.
#include <FC.h>
#include "config.h"
#include "logger.h"
#include <exception>

using namespace demo;
using namespace demo::config;

extern "C" {

// Get the current log level as a string.
// The string MUST be allocated with 9 bytes (including the
// terminating NULL).
// Possible levels: TRACE, DEBUG, INFO, WARNING, ERROR, CRITICAL
void FC_GLOBAL(log_level_fc, LOG_LEVEL_FC)(char* level) {
    if (!level) {
        LOG_ERROR << "Argument 'level' is NULL.";
    } else {
        logger::get_level_as_string().copy(level, 8);
    }
}

// Set the current log level.
// Possible levels: TRACE, DEBUG, INFO, WARNING, ERROR, CRITICAL
void FC_GLOBAL(set_level_fc, SET_LEVEL_FC)(const char* level) {
    if (!level) {
        LOG_ERROR << "Argument 'level' is NULL.";
    } else {
        logger::set_level(level);
    }
}

// Set the current log headers option.
void FC_GLOBAL(set_headers_fc, SET_HEADERS_FC)(const int* enable) {
    if (!enable) {
        LOG_ERROR << "Argument 'enable' is NULL.";
    } else {
        logger::set_headers(*enable);
    }
}

// Log the message.
void FC_GLOBAL(log_fc, LOG_FC)(
    const char* level, const char* file, const int* line, const char* func,
    const char* msg)
{
    try {
        if (!level)
            throw std::invalid_argument("Argument 'level' is NULL.");
        if (!file)
            throw std::invalid_argument("Argument 'file' is NULL.");
        if (!line)
            throw std::invalid_argument("Argument 'line' is NULL.");
        if (!func)
            throw std::invalid_argument("Argument 'func' is NULL.");
        if (!msg)
            throw std::invalid_argument("Argument 'msg' is NULL.");

        logger::DemoLogger(file, *line, func,
                           logger::level_from_string(level)).get() << msg;
    } catch (const std::exception& e) {
        LOG_ERROR << e.what();
    }
}

// Get the config.
// If the buffer 'config' is NULL, just the correct 'size' is returned
// (including the terminating NULL). This function should be called again
// with 'config' allocated with at least 'size' bytes.
// On success, ierr is set to 0.
// If the passed in buffer 'size' is too small ierr is set to 1, and 'size'
// will be set to the correct required value.
// For any other error, ierr is set to 2.
void FC_GLOBAL(get_config_fc, GET_CONFIG_FC)(
    char* config, int* size, int* ierr)
{
    int ret = 0;
    try {
        if (!size)
            throw std::invalid_argument("Argument 'size' is NULL.");
        
        Config cfg;
        const std::string cfg_str(cfg.get_config());
        const int cfg_size = static_cast<int>(cfg_str.size() + 1);
        if (config) {
            if (*size < cfg_size) {
                LOG_ERROR << "The allocated buffer of size " << *size
                          << " is too small. Size should be " << cfg_size << ".";
                *size = cfg_size;
                ret = 1;
            } else {
                cfg_str.copy(config, *size - 1);
            }
        } else {
            *size = cfg_size;
        }
    } catch (const std::exception& e) {
        LOG_ERROR << e.what();
        ret = 2;
    }

    if (ierr) {
        *ierr = ret;
    } else {
        LOG_ERROR << "Argument 'ierr' is NULL.";
    }
}

} // extern "C"
