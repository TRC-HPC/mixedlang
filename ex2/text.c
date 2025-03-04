#include <stdio.h>

int print(const char* t) {
    if (!t)
        return -1;

    printf("%s\n", t);
    return 0;
}
