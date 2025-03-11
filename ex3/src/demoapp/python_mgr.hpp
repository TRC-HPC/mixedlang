#pragma once

#include "exceptions.h"
#include "logger.h"

namespace demo {

    template <typename T, class ... Args>
    T PythonMgr::call_function(const std::string& module_name,
                               const std::string& function_name,
                               const char* format, Args ... args) {
        AcquireGil gil;
        auto py_func = load_function(module_name, function_name);
        PyObjectPtr py_value = PyObject_CallFunction(py_func, format, args...);
        if (!py_value) {
            check_python_exception("PythonMgr::call_function");
            throw PythonException("PythonMgr::call_function: Function '"
                                  + function_name + "' from module '"
                                  + module_name
                                  + "' return value is NULL.");
        }

        return cpp_type_from_py_obj<T>(py_value);
    }

    template <typename T>
    T PythonMgr::cpp_type_from_py_obj(PyObjectPtr obj) {
        if constexpr (std::is_same_v<T, long>)
            return long_from_py_long(obj);
        if constexpr (std::is_same_v<T, PyObjectPtr>)
            return obj;

        throw PythonException("PythonMgr::cpp_type_from_py_obj: C++ type is not supported.");
    }

} // namespace demo
