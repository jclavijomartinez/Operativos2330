/*********
 * SISTEMAS OPERATIVOS
 * FECHA: 08-08-2023
 * AUTOR: JUAN CLAVIJO
 * EJERCICIO 2 - punteros dobles 
 * matriz que reciba los parametros de fil y cols desde terminal y que trabaje con punteros dobles
 * ****/
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int rows,cols,i,j;
	int **mat;
	rows = (int) atof(argv[1]);
	cols = (int) atof(argv[2]);
	mat = (int **)malloc(rows*sizeof(int *));
	for (i=0;i<rows;i++){
		mat[i]=(int *)malloc(cols * sizeof(int));
	}
	//se llena la mat con valores iniciales
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			mat[i][j]=i*j;
		}
	}
	//se imprime la mat
	for(i=0;i<rows;i++){ 
                for(j=0;j<cols;j++){
                        printf(" %d ",mat[i][j]=i*j);
                }                
		printf("\n");
        } 	
	//libero memoria
	
	
	for(int i=0;i<rows;i++){
		free(mat[i]);
	}
	free(mat);
	return 0;
}

