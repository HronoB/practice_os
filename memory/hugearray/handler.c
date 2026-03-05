#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <sys/mman.h>

extern size_t PAGE_SIZE;
extern double* SQRTS;

void CalculateSq(double* sqrt_pos, int start, int n) {
    for (int i = 0; i < n; i++) {
        sqrt_pos[i] = sqrt((double)(start + i));
    }
}

void HandleSigsegv(int sig, siginfo_t* siginfo, void* ctx) {
    static void* prevPage = NULL;

    void* req_addr = siginfo->si_addr;

    if (sig != SIGSEGV) {
        return;
    }

    if (prevPage!=NULL && (req_addr>=(void*)SQRTS)) {
        munmap(prevPage, PAGE_SIZE);
    }

    //void *mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd, __off_t __offset)

    char* page_start = NULL + ((size_t) req_addr)/PAGE_SIZE * PAGE_SIZE;

    void* res = mmap(page_start, PAGE_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    prevPage = page_start;

    if (res==MAP_FAILED) {
        return;
    }


    CalculateSq((double*) page_start,
        (size_t) (page_start-(char*)SQRTS) / sizeof(double) ,
        PAGE_SIZE/sizeof(double));




    return;
}


























