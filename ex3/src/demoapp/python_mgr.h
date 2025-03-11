#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

namespace demo {

  class PyObjectPtr {
    public:
      PyObjectPtr();
      PyObjectPtr(PyObjectPtr& pp);
      PyObjectPtr(PyObjectPtr&& pp);
      PyObjectPtr(PyObject* p);
      ~PyObjectPtr();
      PyObject* add_ref();
      void reset() noexcept;
      void reset(PyObject* p);

      PyObjectPtr& operator=(PyObjectPtr& pp) noexcept;
      PyObjectPtr& operator=(PyObjectPtr&& pp) noexcept;
      PyObjectPtr& operator=(PyObject* p) noexcept;

      PyObject* get() const noexcept { return _p; }
      PyObject** get_ref() noexcept { return &_p; }
      PyObject* operator->() const noexcept { return _p; }
      operator PyObject*() const noexcept { return _p; }
      explicit operator bool() const noexcept { return !!_p; }

    private:
      PyObject* _p;
  };


  class AcquireGil {
    public:
      AcquireGil();
      ~AcquireGil();

      AcquireGil(const AcquireGil&) = delete;
      AcquireGil& operator=(const AcquireGil&) = delete;

    private:
      PyGILState_STATE _gstate;
  };


  class PythonMgr {
      public:
        PythonMgr();
        ~PythonMgr();

        PythonMgr(const PythonMgr&) = delete;
        PythonMgr& operator=(const PythonMgr&) = delete;

        void add_module_load_path(const std::string& path);

        PyObjectPtr load_module(const std::string& module_name);
        PyObjectPtr load_function(const std::string& module_name,
                                  const std::string& function_name);

        template <typename T, class ... Args>
        T call_function(const std::string& module_name,
                        const std::string& function_name,
                        const char* format = nullptr, Args ... args);

        template <typename T>
        T cpp_type_from_py_obj(PyObjectPtr obj);

        void check_python_exception(const std::string& prefix);
        void throw_python_exception(const std::string& prefix);

        PyObjectPtr get_callable_attribute(PyObjectPtr obj,
                                           const std::string& name);

        long long_from_py_long(PyObjectPtr pp);

      private:
        using PythonCache = std::unordered_map<std::string, PyObjectPtr>;

        PyThreadState* _save;
        PyObjectPtr _sys_path;
        std::mutex _mutex;
        PythonCache _py_module_cache;
        PythonCache _py_function_cache;
    };
    using PythonMgrPtr = std::shared_ptr<PythonMgr>;

    extern PythonMgrPtr py_mgr;

} // namespace demo

#include "python_mgr.hpp"
