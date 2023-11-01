/*******************************************************
*Autor: Juan Sebastian Clavijo Martinez
*Fecha: 17 octubre 2023
*Objetivo: Programa principal a ser modularizado
 Crear un programa que crea una matriz dinamica, presentada por pantalla
*******************************************************/

#include "modulo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]){
    size_t **matriz;
    size_t dim =5;
    srand(time(NULL));

    //se llama a la creacion de matriz, que llena tambien
    matriz=initMatriz(dim);
    //impresion de matriz
    imprimirMM(matriz,dim);

    //Se libera la memoria reservada
    for(size_t i=0; i<dim;++i){
        free(matriz[i]);
    }
    free(matriz);
    return 0;
}
