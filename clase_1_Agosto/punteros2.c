/*********
 * SISTEMAS OPERATIVOS
 * FECHA: 01-08-2023
 * AUTOR: JUAN CLAVIJO
 * EJERCICIO 2
 * ****/
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int *vectPtr[3];
	int p=40, q=80, r=120;
	vectPtr[0]=&p;
	vectPtr[1]=&q;
	vectPtr[2]=&r;
	printf("forma de acceso al vector: \n");
	for(int i=0;i<3;i++){
		printf("para la dir: %p \t el valor es %d \n",vectPtr[i],*vectPtr[i]);
	}
	return 0;
}

