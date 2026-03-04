#include "malloc.h"


void* malloc(size_t size) {
    size_t* res = mmap(NULL, size+sizeof(size_t),
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 0);
    *res = size+sizeof(size_t);
    return res+1;
}

void* realloc(void* ptr, size_t size) {
    size_t old_size = *((size_t*)ptr-1);
    size_t* res = mremap(((size_t*)ptr)-1, old_size, size+sizeof(size_t),
        MREMAP_MAYMOVE, NULL);
    *res = size+sizeof(size_t);
    return res+1;
}

void free(void* ptr) {
    munmap(((size_t*)ptr)-1, *(((size_t*)ptr)-1));
}























































