#include "module_pymixedlang.h"
#include "config.h"
#include "logger.h"

using namespace demo;

const char* py_mixedlang_module_name = "pymixedlang";
PyDoc_STRVAR(py_mixedlang_doc, "Access to mixedlang library functions.");

static PyObject* module_error;

// Get the current mixedlang log level as a Python logging level (log_level() -> int).
static PyObject* log_level(PyObject* self, PyObject*) {
    // Import logging (Python's standard logging module)
    PyObject* py_name = PyUnicode_FromString("logging");
    PyObject* py_logging = PyImport_Import(py_name);
    Py_DecRef(py_name);
    if (!py_logging)
        return nullptr;

    std::string level(logger::get_level_as_string());
    // Python does not have a TRACE log level
    if (level == "TRACE")
        level = "DEBUG";

    // Get the Python logging level corresponding to this log level name.
    PyObject* py_attr = PyObject_GetAttrString(py_logging, level.c_str());
    Py_DecRef(py_logging);
    return py_attr;
}

// Set the current mixedlang log level as a Python logging level (set_level(level: int)).
static PyObject* set_level(PyObject* self, PyObject* py_level) {
    // Import logging (Python's standard logging module)
    PyObject* py_name = PyUnicode_FromString("logging");
    PyObject* py_logging = PyImport_Import(py_name);
    Py_DecRef(py_name);
    if (!py_logging)
        return nullptr;

    // Get the log level name for this Python logging level
    py_name = PyUnicode_FromString("getLevelName");
    PyObject* py_level_name = PyObject_CallMethodOneArg(py_logging, py_name, py_level);
    Py_DecRef(py_name);
    Py_DecRef(py_logging);
    if(!py_level_name)
        return nullptr;

    const char* level_name = PyUnicode_AsUTF8(py_level_name);
    Py_DecRef(py_level_name);
    if (!level_name)
        return nullptr;

    const std::string level(level_name);
    try {
        logger::set_level(level);
    } catch (const std::exception& e) {
        PyErr_SetString(module_error, e.what());
        return nullptr;
    }
    Py_RETURN_NONE;
}

// Set the current mixedlang log headers option (set_headers(enable: bool)).
static PyObject* set_headers(PyObject* self, PyObject* py_enable) {
    long enable = PyLong_AsLong(py_enable);
    if (-1 == enable && PyErr_Occurred())
        return nullptr;

    try {
        logger::set_headers(enable);
    } catch (const std::exception& e) {
        PyErr_SetString(module_error, e.what());
        return nullptr;
    }
    Py_RETURN_NONE;
}

// Get the current mixedlang log headers option (get_headers() -> bool).
static PyObject* get_headers(PyObject* self, PyObject*) {
    return PyBool_FromLong(logger::get_headers());
}

// Log the message (log(levelname : str, thread : int, msg : str)).
// msg should be in format 'pathname:lineno funcName(): msg'.
static PyObject* log(PyObject* self, PyObject* args) {
    const char* level = nullptr;
    uint64_t thread_id = 0;
    const char* msg = nullptr;
    if (!PyArg_ParseTuple(args, "sKs", &level, &thread_id, &msg))
        return nullptr;

    try {
        logger::log(level, msg);
    } catch (const std::exception& e) {
        PyErr_SetString(module_error, e.what());
        return nullptr;
    }
    Py_RETURN_NONE;
}

// Get the mixedlang config (get_config() -> str).
static PyObject* get_config(PyObject* self, PyObject*) {
    try {
        config::Config cfg;
        const std::string cfg_str(cfg.get_config());
        return PyUnicode_FromString(cfg_str.c_str());
    } catch (const std::exception& e) {
        PyErr_SetString(module_error, e.what());
        return nullptr;
    }
}

static PyMethodDef MixedlangMethods[] = {
    {"log_level", log_level, METH_NOARGS,
    "Get the current mixedlang log level as a Python logging level (log_level() -> int)."},
    {"set_level", set_level, METH_O,
    "Set the current mixedlang log level as a Python logging level (set_level(level: int))."},
    {"set_headers", set_headers, METH_O,
    "Set the current mixedlang log headers option (set_headers(enable: bool))."},
    {"get_headers", get_headers, METH_NOARGS,
    "Get the current mixedlang log headers option (get_headers() -> bool)."},
    {"log", log, METH_VARARGS,
    "Log the message (log(levelname : str, thread : int, msg : str)). msg should be in format 'pathname:lineno funcName(): msg'."},
    {"get_config", get_config, METH_NOARGS,
    "Get the mixedlang config (get_config() -> str)."},
    {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef MixedlangModule = {
    PyModuleDef_HEAD_INIT,
    py_mixedlang_module_name,
    py_mixedlang_doc,
    -1, // module has global state
    MixedlangMethods
};

PyMODINIT_FUNC PyInit_pymixedlang() {
    PyObject* py_module = PyModule_Create(&MixedlangModule);
    if (!py_module)
        return nullptr;

    module_error = PyErr_NewException("pymixedlang.Error", nullptr, nullptr);
    Py_XINCREF(module_error);
    if (PyModule_AddObject(py_module, "Error", module_error) < 0) {
        Py_XDECREF(module_error);
        Py_CLEAR(module_error);
        Py_DECREF(py_module);
        return nullptr;
    }
    return py_module;
}
