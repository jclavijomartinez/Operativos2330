/*********
 * SISTEMAS OPERATIVOS
 * FECHA: 08-08-2023
 * AUTOR: JUAN CLAVIJO
 * EJERCICIO 1 - punteros dobles
 * ****/
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int variable = 480;
	int *ptr = &variable;
	int **doubleptr = &ptr;
	printf("valor de la var: %d \n",variable);
	printf("valor del puntero: %d \n",*ptr);
	printf("valor del puntero doble: %d \n",**doubleptr);
	printf("direccion de la var: %p \n", &variable);
	printf("direccion del ptr: %p \n",&ptr);
	printf("Valor en el puntero: %p \n",ptr);
	printf("direccion del puntero doble %p \n",*doubleptr);
	printf("valor del puntero doble %p \n",&doubleptr);
	return 0;
}
