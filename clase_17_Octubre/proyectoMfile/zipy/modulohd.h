/**********************
 * Fecha: 24-10-2023
 * Autores: 
 *  - Juan Sebastian Clavijo Martinez (jsebastian.clavijoc@javeriana.edu.co), 
 *  - Santiago Mesa Niño (santiagoa.mesan@javeriana.edu.co), 
 *  - Juliana Lugo Martínez (julugo@javeriana.edu.co)
 * Tema: proyecto 1
 * Objetivo: archivo de modulo hdispersas, que contiene los prototipos de funciones
 *********************/

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

#ifndef MODULOPD_H_INCLUDE

#define MODULOPD_H_INCLUDE

/***PROTOTIPOS DE FUNCIONES***/

void start_timer();
void end_timer();
bool divisionhorizontal(int numpor, int nfilas, int numcols, int numprocesos, int ***matriz);
void *threadFuncHorizontal(void *arg);
void *threadFuncVertical(void *arg);
bool divisionhorizontal(int numpor, int nfilas, int numcols, int numthreads, int ***matriz);
bool divisionvertical(int numpor, int nfilas, int ncols, int numthreads, int ***matriz);
bool filasycolsdelarchivo(char *archivo, int filas, int cols);
void printmat(int filas, int cols, int** matrix);
void **crearmatriz(int nfil, int ncol, int*** mat);
void cargarmatriz(FILE *arch, char *archivo, int filas, int cols, int ***matriz);


#endif
