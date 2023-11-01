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
	int fd1[2]; //creamos el arreglo para el inicio y fianl del pipe
	int fd2[2];
	int nbytes; 
	pid_t p;
	char input_str00[100];
	char input_str01[100];


	printf("Ingrese la primera frase (enter para continuar): ");
	scanf("%s", input_str00);

	printf("Ingrese la segunda frase (enter para continuar): ");
        scanf("%s", input_str01);

	if((pipe(fd1)==-1)||(pipe(fd2)==-1)){ //tengo un caso de control de errores
		printf("Pipe Failed");
		return 1;
        }

	p = fork();

	if(p < 0){
                 printf("fork Failed");
                 return 1;
        }  

	else if(p>0){
		char concat_str[100];
		close(fd1[0]);
		write (fd1[1],input_str01,strlen(input_str01)+1); //escribo la informacion en input_str01
		close(fd1[1]);
		wait(NULL);
		close(fd2[1]);
		read(fd2[0],concat_str,100);
		printf("concatenated string: %s\n",concat_str); //aca imprimo la concatenacion de la pantalla
		close(fd2[0]);
	} else {
		close(fd1[1]);
		char concat_str[100];
                read (fd1[0],concat_str,100); //leo la info que viene desde mi pipe fd1
		int k = strlen(concat_str);
		int i;
		for (i=0;i<strlen(input_str00);i++){
			concat_str[k++] = input_str00[i]; //en el arreglo concat_str pongo la info que estaba en el arreglo input_str00
		}
                close(fd1[0]);
                close(fd2[0]);
                write(fd2[1],concat_str,strlen(concat_str)+1); //escribo la info de mi pipe fd2 a concat_Str
                close(fd2[1]);
		exit(0);
	
	}

	return 0;
}

