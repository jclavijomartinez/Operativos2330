/*******************************************************
*Autor: Juan Sebastian Clavijo Martinez
*Fecha: 17 octubre 2023
*Objetivo: Interfaz de prototipo de funciones 
           disponible modularmente
*******************************************************/

#include<stddef.h>

#ifndef MODULO_H_INCLUDE
#define MODULO_H_INCLUDE

/***PROTOTIPOS DE FUNCIONES***/
size_t **initMatriz(size_t N); //funcion que aparta los recursos de memoria y llena con nbumeros entre 0 y 100
void imprimirMM(size_t **mm, size_t N); //funcion que imprime la metriz en pantalla

#endif
