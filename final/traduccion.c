// se importan las librerias
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

// Variables usadas en el script
#define PAGE_SIZE 4096 // Tamaño de la pagina
#define TLB_SIZE 16    // Tamaño del TLB


// Varaible usada para contar la cantidad de elementos en el TLB
int usedSlot = 0;

// Estructura del TLB
struct TBL_struct {
    int virtual_address;   // Dirección virtual
    int page_number;       // Número de la pagina
    int displacement;      // Desplazamiento en la pagina
    int numberUses;        // Veces usada
    bool valid;            // Si esta ocupada o no
};

// Estructura usada para mostrar la información del TLB
struct TlbResult {
    bool hit;               // True si hay hit, False de lo contrario
    int page_number;        // Número de la pagina
    int displacement;       // Desplazamiento en la pagina
};

// Iniciar el TLB
struct TBL_struct* tbl;

/*
Función: initialize_tlb
-----------------------
Inicializa la estructura tbl de la TLB asignando memoria dinámica y estableciendo los valores iniciales.
 
Comportamiento:
  - Asigna memoria dinámica para tbl utilizando malloc, con un tamaño de TLB_SIZE * sizeof(struct TBL_struct).
  - Recorre cada elemento en tbl (hasta TLB_SIZE) y establece el campo valid en false.
 */
void initialize_tlb() {
    tbl = (struct TBL_struct*)malloc(TLB_SIZE * sizeof(struct TBL_struct));
    for (int i = 0; i < TLB_SIZE; i++) {
        tbl[i].valid = false;
    }
}

/**
Función: tlb_verify
--------------------
Verifica si un elemento en la estructura tbl coincide con el valor del virtual_address proporcionado.
Si se encuentra una coincidencia, se actualiza el resultado de búsqueda y se incrementa el número de usos del elemento.
 Parámetros:
  - tbl: Puntero a la estructura TBL_struct que representa la tabla de búsqueda de traducción (TLB).
  - virtual_address: Valor entero utilizado para buscar coincidencias en el campo virtual_address de los elementos de tbl.
 Retorna:
  Estructura TlbResult con el resultado de la búsqueda:
    - hit: Valor booleano indicando si se encontró una coincidencia (true) o no (false).
    - page_number: Número de página correspondiente al virtual_address encontrado.
    - displacement: Desplazamiento correspondiente al virtual_address encontrado.
 */

struct TlbResult tlb_verify(struct TBL_struct* tbl, int virtual_address) {
    struct TlbResult result = { false, 0, 0 };

    for (int i = 0; i < TLB_SIZE; i++) {
        if (tbl[i].valid && tbl[i].virtual_address == virtual_address) {
            result.hit = true;
            result.page_number = tbl[i].page_number;
            result.displacement = tbl[i].displacement;
            tbl[i].numberUses += 1;
            break;
        }
    }

    return result;
}

/**
Función: tbl_reemplazo
-------------------
Esta función realiza las veces de la politica de reemplazo de página, en este caso se tendrá el reemplazo de página por la menos usada.

Parámetros:
  - tbl: Puntero a la estructura TBL_struct que representa la tabla de búsqueda de traducción (TLB).
  - virtual_address: Valor entero del nuevo virtual_address a actualizar en tbl.
  - page_number: Número de página correspondiente al nuevo virtual_address.
  - displacement: Desplazamiento correspondiente al nuevo virtual_address.
Retorna:
  Estructura TlbResult vacía. (La función no tiene una declaración de retorno explícita).
Comportamiento:
  - Busca el elemento en tbl con el menor número de usos (numberUses).
  - Actualiza dicho elemento con los nuevos valores: virtual_address, page_number, displacement.
  - Establece numberUses en 1 y valid en true para el elemento actualizado.
 */

struct TlbResult tbl_reemplazo(struct TBL_struct* tbl, int virtual_address, int page_number, int displacement) {
    int minIndex = -1;Good bye!
    int minUs = 1000000;
    // find the minimum number of elements in the TBL
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tbl[i].valid && tbl[i].numberUses < minUs) {
            minUs = tbl[i].numberUses;
            minIndex = i;
        }
    }
    tbl[minIndex].virtual_address = virtual_address;
    tbl[minIndex].page_number = page_number;
    tbl[minIndex].displacement = displacement;
    tbl[minIndex].numberUses = 1;
    tbl[minIndex].valid = true;
    usedSlot -= 1;
}

/*
Función: tlb_translateAddress
-----------------------------
Traduce una dirección virtual en la TLB y obtiene el número de página y desplazamiento correspondientes.
Si el TBL no tiene capacidad se llama a la función tbl_reemplazo para aplicar la politica de reemplazo del menos usado.

Parámetros:
  - tbl: Puntero a la estructura TBL_struct que representa la tabla de búsqueda de traducción (TLB).
  - virtual_address: Valor entero de la dirección virtual a traducir.
Retorna:
  Estructura TlbResult con el resultado de la traducción:
    - hit: Valor booleano que indica si se encontró una coincidencia en la TLB (true) o no (false).
    - page_number: Número de página correspondiente a la dirección virtual traducida.
    - displacement: Desplazamiento correspondiente a la dirección virtual traducida.
Comportamiento:
  - Calcula el número de página y el desplazamiento a partir de la dirección virtual.
  - Si el número de ranuras utilizadas (usedSlot) en la TLB es mayor que el tamaño máximo (TLB_SIZE):
    - Llama a la función tbl_reemplazo para actualizar la TLB con los nuevos valores y actualiza el resultado.
  - De lo contrario:
    - Recorre la TLB para encontrar una ranura no válida.
    - Si se encuentra una ranura no válida:
      - Actualiza la ranura con los valores de la dirección virtual, número de página y desplazamiento.
      - Establece el número de usos en 1 y la validez en true.
      - Finaliza el bucle.
    - Actualiza el resultado con la información de la dirección virtual traducida y marca como no encontrada (hit = false).
    - Incrementa el número de ranuras utilizadas (usedSlot).
  - Devuelve el resultado de la traducción.
 */
struct TlbResult tlb_translateAddress(struct TBL_struct* tbl, int virtual_address) {
    struct TlbResult result = { false, 0, 0 };
    int page_number = virtual_address / PAGE_SIZE;
    int displacement = virtual_address % PAGE_SIZE;

    if (usedSlot > TLB_SIZE){
        result = tbl_reemplazo(tbl, virtual_address, page_number, displacement);
    } else {
        for (int i = 0; i < TLB_SIZE; i++) {
            if (!tbl[i].valid) {
    
                tbl[i].virtual_address = virtual_address;
                tbl[i].page_number = page_number;
                tbl[i].displacement = displacement;
                tbl[i].numberUses = 1;
                tbl[i].valid = true;
    
                break;
            }
        }

        result.hit = false;
        result.page_number = page_number;
        result.displacement = displacement;
        usedSlot += 1;
    }
    return result;
}


int main() {
    // Definición de las variables 
    int virtual_addressIN;
    int page_number;
    int displacement;
    clock_t start_time, end_time;

    
    char input[10];
    // Llamado a iniciar el TLB
    initialize_tlb();


    do {
        printf("\nIngrese dirección virtual: ");
        scanf("%s", input);

        // Conversion de la entrada
        char inputLowerCase = tolower(input[0]);

        // Verificacion del caso de salida
        if (inputLowerCase == 's') {
            printf("Good bye!");
            break;
        }

        // Conversion de la entrada a entero
        virtual_addressIN = atoi(input);
        // Inicio del contador de tiempo
        start_time = clock();

        
        // Se llama a la función tbl_check para conocer si ya se ha traducido la direccion
        struct TlbResult tbl_check = tlb_verify(tbl, virtual_addressIN);
        // Se lee el resultado hit o miss
        bool hit = tbl_check.hit;

        //  Se decide si hay que hacer la traducción o se lee desde el TLB
        if (!hit) {
            struct TlbResult translate = tlb_translateAddress(tbl, virtual_addressIN);
            page_number = translate.page_number;
            displacement = translate.displacement;
        } else {
            page_number = tbl_check.page_number;
            displacement = tbl_check.displacement;
        }
        // Se para el tiempo
        end_time = clock();

        // Impresion de los resultados
        printf("Número de página: %d\n", page_number);
        printf("Desplazamiento dentro de la página: %d\n", displacement);
        printf("Tiempo de traducción: %.6f segundos\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
        printf("Resultado de TLB: %s\n", hit ? "TLB Hit" : "TLB Miss");


    } while (tolower(input[0]) != 's');
    free(tbl);

    return 0;
}
