
// Создаёт новый fiber и помещает его в конец очереди исполнения
//
// Input:
//   - func, data - аналогично аргументам pthread_create


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TODO()

TODO() //asembly eax

struct fiberState {
    size_t rip, rsp, rbp, rbx, r12, r13, r14, r15;
};



void* toDelete[100];
int toDelSize = 0;
#define stackSize 20000
#define queCap 500
#define endline ";\n\t"
struct fiberState fibers[queCap];

int fiberSize = 0, fiberIx = 0;

struct fiberState* pushFiber() {
    fiberIx = (fiberIx+1)%queCap;
    fiberSize++;
    return &fibers[fiberIx];
}

struct fiberState *popFiber() {
    if (fiberSize==0) {
        abort();
    }
    int lastix = (fiberIx+queCap-fiberSize+1)%queCap;
    fiberSize--;
    return &fibers[lastix];
}










// 1 if loaded from que
extern int saveStateAsm(struct fiberState *res);
extern int getStateAsm(struct fiberState *nxt);


// -1 if loaded from que
__attribute__((no_sanitize("address"), returns_twice))
volatile int saveState() {
    struct fiberState *res = pushFiber();

    int loaded = saveStateAsm(res);

    if (loaded==1) {
        return -1;
    } else {
        return fiberIx;
    }
}

//no return
__attribute__((no_sanitize("address")))
volatile void getState() {
    if (fiberSize==0) { //no tasks
        return;
    }
    struct fiberState *next = popFiber();

    getStateAsm(next);

    __builtin_unreachable();
}


void FiberDie(void* addr) {
    toDelete[toDelSize++] = addr;
}


extern void fiberspawntrampolinelabel();

__attribute__((no_sanitize("address")))
void FiberSpawn(void (*func)(void*), void* data) {

    //stack shenanigans
    char* stack = malloc(stackSize);
    if (stack==NULL) {
        abort();
    }

    uintptr_t stackTop = (uintptr_t)(stack+stackSize);
    stackTop &= ~((uintptr_t)0xF);
    stackTop -= 32; // aligned

    uintptr_t* nstack = (uintptr_t*)(stackTop);

    nstack[0] = 0;
    nstack[1] = (uintptr_t)func;
    nstack[2] = (uintptr_t)data;
    nstack[3] = (uintptr_t)stack;


    // split
    int loaded = saveState();

    if (loaded!=-1) {
        struct fiberState *res = NULL;
        res = &fibers[loaded];

        res->rsp = stackTop;
        res->rbp = 0;
        res->rip = (uintptr_t)&fiberspawntrampolinelabel;

        return; // created and left
    }

    // trampoline
    asm volatile(
        // now on new stack
        // entry point
        ".global fiberspawntrampolinelabel;"
        "fiberspawntrampolinelabel:;"
        // do the task
        "mov  8(%%rsp), %%rax;"
        "mov 16(%%rsp), %%rdi;"
        "call *%%rax;"

        //free staCK
        "mov 24(%%rsp), %%rdi;"
        "call FiberDie;"

        //yield
        "call getState;"
        "ud2"

        :: "r" (stackTop)
        : "memory", "rax", "rdi"
        );

}




// Останавливает текущий fiber, помещает его в конце очереди исполнения
// Запускает первый fiber из очереди исполнения
__attribute__((no_sanitize("address")))
void FiberYield() {
    while (toDelSize) {
        free(toDelete[--toDelSize]);
    }
    int loaded = saveState();
    if (loaded!=-1) {
        getState();
        __builtin_unreachable();
    } else {
        return;
    }
}

// Возвращает 1, если текущий fiber единственный, иначе 0
int FiberTryJoin() {
    if (fiberSize==0) {
        return 1;
    }
    return 0;
}









































