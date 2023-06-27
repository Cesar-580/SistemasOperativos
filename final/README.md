---
Title : "Trabajo final sistemas operativos"
Date : "27/06/2023"
Author1 : "Cesar Augusto Ospina Muñoz"
Email1 : "caospinamu@unal.edu.co"
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

