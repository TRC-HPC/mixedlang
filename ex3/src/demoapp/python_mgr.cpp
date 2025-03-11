#include "python_mgr.h"
#include "exceptions.h"
#include "logger.h"

namespace demo {

    PythonMgrPtr py_mgr;


    PyObjectPtr::PyObjectPtr() :
        _p(nullptr) {}

    PyObjectPtr::PyObjectPtr(PyObjectPtr& pp) :
        _p(pp._p)
    {
        add_ref();
    }

    PyObjectPtr::PyObjectPtr(PyObjectPtr&& pp) :
        _p(pp._p)
    {
        pp._p = nullptr;
    }

    PyObjectPtr::PyObjectPtr(PyObject* p) :
        _p(p) {}

    PyObjectPtr::~PyObjectPtr() {
        reset();
    }

    PyObject* PyObjectPtr::add_ref() {
        if(_p) {
            AcquireGil gil;
            Py_IncRef(_p);
        }
        return _p;
    }

    void PyObjectPtr::reset() noexcept {
        if(_p) {
            AcquireGil gil;
            Py_DecRef(_p);
        }
        _p = nullptr;
    }

    void PyObjectPtr::reset(PyObject* p) {
        reset();
        _p = p;
    }

    PyObjectPtr& PyObjectPtr::operator=(PyObjectPtr& pp) noexcept {
        reset();
        _p = pp._p;
        add_ref();
        return *this;
    }

    PyObjectPtr& PyObjectPtr::operator=(PyObjectPtr&& pp) noexcept {
        reset();
        _p = pp._p;
        pp._p = nullptr;
        return *this;
    }

    PyObjectPtr& PyObjectPtr::operator=(PyObject* p) noexcept {
        reset();
        _p = p;
        return *this;
    }


    AcquireGil::AcquireGil() :
        _gstate(PyGILState_Ensure()) {}

    AcquireGil::~AcquireGil() {
        PyGILState_Release(_gstate);
    }


    PythonMgr::PythonMgr() {
        // Note: Add any built-in modules (such as pymixedlang)
        //       before Py_Initialize.

        Py_Initialize();

        // Allow threads for the lifetime of this manager instance.
        _save = PyEval_SaveThread();
    }

    PythonMgr::~PythonMgr() {
        // All PyObject's MUST be released BEFORE Python!
        // The GIL state here is just for performance
        // (each object does this individually too).
        {
            AcquireGil gil;
            _py_function_cache.clear();
            _py_module_cache.clear();
            _sys_path.reset();
        }

        // End allow threads
        PyEval_RestoreThread(_save);
        if(0 > Py_FinalizeEx()) {
            LOG_ERROR << "PythonMgr::~PythonMgr: Py_FinalizeEx() failed.";
        }
    }

    void PythonMgr::add_module_load_path(const std::string& path) {
        AcquireGil gil;
        PyObjectPtr path_name = PyUnicode_FromString(path.c_str());
        if (!_sys_path) {
            _sys_path = PySys_GetObject((char*)"path");
            // PySys_GetObject returns a borrowed reference
            _sys_path.add_ref();
        }
        PyList_Append(_sys_path, path_name);
    }

    PyObjectPtr PythonMgr::load_module(const std::string& module_name) {
        AcquireGil gil;

        _mutex.lock();
        auto module_itr = _py_module_cache.find(module_name);
        if (module_itr != _py_module_cache.end()) {
            auto py_module = module_itr->second;
            _mutex.unlock();
            return py_module;
        }
        _mutex.unlock();

        PyObjectPtr py_name = PyUnicode_FromString(module_name.c_str());
        PyObjectPtr py_module = PyImport_Import(py_name);

        if(py_module) {
            _mutex.lock();
            _py_module_cache[module_name] = py_module;
            _mutex.unlock();
        } else {
            check_python_exception("PythonMgr::load_module");
        }
        return py_module;
    }

    PyObjectPtr PythonMgr::load_function(const std::string& module_name,
                                         const std::string& function_name) {
        AcquireGil gil;

        const std::string key(module_name + "->" + function_name);

        _mutex.lock();
        auto func_itr = _py_function_cache.find(key);
        if (func_itr != _py_function_cache.end()) {
            auto py_func = func_itr->second;
            _mutex.unlock();
            return py_func;
        }
        _mutex.unlock();

        PyObjectPtr py_module = load_module(module_name);
        PyObjectPtr py_func = get_callable_attribute(py_module, function_name);
        _mutex.lock();
        _py_function_cache[key] = py_func;
        _mutex.unlock();
        return py_func;
    }

    void PythonMgr::check_python_exception(const std::string& prefix) {
        if (PyErr_Occurred())
            throw_python_exception(prefix);
    }

    void PythonMgr::throw_python_exception(const std::string& prefix) {
        PyObjectPtr e_type, e_value, e_traceback;
        PyErr_Fetch(e_type.get_ref(), e_value.get_ref(), e_traceback.get_ref());
        std::ostringstream err_msg;
        if (!prefix.empty())
            err_msg << prefix << ": ";

        if (e_traceback) {
            // N: object (PyObject*)
            // i: int (int32_t or int64_t)
            PyObjectPtr py_tb = call_function<PyObjectPtr>("traceback", "extract_tb", "Ni",
                                                           e_traceback.get(), -1);
            PyObjectPtr idx0 = PyLong_FromLong(0L);
            PyObjectPtr fs = PyObject_GetItem(py_tb, idx0);
            if (fs) {
                PyObjectPtr filename = PyObject_GetAttrString(fs, "filename");
                PyObjectPtr lineno = PyObject_GetAttrString(fs, "lineno");
                PyObjectPtr name = PyObject_GetAttrString(fs, "name");
                if (filename)
                    err_msg << PyUnicode_AsUTF8(filename);

                if (lineno)
                    err_msg << ":" << PyLong_AsSize_t(lineno);

                if (name) {
                    if (filename || lineno) err_msg << " ";
                    err_msg << PyUnicode_AsUTF8(name) << "()";
                }
                if (filename || lineno || name) err_msg << ": ";
            }
        }
        if (e_type) {
            PyObjectPtr e_name = PyObject_GetAttrString(e_type, "__name__");
            if (e_name)
                err_msg << PyUnicode_AsUTF8(e_name) << ": ";
        }
        if (e_value) {
            PyObjectPtr e_msg = PyObject_Str(e_value);
            if (!e_msg)
                PyErr_Clear();
            else
                err_msg << PyUnicode_AsUTF8(e_msg);
        }
        throw PythonException(err_msg.str());
    }

    PyObjectPtr PythonMgr::get_callable_attribute(PyObjectPtr obj,
                                                  const std::string& name) {
        AcquireGil gil;
        PyObjectPtr py_func = PyObject_GetAttrString(obj, name.c_str());
        if(!py_func)
            check_python_exception("PythonMgr::get_callable_attribute");

        if(!PyCallable_Check(py_func)) {
            throw PythonException("PythonMgr::get_callable_attribute: Attribute '"
                                  + name + "' is not callable.");
        }
        return py_func;
    }

    long PythonMgr::long_from_py_long(PyObjectPtr pp) {
        AcquireGil gil;
        const auto v = PyLong_AsLong(pp);
        if (-1 == v)
            check_python_exception("PythonMgr::long_from_py_long");

        return v;
    }

} // namespace demo
