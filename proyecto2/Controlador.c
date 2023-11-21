/****************************************************************
 * Fecha: 20-11-2023
 * Autores: 
 *  - Juan Sebastian Clavijo Martinez (jsebastian.clavijoc@javeriana.edu.co), 
 *  - Santiago Mesa Niño (santiagoa.mesan@javeriana.edu.co), 
 *  - Juliana Lugo Martínez (julugo@javeriana.edu.co)
 *  - Juan Camilo Martínez ()
 *  - Juan David Castillo ()
 *  - Juan Daniel Vargas ()
 * Tema: proyecto 1
 * Objetivo: 
 ***************************************************************/

#include "Controlador.h"
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



// Descripción:
// Esta función es ejecutada por un hilo y simula el paso del tiempo, actualizando la hora cada segundo y reiniciando el contador de visitantes por hora.
// Parámetros:
// datosEspera (struct DatosEspera*): Puntero a una estructura que contiene la hora de inicio, la hora de finalización y los segundos por hora.
// Retorno:
// NULL: La función no devuelve ningún valor.
// Funcionamiento Interno:
// Utiliza nanosleep para esperar un segundo.
// Incrementa la hora actual.
// Reinicia el contador de visitantes para la nueva hora.
// Continúa el bucle hasta que se alcanza la hora de finalización.
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

// Descripción:
// Esta función es ejecutada por un hilo y se encarga de recibir y procesar solicitudes a través de un pipe de comunicación.
// Parámetros:
// datosPipe (struct DatosPipe*): Puntero a una estructura que contiene la hora de finalización, el nombre del pipe y los segundos por hora.
// Retorno:
// NULL: La función no devuelve ningún valor.
// Funcionamiento Interno:
// Crea un pipe con el nombre proporcionado.
// Abre el pipe en modo lectura.
// Lee continuamente solicitudes del pipe y las procesa.
// Responde al agente según el resultado de la solicitud.
// Continúa el bucle hasta que se alcanza la hora de finalización.
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
      datosSolicitud.horaCierre = horaFinal;

      // Respondiendo al agente
      write(pipe_fd, &datosSolicitud, sizeof(struct DatosSolicitud));
      pthread_mutex_unlock(&mutex);
    }
  }

  close(pipe_fd);
  printf("El hilo de solicitudes se ha cerrado\n");
  pthread_exit(NULL);
}



