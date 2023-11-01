/****************************************************************
 * Fecha: 15 - Agosto - 2023
 * Autor: Juan Clavijo
 * Tema: Ejercicio 2
 * Objetivo: 
 ***************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
	int pID = fork(); //se usa fork para crear procesos padres y procesos hjios
	if (pID>0){  //se verifica que el PID  es mayor a 0, o sea que si se crearon los procesos
		printf("fork() devuelve un valor, este es el proceso padre con ID: %d \n",getpid()); 
	} 
	else if (pID == 0) //se imprimen los PID de los procesos hijos
	{
		printf("fork() devuelve un valor, este es el proceso hijo recien creado con ID: %d \n",getpid());
		printf("fork() devuelve un valor, este es el proceso hijo, tiene el ID: %d \n",getpid());

	}
	else{ //si el sistema falla, se avisa al usr
		printf("fork() devuelve un valor, 0, por lo que la llamada al sist. falló\n");
	}

	 printf("\n\n esta es una unica impresion si la llama all sist. ha tenido exito");
	 printf("tanto el proceso padre como el proceso hijo se han ejecutado simultaneamente\n");
	 printf("y esta impresion aparecerá 2 veces\n\n");


	return 0;
}

