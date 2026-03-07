#include <stdio.h>

int a = 1;
int b = 2;

void lol() {
    puts("lol");
}

void kek() {
    puts("kek");
}

int main() {
    lol();
    kek();
    kek();
    lol();
    printf("%d\n", a + b);
    return 0;
}