#pragma once

#include <iostream>
#include <sstream>

namespace demo {
    namespace logger {

        #define __NS_FUNCTION__ demo::logger::ns_function_name(__PRETTY_FUNCTION__)
        // WARNING: Always include LOG_* macros inside {} when inside an
        //          if-else clause!!
        #define LOG_LINE(__level) \
            if (demo::logger::current_log_level <= __level) \
                demo::logger::DemoLogger(__FILE__, __LINE__, __NS_FUNCTION__, __level).get()

        // Define macros for all 6 log levels:
        #define LOG_TRACE LOG_LINE(demo::logger::LogLevel::TRACE)
        #define LOG_DEBUG LOG_LINE(demo::logger::LogLevel::DEBUG)
        #define LOG_INFO LOG_LINE(demo::logger::LogLevel::INFO)
        #define LOG_WARNING LOG_LINE(demo::logger::LogLevel::WARNING)
        #define LOG_ERROR LOG_LINE(demo::logger::LogLevel::ERROR)
        #define LOG_CRITICAL LOG_LINE(demo::logger::LogLevel::CRITICAL)

        enum class LogLevel {
            TRACE = 0,
            DEBUG,
            INFO,
            WARNING,
            ERROR,
            CRITICAL,
        };

        extern LogLevel current_log_level;

        const std::string& get_level_as_string();

        void set_level(const LogLevel level);
        void set_level(const std::string& level);

        const std::string& level_to_string(const LogLevel level);
        LogLevel level_from_string(const std::string& level);

        // Control if log messages are prefixed with a header.
        void set_headers(const bool enable);
        bool get_headers();

        // Extract class and function name from __PRETTY_FUNCTION__.
        inline std::string ns_function_name(const std::string& pretty_function) {
            const size_t open_paren = pretty_function.rfind("(");
            const size_t space = pretty_function.rfind(" ", open_paren) + 1;
            return pretty_function.substr(space, open_paren - space) + "()";
        }

        class DemoLogger {
          public:
            DemoLogger(const std::string& file, const int line,
                       const std::string& func, const LogLevel level);
            ~DemoLogger();

            std::stringstream& get() { return os; }

          private:
            std::stringstream os;
            LogLevel log_level;
        };

    } // namespace logger
} // namespace demo
