/****************************************************************
 * Fecha: 01 - Septiembre - 2023
 * Autores: Juan Sebastian Clavijo Martinez, Jorge Torrado 
 * Clase: Sistemas Operativos
 * Objetivo: Aplicacion de conceptos y comunicación entre procesos
 ***************************************************************/

//includes respectivos 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>

//funcion que lee los archivos y carga a los vectores de punteros

void cargarPunteros(FILE *arch , char *archivo ,int *vect){
    char contenido;
    char *buffer = (char*)malloc(1024);
    arch = fopen(archivo,"r"); //se abre el archivo 
    int i = 0;
    //se lee el archivo
    while (fgets(buffer, 1024, arch) != NULL) {
        char *token = strtok(buffer, " ");
        while (token != NULL) {
            vect[i] = atoi(token);
            i++;
            token = strtok(NULL, " ");
        }
    }
    fclose(arch);
}

//main en el cual se hace la verificacion de que la info ingresada en el comando sea correcta

int main(int argc, char *argv[]){
    //verifico que haya los 5 parámetros mínimos para la apropiada ejecución del código
    if (argc<5){
        printf("\nSeguro que ingresaste por consola los datos bien?\n");
        printf("Recuerda que la estructura del comando debe ser:\n");
        printf("./taller_procesos N1 archivo00.txt N2 archivo01.txt\n");
        printf("DONDE:\n");
        printf("N1 es el numero de elementos que tiene archivo00.txt NO SE TE OLVIDE EL .txt\n");
        printf("archivo00.txt es uno de los 2 archivos que contiene datos\n");
        printf("N2 es el numero de elementos que tiene archivo01.txt\n");
        printf("archivo01.txt es el otro de los 2 archivos que contiene datos NO SE TE OLVIDE EL .txt\n");
        return -1;
    }
    //declaración de variables
    int pid1; 
    int pid2;
    int *hilo1 = (int*) malloc(2*sizeof(int)); //se reserva el espacio de memoria para los pipes
    int *hilo2 = (int*) malloc(2*sizeof(int));
    int *hilo3 = (int*) malloc(2*sizeof(int));
    int *hilo4 = (int*) malloc(2*sizeof(int));
    int *hilo5 = (int*) malloc(2*sizeof(int));
    pipe(hilo1); //se crean los pipes del tamanio definido arriba
    pipe(hilo2);
    pipe(hilo3);
    pipe(hilo4);
    pipe(hilo5);
    int N1 = (int)atof(argv[1]); //se almacenan los enteros N1 y N2 desde terminal
    int N2 = (int)atof(argv[3]);
    char *archivo00 = argv[2];//se almacenan los nombres de los archivos donde estan los enteros
    char *archivo01 = argv[4];
    int *vector0=(int*)malloc(N1*sizeof(int));// se crean los vectores donde se almacenara la info de los archivos
    int *vector1=(int*)malloc(N2*sizeof(int));
    FILE *arch00; //se crean los datos de tipo archivo
    FILE *arch01;
    cargarPunteros(arch00,archivo00,vector0);//se llama a la funcion que pasa la informacion de los archivos a los vectores
    cargarPunteros(arch01,archivo01,vector1);
    pid1 = fork(); //se crean los fork donde se hacen las operaciones
    pid2 = fork();
    if(pid1 == 0)
    {
      if(pid2 == 0)
      {
        //si ambos id son 0 significa que estamos en el grand hijo
        //procedo a hacer la suma de los numeros enteros del fichero 0
        int suma = 0;
        for(int i = 0; i < N1; i++)
        {
          suma+=vector0[i]; 
        }
        /*uso el pipe hilo1 para mandar la información al padre (en este caso la
          info es la suma de los enteros del fichero 0*/
        close(hilo1[0]);
        write(hilo1[1],&suma,sizeof(int));
        close(hilo1[1]);
        exit(0);
      }
      else
      {
        //si el pid1 es 0 y el pid2 no es 0 estamos en el segundo hijo
        //procedo a hacer la suma de los numeros enteros del fichero 1
        int suma = 0;
        for(int i = 0; i < N2; i++)
        {
          suma+=vector1[i]; 
        }
        /*uso el pipe hilo1 para mandar la información al padre (en este caso la
          info es la suma de los enteros del fichero 1*/
        close(hilo2[0]);
        write(hilo2[1],&suma,sizeof(int));
        close(hilo2[1]);
        exit(0);
      }
    }
    else
    {
      if(pid2 == 0)
      { 
        //si el pid1 no es 0 y el pid2 es 0 estamos en el primer hijo
        /*este proceso debe calcular la suma de los 2 arreglos, como ya
         tengo las sumas parciales de los arreglos de los archivos 1 y 2
         puedo "escuchar" dichos hilos y simplemente sumo las sumas parciales
         para sacar la suma total*/
         wait(NULL);
         wait(NULL);
         //declaro 2 variables que almacenarán las sumas parciales 1 y 2
         int sumPar1;
         int sumPar2;
         //variable de la suma total
         int sumaTotal;
         //leo la suma parcial 1 almacenada en el pipe hilo1
         close(hilo1[1]);
         read(hilo1[0], &sumPar1, sizeof(int));
         close(hilo1[0]);
         //leo la suma parcial 2 almacenada en el pipe hilo2
         close(hilo2[1]);
         read(hilo2[0], &sumPar2, sizeof(int));
         close(hilo2[0]);
         //hago la suma total con las sumas parciales
         sumaTotal = sumPar1+sumPar2;
         //la mando por el pipe
         close(hilo3[0]);
         write(hilo3[1], &sumaTotal, sizeof(int));
         close(hilo3[1]);
         /*mando las sumas parciales a sus respectivos pipes, pues al leerlas
           se extran de la cola que funciona como pipe*/
         close(hilo4[0]);
         write(hilo4[1], &sumPar1, sizeof(int));
         close(hilo4[1]);
         close(hilo5[0]);
         write(hilo5[1], &sumPar2, sizeof(int));
         close(hilo5[1]);
      }
      else
      {
        //si el pid1 no es 0 y el pid2 tampoco es 0 estamos en el padre
        //espero a que los 3 hijos terminen sus procesos
        wait(NULL);
        wait(NULL);
        wait(NULL);
        //declaro 2 variables que almacenarán las sumas parciales 1 y 2
        int sumPar1;
        int sumPar2;
        //variable de la suma total
        int sumaTotal;
        //leo la suma parcial 1 almacenada en el pipe hilo1
        close(hilo4[1]);
        read(hilo4[0], &sumPar1, sizeof(int));
        close(hilo4[0]);
        //leo la suma parcial 2 almacenada en el pipe hilo2
        close(hilo5[1]);
        read(hilo5[0], &sumPar2, sizeof(int));
        close(hilo5[0]);
        //leo la suma total almacenada en el pipe hilo3
        close(hilo3[1]);
        read(hilo3[0], &sumaTotal, sizeof(int));
        close(hilo3[0]);
        //imprimo los resultados de los hijos usando al padre:
        printf("\nSuma del arreglo del fichero %s: %d\n\nSuma del arreglo del fichero %s: %d\n\nSuma de las 2 sumas enunciadas anteriormente: %d\n",archivo00, sumPar1,archivo01, sumPar2,sumaTotal);
      }
    }
    //libero la memoria que tomé prestada
    free(hilo1);
    free(hilo2);
    free(hilo3);
    free(hilo4);
    free(hilo5);
    free(vector0);
    free(vector1);
    return 0;
}
