/*********
 * SISTEMAS OPERATIVOS
 * FECHA: 01-08-2023
 * AUTOR: JUAN CLAVIJO
 * EJERCICIO 1
 * ****/
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int variable = 123;
	int *ptr = &variable;
	printf("valor de la var: %d \n",variable);
	printf("direccion: %p \n", &variable);
	printf("direccion del ptr: %p \n",(void *)ptr);
	printf("Valor de la var: %i \n",*ptr);
	return 0;
}
