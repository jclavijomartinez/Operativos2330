/****************************************************************
 * Fecha: 19 - septiembre - 2023
 * Autor: Juan Sebastian Clavijo Martinez (jsebastian.clavijoc@javeriana.edu.co)
 * Tema: Arreglo de hilos y trabajos
 ***************************************************************/
//includes respectivos
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define N 5

//funcion que me imprime el id del trabajo
void *funcion(void *arg){
    size_t job = *(size_t*)arg;//se castea arg a tipo size_t*
    printf("Job %zu\n",job); //%zu formatea para cualquier tipo de dato
    return NULL; //no tiene nada que ver con nada, es solo para que haya coherencia
}

int main(int argc, char **argv[]){
    size_t jobs[N]; //vector de trabajos de tipo size_t, que es un tipo de dato del sistema
    pthread_t hilos[N]; //vector de hilos
    for (size_t i = 0; i < N; i++)
    {
        jobs[i] = i;
        pthread_create(&hilos[i],NULL,funcion,jobs+i);//jobs+i representa el argumento que envio a funcion
    }
    for (size_t i = 0; i < N; i++)
    {
        jobs[i] = i;
        pthread_join(hilos[i],NULL); //se espera que cada hilo finalice
    }
    return 0;
}