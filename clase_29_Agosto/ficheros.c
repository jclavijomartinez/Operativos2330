/****************************************************************
 * Fecha: 29 - Agosto - 2023
 * Autor: Juan Clavijo
 * Tema: Fuente para generar ficheros con un array de enteros se recibe el nombre del fichero y la conudad de elementos 
 * Objetivo: 
 ***************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <time.h>

int main(int argc, char *argv[]){
	if(argc <3 ){
        printf("argumentos: ./ejecutable nomfichero cantidad\n");
        return -1;
    }
    int valor; //valor que voy a escribir en mi fichero
    char *ficheroa = argv[1]; //capturamos el nombre del fichero
    FILE *fileA; //Dato de tipo arcchivo que guarda la informacion
    int cantidad = (int) atof(argv[2]); //datos a guardar
    char buff[10]={0};//buffer para guardar mis datos que escribire al archivo
    fileA = fopen(ficheroa,"w"); //abro mi archivo cuyo nombre guarde en ficheroA
    srand(time(NULL));//genero la semilla de aleatoreidad
    for (int i =0;i<cantidad;i++){ //bucle para rellenar fichero
        valor = rand()%1000+0;//genero los datos para guardar en mi fichero, son aleatorios
        //PRUEBA: impresion de valores segun cantidad en pantalla
        //printf(" %d ",valor);
        //impresion de valores segun cantidad en fichero
        sprintf(buff," %d " , valor);
        fputs(buff,fileA);
    }
    fclose(fileA);


	return 0;
}

