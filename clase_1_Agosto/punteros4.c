/*********
 * SISTEMAS OPERATIVOS
 * FECHA: 01-08-2023
 * AUTOR: JUAN CLAVIJO
 * EJERCICIO 4
 * ****/
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int vector[] = {40,80,120,230};
	int *ptrv1;
	ptrv1 = vector;

	printf("\n impresion por indices\n");
	for(int i=0;i<3;i++){
		printf("por indices el valor del vect{%d}=%d\n",i,ptrv1[i]);
	}

	printf("\n impresion por sucesor\n");
        for(int i=0;i<4;i++){
                printf("por indices el valor del vect{%d}=%d\n",i,*ptrv1);
		ptrv1++;
        }
	return 0;
}

