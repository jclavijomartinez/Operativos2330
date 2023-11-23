# Proyecto de Reserva en Parque Berlín

## Objetivos del Proyecto

El objetivo de este proyecto es resolver un problema concurrente mediante el uso de procesos y hilos de la librería POSIX. Se utilizarán pipes nominales para la comunicación entre procesos y señales para realizar operaciones periódicas y comunicación asincrónica. Además, se familiarizará con llamadas al sistema para la gestión de procesos e hilos y el manejo de archivos.

## Descripción del Proyecto

El proyecto consiste en implementar un sistema de reserva de horas en el parque privado "Berlín". Debido a la pequeñez del parque y la alta demanda durante las vacaciones, se ha decidido implementar un sistema de reservas para controlar el aforo máximo. El sistema seguirá una arquitectura cliente/servidor, donde el servidor, llamado "Controlador de Reserva", gestionará las solicitudes de reserva realizadas por los "Agentes de Reserva", que son los clientes.

## Controlador de Reserva

El "Controlador de Reserva" tiene las siguientes funciones:

- Recibe y autoriza o rechaza las solicitudes de los Agentes de Reserva según la disponibilidad.
- Simula el tiempo transcurrido, gestionando la entrada y salida de personas del parque.
- Emite un informe al final del día sobre la ocupación del parque.

### Implementación del Controlador de Reserva

El proceso se invoca desde el shell de la siguiente forma:

```bash
$ ./controlador –i horaInicio –f horafinal –s segundoshora –t totalpersonas –p pipecrecibe

Donde:

nombre: nombre del Agente.
archivosolicitudes: archivo de entrada con las solicitudes de reserva.
pipecrecibe: pipe nominal para la comunicación inicial con el controlador.
Detalle de las Funciones del Agente:

Invocación y Registro con el Controlador:

El Agente se registra con el Controlador enviando su nombre y cualquier información adicional necesaria.
Se abre el pipe de comunicación con el Controlador.
Lectura del Archivo y Solicitudes de Reserva:

El Agente lee una línea del archivo de solicitudes.
Valida que la hora de reserva no sea inferior a la hora actual de simulación.
Envía la solicitud al Controlador y espera la respuesta.
Imprime la respuesta (positiva, reprogramación o denegada).
Espera 2 segundos y repite el proceso.
Fin de Archivo:

Cuando el archivo termina, el Agente emite un mensaje indicando su terminación.
Reporte Final del Controlador:

Al finalizar la simulación, el Controlador imprimirá un informe que incluye:

Horas pico.
Hora u horas con menor número de personas.
Número de solicitudes negadas.
Número de solicitudes aceptadas en su hora.
Número de solicitudes reprogramadas.
El Controlador debe finalizar a los Agentes con una señal o enviándoles un mensaje para que terminen.
