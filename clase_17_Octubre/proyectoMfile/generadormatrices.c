#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void generarMatriz(int filas, int columnas, float porcentajeCeros, FILE *archivo) {
    int totalElementos = filas * columnas;
    int totalCeros = round(totalElementos * porcentajeCeros); // Redondear al entero más cercano
    int **matriz = (int **)malloc(filas * sizeof(int *));
    for (int i = 0; i < filas; i++) {
        matriz[i] = (int *)malloc(columnas * sizeof(int));
    }
    int contadorCeros = 0;

    // Inicializar la matriz con unos
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = 1;
        }
    }

    // Rellenar la matriz con ceros
    for (int k = 0; k < totalCeros; k++) {
        int filaRandom, columnaRandom;
        do {
            filaRandom = rand() % filas;
            columnaRandom = rand() % columnas;
        } while (matriz[filaRandom][columnaRandom] == 0);

        matriz[filaRandom][columnaRandom] = 0;
        contadorCeros++;
    }

    // Escribir la matriz en el archivo
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            fprintf(archivo, "%d ", matriz[i][j]);
        }
        fprintf(archivo, "\n");
    }

    printf("La matriz tiene %d ceros y %d unos.\n", contadorCeros, totalElementos - contadorCeros);

    // Liberar memoria
    for (int i = 0; i < filas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s <nombreArchivo> <filas> <columnas> <porcentajeCeros>\n", argv[0]);
        return 1;
    }

    char* nombreArchivo = argv[1];
    int filas = atoi(argv[2]);
    int columnas = atoi(argv[3]);
    float porcentajeCeros = atof(argv[4]);

    srand(time(NULL)); // Inicializar la semilla para números aleatorios

    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    generarMatriz(filas, columnas, porcentajeCeros, archivo);

    fclose(archivo);
    printf("Matriz generada en '%s'.\n", nombreArchivo);

    return 0;
}


