#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct DatosSolicitud {
    char nombre[50];
    int hora;
    int numPersonas;
    int respuesta; // 0: denegada, 1: positiva, 2: reprogramación
    int horaActual; // Agregado para sincronizar la hora actual
};

int horaActual;  // Declaración de la variable global horaActual

int main(int argc, char *argv[]) {
    // Variables para recibir los datos del comando.
    int nombre = -1, archivo = -1, nomPipe = -1;

    // Se necesitan 3 argumentos para ejecutarse correctamente.
    if (argc != 7) {
        printf("Uso: ./agente –s nombre –a archivosolicitudes –p pipecrecibe\n");
        exit(1);
    }

    // Se revisan los argumentos para obtener todos los datos necesarios.
    for (int i = 1; i < 7; i += 2) {
        if (strcmp(argv[i], "-s") == 0) {
            nombre = i + 1;
        }
        if (strcmp(argv[i], "-a") == 0) {
            archivo = i + 1;
        }
        if (strcmp(argv[i], "-p") == 0) {
            nomPipe = i + 1;
        }
    }

    // Se revisa si las variables introducidas en el comando son válidas.
    if (nombre == 0 || archivo == 0 || nomPipe == 0) {
        printf("Algunos de los argumentos no son válidos.\n");
        exit(1);
    }

    // Se revisa si algunas de las variables no fueron introducidas en el comando.
    if (nombre == -1 || archivo == -1 || nomPipe == -1) {
        printf("Falta al menos un argumento.\n");
        exit(1);
    }

    // Variables para extraer información del archivo
    FILE *archivoEntrada;
    char linea[100];
    char nomFamilia[100];
    int hora, numPersonas;

    archivoEntrada = fopen(argv[archivo], "r");
    if (archivoEntrada == NULL) {
        printf("No se pudo abrir el archivo\n");
        return 1;
    }

    // Se obtiene el nombre del pipe de los argumentos
    char *pipeName = argv[nomPipe];

    // Se crea el pipe
    mkfifo(pipeName, 0666);

    // Abrir el pipe para escribir
    int pipe_fd = open(pipeName, O_WRONLY);

    if (pipe_fd == -1) {
        perror("Error al abrir el pipe");
        exit(1);
    }

    // Se crea la estructura para enviar los datos
    struct DatosSolicitud datosSolicitud;

    while (fgets(linea, sizeof(linea), archivoEntrada) != NULL) {
        sscanf(linea, "%[^,],%d,%d", nomFamilia, &hora, &numPersonas);

        // Validación de la hora de reserva
        if (hora < horaActual) {
            printf("Error: La hora de reserva es anterior a la hora actual de simulación.\n");
            continue; // Salta a la siguiente iteración del bucle
        }

        printf("Nombre: %s, Hora: %d, Número de personas: %d\n", nomFamilia, hora, numPersonas);
        strcpy(datosSolicitud.nombre, nomFamilia);
        datosSolicitud.hora = hora;
        datosSolicitud.numPersonas = numPersonas;

        // Envío de la solicitud
        write(pipe_fd, &datosSolicitud, sizeof(struct DatosSolicitud));

        // Manejo de la respuesta del controlador
        read(pipe_fd, &datosSolicitud, sizeof(struct DatosSolicitud));

        switch (datosSolicitud.respuesta) {
            case 0:
                printf("Respuesta: Denegada\n");
                break;
            case 1:
                printf("Respuesta: Positiva\n");
                break;
            case 2:
                printf("Respuesta: Reprogramación\n");
                break;
            default:
                printf("Error: Respuesta no válida recibida\n");
                break;
        }

        // Esperar 2 segundos antes de volver al paso 1
        sleep(2);
    }

    printf("Agente %s termina\n", argv[nombre]);

    // Se cierra el pipe y el archivo
    close(pipe_fd);
    fclose(archivoEntrada);

    return 0;
}

