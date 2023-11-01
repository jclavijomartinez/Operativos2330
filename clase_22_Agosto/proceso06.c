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
	int arreglo[] = {1,2,3,4,1,2,7}; 
	int arrTam = sizeof(arreglo)/sizeof(int);
	int fd[2]; 
	int inicio,final;

	if(pipe(fd) == -1){
		return 1; //si hay un error sera el 1
	}

	int pid = fork();
	
	if(pid == -1){
                return 2; //si hay un error sera el 2
        }

	if( pid == 0){
		inicio = 0;
		final = arrTam/2; //final va hasta la mitad porque quiero 2 procesos
	} else {
		inicio= arrTam/2;
		final = arrTam;
	}
	int suma=0;
	for (int i =inicio;i<final;i++){
		suma += arreglo[i]; //agregamos a la suma parcial lo que haya en el arreglo
	}
	printf("calculo de la suma parcial %d\n", suma);
	
	//Se pasa los mensajes entre los procesos
	if(pid==0){
		close(fd[0]);
		write(fd[1],&suma,sizeof(suma)); //escribimos en el final del pipe la info de suma
		close(fd[1]);
	} else {
		int sumaHijo; //se recibe el valor del hijo
		close(fd[1]);
                read(fd[0],&sumaHijo,sizeof(sumaHijo)); //leemos del pipe lo que haya en sumaHijo
                close(fd[0]);
		int sumaTotal = sumaHijo+suma;
		printf("suma total %d\n", sumaTotal);
	}
	
	return 0;
}

