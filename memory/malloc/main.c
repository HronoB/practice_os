#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef uint64_t pte_t;   // Page Table Entry
typedef uint64_t pde_t;   // Page Directory Entry
typedef uint64_t pdpte_t; // Page Directory Pointer Table Entry

// Нужно реализовать страничное преобразование аналогичное структуре в x86.
// Путь нахождения физического адреса по виртуальному. Вспомните что является индексом в каждой из этих таблиц.
// PDPTE -> PDE -> PTE -> phys_addr

// Сбрасывает tlb кеш для данного виртульного адреса.
// Функция является пустой заглушкой, реализовывать ее не надо.
void ClearTlb(void* virt_addr) {
    (void*)virt_addr;
}

// Аллокатор страниц, возвращает "физический" адрес страницы в памяти.
void* AllocPage() {
    void* ptr = calloc(2 << 12, 1);
    if (ptr == NULL) {
        return NULL;
    }
    // Смещение на одну страницу вперед, чтобы не получить NULL в результате успешного вызова.
    return (void*)((uint64_t)ptr & ~0xFFF) + 0x1000;
}

// Вставить `phys_addr` с флагами `perm`, по адресу `virt_addr`
// Требуется так же выставить флаг PRESENT для такой страницы (выставляется в нулевом бите записи).
// Если во время вставки требуется создать дополнительные страницы,
// то для них нужно выставить флаги PRESENT и WRITE, нулевой и первый биты соответственно.
// Если запись по адресу `virt_addr` уже существует, то вызовите для этого виртуального адреса функцию `ClearTlb`.
// Функция должна вернуть 0 в случае успеха и -1 в противном случае (подумайте из-за чего может произойти ошибка).

const uint64_t F_Present = 0x1<<60;
const uint64_t F_Writable = 0x1<<61;
const uint64_t Mask = ~(F_Present | F_Writable);

int Insert(pdpte_t* root, void* virt_addr, void* phys_addr, int perm) {

    uint64_t* current_table = root;
    int level = 0;

    for (int i = 12+9*3; i >= 12+9; i-=9) {
        int idx = ((uint64_t)virt_addr >> i) & 0x1FF & Mask;

        if (!(current_table[idx] & F_Present)) {
            uint64_t* new_table = (uint64_t*)AllocPage();
            if (!new_table) {
                return -1;
            }
            current_table[idx] = ((uint64_t)new_table | F_Present | F_Writable)& Mask;
        }

        current_table = (uint64_t*)(current_table[idx] & ~0xFFF & Mask);
        level++;
    }

    uint64_t pt_idx = ((uint64_t)virt_addr >> 12) & 0x1FF & Mask;

    if (current_table[pt_idx] & F_Present) {
        ClearTlb(virt_addr);
    }

    current_table[pt_idx] = (uint64_t)phys_addr | F_Present | perm;

    return 0;
}




void* Lookup(pdpte_t* root, void* virt_addr) {

    uint64_t pml4_index = ((uint64_t)virt_addr >> 39) & 0x1FF;
    uint64_t pdpt_index = ((uint64_t)virt_addr >> 30) & 0x1FF;
    uint64_t pd_index   = ((uint64_t)virt_addr >> 21) & 0x1FF;
    uint64_t pt_index   = ((uint64_t)virt_addr >> 12) & 0x1FF;
    uint64_t offset     = (uint64_t)virt_addr & 0xFFF;  // offset


    if (!(root[pml4_index] & F_Present)) return NULL;
    pdpte_t* pdpt = (pdpte_t*)(root[pml4_index] & ~0xFFF);

    if (!(pdpt[pdpt_index] & F_Present)) return NULL;
    pde_t* pd = (pde_t*)(pdpt[pdpt_index] & ~0xFFF);

    if (!(pd[pd_index] & F_Present)) return NULL;
    pte_t* pt = (pte_t*)(pd[pd_index] & ~0xFFF);

    if (!(pt[pt_index] & F_Present)) return NULL;
    uint64_t* res = (void*)((pt[pt_index] & ~0xFFF) + offset);

    return res;
}



// Протестируйте свой код
int main() {


    pdpte_t* root = (pdpte_t*)AllocPage();
    if (root == NULL) {
        printf("WRONG 1");
        return -1;
    }


    void* virt = (void*)0x11615771000;  // desired
    void* phys = (void*)0x712323000;    // real


    printf("Inserting des=%p, real=%p\n", virt, phys);
    int result = Insert(root, virt, phys, F_Present);
    if (result == 0) {
        printf("GOOD 1\n");
    } else {
        printf("WONG 2\n");
        return -1;
    }


    void* found = Lookup(root, virt);
    if (found != NULL) {
        printf("found %p\n", found);
        if (found == phys) {
            printf("FINEFINEFINE\n");
        } else {
            printf("WRONG 3\n");
        }
    } else {
        printf("WONG 4\n");
    }

    return 0;
}