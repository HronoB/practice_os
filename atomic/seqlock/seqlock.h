#pragma once

#pragma once




#pragma once

#pragma once

#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>





#include <linux/futex.h>  /* Definition of FUTEX_* constants */
#include <sys/syscall.h>  /* Definition of SYS_* constants */
#include <unistd.h>




inline void AtomicAdd(int64_t* atomic, int64_t value) {
    asm volatile (
        "lock "
        "add %1, (%0)"
        : : "r" (atomic), "r" (value)
        : "memory"
    );
}


inline int64_t AtomicGet(int64_t* atomic) {
    return *((volatile int64_t*) atomic);
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



// if (atomic == expected): atomic = value
// else : expected = atomic
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







inline long futexAll(void* addr, int op, int64_t val, struct timespec* timeout, uint32_t* uaddr2, uint32_t val3) {
    return syscall(SYS_futex, addr, op, val, timeout, uaddr2, val3);
}

inline long futex(void* addr, int op, int64_t value) {
    return futexAll(addr, op, value, NULL, NULL, 0);
}

inline void WaitOnAddr(void* addr, int64_t val) {
    futex(addr, FUTEX_WAIT_PRIVATE, val);
}

inline void NotifyOne(void* addr) {
    futex(addr, FUTEX_WAKE_PRIVATE, 1);
}

inline void NotifyAll(void* addr) {
    futex(addr, FUTEX_WAKE_PRIVATE, INT_MAX);
}



int64_t max(int64_t a, int64_t b) {
    if (a > b) {
        return a;
    }
    return b;
}



struct RwLock {
    // how mnay reading
    // how many wanting to write / writing
    // is anyone writing
    int64_t cntR, cntW, writing;
};

inline void RwLock_Init(struct RwLock* lock) {
    lock->writing = lock->cntR = lock->cntW = 0;
}

inline void RwLock_ReadLock(struct RwLock* lock) {
    while (1) {
        AtomicAdd(&lock->cntR, 1);
        if (AtomicGet(&lock->cntW)>0) {
            AtomicSub(&lock->cntR, 1);
            NotifyOne(&lock->cntR);
            WaitOnAddr(&lock->cntW, max(AtomicGet(&lock->cntW), 1));
            continue;
        } else {
            break;
        }
    }
}

inline void RwLock_ReadUnlock(struct RwLock* lock) {
    AtomicSub(&lock->cntR, 1);
    NotifyOne(&lock->cntR);
}

inline void RwLock_WriteLock(struct RwLock* lock) {
    AtomicAdd(&lock->cntW, 1);

    while (1) {
        if (AtomicGet(&lock->cntR)==0) {
            break;
        }
        WaitOnAddr(&lock->cntR, max(AtomicGet(&lock->cntR),1));
    }
    while (1){
        while (AtomicXchg(&lock->writing, 1)==1) {
            WaitOnAddr(&lock->writing, 1);
        }
        //mine now
        break;
    }
}

inline void RwLock_WriteUnlock(struct RwLock* lock) {
    AtomicXchg(&lock->writing, 0);
    AtomicSub(&lock->cntW, 1);
    NotifyOne(&lock->writing);

    NotifyOne(&lock->cntR);
    NotifyAll(&lock->cntW);
}






























































struct SpinLock {
    int64_t cntIn;
};

inline void SpinLock_Init(struct SpinLock* lock) {
    lock->cntIn = 0;
}

inline void SpinLock_Lock(struct SpinLock* lock) {
    while (AtomicXchg(&lock->cntIn, 1)==1) {
        continue;
    }
}

inline void SpinLock_Unlock(struct SpinLock* lock) {
    AtomicXchg(&lock->cntIn, 0);
}

























struct SeqLock {
    int64_t cnt;
};

inline void SeqLock_Init(struct SeqLock* lock) {
    lock->cnt = 0;
}

inline int64_t SeqLock_ReadLock(struct SeqLock* lock) {
    int64_t old = 0;
    while (1) {
        old = AtomicGet(&lock->cnt);
        if (old%2==0) {
            break;
        }
    }
    return old;
}

inline int SeqLock_ReadUnlock(struct SeqLock* lock, int64_t value) {
    return  (AtomicGet(&lock->cnt)==value);
}

inline void SeqLock_WriteLock(struct SeqLock* lock) {
    while (1) {
        int64_t exp = AtomicGet(&lock->cnt);
        if (exp%2==1) {
            continue;
        }
        int64_t old = exp;
        AtomicCas(&lock->cnt, &exp, lock->cnt+1);
        if (exp==old) {
            break;
        }
    }
}

inline void SeqLock_WriteUnlock(struct SeqLock* lock) {
    AtomicAdd(&lock->cnt, 1);
}
























