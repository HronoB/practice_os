#pragma once

#include <stdint.h>



inline void AtomicAdd(int64_t* atomic, int64_t value) {
    asm volatile (
        "lock add %1, (%0)"
        : : "r" (atomic), "r" (value)
        : "memory"
    );
}

inline void AtomicSub(int64_t* atomic, int64_t value) {

    asm volatile  (
        "lock "
        "sub %1, (%0)"
        : : "r" (atomic), "r" (value)
        : "memory"
        );
}

inline int64_t AtomicXchg(int64_t* atomic, int64_t value) {
    asm volatile (
        "lock "
        "xchg %0, (%1)"
        : "+r" (value)
        : "r" (atomic)
        : "memory"
        );
    return value;
}

inline int64_t AtomicCas(int64_t* atomic, int64_t* expected, int64_t value) {
    int64_t flag = 0;
    asm volatile (
        "lock cmpxchg %3, (%2) \n"
        "setz %b1"
        : "+a" (*expected), "+r" (flag)
        : "r" (atomic), "r" (value)
        : "memory", "cc"
        );
    return flag;
}














































