/*********
 * SISTEMAS OPERATIVOS QUIZ
 * FECHA: 08-08-2023
 * AUTOR: JUAN CLAVIJO
 * quiz 
 * programa que reciba como args 5 datos, los cuales deben ser del tipo int y double y hacer operaciones con estos datos
 * ****/
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int dato1=(int) atoi(argv[1]);
	int dato2=(int) atoi(argv[2]);
	int dato3=(int) atoi(argv[3]);
	double dato4=(double) atof(argv[4]); 
	double dato5=(double) atof(argv[5]);
	printf("la suma de los enteros (3 primeros datos que ingresaste) es = %d \t\n",dato1+dato2+dato3);
	printf("la suma de los doubles (2 ultimos datos que ingresaste) es =  %f \t\n",dato4+dato5);
	printf("el producto de los enteros (3 primeros datos que ingresaste) es = %d \t\n",dato1*dato2*dato3);
        printf("el producto de los doubles (2 ultimos datos que ingresaste) es = %f \t\n",dato4*dato5);
	printf("el promedio de los enteros (3 primeros datos que ingresaste) es = %d \t\n",(dato1+dato2+dato3)/3);
        printf("el promedio de los doubles (2 ultimos datos que ingresaste) es = %f \t\n",(dato4,dato5)/2);

}
