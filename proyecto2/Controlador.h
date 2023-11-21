#ifndef PARQUE_H
#define PARQUE_H

#include <pthread.h>

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
    int respuesta;
    int horaActual;
    int horaCierre;
};

extern int horaActual;
extern int personasEnParque;
extern int numVisitantesPorHora[24];
extern int solicitudesNegadas;
extern int solicitudesReprogramadas;
extern int totalPersonas;

extern pthread_mutex_t mutex;

void *esperaHora(void *datosEspera);
void *recibirSolicitud(void *datosPipe);
void procesarSolicitud(struct DatosSolicitud *datosSolicitud, int horaFinal);
void generarReporte(int horaI, int horaF);

#endif

