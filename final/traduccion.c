#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define PAGE_SIZE 4096
#define TLB_SIZE 16

int numberUses;

struct TBL_struct {
    int physical_address;
    int page_number;
    int displacement;
    bool valid;
};

struct TlbResult {
    bool hit;
    int page_number;
    int displacement;
};

struct TBL_struct* tbl;

void initialize_tlb() {
    tbl = (struct TBL_struct*)malloc(TLB_SIZE * sizeof(struct TBL_struct));
    for (int i = 0; i < TLB_SIZE; i++) {
        tbl[i].valid = false;
    }
}

struct TlbResult tlb_verify(struct TBL_struct* tbl, int physical_address) {
    struct TlbResult result = { false, 0, 0 };

    for (int i = 0; i < TLB_SIZE; i++) {
        // printf("Las direcciones son: %d", tbl[i].physical_address);
        // printf("Las pagina es son: %d", tbl[i].page_number);
        // printf("El desplaza son: %d\n ", tbl[i].displacement);
        if (tbl[i].valid && tbl[i].physical_address == physical_address) {
            result.hit = true;
            result.page_number = tbl[i].page_number;
            result.displacement = tbl[i].displacement;
            break;
        }
    }

    return result;
}

struct TlbResult tlb_translateAddress(struct TBL_struct* tbl, int physical_address) {
    int page_number = physical_address / PAGE_SIZE;
    int displacement = physical_address % PAGE_SIZE;

    for (int i = 0; i < TLB_SIZE; i++) {
        if (!tbl[i].valid) {
            tbl[i].physical_address = physical_address;
            tbl[i].page_number = page_number;
            tbl[i].displacement = displacement;
            tbl[i].valid = true;
            break;
        }
    }

    struct TlbResult result = { false, page_number, displacement };
    return result;
}

int main() {
    int virtual_address;
    int page_number;
    int displacement;

    clock_t start_time, end_time;

    
    char input[10];

    initialize_tlb();


    do {
        printf("\nEnter a virtual address: ");
        scanf("%s", input);

        char inputLowerCase = tolower(input[0]);

        if (inputLowerCase == 's') {
            break;
        }

        virtual_address = atoi(input);

        start_time = clock();

        

        struct TlbResult tbl_check = tlb_verify(tbl, virtual_address);

        bool hit = tbl_check.hit;

        if (!hit) {
            struct TlbResult translate = tlb_translateAddress(tbl, virtual_address);
            page_number = translate.page_number;
            displacement = translate.displacement;
        } else {
            page_number = tbl_check.page_number;
            displacement = tbl_check.displacement;
        }

        end_time = clock();

        printf("Número de página: %d\n", page_number);
        printf("Desplazamiento dentro de la página: %d\n", displacement);
        printf("Tiempo de traducción: %.6f segundos\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
        printf("Resultado de TLB: %s\n", hit ? "TLB Hit" : "TLB Miss");


    } while (tolower(input[0]) != 's');
    free(tbl);

    return 0;
}
