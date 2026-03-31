#pragma once

#include <stdint.h>

inline void AtomicAdd(int64_t* atomic, int64_t value) {


    asm (
        "lock "
        "add %1, (%0)"
        :
        : "r" (atomic), "r" (value)
        );




}

inline void AtomicSub(int64_t* atomic, int64_t value) {

    asm (
        "lock "
        "sub %1, (%0)"
        :
        : "r" (atomic), "r" (value)
        );




}

inline int64_t AtomicXchg(int64_t* atomic, int64_t value) {
    asm(
        "lock "
        "xchg %1, (%0)"
        : "+r" (atomic), "+r" (value)
        );
    return value;
}

inline int64_t readFlagZF() {
    int64_t res = 0;
    asm (
        "setz %b0"
        : "+r" (res)
        );
    return res;
}

inline int64_t AtomicCas(int64_t* atomic, int64_t* expected, int64_t value) {
    asm (
        "lock cmpxchg (%2), (%1) "
        : "+r" (atomic), "+r" (expected), "+r" (value)
        );
    return readFlagZF();
}














































