/****************************************************************
 * Fecha: X - Octubre - 2023
 * Autor: Juan Sebastian Clavijo Martinez
 * Tema:
 * Objetivo:
 ***************************************************************/

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

// Definición de la estructura para mantener el estado del parque
struct ParkStatus {
  int current_time;
  int max_capacity;
  int current_capacity;
  // ... otras variables de estado
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
  mkfifo(pipe_name, 0666);
}

// Función para manejar el paso del tiempo
void *time_handler(void *args) {
  while (park_status.current_time < 24) {
    sleep(1); // Simula una hora con sleep
    park_status.current_time++;
    // Manejar la lógica de cambio de hora aquí
  }
  return NULL;
}

// Función para escuchar y procesar las solicitudes de los agentes
void *agent_listener(void *args) {
  int pipe_fd;
  char buffer[1024];

  // Abrir el pipe en modo lectura
  pipe_fd = open(pipe_name, O_RDONLY);

  while (1) {
    read(pipe_fd, buffer, sizeof(buffer));
    // Procesar la solicitud aquí
    // ...
  }
  close(pipe_fd);
  return NULL;
}

// Punto de entrada principal
int main(int argc, char *argv[]) {
  pthread_t time_thread, agent_thread;

  // Inicializar el controlador con los argumentos apropiados
  initialize_controller(7, 100, "reservation_pipe");

  // Crear hilos para manejar el tiempo y las solicitudes de los agentes
  pthread_create(&time_thread, NULL, time_handler, NULL);
  pthread_create(&agent_thread, NULL, agent_listener, NULL);

  // Esperar a que los hilos terminen
  pthread_join(time_thread, NULL);
  pthread_join(agent_thread, NULL);

  return 0;
}
