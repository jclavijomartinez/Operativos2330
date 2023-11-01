/****************************************************************
 * Fecha: 15 - Agosto - 2023
 * Autor: Juan Clavijo
 * Tema: Ejercicio 1 
 * Objetivo: 
 ***************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
	int x;
	for (x=0;x<2;x++){ //ponemos 2 para que se impriman pocas lineas y poder ver el PID
		fork(); //aca se crean los procesos padre e hijo, por eso se imprimen 6 lineas, es exponencial
		printf("The process is PID : %d \n",getpid());
	}
	return 0;
}

