/****************************************************************
 * Fecha: 29 - Agosto - 2023
 * Autor: Juan Sebastian Clavijo Martinez
 * Clase: Sistemas Operativos
 * Objetivo: RR scheduler, pide por teclado: TQ, procesos, arrival time, burst time 
 * imprime una tabla con completion time, TAT, waiting time 
 ***************************************************************/

#include <stdio.h>

// Estructura para representar un proceso tiene los parametros relevantes
typedef struct {
    int arrival_time;       // Tiempo de llegada del proceso
    int burst_time;         // Tiempo de ráfaga del proceso
    int remaining_time;     // Tiempo restante de ejecución
    int completion_time;    // Tiempo de completación
    int turnaround_time;    // Tiempo de retorno
    int waiting_time;       // Tiempo de espera
} Proceso;

int main() {
    // Pedir valores por teclado
    int quantum_time;
    int num_procesos;
    printf("Ingrese el Quantum Time: ");
    scanf("%d", &quantum_time);
    printf("Ingrese la Cantidad de Procesos: ");
    scanf("%d", &num_procesos);

    // Definir un array de procesos según la cantidad ingresada
    Proceso procesos[num_procesos];

    // Ingresar los tiempos de llegada y ráfaga para cada proceso
    for (int i = 0; i < num_procesos; i++) {
        printf("Proceso %d:\n", i + 1);
        printf("   Ingrese Arrival Time: ");
        scanf("%d", &procesos[i].arrival_time);
        printf("   Ingrese Burst Time: ");
        scanf("%d", &procesos[i].burst_time);
        procesos[i].remaining_time = procesos[i].burst_time;
    }

    int tiempo_actual = 0;    // Tiempo actual de simulación
    int procesos_restantes = num_procesos;

   // Simulación de la planificación Round Robin
    while (procesos_restantes > 0) {
        for (int i = 0; i < num_procesos; i++) {
        // Obtener un puntero al proceso actual para facilitar la manipulación
            Proceso *p = &procesos[i];
        
        // Verificar si el proceso tiene tiempo de ejecución restante
            if (p->remaining_time > 0) {
            // Calcular el tiempo de ejecución en este quantum
                int tiempo_ejecucion = quantum_time < p->remaining_time ? quantum_time : p->remaining_time;
            
            // Actualizar el tiempo actual de simulación
                tiempo_actual += tiempo_ejecucion;
            
            // Reducir el tiempo de ejecución restante del proceso
                p->remaining_time -= tiempo_ejecucion;

            // Verificar si el proceso se ha completado en este ciclo
                if (p->remaining_time == 0) {
                // Actualizar el tiempo de completación del proceso
                    p->completion_time = tiempo_actual;
                
                // Calcular el tiempo de retorno (turnaround time) del proceso
                    p->turnaround_time = p->completion_time - p->arrival_time;
                
                // Calcular el tiempo de espera (waiting time) del proceso
                    p->waiting_time = p->turnaround_time - p->burst_time;
                
                // Disminuir el contador de procesos restantes por ejecutar
                    procesos_restantes--;
            }
        }
    }
} 

    // Mostrar la tabla con los tiempos calculados para cada proceso
    printf("\nProceso\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < num_procesos; i++) {
        printf("%d\t%d\t\t%d\t\t%d\n", i + 1, procesos[i].completion_time, procesos[i].turnaround_time, procesos[i].waiting_time);
    }

    return 0;
}
