/****************************************************************
 * Fecha: X - Octubre - 2023
 * Autor: Juan Sebastian Clavijo Martinez
 * Tema:
 * Objetivo:
 ***************************************************************/

#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Definición de la estructura para mantener el estado del parque
struct ParkStatus {
  int current_time;
  int max_capacity;
  int current_capacity;
};

// Variables globales
struct ParkStatus park_status;
char *pipe_name;

// Función para inicializar el controlador
void initialize_controller(int start_hour, int total_capacity, char *pipe) {
  park_status.current_time = start_hour;
  park_status.max_capacity = total_capacity;
  park_status.current_capacity = 0;
  pipe_name = pipe;
  // Crear el pipe
  if (mkfifo(pipe_name, 0666) == -1) {
    perror("mkfifo");
    exit(EXIT_FAILURE);
  }
}

// Función para manejar el paso del tiempo
void *time_handler(void *args) {
  while (park_status.current_time < 24) {
    sleep(1); // Simula una hora con sleep
    park_status.current_time++;
    printf("Current simulated hour: %d\n", park_status.current_time);
  }
  return NULL;
}

// Función para escuchar y procesar las solicitudes de los agentes
void *agent_listener(void *args) {
  int pipe_fd;
  char buffer[1024];

  // Abrir el pipe en modo lectura
  pipe_fd = open(pipe_name, O_RDONLY);
  if (pipe_fd == -1) {
    perror("Error opening pipe");
    exit(EXIT_FAILURE);
  }

  while (1) {
    ssize_t read_bytes = read(pipe_fd, buffer, sizeof(buffer) - 1);
    if (read_bytes > 0) {
      buffer[read_bytes] = '\0';
      printf("Received reservation request: %s\n", buffer);
      // Aquí se debería procesar la solicitud y tomar acciones
    }
  }

  close(pipe_fd);
  return NULL;
}

// Punto de entrada principal
int main(int argc, char *argv[]) {
  pthread_t time_thread, agent_thread;

  if (argc != 6) {
    fprintf(stderr,
            "Usage: %s <start_hour> <end_hour> <seconds_per_hour> "
            "<total_capacity> <pipe_name>\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }

  int start_hour = atoi(argv[1]);
  int end_hour = atoi(argv[2]);
  int seconds_per_hour = atoi(argv[3]);
  int total_capacity = atoi(argv[4]);
  char *pipe_name = argv[5];

  initialize_controller(start_hour, total_capacity, pipe_name);

  pthread_create(&time_thread, NULL, time_handler, NULL);
  pthread_create(&agent_thread, NULL, agent_listener, NULL);

  pthread_join(time_thread, NULL);
  pthread_join(agent_thread, NULL);

  unlink(pipe_name); // Remove the FIFO file

  return 0;
}