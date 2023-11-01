/**********************
 * Fecha: 24-10-2023
 * Autores: 
 *  - Juan Sebastian Clavijo Martinez (jsebastian.clavijoc@javeriana.edu.co), 
 *  - Santiago Mesa Niño (santiagoa.mesan@javeriana.edu.co), 
 *  - Juliana Lugo Martínez (julugo@javeriana.edu.co)
 * Tema: proyecto 1
 * Objetivo: archivo de modulo, contiene todo el cuerpo de las funciones
 *********************/

//includes respectivos
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

struct timespec start_time, end_time;

void start_timer() {
    clock_gettime(CLOCK_MONOTONIC, &start_time);
}

void end_timer() {
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + 
                          (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
    printf("Tiempo de ejecución: %f segundos\n", elapsed_time);
}


// Definición de la estructura ThreadData utilizada para pasar datos a los hilos
typedef struct {
    int inicio;       // Índice de inicio de las filas asignadas al hilo
    int fin;          // Índice de fin de las filas asignadas al hilo
    int numcols;      // Número de columnas en la matriz
    int **matriz;     // Puntero a la matriz que se procesará en el hilo
} ThreadData;


/****************************************************************
 * Función: threadFuncHorizontal
 * Descripción: Esta función se utiliza para procesar las filas de una matriz en paralelo.
 *              Cuenta los elementos diferentes de cero en las filas asignadas al hilo.
 * Parámetros de Entrada:
 *   - arg: Puntero a una estructura ThreadData que contiene información sobre la tarea del hilo.
 * Valor de Salida: La cantidad de elementos diferentes de cero contados en las filas asignadas.
 ****************************************************************/
void *threadFuncHorizontal(void *arg) {
    ThreadData *data = (ThreadData *)arg; // Obtiene los datos del hilo
    int count = 0; // Inicializa un contador para elementos diferentes de cero
    // Recorre las filas asignadas al hilo
    for (int j = data->inicio; j < data->fin; j++) {
        for (int k = 0; k < data->numcols; k++) {
            if (data->matriz[j][k] != 0) {
                count++; // Si encuentra un elemento diferente de cero, aumenta el contador
            }
        }
    }
    
    return (void *)(intptr_t)count; // Convierte el contador a un tipo que puede ser retornado
}


/****************************************************************
 * Función: threadFuncVertical
 * Descripción: Esta función se utiliza para procesar las columnas de una matriz en paralelo.
 *              Cuenta los elementos diferentes de cero en las columnas asignadas al hilo.
 * Parámetros de Entrada:
 *   - arg: Puntero a una estructura ThreadData que contiene información sobre la tarea del hilo.
 * Valor de Salida: La cantidad de elementos diferentes de cero contados en las columnas asignadas.
 ****************************************************************/
void *threadFuncVertical(void *arg) {
    ThreadData *data = (ThreadData *)arg; // Obtiene los datos del hilo
    int count = 0; // Inicializa un contador para elementos diferentes de cero

    // Recorre las columnas asignadas al hilo
    for (int j = data->inicio; j < data->fin; j++) {
        for (int k = 0; k < data->numcols; k++) {
            if (data->matriz[k][j] != 0) {
                count++; // Si encuentra un elemento diferente de cero, aumenta el contador
            }
        }
    }
    
    return (void *)(intptr_t)count; // Convierte el contador a un tipo que puede ser retornado
}


/****************************************************************
 * Función: divisionhorizontal
 * Descripción: Esta función verifica si una matriz es dispersa (sparse) en función de un umbral dado.
 *              Divide las filas de la matriz entre varios hilos para el procesamiento en paralelo.
 * Parámetros de Entrada:
 *   - numpor: Porcentaje de elementos diferentes de cero que se considera como disperso.
 *   - nfilas: Número de filas de la matriz.
 *   - numcols: Número de columnas de la matriz.
 *   - numthreads: Número de hilos para el procesamiento en paralelo.
 *   - matriz: Puntero a la matriz de enteros.
 * Valor de Salida: true si la matriz se considera dispersa, false en caso contrario.
 ****************************************************************/
bool divisionhorizontal(int numpor, int nfilas, int numcols, int numthreads, int ***matriz) {
    int filasPorThread = nfilas / numthreads; // Número de filas por hilo
    int filasRestantes = nfilas % numthreads; // Filas restantes que no se distribuyen igualmente
    int totalElementosDiferentesDeCero = 0; // Contador total de elementos diferentes de cero
    pthread_t threads[numthreads]; // Arreglo de identificadores de hilos
    ThreadData threadData[numthreads]; // Arreglo de datos de hilo

    // Ciclo para crear y lanzar los hilos
    for (int i = 0; i < numthreads; i++) {
        threadData[i].inicio = i * filasPorThread; // Índice de inicio para este hilo
        threadData[i].fin = threadData[i].inicio + filasPorThread; // Índice de finalización para este hilo
        threadData[i].numcols = numcols; // Número de columnas en la matriz
        threadData[i].matriz = *matriz; // Puntero a la matriz

        if (i == numthreads - 1) {
            threadData[i].fin += filasRestantes; // Si es el último hilo, se encarga de las filas restantes
        }

        pthread_create(&threads[i], NULL, threadFuncHorizontal, &threadData[i]); // Crea y lanza el hilo
    }

    // Ciclo para esperar a que todos los hilos terminen y sumar sus resultados
    for (int i = 0; i < numthreads; i++) {
        void *ret;
        pthread_join(threads[i], &ret); // Espera a que el hilo termine y obtiene su resultado
        totalElementosDiferentesDeCero += (intptr_t)ret; // Suma el resultado al contador total
    }

    printf("el total que escucha el proceso padre es %d\n", totalElementosDiferentesDeCero);

    // Calcula el porcentaje de elementos diferentes de cero en la matriz como un entero
    int total = nfilas * numcols; // Total de elementos en la matriz
    int totalceros = round(total * (numpor / 100.0)); // Asegura que sea una división flotante
    int totalNoCerosPermitidos = total - totalceros; // Número de elementos no cero permitidos
    
    printf("el numero de ceros debe ser %d\n", totalceros);
    printf("el numero de elementos diferentes de cero debe ser %d\n", totalNoCerosPermitidos);
    
    if (totalceros == 0 || totalceros <= round(total * (10 / 100.0))) {
        return false; // Si no hay ceros o menos del 10%, la matriz no se considera dispersa
    }
    
    // Decide si la matriz es dispersa o no
    return totalElementosDiferentesDeCero <= totalNoCerosPermitidos;
}


/****************************************************************
 * Función: divisionvertical
 * Descripción: Esta función verifica si una matriz es dispersa (sparse) en función de un umbral dado.
 *              Divide las columnas de la matriz entre varios hilos para el procesamiento en paralelo.
 * Parámetros de Entrada:
 *   - numpor: Porcentaje de elementos diferentes de cero que se considera como disperso.
 *   - nfilas: Número de filas de la matriz.
 *   - ncols: Número de columnas de la matriz.
 *   - numthreads: Número de hilos para el procesamiento en paralelo.
 *   - matriz: Puntero a la matriz de enteros.
 * Valor de Salida: true si la matriz se considera dispersa, false en caso contrario.
 ****************************************************************/
bool divisionvertical(int numpor, int nfilas, int ncols, int numthreads, int ***matriz) {
    int colsPorThread = ncols / numthreads; // Número de columnas por hilo
    int colsRestantes = ncols % numthreads; // Columnas restantes que no se distribuyen igualmente
    int totalElementosDiferentesDeCero = 0; // Contador total de elementos diferentes de cero
    pthread_t threads[numthreads]; // Arreglo de identificadores de hilos
    ThreadData threadData[numthreads]; // Arreglo de datos de hilo

    // Ciclo para crear y lanzar los hilos
    for (int i = 0; i < numthreads; i++) {
        threadData[i].inicio = i * colsPorThread; // Índice de inicio para este hilo
        threadData[i].fin = threadData[i].inicio + colsPorThread; // Índice de finalización para este hilo
        threadData[i].numcols = nfilas; // Número de filas en la matriz
        threadData[i].matriz = *matriz; // Puntero a la matriz

        if (i == numthreads - 1) {
            threadData[i].fin += colsRestantes; // Si es el último hilo, se encarga de las columnas restantes
        }

        pthread_create(&threads[i], NULL, threadFuncVertical, &threadData[i]); // Crea y lanza el hilo
    }

    // Ciclo para esperar a que todos los hilos terminen y sumar sus resultados
    for (int i = 0; i < numthreads; i++) {
        void *ret;
        pthread_join(threads[i], &ret); // Espera a que el hilo termine y obtiene su resultado
        totalElementosDiferentesDeCero += (intptr_t)ret; // Suma el resultado al contador total
    }

    printf("el total que escucha el proceso padre es %d\n", totalElementosDiferentesDeCero);

    // Calcula el porcentaje de elementos diferentes de cero en la matriz como un entero
    int total = ncols * nfilas; // Total de elementos en la matriz
    int totalceros = round(total * (numpor / 100.0)); // Asegura que sea una división flotante
    int totalNoCerosPermitidos = total - totalceros; // Número de elementos no cero permitidos
    
    printf("el numero de ceros debe ser %d\n", totalceros);
    printf("el numero de elementos diferentes de cero debe ser %d\n", totalNoCerosPermitidos);
    
    if (totalceros == 0 || totalceros <= round(total * (10 / 100.0))) {
        return false; // Si no hay ceros o menos del 10%, la matriz no se considera dispersa
    }
    
    // Decide si la matriz es dispersa o no
    return totalElementosDiferentesDeCero <= totalNoCerosPermitidos;
}


/****************************************************************
 * Función: filasycolsdelarchivo
 * Descripción: Esta función verifica si las dimensiones de una matriz en un archivo coinciden con las proporcionadas.
 * Parámetros de Entrada:
 *   - archivo: Nombre del archivo que contiene la matriz.
 *   - filas: Número de filas esperado de la matriz.
 *   - cols: Número de columnas esperado de la matriz.
 * Valor de Salida: true si las dimensiones coinciden, false en caso contrario o si hay un error en la lectura del archivo.
 ****************************************************************/
bool filasycolsdelarchivo(char *archivo, int filas, int cols) {
    FILE *file = fopen(archivo, "r"); // Abre el archivo en modo lectura

    if (file == NULL) {
        perror("Error al abrir el archivo"); // Imprime un mensaje de error si no se puede abrir el archivo
        return false;
    }

    int num_filas_arch = 0;
    int num_columnas_arch = 0;
    int elementos_primera_linea = 0;
    char linea[51200]; // Almacena cada línea leída del archivo

    // Lee la primera línea del archivo para determinar el número de columnas
    if (fgets(linea, sizeof(linea), file) != NULL) {
        char *token = strtok(linea, " "); // Separa la línea en tokens basados en espacios
        while (token != NULL) {
            elementos_primera_linea++; // Contador de elementos en la primera línea
            token = strtok(NULL, " "); // Avanza al siguiente token
        }
        elementos_primera_linea -= 1; // Resta 1 para excluir el último token (posible salto de línea)
        num_columnas_arch = elementos_primera_linea; // El número de columnas es igual a los elementos en la primera línea
    }

    // Lee el resto de las líneas para determinar el número de filas
    while (fgets(linea, sizeof(linea), file)) {
        num_filas_arch++; // Incrementa el contador de filas por cada línea leída
        char *token = strtok(linea, " \t"); // Divide la línea en tokens basados en espacios o tabulaciones
        while (token) {
            token = strtok(NULL, " \t"); // Avanza al siguiente token
        }
    }
    num_filas_arch++; // Incrementa el contador de filas para la última línea

    fclose(file); // Cierra el archivo después de leerlo

    // Compara el número de filas y columnas del archivo con los proporcionados por el usuario
    if (filas == num_filas_arch && cols == num_columnas_arch) {
        return true; // Retorna verdadero si coinciden
    } else {
        return false; // Retorna falso si no coinciden
    }
}


/****************************************************************
 * Función: printmat
 * Descripción: Esta función imprime una matriz de enteros en la consola.
 * Parámetros de Entrada:
 *   - filas: Número de filas de la matriz.
 *   - cols: Número de columnas de la matriz.
 *   - matrix: Puntero a la matriz de enteros.
 * Valor de Salida: No tiene valor de salida explícito, pero imprime la matriz en la consola.
 ****************************************************************/
void printmat(int filas, int cols, int** matrix) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]); // Imprime cada elemento de la matriz seguido de un espacio
        }
        printf("\n"); // Al final de cada fila, imprime una nueva línea
    }
}


/****************************************************************
 * Función: crearmatriz
 * Descripción: Esta función reserva memoria para una matriz de enteros de tamaño especificado.
 * Parámetros de Entrada:
 *   - nfil: Número de filas de la matriz.
 *   - ncol: Número de columnas de la matriz.
 *   - mat: Puntero a un puntero de punteros que contendrá la matriz.
 * Valor de Salida: No tiene valor de salida explícito, pero asigna memoria para la matriz.
 ****************************************************************/
void **crearmatriz(int nfil, int ncol, int*** mat) {
    *mat = (int**)malloc(nfil * sizeof(int*)); // Reservar espacio para el arreglo de punteros a filas

    if (mat == NULL) {
        printf("ERROR: No se ha podido reservar memoria para la matriz!\n");
        exit(1);
    }

    for (int i = 0; i < nfil; i++) {
        (*mat)[i] = (int*)malloc(ncol * sizeof(int)); // Reservar espacio para cada fila

        if ((*mat)[i] == NULL) {
            printf("ERROR: No se ha podido reservar memoria para las columnas!\n");
            exit(1);
        }
    }
    return NULL;
}


/****************************************************************
 * Función: cargarmatriz
 * Descripción: Esta función carga una matriz de enteros desde un archivo en memoria.
 * Parámetros de Entrada:
 *   - arch: Puntero al archivo abierto para lectura.
 *   - archivo: Nombre del archivo que contiene la matriz.
 *   - filas: Número de filas de la matriz.
 *   - cols: Número de columnas de la matriz.
 *   - matriz: Puntero a la matriz de enteros donde se almacenarán los datos.
 * Valor de Salida: No tiene valor de salida explícito, pero llena la matriz con los datos del archivo.
 ****************************************************************/
void cargarmatriz(FILE *arch, char *archivo, int filas, int cols, int ***matriz) {
    arch = fopen(archivo, "r"); // Abrir el archivo en modo lectura

    if (arch == NULL) {
        perror("Error al abrir el archivo"); // Mostrar un mensaje de error si no se pudo abrir el archivo
        exit(EXIT_FAILURE);
    }

    char *linea = NULL;
    size_t longitud = 0;
    ssize_t leidos;

    for (int i = 0; i < filas; i++) {
        leidos = getline(&linea, &longitud, arch); // Leer una línea del archivo

        if (leidos == -1) {
            perror("Error al leer desde el archivo"); // Mostrar un mensaje de error si no se pudo leer desde el archivo
            free(linea);
            fclose(arch);
            exit(EXIT_FAILURE);
        }
        
        char *token = strtok(linea, " ");
        for (int j = 0; j < cols && token != NULL; j++) {
            int valor;
            if (sscanf(token, "%d", &valor) == 1) {
                (*matriz)[i][j] = valor; // Almacenar el valor en la matriz
            } else {
                perror("Error al convertir token a entero"); // Mostrar un mensaje de error si no se pudo convertir el token a un entero
                free(linea);
                fclose(arch);
                exit(EXIT_FAILURE);
            }
            token = strtok(NULL, " ");
        }
    }
    free(linea); // Liberar la memoria utilizada por la línea
    fclose(arch); // Cerrar el archivo después de cargar los datos en la matriz
}
