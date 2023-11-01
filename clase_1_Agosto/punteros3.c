/*********
 * SISTEMAS OPERATIVOS
 * FECHA: 01-08-2023
 * AUTOR: JUAN CLAVIJO
 * EJERCICIO 3
 * ****/
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int valor1,valor2;
	int *ptrv1,*ptrv2;
	ptrv1=&valor1;
	ptrv2=&valor2;
	printf("\n ingrese dos valores a sumar: \n");
	scanf("%d %d",&valor1,&valor2);
	printf("\n la suma es: %d\n",*ptrv1+*ptrv2);
	return 0;
}
