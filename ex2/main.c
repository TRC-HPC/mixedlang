#include <stdio.h>
#include <stddef.h>
#include "textc.h"

int main() {
    if (-1 == print(NULL))
        printf("Printing a NULL string failed.\n");

    return print("Example 2: Calling C from C.");
}

