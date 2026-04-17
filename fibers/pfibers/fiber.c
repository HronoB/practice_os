#include "fiber.h"

#include <assert.h>
#include <signal.h>
#include <stdlib.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <ucontext.h>
#include <sys/ucontext.h>

#include <unistd.h>

#define TODO()

#define FIBER_STACK_SIZE (1024 * 1024)
#define FIBER_QUE_CAP 500

struct fiberState {
    ucontext_t ctx;
    void* stack_ptr;
    int active;
};

struct fiberState fibers[FIBER_QUE_CAP];

int fiberIx = 0;
int fiberSize = 1;

int queue[FIBER_QUE_CAP];
int q_head = 0, q_tail = 0;

void enqueue(int idx) {
    queue[q_tail] = idx;
    q_tail = (q_tail + 1) % FIBER_QUE_CAP;
}

int dequeue() {
    if (q_head == q_tail) {
        return -1;
    }

    int idx = queue[q_head];
    q_head = (q_head + 1) % FIBER_QUE_CAP;
    return idx;
}

void FiberDie() {
    fibers[fiberIx].active = 0;
    raise(SIGALRM);
    while (1) {
        pause();
    }
}

__attribute__((no_sanitize("address")))
void fiber_sched(int signum, siginfo_t *info, void *ncontext) {
    ucontext_t *ctx = ncontext;

    if (fibers[fiberIx].active) {
        memcpy(
            &fibers[fiberIx].ctx.uc_mcontext,
            &ctx->uc_mcontext,
            sizeof(mcontext_t)
        );
        enqueue(fiberIx);
    }

    int nextIx = dequeue();

    if (nextIx == -1) {
        if (fibers[fiberIx].active) {
            nextIx = fiberIx;
        } else {
            exit(0);
        }
    }

    fiberIx = nextIx;

    memcpy(
        &ctx->uc_mcontext,
        &fibers[fiberIx].ctx.uc_mcontext,
        sizeof(mcontext_t)
    );

    return;
}

__attribute__((no_sanitize("address")))
void FiberSpawn(void (*func)(void*), void* data) {
    int idx = fiberSize++;

    void* stack = malloc(FIBER_STACK_SIZE);
    fibers[idx].stack_ptr = stack;
    fibers[idx].active = 1;

    getcontext(&fibers[idx].ctx);

    fibers[idx].ctx.uc_stack.ss_sp = stack;
    fibers[idx].ctx.uc_stack.ss_size = FIBER_STACK_SIZE;
    fibers[idx].ctx.uc_link = NULL;

    makecontext(&fibers[idx].ctx, (void (*)(void))func, 1, data);

    enqueue(idx);
}

void FiberYield() {
    raise(SIGALRM);
}

int FiberTryJoin() {
    if (q_head == q_tail) {
        return 1;
    }
    return 0;
}

void FiberInit() {
    // Need to disable stdlib buffer to use printf from fibers
    setbuf(stdout, NULL);

    struct sigaction confs;
    memset(&confs, 0, sizeof(confs));

    confs.sa_sigaction = fiber_sched;
    confs.sa_flags = SA_SIGINFO | SA_NODEFER;

    sigemptyset(&confs.sa_mask);

    sigaction(SIGALRM, &confs, NULL);
    ualarm(10000, 10000);
}