#include <stdio.h>

extern int add(int i, int j);

int main() {
    int sum = add(5, 7);
    printf("Sum: %d\n", sum);
    return 0;
}


