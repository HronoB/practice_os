#include <stdio.h>

void bar(int);

void foo(int limit) {
    if (limit <= 0) {
        return;
    }
    puts("foo");
    bar(limit - 1);
}