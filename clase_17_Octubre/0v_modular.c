/*******************************************************
* Autor: Juan Sebastian Clavijo Maritnez
* Fecha: 17 octubre 2023
* Objetivo: Programa principal a ser modularizado
* Crear un programa que crea una matriz dinamica, presentada por pantalla
*******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]){
size_t **matriz;
size_t dim =5;
srand(time(NULL));

//Se reserva el espacio de memoria
matriz = (size_t **) calloc(dim, sizeof(size_t));
for(size_t i = 0; i<dim; i++)
matriz[i] = (size_t *) malloc (dim * sizeof(size_t));

//Inicialización de la matriz mediante numeros aleatorios en un rango de 1 a 100
for(size_t i=0; i<dim;++i){
for(size_t j=0; j<dim;++j){
matriz[i][j] = (size_t) rand()%100 + 1;
}
}

//Se hace impresión de la matriz dinamica resultante
for(size_t i=0; i<dim;++i){
for(size_t j=0; j<dim;++j){
printf("%zu ",matriz[i][j]);
}
printf("\n");
}

//Se libera la memoria reservada
for(size_t i=0; i<dim;++i)
free(matriz[i]);
free(matriz);

return 0;
}

