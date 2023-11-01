/****************************************************************
 * Fecha: 19 - septiembre - 2023
 * Autor: Juan Sebastian Clavijo Martinez (jsebastian.clavijoc@javeriana.edu.co)
 * Tema: Pthread ejercicoi 1
 ***************************************************************/
//includes respectivos
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//funcion que me avisa cuando se inicia y termina mi programa
void *funcion(void *arg){
    printf("funcion de hilo: inicio \n");
    sleep(2); //espera 2 seg
    printf("Funcion hilo FIN");
    return NULL;
}

int main(int argc, char **argv){
    pthread_t hilo; //se identifica el hilo
    int hiloCreacion = pthread_create(&hilo,NULL,&funcion,NULL); //se verifica la creacion exitosa del hilo, usando pthread_create
    if (hiloCreacion)
    {
        printf("Fallo de creacion del hilo :: %d \n",hiloCreacion);
    } else
    {
        printf("Hilo creado con ID :: %d \n",hiloCreacion);
    }
    hiloCreacion = pthread_join(hilo,NULL); //se espera el proceso del hilo termine
    printf("Espera de salida del hilo\n");
    if (hiloCreacion) //se verifica la salida exitosa, es redundante
    {
        printf("Falo de salida del hilo:: %d \n",hiloCreacion);
    }
    printf("Final del programa principal:: %d \n",hiloCreacion);
}