#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *agent_name;
char *reservation_file;
char *controller_pipe;

// Función para registrar el agente con el controlador
void register_agent() {
  int pipe_fd;
  // Abrir el pipe en modo escritura
  pipe_fd = open(controller_pipe, O_WRONLY);
  // Escribir el nombre del agente en el pipe
  write(pipe_fd, agent_name, strlen(agent_name) + 1);
  close(pipe_fd);
}

// Función para leer y enviar solicitudes de reserva
void send_reservations() {
  FILE *file = fopen(reservation_file, "r");
  char line[256];
  int pipe_fd;

  while (fgets(line, sizeof(line), file)) {
    // Enviar cada línea al controlador como una solicitud de reserva
    pipe_fd = open(controller_pipe, O_WRONLY);
    write(pipe_fd, line, strlen(line) + 1);
    close(pipe_fd);
    sleep(2); // Esperar entre envíos
  }

  fclose(file);
}

// Punto de entrada principal
int main(int argc, char *argv[]) {
  // Inicializar las variables globales con los argumentos apropiados
  agent_name = argv[1];
  reservation_file = argv[2];
  controller_pipe = argv[3];

  // Registrar el agente con el controlador
  register_agent();
  // Leer y enviar las solicitudes de reserva
  send_reservations();

  return 0;
}
