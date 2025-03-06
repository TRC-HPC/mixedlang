#include "logger.h"
#include <cstdarg>
#include <stdio.h>
#include <exception>

using namespace demo;

extern "C" {
    int current_log_level() {
        return static_cast<int>(logger::current_log_level);
    }

    const char* get_level_as_string() {
        return logger::get_level_as_string().c_str();
    }

    void set_level(const int level) {
        const logger::LogLevel new_level = static_cast<logger::LogLevel>(level);
        if (new_level < logger::LogLevel::TRACE || new_level > logger::LogLevel::CRITICAL) {
            LOG_ERROR << "Invalid level=" << level;
        } else {
            logger::set_level(new_level);
        }
    }

    void set_level_from_string(const char* level) {
        logger::set_level(level);
    }

    const char* level_to_string(const int level) {
        return logger::level_to_string(static_cast<logger::LogLevel>(level)).c_str();
    }

    int level_from_string(const char* level) {
        try {
            return static_cast<int>(logger::level_from_string(level));
        } catch (const std::exception& e) {
            return -1;
        }
    }

    void set_headers(const int enable) {
        logger::set_headers(enable);
    }

    int get_headers() {
        return logger::get_headers() ? 1 : 0;
    }

    void log_msg(const int level, const char* file, const int line, const char* func,
                 const char* format, ...) {
        va_list vargs;
        va_start(vargs, format);
        char* buf = nullptr;
        size_t buf_len = 0;
        const int n = vsnprintf(buf, buf_len, format, vargs);
        va_end(vargs);
        if (n >= 0) {
            buf_len = static_cast<size_t>(n) + 1;
            buf = static_cast<char*>(malloc(buf_len));
            if (buf) {
                va_start(vargs, format);
                vsnprintf(buf, buf_len, format, vargs);
                va_end(vargs);
                logger::DemoLogger(file, line, func, static_cast<logger::LogLevel>(level)).get() << buf;
                free(buf);
            }
        }
    }
}

