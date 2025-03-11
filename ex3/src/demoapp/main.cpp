#include <cstdlib>
#include "demoimpl.h"
#include "exceptions.h"
#include "logger.h"

using namespace demo;

int main(int argc, char *argv[]) {
    try {
        DemoImpl impl;
        impl.run();
    } catch(const std::exception& e) {
        LOG_ERROR << "An exception occurred: " << e.what();
        return 2;
    } catch(...) {
        LOG_ERROR << "An unknown exception occurred.";
        return 3;
    }
    return 0;
}
