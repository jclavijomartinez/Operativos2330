# Compiler and flags
GCC = gcc
FLAGS = -Wall
CFLAGS = -lm

# Targets
all: principalhd principalpd

principalhd: principalhd.c modulohd.c
	$(GCC) $(FLAGS) principalhd.c modulohd.c -o principalhd $(CFLAGS)

principalpd: principalpd.c modulopd.c
	$(GCC) $(FLAGS) principalpd.c modulopd.c -o principalpd $(CFLAGS)

clean:
	rm -f principalhd principalpd

.PHONY: all clean
