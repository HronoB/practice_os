#include "libcustom.h"

#include <stdio.h>

int main() {
    printf("%d\n", sum(1, 2));

    float arr[] = {1, 2, 3, 4, 5};
    printf("%f\n", average(arr, 5));
    return 0;
}