#include <stdio.h>
#include <stddef.h>
#include "textcpp_c.h"

int main() {
    if(NULL == init_text(NULL))
        printf("Init text with NULL prefix failed.\n");

    void* text = init_text("DEMO2");
    print(text, "Calling C++ from C.");
    return finalize_text(text);
}

