#include "textc.h"
#include <string>

int main(int argc, char** argv) {

    const std::string t("Calling C from C++.");
    print(t.c_str());

    return 0;
}
