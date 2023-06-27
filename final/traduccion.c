#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define PAGE_SIZE 4096
#define TLB_SIZE 16



int usedSlot = 0;

struct TBL_struct {
    int physical_address;
    int page_number;
    int displacement;
    int numberUses;
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
        if (tbl[i].valid && tbl[i].physical_address == physical_address) {
            result.hit = true;
            result.page_number = tbl[i].page_number;
            result.displacement = tbl[i].displacement;
            tbl[i].numberUses += 1;
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
            tbl[i].numberUses = 1;
            tbl[i].valid = true;
            break;
        }
    }

    struct TlbResult result = { false, page_number, displacement };
    return result;
}


// struct TlbResult tlb_update(unsigned int page_number, unsigned int frame_number) {
//     // Replace the lastest input 
//     int oldest_entry_index = 0;
//     time_t oldest_entry_time = tlb[0].valid ? tlb[0].frame_number : 0;

//     for (int i = 1; i < TLB_SIZE; i++) {
//         if (!tlb[i].valid) {
//             oldest_entry_index = i;
//             break;
//         }
//         if (tlb[i].frame_number < oldest_entry_time) {
//             oldest_entry_index = i;
//             oldest_entry_time = tlb[i].frame_number;
//         }
//     }

//     // Actualizar la entrada del TLB
//     tlb[oldest_entry_index].valid = true;
//     tlb[oldest_entry_index].page_number = page_number;
//     tlb[oldest_entry_index].frame_number = frame_number;
// }



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
