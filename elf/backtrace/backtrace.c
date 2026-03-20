#include "backtrace.h"




#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>
#include <unistd.h>





char* AddrToName(void* addr) {
    static char* result;

    static char* path = "/proc/self/exe";
    static int fd = -1;
    static uint64_t size = 0;
    static void* data = NULL;

    if (fd==-1) {
        fd = open(path, O_RDONLY);
        size = lseek(fd, 0, SEEK_END);
        data = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    }


    Elf64_Ehdr* header = (Elf64_Ehdr*) data;

    Elf64_Shdr* sections = (Elf64_Shdr*) (data + header->e_shoff);

    Elf64_Shdr* symbol_table = NULL;
    Elf64_Shdr* names_table = NULL;

    for (int i = 0; i < header->e_shnum; i++) {
        if (sections[i].sh_type == SHT_SYMTAB) {
            symbol_table = &sections[i];
        }
    }

    names_table = &sections[symbol_table->sh_link];

    if (symbol_table==NULL) {
        printf("DEBUG symbol table not found\n");
        return NULL;
    }



    uint64_t number_symbols = symbol_table->sh_size / symbol_table->sh_entsize;
    Elf64_Sym* all_symbols = (Elf64_Sym*)((char*)data + symbol_table->sh_offset);

    Elf64_Sym* desired_symbol = NULL;
    for (int i = 0; i < number_symbols; i++) {
        Elf64_Addr function_start = all_symbols[i].st_value;
        Elf64_Addr function_end = function_start+all_symbols[i].st_size;

        if ((void*)function_start <= addr && addr < (void*)function_end) {
            desired_symbol = &all_symbols[i];
        }
    }

    if (desired_symbol==NULL) {
        printf("DEBUG desired symbol not found\n");
        exit(-1);
        return NULL;
    }

    result = data + names_table->sh_offset + desired_symbol->st_name;

    return result;
}


int Backtrace(void* backtrace[], int limit) {


    uint64_t *rbp = NULL;      // previous rbp : return addr
    asm volatile ("mov %%rbp, %0"
        : "=r"(rbp)
        :
        );

    //printf("BACKTRACE ADDR = %p\n", &Backtrace);


    int ix = 0;
    while (true) {
        if ((void*)*rbp==NULL+1 || (void*)*rbp==NULL) {
            break;
        }
        if (ix >= limit) {
            break;
        }


        //printf("debug rbp = %p:\n", (void*)*(rbp+1));
        backtrace[ix] = (void*)*(rbp+1);
        ix++;

        if ((uint64_t*)*rbp <= rbp) {
            break;
        }


        rbp = (uint64_t*)*rbp;


    }


    return ix;



    // Your code here
    return 0;
}










void PrintBt() {

    void* stack[256];
    int height = Backtrace(stack, 256);

    printf("PRINTING TRACE : %d calls\n", height);

    for (int i = 0; i < height; i++) {
        printf("\t #(%d): %s (%p)\n", i, AddrToName(stack[i]), stack[i]);
    }



    // Your code here
}





















