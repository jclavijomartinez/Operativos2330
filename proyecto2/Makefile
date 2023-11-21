# Nombre de los archivos ejecutables
CONTROLADOR = controlador
AGENTE = agente

# Compilador a utilizar
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -Wextra

# Fuentes del proyecto
SOURCES = ControladorPrincipal.c agente.c

# Objetivos intermedios generados durante la compilación
OBJECTS = $(SOURCES:.c=.o)

# Regla para construir los archivos ejecutables
all: $(CONTROLADOR) $(AGENTE)

$(CONTROLADOR): ControladorPrincipal.o Controlador.o
	$(CC) $(CFLAGS) $^ -o $@

$(AGENTE): agente.o
	$(CC) $(CFLAGS) $^ -o $@

# Regla para compilar cada archivo fuente en un objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados durante la compilación
clean:
	rm -f $(OBJECTS) $(CONTROLADOR) $(AGENTE)

