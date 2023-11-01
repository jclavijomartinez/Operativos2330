# DETERMINACION DE MATRICES DISPERSAS CON PROCESOS E HILOS

## Autores

- [JUAN SEBASTIÁN CLAVIJO MARTÍNEZ](https://github.com/jclavijomartinez/sistemasOperativos2330/tree/produccion/clase_17_Octubre/proyectoMfile)
- [SANTIAGO MESA NIÑO](santiagoa.mesan@javeriana.edu.co)
- [JULIANA LUGO MARTÍNEZ](julugo@javeriana.edu.co)

## Descripción

El proyecto se enfoca en la implementación de dos programas ejecutables, "pdispersa" y "hdispersa", que buscan determinar si una matriz dada es dispersa de manera concurrente. Una matriz dispersa es aquella en la que la mayoría de sus elementos son cero. Para lograr esto, se utilizarán dos enfoques de concurrencia: procesos e hilos.

Los programas recibirán como argumentos las dimensiones de la matriz, la ubicación de un archivo que contiene la matriz en formato de texto, el número de procesos o hilos concurrentes a crear y el porcentaje de ceros necesario para clasificar la matriz como dispersa.

## Objetivo

El objetivo principal de este proyecto es implementar dos programas ejecutables, "pdispersa" y "hdispersa", que utilicen la concurrencia a través de procesos e hilos para determinar si una matriz dada se puede considerar dispersa o no. El término "matriz dispersa" se refiere a matrices de gran tamaño en las que la mayoría de sus elementos son cero.

En esencia, estos programas deben analizar una matriz, cuyas dimensiones y datos se proporcionan como entrada, y determinar si el porcentaje de ceros en la matriz cumple con cierto umbral para ser considerada dispersa. El proyecto busca lograr esta clasificación de manera eficiente y concurrente, lo que puede ser útil en aplicaciones que involucren el manejo de matrices extensas en el ámbito de la álgebra lineal numérica u otras disciplinas relacionadas.

## Características Principales

- compila usando un Makeflie, para comodidad del usuario
- no imprime la matriz, para facilidad de lectura!
- Mide el tiempo de ejecución, sin necesidad de usar la funcion time

### Requisitos Previos

Antes de ejecutar revisar el siguiente checlist:

- [ ] Estar dentro de una terminal de linux, en cualquiera de sus distros (preferiblemente ubuntu), que tenga el kernel 6.2.0-35-generic o posterior
- [ ] Verificar que tenga la herramienta UnZip, preferiblemente version 6.00 o posterior, en caso de no tenerla usar el comando sudo apt install unzip -y o el equivalente en la distro que use el lector de linux
- [ ] Verificar que tenga la herramienta make, en caso de no tenerla usar el comando sudo apt install make -y o el equivalente en la distro que use el lector de linux
- [ ] Descargar el archivo entrega2proyectoClavijoMesaLugo.zip
- [ ] Crear una nueva carpeta y mover el .zip a ella
- [ ] Descomprimir el archivo entrega2proyectoClavijoMesaLugo.zip con el comando unzip entrega2proyectoClavijoMesaLugo.zip

## Cómo Usar

1. Una vez dentro de la carpeta donde se descomprimió el .zip, ejecute el comando:

`$ make`

![descomprimido](https://github.com/jclavijomartinez/sistemasOperativos2330/blob/produccion/clase_17_Octubre/proyectoMfile/img/Screenshot_1.png)

![ejecucion make](https://github.com/jclavijomartinez/sistemasOperativos2330/blob/produccion/clase_17_Octubre/proyectoMfile/img/Screenshot_2.png)

2. Para ejecutar el codigo con procesos use la instruccion:
`$ ./principalpd -f 80 -c 80 -a ejemplo280X80.txt -n 2 -p 90`
(RECUERDE QUE EL NUMERO DE PROCESOS DEBE SER PAR Y MENOR QUE EL NUMERO DE NUCLEOS DEL PROCESADOR
DEL COMPUTADOR EN DONDE SE EJECUTE EL CÓDIGO) 

Puedes cambiar diferentes parámetros dependiendo de la matriz que desees evaluar como `-f ## y -c ##`, por el numero de filas y columnas del archivo que desea evaluar, `nombreDeTuArchivo.txt` por el nombre del archivo que desees evaluar, `-n #` por el numero de procesadores que deseas uasar, y `-p ##` por el porcentaje que desees evaluar si la matriz es dispersa o no.

![ejecucion procesos](https://github.com/jclavijomartinez/sistemasOperativos2330/blob/produccion/clase_17_Octubre/proyectoMfile/img/Screenshot_3.png)

3. Para ejecutar el codigo con hilos use la instruccion: `./principalhd -f 80 -c 80 -a ejemplo280X80.txt -n 15 -p 90`

Puedes cambiar diferentes parámetros dependiendo de la matriz que desees evaluar como `-f ## y -c ##`, por el numero de filas y columnas del archivo que desea evaluar, `nombreDeTuArchivo.txt` por el nombre del archivo que desees evaluar, `-n #` por el numero de hilos que deseas uasar, y `-p ##` por el porcentaje que desees evaluar si la matriz es dispersa o no.

![ejecucion hilos](https://github.com/jclavijomartinez/sistemasOperativos2330/blob/produccion/clase_17_Octubre/proyectoMfile/img/Screenshot_4.png)

4. Si quiere limpiar los ejecutables use el comando make clean
![make clean](https://github.com/jclavijomartinez/sistemasOperativos2330/blob/produccion/clase_17_Octubre/proyectoMfile/img/Screenshot_5.png)
