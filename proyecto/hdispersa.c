/****************************************************************
 * Fecha: 10-09-2023
 * Autor: Juan Sebastian Clavijo Martinez (jsebastian.clavijoc@javeriana.edu.co)
 * Tema: proyecto 1
 * Objetivo: 
 ***************************************************************/

//includes respectivos
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>

typedef struct {
    int inicio;
    int fin;
    int numcols;
    int **matriz;
} ThreadData;

void *threadFuncHorizontal(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int count = 0;
    for (int j = data->inicio; j < data->fin; j++) {
        for (int k = 0; k < data->numcols; k++) {
            if (data->matriz[j][k] != 0) {
                count++;
            }
        }
    }
    return (void *)(intptr_t)count;
}

void *threadFuncVertical(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int count = 0;
    for (int j = data->inicio; j < data->fin; j++) {
        for (int k = 0; k < data->numcols; k++) {
            if (data->matriz[k][j] != 0) {
                count++;
            }
        }
    }
    return (void *)(intptr_t)count;
}


bool divisionhorizontal(int numpor, int nfilas, int numcols, int numthreads, int ***matriz) {
    int filasPorThread = nfilas / numthreads;
    int filasRestantes = nfilas % numthreads;
    int totalElementosDiferentesDeCero = 0;
    pthread_t threads[numthreads];
    ThreadData threadData[numthreads];

    for (int i = 0; i < numthreads; i++) {
        threadData[i].inicio = i * filasPorThread;
        threadData[i].fin = threadData[i].inicio + filasPorThread;
        threadData[i].numcols = numcols;
        threadData[i].matriz = *matriz;

        if (i == numthreads - 1) {
            threadData[i].fin += filasRestantes;
        }

        pthread_create(&threads[i], NULL, threadFuncHorizontal, &threadData[i]);
    }

    for (int i = 0; i < numthreads; i++) {
        void *ret;
        pthread_join(threads[i], &ret);
        totalElementosDiferentesDeCero += (intptr_t)ret;
    }

    printf("el total que escucha el proceso padre es %d\n\n", totalElementosDiferentesDeCero);

    // Calcula el porcentaje de elementos diferentes de cero en la matriz como un entero
    int total = nfilas * numcols;
    int totalceros = round(total * (numpor / 100.0)); // Asegúrate de que sea una división flotante
    int totalNoCerosPermitidos = total - totalceros;
    
    printf("el numero de ceros debe ser %d\n\n", totalceros);
    printf("el numero de elementos diferentes de cero debe ser %d\n\n", totalNoCerosPermitidos);
    
    // Decide si la matriz es dispersa o no
    if (totalceros==0 || totalceros <= round(total * (10/100.0))){
        return false;
    }
    
    return totalElementosDiferentesDeCero <= totalNoCerosPermitidos;
}


bool divisionvertical(int numpor, int nfilas, int ncols, int numthreads, int ***matriz) {
    int colsPorThread = ncols / numthreads;
    int colsRestantes = ncols % numthreads;
    int totalElementosDiferentesDeCero = 0;
    pthread_t threads[numthreads];
    ThreadData threadData[numthreads];

    for (int i = 0; i < numthreads; i++) {
        threadData[i].inicio = i * colsPorThread;
        threadData[i].fin = threadData[i].inicio + colsPorThread;
        threadData[i].numcols = nfilas;
        threadData[i].matriz = *matriz;

        if (i == numthreads - 1) {
            threadData[i].fin += colsRestantes;
        }

        pthread_create(&threads[i], NULL, threadFuncVertical, &threadData[i]);
    }

    for (int i = 0; i < numthreads; i++) {
        void *ret;
        pthread_join(threads[i], &ret);
        totalElementosDiferentesDeCero += (intptr_t)ret;
    }

    printf("el total que escucha el proceso padre es %d\n\n", totalElementosDiferentesDeCero);

    // Calcula el porcentaje de elementos diferentes de cero en la matriz como un entero
    int total = ncols * nfilas;
    int totalceros = round(total * (numpor/100.0)); // Asegurarse de que la división sea en punto flotante
    int totalNoCerosPermitidos = total - totalceros;
    
    printf("el numero de ceros debe ser %d\n\n",totalceros);
    printf("el numero de elementos diferentes de cero debe ser %d\n\n",totalNoCerosPermitidos);
    
    if (totalceros==0 || totalceros <= round(total * (10/100.0))){
         return false;
    }
    
    // Decide si la matriz es dispersa o no
    return totalElementosDiferentesDeCero <= totalNoCerosPermitidos;
}




bool filasycolsdelarchivo(char *archivo, int filas, int cols) {
    FILE *file = fopen(archivo, "r");

    if (file == NULL) {
        perror("Error al abrir el archivo");
        return false;
    }

    int num_filas_arch = 0;
    int num_columnas_arch = 0;
    int elementos_primera_linea = 0;
    char linea[1024];
    if(fgets(linea, sizeof(linea), file) != NULL){
        char *token = strtok(linea, " "); // Separa por espacios o tabulaciones
        while (token != NULL) {
            elementos_primera_linea++;
            token = strtok(NULL, " ");
        }
        elementos_primera_linea-=1;
        num_columnas_arch=elementos_primera_linea;
    }

    while (fgets(linea, sizeof(linea), file)) {
        num_filas_arch++;
        char *token = strtok(linea, " \t"); // Dividir la línea en tokens basados en espacios o tabulaciones
        while (token) {

            token = strtok(NULL, " \t");
        }
    }
    num_filas_arch++;
    fclose(file); 
    printf("Número de filas: %d\n", num_filas_arch);
    printf("Número de columnas: %d\n", num_columnas_arch);

    if (filas == num_filas_arch && cols == num_columnas_arch) {
        return true;
    } else {
        return false;
    }
}

void printmat(int filas, int cols, int** matrix){
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
    
}

//funcion que crea (reserva el espacio para las matrices) una matriz
void **crearmatriz(int nfil, int ncol, int*** mat){
    *mat = (int**)malloc(nfil*sizeof(int*));
    if (mat == NULL)
    {
        printf("ERROR: No se ha podido reservar memoria para la matriz!\n");
        exit(1);
    }
    for (int i = 0; i < nfil; i++)
    {
        (*mat)[i] = (int *)malloc(ncol*sizeof(int));
        if ((*mat)[i] == NULL) {
            printf("ERROR: No se ha podido reservar memoria para las cols!\n");
            exit(1);
        }
    }
    
    printf("se ha reservado la memoria para la matriz con exito!\n");
}


//funcion que carga la matriz desde el archivo a memoria
void cargarmatriz(FILE *arch, char *archivo, int filas, int cols, int ***matriz) {
    arch = fopen(archivo, "r");
    if (arch == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    char *linea = NULL;
    size_t longitud = 0;
    ssize_t leidos;

    for (int i = 0; i < filas; i++) {
        leidos = getline(&linea, &longitud, arch);
        //printf("el contenido de leidos es: %c\n",leidos);
        if (leidos == -1) {
            perror("Error al leer desde el archivo");
            free(linea);
            fclose(arch);
            exit(EXIT_FAILURE);
        }
        
        char *token = strtok(linea, " ");
        for (int j = 0; j < cols && token != NULL; j++) {
            int valor;
            if (sscanf(token, "%d", &valor) == 1) {
                (*matriz)[i][j] = valor;
            } else {
                perror("Error al convertir token a entero");
                free(linea);
                fclose(arch);
                exit(EXIT_FAILURE);
            }
            token = strtok(NULL, " ");
        }
    }
    free(linea);
    printf("Los contenidos del archivo se cargaron exitosamente en memoria\n");
    fclose(arch);
    printf("El archivo se abrió y cerró exitosamente\n");
}

//se crea el numero de procesos 

int main(int argc, char *argv[]){
    // se inicializan las variables para recibir los datos del usr dados en la terminal
    int numfils=-1;
    int numcols=-1;
    char *archivo = NULL;
    FILE *arch;
    int numthreads=-1;
    int numpor=-1;
    int opc;
    int **matriz;

    // Inicializar la librería de hilos
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // se verifica que lo que haya pasado por consola del usr si tenga el largo esperado
    if (argc < 10) {
        printf("no estoy seguro que lo que hayas ingresado sea correcto, REVISA\n\n");
        return -1;
    }

    // se recorre la llamada desde terminal, para identificar los selectores y almacenar las varibales donde correspondan
    while ((opc = getopt(argc, argv, "f:c:a:n:p:")) != -1) {
        switch (opc) {
            case 'f':
                numfils = atoi(optarg);
                break;
            case 'c':
                numcols = atoi(optarg);
                break;
            case 'a':
                archivo = optarg;
                break;
            case 'n':
                numthreads = atoi(optarg);
                break;
            case 'p':
                numpor = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Uso: %s -f filas -c columnas -a archivo.txt -n nthreads -p porcentaje\n\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // verificar que las cols y filas que me pasa el usr y las del arch sean las mismas
    if (filasycolsdelarchivo(archivo, numfils, numcols)) {
        // se llama la funcion que reserva el espacio de memoria para la matriz
        crearmatriz(numfils, numcols, &matriz);
        // se llama a la funcion que almacena los elementos en memoria
        cargarmatriz(arch, archivo, numfils, numcols, &matriz);
        // se imprime la matriz del archivo
        printf("La matriz en memoria se ve asi: \n\n");
        printmat(numfils, numcols, matriz);
        printf("\n");

        // se verifica si es posible dividir las filas o columnas entre los hilos
        if (numfils % numthreads == 0) {
            if (divisionhorizontal(numpor, numfils, numcols, numthreads, &matriz)) {
                printf("La matriz es dispersa.\n\n"); // es sparse
            } else {
                printf("La matriz no es dispersa.\n\n"); // NO es sparse
            }
        } else if (numcols % numthreads == 0) {
            if (divisionvertical(numpor, numfils, numcols, numthreads, &matriz)) {
                printf("La matriz es dispersa.\n\n"); // es sparse
            } else {
                printf("La matriz no es dispersa.\n\n"); // NO es sparse
            }
        } else {
            // Si no es posible dividir exactamente, se decide basándose en la dimensión más grande
            if (numfils > numcols) {
                if (divisionhorizontal(numpor, numfils, numcols, numthreads, &matriz)) {
                    printf("La matriz es dispersa.\n\n"); // es sparse
                } else {
                    printf("La matriz no es dispersa.\n\n"); // NO es sparse
                }
            } else if (numcols > numfils) {
                if (divisionvertical(numpor, numfils, numcols, numthreads, &matriz)) {
                    printf("La matriz es dispersa.\n\n"); // es sparse
                } else {
                    printf("La matriz no es dispersa.\n\n"); // NO es sparse
                }
            } else {
                // Si ambas dimensiones son iguales, se puede elegir cualquier método
                if (divisionhorizontal(numpor, numfils, numcols, numthreads, &matriz)) {
                    printf("La matriz es dispersa.\n\n"); // es sparse
                } else {
                    printf("La matriz no es dispersa.\n\n"); // NO es sparse
                }
            }
        }
    } else {
        printf("estas seguro de que pusiste el numero de columnas y filas correcto?\n\n");
        return -1;
    }

    // Limpiar la librería de hilos
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);

    // se liberan los recursos de memoria usados, empezando por la matriz
    free(matriz);
    return 0;
}


