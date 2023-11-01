/****************************************************************
 * Fecha: 22 - Agosto - 2023
 * Autor: Juan Clavijo
 * Tema: fork() 
 * Objetivo: 
 ***************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
	int pID1 = fork();//creo un proceso fork que llamo pID1 
	int pID2 = fork();//creo un proceso fork que llamo pID2
	//De acuerdo al pID accedemos al fork
	if(pID1 ==0){
		if (pID2==0){
			printf("Proceso Y\n"); //se accede al proceso hijo 
		} else {
			printf("Proceso x\n"); //se imprime porque estamos en el proceso x, con pID1 0 y pID2 0, es un hijo
		}
	} else {
		if(pID2==0){
			printf("Proceso z\n"); //se imprime porque creamos el proceso Z
		} else {
			printf("proceso padre\n"); //se imprime porque estamos en el proceso abuelo
		}

	}
	//Se espera por finalización de todos los hilos
	while(wait(NULL) != -1 || errno != ECHILD){ //espera latente para el cierre de todos los hilos
	       printf("A la espera del cierre de todos los hilos\n");
	}	       
	return 0;
}

