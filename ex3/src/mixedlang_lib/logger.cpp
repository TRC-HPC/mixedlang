#include "logger.h"
#include <unordered_map>
#include <exception>

namespace demo {
    namespace logger {

        LogLevel current_log_level = LogLevel::INFO;
        static bool log_headers = true;

        static std::unordered_map<LogLevel, std::string> log_level_to_str = {
            {LogLevel::TRACE, "TRACE"},
            {LogLevel::DEBUG, "DEBUG"},
            {LogLevel::INFO, "INFO"},
            {LogLevel::WARNING, "WARNING"},
            {LogLevel::ERROR, "ERROR"},
            {LogLevel::CRITICAL, "CRITICAL"}
        };

        static std::unordered_map<std::string, LogLevel> str_to_log_level = {
            {"TRACE", LogLevel::TRACE},
            {"DEBUG", LogLevel::DEBUG},
            {"INFO", LogLevel::INFO},
            {"WARNING", LogLevel::WARNING},
            {"ERROR", LogLevel::ERROR},
            {"CRITICAL", LogLevel::CRITICAL}
        };

        const std::string& get_level_as_string() {
            return log_level_to_str[current_log_level];
        }

        void set_level(const LogLevel level) {
            if (current_log_level != level) {
                LOG_INFO << "Changing current log level (" <<
                    log_level_to_str[current_log_level] << ") to " <<
                    log_level_to_str[level];

                current_log_level = level;
            }
        }

        void set_level(const std::string& level) {
            const auto new_level = str_to_log_level.find(level);
            if (new_level == str_to_log_level.end()) {
                LOG_ERROR << "Invalid level=" << level;
            } else {
                set_level(new_level->second);
            }
        }

        const std::string& level_to_string(const LogLevel level) {
            return log_level_to_str[level];
        }

        LogLevel level_from_string(const std::string& level) {
            const auto level_iter = str_to_log_level.find(level);
            if (level_iter != str_to_log_level.end())
                return level_iter->second;

            LOG_ERROR << "Invalid level=" << level;
            throw std::invalid_argument("Invalid level=" + level);
        }

        void set_headers(const bool enable) {
            log_headers = enable;
        }

        bool get_headers() {
            return log_headers;
        }


        DemoLogger::DemoLogger(const std::string& file, const int line,
                               const std::string& func, const LogLevel level) :
            log_level(level)
        {
            if (log_headers && current_log_level <= log_level) {
                os << "[" << log_level_to_str[level] << "]"
                << " " << file << ":" << line << " " << func << ": ";
            }
        }

        DemoLogger::~DemoLogger() {
            if (current_log_level <= log_level)
                std::cerr << os.str() << std::endl;
        }

    } // namespace logger
} // namespace demo
