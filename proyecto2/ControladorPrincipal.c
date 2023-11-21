#include "Controlador.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int horaActual;
int personasEnParque = 0;
int numVisitantesPorHora[24];
int solicitudesNegadas = 0;
int solicitudesReprogramadas = 0;
int totalPersonas;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// Descripción:
// Función principal que inicia los hilos de espera y manejo de solicitudes.
// Parámetros:
// argc (int): Número de argumentos de la línea de comandos.
// argv (char*[]): Array de cadenas que representan los argumentos de la línea de comandos.
// Retorno:
// 0 (int): La función devuelve 0 al finalizar sin errores.
// Funcionamiento Interno:
// Parsea los argumentos de la línea de comandos.
// Inicia los hilos de espera y manejo de solicitudes.
// Espera a que ambos hilos terminen.
// Genera un reporte final con estadísticas del parque.
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

  for (int i = horaI; i <= horaF; i++) {
    personasPromedioPorHora += numVisitantesPorHora[i % 24];

    if (numVisitantesPorHora[i % 24] > maxPersonasEnHora) {
      maxPersonasEnHora = numVisitantesPorHora[i % 24];
      horaMasConcurrida = i % 24;
    }

    printf("Hora %d: %d personas\n", i % 24, numVisitantesPorHora[i % 24]);
  }

  personasPromedioPorHora /= (horaF - horaI + 1);

  printf("\nPersonas promedio por hora: %d\n", personasPromedioPorHora);
  printf("Hora más concurrida: %d con %d personas\n", horaMasConcurrida,
         maxPersonasEnHora);
  printf("Solicitudes denegadas: %d\n", solicitudesNegadas);
  printf("Solicitudes reprogramadas: %d\n", solicitudesReprogramadas);

  return 0;
}
