#pragma once

// WARNING: Always include LOG_* macros inside {} when inside an
//          if-else clause!!
#define LOG(__level, __format, ...) \
    if (current_log_level() <= __level) \
        log_msg(__level, __FILE__, __LINE__, __FUNCTION__, __format __VA_OPT__(,) __VA_ARGS__);

enum LogLevel {
    TRACE = 0,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL,
};

int current_log_level();

const char* get_level_as_string();

void set_level(const int level);
void set_level_from_string(const char* level);

const char* level_to_string(const int level);
int level_from_string(const char* level);

// Control if log messages are prefixed with a header.
void set_headers(const int enable);
int get_headers();

void log_msg(const int level, const char* file, const int line, const char* func,
             const char* format, ...);
