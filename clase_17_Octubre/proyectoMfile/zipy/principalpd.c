/**********************
 * Fecha: 24-10-2023
 * Autores: 
 *  - Juan Sebastian Clavijo Martinez (jsebastian.clavijoc@javeriana.edu.co), 
 *  - Santiago Mesa Niño (santiagoa.mesan@javeriana.edu.co), 
 *  - Juliana Lugo Martínez (julugo@javeriana.edu.co)
 * Tema: proyecto 1
 * Objetivo: archivo principal, que contiene la funcion main 
 *********************/


//includes respectivos
#include "modulopd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdbool.h>

#include <math.h>

/**********************
 * Función: main
 * Descripción: Esta función es la entrada principal del programa. Realiza la gestión de argumentos,
 *              reserva memoria para la matriz, carga la matriz desde un archivo, verifica si es dispersa
 *              y realiza la liberación de recursos al final.
 * Parámetros de Entrada:
 *   - argc: Cantidad de argumentos en la línea de comandos.
 *   - argv: Arreglo de argumentos en la línea de comandos.
 * Valor de Salida: 0 si el programa se ejecuta correctamente, -1 en caso de error.
 **********************/ 

int main(int argc, char *argv[]){
    // Se inicializan las variables para recibir los datos del usuario dados en la terminal.
    int numfils=-1;
    int numcols=-1;
    char *archivo = NULL;
    FILE *arch = NULL;
    int numproc=-1;
    int numpor=-1;
    int opc;
    int **matriz;
    long num_procesadores = sysconf(_SC_NPROCESSORS_ONLN); // Obtiene el número de procesadores disponibles.

    // Iniciar el temporizador
    //start_timer();

    // Se verifica que el número de argumentos pasados por consola sea el esperado.
    if (argc < 10)
    {
        printf("Los argumentos ingresados no son correctos\n");
        printf("Debes ingresar los siguientes argumentos:\n");
        printf("Nombre del programa -f #filas -c #columnas -a Nombre del archivo.txt -n #de procesadores -p #de porcentaje de dispersion\n\n");
        printf("Un ejemplo seria: ./principalpd -f 80 -c 80 -a ejemplo280X80.txt -n 2 -p 90\n");
        return -1;
    }

    // Se recorre la llamada desde terminal para identificar los selectores y almacenar las variables donde correspondan.
    while ((opc = getopt(argc, argv, "f:c:a:n:p:")) != -1) {
        switch (opc) {
            case 'f':
                numfils = atoi(optarg); // Convierte el argumento a entero y lo asigna a numfils.
                break;
            case 'c':
                numcols = atoi(optarg); // Convierte el argumento a entero y lo asigna a numcols.
                break;
            case 'a':
                archivo = optarg; // Asigna el nombre del archivo.
                break;
            case 'n':
                numproc = atoi(optarg); // Convierte el argumento a entero y lo asigna a numproc.
                break;
            case 'p':
                numpor = atoi(optarg); // Convierte el argumento a entero y lo asigna a numpor.
                break;
            default:
                fprintf(stderr, "Uso: %s -f filas -c columnas -a archivo.txt -n nprocesos -p porcentaje\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    
    // Se verifica que el número de procesos sea par.
    /*if (numproc%2!=0){
        printf("recuerda que necesito que el numero de procesos sea PAR revisa!\n");
        return -1;
    }*/

    // Se verifica que las filas y columnas proporcionadas por el usuario coincidan con las del archivo.
    if(filasycolsdelarchivo(archivo,numfils,numcols)){
        // Se reserva espacio en memoria para la matriz.
        crearmatriz(numfils,numcols,&matriz);
        // Se carga la matriz con los datos del archivo.
        cargarmatriz(arch,archivo,numfils,numcols,matriz);
        // Se imprime la matriz.
        if(numfils <= 10 && numcols <= 10){
            printf("La matriz en memoria se ve asi: \n");
            printmat(numfils,numcols,matriz);
            printf("\n");
        }

        // Se verifica que el número de procesadores sea suficiente.
        if (num_procesadores < 1) {
            perror("Error al obtener el número de procesadores");
            return 1;
        } else if (numproc > num_procesadores){
            printf("no es posible ejecutar el programa ya que se piden %d procesos y el computador tiene %ld nucleos, me pides mas procesos que nucleos\n",numproc,num_procesadores);
            return -1;
        }

        start_timer();
        // Se divide la matriz en grupos y se verifica si es dispersa (sparse).
        if (numfils%numproc==0) {
            if (divisionhorizontal(numpor, numfils, numcols, numproc, &matriz)){
                end_timer();
                printf("La matriz es dispersa.\n");
            } else {
                end_timer();
                printf("La matriz no es dispersa.\n");
            }
        } else if (numcols%numproc==0) {
            if (divisionvertical(numpor, numfils, numcols, numproc, &matriz)){
                end_timer();
                printf("La matriz es dispersa.\n");
            } else {
                end_timer();
                printf("La matriz no es dispersa.\n");
            }
        } else {
            if (numfils > numcols){
                if (divisionhorizontal(numpor, numfils, numcols, numproc, &matriz)){
                    end_timer();
                    printf("La matriz es dispersa.\n");
                } else {
                    end_timer();
                    printf("La matriz no es dispersa.\n");
                }
            }else if (numcols > numfils){
                if (divisionvertical(numpor, numfils, numcols, numproc, &matriz)){
                    end_timer();
                    printf("La matriz es dispersa.\n");
                } else {
                    end_timer();
                    printf("La matriz no es dispersa.\n");
                }
            }else{
                if (divisionhorizontal(numpor, numfils, numcols, numproc, &matriz)){
                    end_timer();
                    printf("La matriz es dispersa.\n");
                } else {
                    end_timer();
                    printf("La matriz no es dispersa.\n");
                }
            }
        }
        // Detener el temporizador y mostrar el tiempo de ejecución
    } else {
        printf("estas seguro de que pusiste el numero de columnas y filas correcto?\n");
        return -1;
    }

    for (int i = 0; i < numfils; i++) {
    free(matriz[i]);
    }

    // Detener el temporizador y mostrar el tiempo de ejecución
    //end_timer();

    // Se liberan los recursos de memoria usados, empezando por la matriz.
    free(matriz);
    return 0;
} //EOF
