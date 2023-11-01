# Nombre del archivo ejecutable
TARGET = pdispersa hdispersa

# Compilador a utilizar
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -Wextra -lm

# Fuentes del proyecto (agrega aquí tus archivos fuente)
SOURCES = pdispersa.c hdispersa.c

# Objetivos intermedios generados durante la compilación
OBJECTS = $(SOURCES:.c=.o)

# Regla para construir el archivo ejecutable
all: $(TARGET)

$(TARGET): $(OBJECTS)
    $(CC) $(CFLAGS) $(OBJECTS) -o $@

# Regla para compilar cada archivo fuente en un objeto
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados durante la compilación
clean:
    rm -f $(OBJECTS) $(TARGET)

