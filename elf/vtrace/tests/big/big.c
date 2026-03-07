#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    for (int i = 0; i < 1000; i++) {
        char* p = malloc(10);
        memset(p, 'a', 9);
        p[9] = '\0';
        printf("%s\n", p);
        free(p);
    }
}