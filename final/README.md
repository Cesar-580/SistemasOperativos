---
Title : "Trabajo final sistemas operativos"
Date : "27/06/2023"
Author : "Cesar Augusto Ospina Muñoz"
Email : "caospinamu@unal.edu.co"
---

# Planteamiento

Escriba un programa en C que realice la traducción de direcciones de memoria en un sistema que
tiene un espacio virtual de direccionamiento de 32 bits con páginas de 4 KB. El programa debe leer
de manera indefinida y hasta que el usuario pulse la letra ‘s’, una dirección virtual en decimal y
mostrar: 

- (a) el número de página
- (b) el desplazamiento dentro de la página
- (c) el tiempo que tomó la traducción
- (d) si la traducción produce un TLB Hit o un TLB Miss. 
 
Para simular el TLB debe usar memoria en el segmento heap de su programa. Usted define el tamaño y estructura del TLB como
caché de las traducciones. Implemente toda la lógica de la traducción usando el TLB como caché, de
acuerdo con los conceptos explicados en clase.

# Solución

Para realazar el código esperado se necesitan explorar los conceptos de:

## Traducción

Para la traducción se aplicará el método más sencillo que consta de, dada una dirección dividirla entre el tamaño la página y el desplazamiento co


```c
// Import libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

// Gobal variables
#define PAGE_SIZE 4096
#define TLB_SIZE 16

// function main
int main(){
    int virtual_address;
    int page_number;
    int offset;

    // variables to measure time
    clock_t start_time, end_time; 

    char InPut;

    do{
        printf("Enter a virtual address: ");
        scanf("%c", &InPut);

        // convert the input to lower case
        InPut = tolower(InPut);

        if(InPut == 's'){
            break;
        }

        // if not s, then continue with the program and convert the input to int

        virtual_address = atoi(&InPut);

        // start time
        start_time = clock();


        // Logic to translate the address


        // end time
        end_time = clock();





    }while (InPut != 's');

}
```
