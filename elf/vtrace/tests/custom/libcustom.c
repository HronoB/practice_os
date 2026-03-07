#include "libcustom.h"

int sum(int a, int b) {
    return a + b;
}

float average(float* a, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += a[i];
    }
    return sum / size;
}
