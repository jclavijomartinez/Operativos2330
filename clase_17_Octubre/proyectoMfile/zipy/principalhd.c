/**********************
 * Fecha: 24-10-2023
 * Autores: 
 *  - Juan Sebastian Clavijo Martinez (jsebastian.clavijoc@javeriana.edu.co), 
 *  - Santiago Mesa Niño (santiagoa.mesan@javeriana.edu.co), 
 *  - Juliana Lugo Martínez (julugo@javeriana.edu.co)
 * Tema: proyecto 1
 * Objetivo: archivo principal de hdispersa, contiene la funcion main 
 *********************/

#include "modulohd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>


/****************************************************************
 * Función: main
 * Descripción: Esta función es la entrada principal del programa. Realiza la gestión de argumentos,
 *              reserva memoria para la matriz, carga la matriz desde un archivo, verifica si es dispersa
 *              y realiza la liberación de recursos al final.
 * Parámetros de Entrada:
 *   - argc: Cantidad de argumentos en la línea de comandos.
 *   - argv: Arreglo de argumentos en la línea de comandos.
 * Valor de Salida: 0 si el programa se ejecuta correctamente, -1 en caso de error.
 ****************************************************************/ 


int main(int argc, char *argv[]){
    // Iniciar el temporizador
    //start_timer();
    // Inicializar las variables para recibir los datos del usuario dados en la terminal
    int numfils=-1;
    int numcols=-1;
    char *archivo = NULL;
    FILE *arch = NULL;
    int numthreads=-1;
    int numpor=-1;
    int opc;
    int **matriz;

    // Inicializar la librería de hilos
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Verificar que los argumentos pasados por la terminal sean suficientes
    if (argc < 10) {
        printf("Los argumentos ingresados no son correctos\n");
        printf("Debes ingresar los siguientes argumentos:\n");
        printf("Nombre del programa -f #filas -c #columnas -a Nombre del archivo.txt -n #de hilos -p #de porcentaje de dispersion\n\n");
        printf("Un ejemplo seria: ./principalhd -f 80 -c 80 -a ejemplo280X80.txt -n 15 -p 90\n");
        return -1;
    }

    // Recorrer la llamada desde la terminal para identificar los selectores y almacenar las variables donde correspondan
    while ((opc = getopt(argc, argv, "f:c:a:n:p:")) != -1) {
        switch (opc) {
            case 'f':
                numfils = atoi(optarg); // Obtener el número de filas desde la terminal
                break;
            case 'c':
                numcols = atoi(optarg); // Obtener el número de columnas desde la terminal
                break;
            case 'a':
                archivo = optarg; // Obtener el nombre del archivo desde la terminal
                break;
            case 'n':
                numthreads = atoi(optarg); // Obtener el número de hilos desde la terminal
                break;
            case 'p':
                numpor = atoi(optarg); // Obtener el porcentaje desde la terminal
                break;
            default:
                fprintf(stderr, "Uso: %s -f filas -c columnas -a archivo.txt -n nthreads -p porcentaje\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Verificar que las filas y columnas especificadas coincidan con las del archivo
    printf("Antes de verificar filas y columnas del archivo.\n");
    if (filasycolsdelarchivo(archivo, numfils, numcols)) {
        printf("Las filas y columnas del archivo son correctas.\n");
        // Reservar espacio de memoria para la matriz
        crearmatriz(numfils, numcols, &matriz);
        // Cargar los elementos de la matriz desde el archivo a la memoria
        cargarmatriz(arch, archivo, numfils, numcols, &matriz);
        // Imprimir la matriz cargada en la memoria
        if(numfils <= 10 && numcols <= 10){
            printf("La matriz en memoria se ve asi: \n");
            printmat(numfils,numcols,matriz);
            printf("\n");
        }

        start_timer();
        // Verificar si es posible dividir las filas o columnas entre los hilos
        if (numfils % numthreads == 0) {
            // Llamar a la función de división horizontal y determinar si la matriz es dispersa
            if (divisionhorizontal(numpor, numfils, numcols, numthreads, &matriz)) {
                printf("La matriz es dispersa.\n"); // Es dispersa
            } else {
                printf("La matriz no es dispersa.\n"); // No es dispersa
            }
        } else if (numcols % numthreads == 0) {
            // Llamar a la función de división vertical y determinar si la matriz es dispersa
            if (divisionvertical(numpor, numfils, numcols, numthreads, &matriz)) {
                printf("La matriz es dispersa.\n"); // Es dispersa
            } else {
                printf("La matriz no es dispersa.\n"); // No es dispersa
            }
        } else {
            // Si no es posible dividir exactamente, decidir basándose en la dimensión más grande
            if (numfils > numcols) {
                if (divisionhorizontal(numpor, numfils, numcols, numthreads, &matriz)) {
                   end_timer();
		   printf("La matriz es dispersa.\n"); // Es dispersa
                } else {
			end_timer();
    			printf("La matriz no es dispersa.\n"); // No es dispersa
                }
            } else if (numcols > numfils) {
                if (divisionvertical(numpor, numfils, numcols, numthreads, &matriz)) {
                    end_timer();
		    printf("La matriz es dispersa.\n"); // Es dispersa
                } else {
			end_timer();
                    printf("La matriz no es dispersa.\n"); // No es dispersa
                }
            } else {
                // Si ambas dimensiones son iguales, se puede elegir cualquier método
                if (divisionhorizontal(numpor, numfils, numcols, numthreads, &matriz)) {
			end_timer();
                    printf("La matriz es dispersa.\n"); // Es dispersa
                } else {
			end_timer();
                    printf("La matriz no es dispersa.\n"); // No es dispersa
                }
            }
        }
       // end_timer();
    } else {
        printf("Las filas y columnas del archivo NO son correctas.\n");
        printf("¿Estás seguro de que ingresaste el número de columnas y filas correctamente?\n");
        return -1;
    }

    // Limpiar la librería de hilos
    pthread_attr_destroy(&attr);
    // Detener el temporizador y mostrar el tiempo de ejecución
    //end_timer();
    pthread_exit(NULL);
    
    // Liberar los recursos de memoria utilizados, empezando por la matriz
    free(matriz);
    return 0;
} //EOF
