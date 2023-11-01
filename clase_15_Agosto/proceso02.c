/****************************************************************
 * Fecha: 15 - Agosto - 2023
 * Autor: Juan Clavijo
 * Tema: Ejercicio 3
 * Objetivo: 
 ***************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//este codigo hace algo muy parecido al de procesos01.c, solamente es mas resumido

int main(int argc, char *argv[]){
	int pID = fork();
	if (pID>0){ //se verifica que el proceso creado tenga un PID valido
		printf("proceso padre \n");
	}
	else if (pID == 0) //se verifica que se hallan creado los procesos hijo y se imprime la verifiacion
	{
		printf(" proceso hijo\n");

	}
	else{
		printf("fork() devuelve un valor, 0, por lo que la llamada al sist. falló\n"); //se tiene un mensaje si el sistema falla
	}

	 printf("\n\n Imprimiendo... \n");
	 for(int i = 0; i<5;i++){
		 printf(" %d", i);
	 }
	 printf("\n fin \n");


	return 0;
}

