/*******************************************************
*Autor: Juan Sebastian Clavijo Martinez
*Fecha: 17 octubre 2023
*Objetivo: Modulo de funciones 
*******************************************************/

#include "modulo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//Función reserva el espacio de memoria
 size_t **initMatriz(size_t N){
     size_t **entregaMM;
     entregaMM = (size_t **) calloc(N, sizeof(size_t));
     for(size_t i = 0; i<N; i++){
         entregaMM[i] = (size_t *) malloc (N * sizeof(size_t));
     }
     for(size_t i=0; i<N;++i){
          for(size_t j=0; j<N;++j){
              entregaMM[i][j] = (size_t) rand()%100 + 1;
          }
      }
     return entregaMM;
 }

 //Función para impresión de la matriz dinamica resultante
 void imprimirMM(size_t **mm, size_t N){
     for(size_t i=0; i<N;++i){
         for(size_t j=0; j<N;++j){
             printf("%zu ",mm[i][j]);
         }
         printf("\n");
     }
 }
