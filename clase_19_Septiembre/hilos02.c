/****************************************************************
 * Fecha: 19 - septiembre - 2023
 * Autor: Juan Sebastian Clavijo Martinez (jsebastian.clavijoc@javeriana.edu.co)
 * Materia: Sistemas Operativos
 * Tema: POSIX
 * Fuente: https://github.com/codophobia/producer-consumer-problem-solution-in-c/blob/master/producer-consumer.c
 * Problema: El problema del productor-consumidor trata sobre proccesos concurrentes,
 * como (González-Nalda, 2019) comenta "Plantearemos el problema de un proceso productor que se encarga de crear 
 * un tipo de elementos que otro proceso (el consumidor) necesita consumir para continuar con su ejecución. Ambos utilizan 
 * como canal de comunicación un Buffer finito que está en memoria compartida sobre el que depositarán y recogerán los elementos producidos." 
 * Se puede notar que para resolver esta situacion en necesario usar herramientas de POSIX como pthread.
 * 
 * REFERENCIA:
 * González-Nalda, P. (2019). Problema del Productor Consumidor resuelto con Semáforos. Ehu.eus. https://lsi.vc.ehu.eus/pablogn/docencia/manuales/SO/TemasSOuJaen/CONCURRENCIA/ProductorConsumidor/problema_del_productor_consumidor_resuelto_con_semaforos.html
 ***************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//esta una modificacion del codigo, cambio el maximo de items y el tamaño del buffer
#define MaxItems 15 // Máximo de elementos que un productor puede producir o un consumidor puede consumir
#define BufferSize 15 // Tamaño del búfer

int in = 0;  // Índice de inserción en el búfer
int out = 0; // Índice de extracción del búfer
int buffer[BufferSize]; // Búfer compartido

int itemCount = 0; // Contador de elementos en el búfer

void *producer(void *pno)
{   
    int item;
    for(int i = 0; i < MaxItems; i++) {
        item = rand(); // Producir un elemento aleatorio

        while (itemCount == BufferSize) {
            // Esperar si el búfer está lleno (spinlock)
        }

        buffer[in] = item;
        printf("Producer %d: Insert Item %d at %d\n", *((int *)pno), buffer[in], in);
        in = (in + 1) % BufferSize; // Actualizar el índice de inserción
        
        itemCount++; // Incrementar el contador de elementos en el búfer
    }
}

void *consumer(void *cno)
{   
    for(int i = 0; i < MaxItems; i++) {
        while (itemCount == 0) {
            /* Esperar si el búfer está vacío (spinlock) esta es la mayor modificacion del codigo,
            el acercamiento de spinlock que es un mecanismo de sincronización en programación 
            concurrente que se utiliza para evitar que múltiples hilos accedan o modifiquen 
            simultáneamente una sección crítica de código o recursos compartidos*/
        }

        int item = buffer[out];
        printf("Consumidor %d: quito el Item %d de %d\n", *((int *)cno), item, out);
        out = (out + 1) % BufferSize; // Actualizar el índice de extracción
        
        itemCount--; // Decrementar el contador de elementos en el búfer
    }
}

int main()
{   
    pthread_t pro[5], con[5];

    int a[5] = {1, 2, 3, 4, 5}; // Utilizado para numerar a los productores y consumidores

    for(int i = 0; i < 5; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]); // Crear hilos de productores
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]); // Crear hilos de consumidores
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(pro[i], NULL); // Esperar a que los hilos de productores terminen
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(con[i], NULL); // Esperar a que los hilos de consumidores terminen
    }

    return 0;
}
