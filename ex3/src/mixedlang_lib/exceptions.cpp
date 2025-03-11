#include "exceptions.h"

namespace demo {

    DemoException::DemoException(const std::string& msg) : std::runtime_error(msg) {}

    PythonException::PythonException(const std::string& msg) : DemoException(msg) {}

} // namespace demo
