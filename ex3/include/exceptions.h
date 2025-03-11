#pragma once

#include <stdexcept>
#include <string>

namespace demo {

    class DemoException : public std::runtime_error {
      public:
       explicit DemoException(const std::string& msg);
       virtual ~DemoException() noexcept = default;
    };

    class PythonException : public DemoException {
      public:
        explicit PythonException(const std::string& msg);
    };

} // namespace demo

