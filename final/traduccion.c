// Import libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

// Global variables
#define PAGE_SIZE 4096
#define TLB_SIZE 16

// Define the structure for the memory
struct TBL_struct {
    int physical_address;
    int page_number;
    int displacement;
    int numberUses;
    bool valid;
};

// Structure to hold the result of the TLB verification
struct TlbResult {
    bool hit;
    int page_numberr;
    int displacementt;
};

// Function to determine if the direccion is in the memory
struct TlbResult tlb_verify(struct TBL_struct* tbl, int physical_address) {
    struct TlbResult result = {false, 0, 0};
    printf("Si me llama para hacer la verificacion, la direccion fisica es %d\n", physical_address);
    for (int i = 0; i < TLB_SIZE; i++) {
        printf("La direccion fisica es %d\n", tbl[i].physical_address);
        if (tbl[i].valid && tbl[i].physical_address == physical_address) {
            printf("Encontre una que sirve");
            result.hit = true;
            result.page_numberr = tbl[i].page_number;
            result.displacementt = tbl[i].displacement;
            break;
        }
    }
    return result;
}

// Function to translate the address
struct TlbResult tlb_tranlateAddress(struct TBL_struct* tbl, int physical_address){
    printf("Si me llama para hacer la traduccion\n");
    // Calculate the page
    // printf(" Se divide the physical address %d by %d\n", physical_address, TLB_SIZE);
    int page_number = physical_address / PAGE_SIZE;
    // printf("Page number %d\n", page_number);

    // Calculate the displacement
    int displacement = physical_address % PAGE_SIZE;
    // printf("Displacement %d\n", displacement);

    // insert the address into the memory
    for (int i = 0; i < TLB_SIZE; i++) {
        if (!tbl[i].valid){
            tbl[i].physical_address = physical_address;
            tbl[i].page_number = page_number;
            tbl[i].displacement = displacement;
            tbl[i].numberUses = 0;
            tbl[i].valid = true;
        }
    }
    struct TlbResult result = {false, page_number, displacement};
    return result;
}




// Function main
int main() {
    int virtual_address;
    int page_number;
    int offset;

    // Variables to measure time
    clock_t start_time, end_time;

    // Define the type of TLB
    struct TBL_struct* tbl;

    // Input
    char input[10]; // Buffer to store the user input

    do {
        printf("Enter a virtual address: ");
        scanf("%s", input);

        // Convert the input to lower case
        char inputLowerCase = tolower(input[0]);

        if (inputLowerCase == 's') {
            break;
        }

        // If not 's', then continue with the program and convert the input to an integer
        virtual_address = atoi(input);

        // Start time
        start_time = clock();

        // Logic to translate the addressstruct TBL_struct* tbl, int physical_address) {

        // Create a malloc memory object
        tbl = (struct TBL_struct*)malloc(TLB_SIZE * sizeof(struct TBL_struct));

        // Initialize the memory object
        for (int i = 0; i < TLB_SIZE; i++) {
            tbl[i].valid = false;
        }

        // Verify if the directory exists in the memory object
        struct TlbResult tbl_check = tlb_verify(tbl, virtual_address);

        // Assign the returned values to separate variables
        bool hit = tbl_check.hit;
        int page_number;
        int displacement;

        // printf("Hasta aca voy bien,  tenemos un hit: %d, las paginas es: %d y el desplazamiento es: %d ", hit, page_number,displacement);
        // if (!hit) make the tranlate
        if (!hit) {
            struct TlbResult translate = tlb_tranlateAddress(tbl, virtual_address);
            page_number = translate.page_numberr;
            displacement = translate.displacementt;
            // printf("Se hizo la traduccion,  tenemos un hit: %d, las paginas es: %d y el desplazamiento es: %d ", hit, page_number,displacement);
        }else {
            page_number = tbl_check.page_numberr;
            displacement = tbl_check.displacementt;
        }

        // End time
        end_time = clock();

        printf("Número de página: %d\n", page_number);
        printf("Desplazamiento dentro de la página: %d\n", displacement);
        printf("Tiempo de traducción: %.6f segundos\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
        printf("Resultado de TLB: %s\n", hit ? "TLB Hit" : "TLB Miss");

    } while (tolower(input[0]) != 's');

    // Clean up the allocated memory
    free(tbl);

    return 0;
}
