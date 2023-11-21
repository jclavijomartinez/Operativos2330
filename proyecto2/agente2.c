#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct DatosSolicitud {
  char nombre[50];
  int hora;
  int numPersonas;
  int respuesta; // Nueva variable para la respuesta del controlador (0:
                 // denegada, 1: positiva, 2: reprogramación)
};

int main(int argc, char *argv[]) {
  // Variables para recibir los datos del comando.
  int nombre = -1, archivo = -1, nomPipe = -1;

  if (argc != 7) {
    printf("Uso: ./agente –s nombre –a archivosolicitudes –p pipecrecibe\n");
    exit(1);
  }

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

  if (nombre == 0 || archivo == 0 || nomPipe == 0) {
    printf("Algunos de los argumentos no son válidos.\n");
    exit(1);
  }

  if (nombre == -1 || archivo == -1 || nomPipe == -1) {
    printf("Falta al menos un argumento.\n");
    exit(1);
  }

  FILE *archivoEntrada;
  char linea[100];
  char nomFamilia[100];
  int hora, numPersonas;

  archivoEntrada = fopen(argv[archivo], "r");
  if (archivoEntrada == NULL) {
    printf("No se pudo abrir el archivo\n");
    return 1;
  }

  char *pipeName = argv[nomPipe];

  mkfifo(pipeName, 0666);

  int pipe_fd = open(pipeName, O_RDWR); // Modificado para O_RDWR

  if (pipe_fd == -1) {
    perror("Error al abrir el pipe");
    exit(1);
  }

  struct DatosSolicitud datosSolicitud;

  while (fgets(linea, sizeof(linea), archivoEntrada) != NULL) {
    sscanf(linea, "%[^,],%d,%d", nomFamilia, &hora, &numPersonas);

    printf("Nombre: %s, Hora: %d, Número de personas: %d\n", nomFamilia, hora,
           numPersonas);
    strcpy(datosSolicitud.nombre, nomFamilia);
    datosSolicitud.hora = hora;
    datosSolicitud.numPersonas = numPersonas;

    write(pipe_fd, &datosSolicitud, sizeof(struct DatosSolicitud));

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

    sleep(2);
  }

  printf("Agente %s termina\n", argv[nombre]);

  close(pipe_fd);
  fclose(archivoEntrada);

  return 0;
}
