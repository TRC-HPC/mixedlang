#include "demoimpl.h"
#include "logger.h"
#include "python_mgr.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace demo {

    DemoImpl::DemoImpl() {
        LOG_INFO << "Starting Demo...";
    }

    DemoImpl::~DemoImpl() {
        LOG_INFO << "Ending Demo...";
        // Since the DemoImpl creates the PythonMgr instance,
        // it must release it too.
        py_mgr.reset();
    }

    void DemoImpl::run() {
        LOG_INFO << "Running...";
        if (!py_mgr) {
            py_mgr = std::make_shared<PythonMgr>();

            const auto exe = fs::read_symlink("/proc/self/exe");
            const auto root_path = fs::canonical(exe.parent_path());
            // All modules will be in the modules directory.
            py_mgr->add_module_load_path(root_path / "modules");
            // Add root directory too
            py_mgr->add_module_load_path(root_path);
        }

        const int64_t i = 7;
        const int64_t j = 21;
        // L: int (int64_t)
        auto sum = py_mgr->call_function<int64_t>("modules.demomodule",
                                                  "add", "LL", i, j);

        LOG_INFO << i << " + " << j << " = " << sum;
    }

} // namespace demo
