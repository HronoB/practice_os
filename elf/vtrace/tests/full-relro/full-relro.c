#include <stdio.h>

int main() {
    puts("I am running with full RELRO");
    puts("That means that GOT should be read-only");
    puts("He-he");
    return 0;
}
