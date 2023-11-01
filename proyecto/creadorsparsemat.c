#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Estructura para representar una matriz dispersa
typedef struct {
    int **matrix; // Matriz
    int rows;     // Número de filas
    int cols;     // Número de columnas
} SparseMatrix;

// Función para inicializar una matriz llena de ceros
SparseMatrix initializeMatrix(int rows, int cols) {
    SparseMatrix sparseMatrix;
    sparseMatrix.rows = rows;
    sparseMatrix.cols = cols;

    // Asignar memoria para la matriz
    sparseMatrix.matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        sparseMatrix.matrix[i] = (int *)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            sparseMatrix.matrix[i][j] = 0; // Inicializar con ceros
        }
    }

    return sparseMatrix;
}

// Función para generar valores aleatorios en la matriz
void generateRandomValues(SparseMatrix *matrix, int nonzeroCount) {
    srand(time(NULL));

    for (int i = 1; i <= nonzeroCount; i++) {
        int row = rand() % matrix->rows;
        int col = rand() % matrix->cols;
        // Verificar si la celda está ocupada
        if (matrix->matrix[row][col] == 0) {
            matrix->matrix[row][col] = 1; // Colocar un elemento

            // Incrementar el contador solo si se coloca un elemento
            i++;
        }

        // Moverse a la siguiente celda (fila, columna)
        col++;
        if (col >= matrix->cols) {
            // Si se llega al final de la columna, continuar en la siguiente fila desde la columna 0
            row++;
            col = 0;
        }

        // Volver a la primera fila si se llega al final de la matriz
        if (row >= matrix->rows) {
            row = 0;
        }

    }
}

// Función para escribir la matriz en un archivo
void writeMatrixToFile(SparseMatrix matrix, const char *filename) {
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            fprintf(file, "%d ", matrix.matrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int aproximacionEntero(float numeroDecimal) {
    // Extraer la parte decimal
    float parteDecimal = numeroDecimal - floor(numeroDecimal);

    // Aproximar al entero basado en el criterio
    if (parteDecimal >= 0.5) {
        return (int)ceil(numeroDecimal); // Redondear hacia arriba
    } else {
        return (int)floor(numeroDecimal); // Redondear hacia abajo
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s <nombre_archivo> <filas> <columnas> <porcentaje>\n", argv[0]);
        return -1;
    }

    const char *filename = argv[1];
    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);
    float porcentaje = atoi(argv[4]);
    float pospro = porcentaje/100;
    int nonzeroCount = 0;
    int zeroCount = aproximacionEntero((rows*cols)*(porcentaje/100));

    nonzeroCount = (rows*cols)-zeroCount;

    if (rows <= 0 || cols <= 0 || nonzeroCount <= 0 || nonzeroCount > rows * cols) {
        printf("Los parámetros ingresados son incorrectos.\n");
        return -1;
    }

    SparseMatrix sparseMatrix = initializeMatrix(rows, cols);
    generateRandomValues(&sparseMatrix, nonzeroCount);
    writeMatrixToFile(sparseMatrix, filename);
    printf("el numero de zerocount es: %d.\n", zeroCount);
    printf("el numero de nonzero es: %d.\n", nonzeroCount);
    printf("Matriz generada y escrita en el archivo %s.\n", filename);

    // Liberar memoria de la matriz
    for (int i = 0; i < sparseMatrix.rows; i++) {
        free(sparseMatrix.matrix[i]);
    }
    free(sparseMatrix.matrix);

    return 0;
}
