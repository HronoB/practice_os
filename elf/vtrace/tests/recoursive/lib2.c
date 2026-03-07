#include <stdio.h>

void foo(int);

void bar(int limit) {
    if (limit <= 0) {
        return;
    }
    puts("bar");
    foo(limit - 1);
}