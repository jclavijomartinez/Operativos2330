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
void send_reservations(const char *controller_pipe,
                       const char *reservation_file) {
  int pipe_fd;
  FILE *file = fopen(reservation_file, "r");
  char line[256];

  if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  // Abrir el pipe en modo escritura
  pipe_fd = open(controller_pipe, O_WRONLY);
  if (pipe_fd == -1) {
    perror("Error opening pipe");
    exit(EXIT_FAILURE);
  }

  printf("Enviando solicitudes de reserva...\n");
  while (fgets(line, sizeof(line), file)) {
    printf("Enviando: %s", line);
    // Enviar la solicitud al controlador
    if (write(pipe_fd, line, strlen(line)) == -1) {
      perror("Error writing to pipe");
      exit(EXIT_FAILURE);
    }
    sleep(2); // Esperar 2 segundos entre envíos
  }

  fclose(file);
  close(pipe_fd);
}

// Punto de entrada principal
int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr,
            "Uso: %s <nombre_agente> <archivo_solicitudes> <nombre_pipe>\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }

  const char *agent_name = argv[1];
  const char *reservation_file = argv[2];
  const char *controller_pipe = argv[3];

  printf("Agente %s iniciado.\n", agent_name);
  send_reservations(controller_pipe, reservation_file);

  printf("Agente %s terminado.\n", agent_name);
  return 0;
}