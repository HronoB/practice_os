#pragma once

#include <math.h>
#include <stdint.h>



static inline void AtomicAdd(int64_t* atomic, int64_t value) {
    asm volatile (
        "lock add %1, (%0)"
        :
        : "r" (atomic), "r" (value)
        : "memory"
    );
}

static inline void AtomicSub(int64_t* atomic, int64_t value) {

    asm (
        "lock "
        "sub %1, (%0)"
        :
        : "r" (atomic), "r" (value)
        );




}

static inline int64_t AtomicXchg(int64_t* atomic, int64_t value) {
    asm(
        "lock "
        "xchg %1, (%0)"
        : "+r" (atomic), "+r" (value)
        );
    return value;
}

// probably doesnt work but  whatever
static inline int64_t readFlagZF() {
    int64_t res = 0;
    asm (
        "setz %b0"
        : "+r" (res)
        );
    return res;
}

static inline int64_t AtomicCas(int64_t* atomic, int64_t* expected, int64_t value) {
    int64_t flag = 0;
    asm (
        "lock cmpxchg %2, (%1) \n"
        "setz %b3"
        : "+a" (*expected), "+r" (atomic), "+r" (value), "+r" (flag)
        );
    return flag;
}














































