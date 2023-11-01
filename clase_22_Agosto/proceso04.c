/****************************************************************
 * Fecha: 22 - Agosto - 2023
 * Autor: Juan Clavijo
 * Tema: pipe() 
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
	int fd[2]; //creamos el arreglo para el inicio y fianl del pipe
	int nbytes; 
	pid_t childpid;
	char frase_enviada[] = "Hola desde entrada a pipe\n"; //la frase que voy a enviar
	char buffer[80]; //creamos el buffer de lectura

	pipe(fd); //se crea el pipe que vamos a usar

	if((childpid = fork())==-1){ //si hay un error con el fork(), que creo aca, salgo del programa
		perror("fork"); 
		exit(1);
	}

	if(childpid ==0){ //si estoy dentro de un proceso hijo del fork, ejecute lo de adentro
			close(fd[0]); //cierro mi inicio del pipe
			write(fd[1],frase_enviada,(strlen(frase_enviada)+1)); //escribo el mensaje en mi salida del pipe y envio el tamaño 
			exit(0);
        } else { //entro al padre
	 close(fd[1]); //cierro la salida
	 nbytes = read(fd[0],buffer,sizeof(buffer)); //leo el buffer de lectura
	 printf("oracion recibida: %s\n",buffer); //imprimo lo que haya en el buffer
	}

	return 0;
}

