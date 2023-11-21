#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

struct DatosEspera {
  int horaI;
  int horaF;
  int segHora;
};

struct DatosPipe {
  int horaF;
  char *pipeName;
  int segHora;
};

struct DatosSolicitud {
  char nombre[50];
  int hora;
  int numPersonas;
  int respuesta;  // 0: denegada, 1: positiva, 2: reprogramación
  int horaActual; // Agregado para sincronizar la hora actual
};

int horaActual;
int personasEnParque = 0;
int numVisitantesPorHora[24];
int solicitudesNegadas = 0;
int solicitudesReprogramadas = 0;
int totalPersonas;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *esperaHora(void *datosEspera) {
  struct DatosEspera *datos = (struct DatosEspera *)datosEspera;

  horaActual = datos->horaI;
  int horaFinal = datos->horaF;

  printf("Hora de inicio: %d\n", horaActual);
  printf("Hora final: %d\n", horaFinal);

  while (horaActual < horaFinal) {
    struct timespec tiempoEspera;
    tiempoEspera.tv_sec = datos->segHora;
    tiempoEspera.tv_nsec = 0;

    nanosleep(&tiempoEspera, NULL);
    horaActual++;

    printf("Hora Actual: %d\n", horaActual);

    pthread_mutex_lock(&mutex);
    numVisitantesPorHora[horaActual % 24] = 0;
    pthread_mutex_unlock(&mutex);
  }

  pthread_exit(NULL);
}

void *recibirSolicitud(void *datosPipe) {
  struct DatosPipe *datos = (struct DatosPipe *)datosPipe;

  int horaFinal = datos->horaF;

  mkfifo(datos->pipeName, 0666);
  int pipe_fd = open(datos->pipeName, O_RDONLY);

  if (pipe_fd == -1) {
    perror("Error al abrir el pipe");
    exit(1);
  }

  struct DatosSolicitud datosSolicitud;

  while (horaActual < horaFinal) {
    if (read(pipe_fd, &datosSolicitud, sizeof(struct DatosSolicitud)) > 0) {
      pthread_mutex_lock(&mutex);

      // Procesamiento de la solicitud
      if (datosSolicitud.hora < horaActual) {
        datosSolicitud.respuesta = 0; // Denegada
        printf(
            "Solicitud denegada: %s, Hora: %d, Personas: %d (Hora inválida)\n",
            datosSolicitud.nombre, datosSolicitud.hora,
            datosSolicitud.numPersonas);
        solicitudesNegadas++;
      } else if (numVisitantesPorHora[datosSolicitud.hora % 24] +
                     datosSolicitud.numPersonas <=
                 totalPersonas) {
        numVisitantesPorHora[datosSolicitud.hora % 24] +=
            datosSolicitud.numPersonas;
        personasEnParque += datosSolicitud.numPersonas;
        datosSolicitud.respuesta = 1; // Positiva
        printf("Solicitud autorizada: %s, Hora: %d, Personas: %d\n",
               datosSolicitud.nombre, datosSolicitud.hora,
               datosSolicitud.numPersonas);
      } else {
        datosSolicitud.respuesta = 2; // Reprogramación
        solicitudesReprogramadas++;
        printf("Solicitud reprogramada: %s, Hora: %d, Personas: %d (Capacidad "
               "excedida)\n",
               datosSolicitud.nombre, datosSolicitud.hora,
               datosSolicitud.numPersonas);
      }

      // Agregando la hora actual a la respuesta
      datosSolicitud.horaActual = horaActual;

      // Respondiendo al agente
      write(pipe_fd, &datosSolicitud, sizeof(struct DatosSolicitud));
      pthread_mutex_unlock(&mutex);
    }
  }

  close(pipe_fd);
  printf("El hilo de solicitudes se ha cerrado\n");
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  int horaI = -1, horaF = -1, segHora = -1, nomPipe = -1;

  if (argc != 11) {
    printf("Uso: ./controlador –i horaInicio –f horafinal –s segundoshora –t "
           "totalpersonas –p pipecrecibe\n");
    exit(1);
  }

  for (int i = 1; i < 11; i += 2) {
    if (strcmp(argv[i], "-i") == 0) {
      horaI = atoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-f") == 0) {
      horaF = atoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-s") == 0) {
      segHora = atoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-t") == 0) {
      totalPersonas = atoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-p") == 0) {
      nomPipe = i + 1;
    }
  }

  if (horaI == 0 || horaF == 0 || segHora == 0 || totalPersonas == 0 ||
      nomPipe == 0) {
    printf("Algunos de los argumentos no son válidos.\n");
    exit(1);
  }

  if (horaI == -1 || horaF == -1 || segHora == -1 || totalPersonas == -1 ||
      nomPipe == -1) {
    printf("Falta al menos un argumento.\n");
    exit(1);
  }

  if (horaI >= horaF) {
    printf("La hora final debe ser mayor que la hora de inicio.\n");
    exit(1);
  }

  printf("\nHora de inicio: %d\n", horaI);
  printf("Hora final: %d\n", horaF);
  printf("Segundos por hora: %d\n", segHora);
  printf("Total de personas: %d\n", totalPersonas);
  printf("Nombre del pipe: %s\n\n\n", argv[nomPipe]);

  pthread_t hilo_tiempo, hilo_solicitudes;

  struct DatosEspera datosEspera;
  datosEspera.horaI = horaI;
  datosEspera.horaF = horaF;
  datosEspera.segHora = segHora;

  if (pthread_create(&hilo_tiempo, NULL, esperaHora, (void *)&datosEspera) !=
      0) {
    perror("Error al crear el hilo de espera");
    exit(EXIT_FAILURE);
  }

  struct DatosPipe datosPipe;
  datosPipe.horaF = horaF;
  datosPipe.pipeName = argv[nomPipe];
  datosPipe.segHora = segHora;

  if (pthread_create(&hilo_solicitudes, NULL, recibirSolicitud,
                     (void *)&datosPipe) != 0) {
    perror("Error al crear el hilo del pipe");
    exit(EXIT_FAILURE);
  }

  pthread_join(hilo_tiempo, NULL);
  pthread_join(hilo_solicitudes, NULL);

  int personasPromedioPorHora = 0;
  int horaMasConcurrida = -1;
  int maxPersonasEnHora = 0;

  printf("\nReporte final:\n");
  printf("Total de personas en el parque: %d\n", personasEnParque);

  for (int i = 0; i < 24; i++) {
    personasPromedioPorHora += numVisitantesPorHora[i];

    if (numVisitantesPorHora[i] > maxPersonasEnHora) {
      maxPersonasEnHora = numVisitantesPorHora[i];
      horaMasConcurrida = i;
    }

    printf("Hora %d: %d personas\n", i, numVisitantesPorHora[i]);
  }

  personasPromedioPorHora /= (horaF - horaI);

  printf("\nPersonas promedio por hora: %d\n", personasPromedioPorHora);
  printf("Hora más concurrida: %d con %d personas\n", horaMasConcurrida,
         maxPersonasEnHora);
  printf("Solicitudes denegadas: %d\n", solicitudesNegadas);
  printf("Solicitudes reprogramadas: %d\n", solicitudesReprogramadas);

  return 0;
}
