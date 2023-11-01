/*

25-06-2023
AUTOR: JUAN CLAVIJO
OBJETIVO: crear app de multiplicacion de matrices en compilacion separada
NOTAS:

*/
#include<stdio.h>
#include<stdlib.h>

/*se reserva memoria*/
#define SIZE_DATA (1024*1024*8)

static double MEM_CHUNK[SIZE_DATA];//RESERVA DE TAMAÑO


//inicializacion de matrices
void init_Matriz(int n, double *a, double *b, double *c){

	for(int i=0; i< n; i++){
		for(int j=0; j< n; j++){
			a[j+i*n] = 2.1+i;
			b[j+i*n] = 3.2+i;
			c[j+i*n] = 0.0+i;
		}
	}
}


void imprimir_Matriz(int n, double *a){
	
	//visualizacion de las matrices
	for(int i=0; i< n; i++){
		for(int j=0; j< n; j++){
			printf(" %f ", a[j+i*n]);
		}
		printf("\n");
	}
	printf("\n");
}


int main(int argc, char *argv[]){
	/*se captura el argumento de entrada*/
	int N = (int) atof (argv[1]);
	/**/
	if(argc < 2){
		return -1;
	}
	// se reserva el espacio para las matrices con punteros
	double *A = MEM_CHUNK;
	double *B = A + N*N;
	double *C = B + N*N;
	
	
	init_Matriz(N, A, B, C);
	imprimir_Matriz(N, A);
	imprimir_Matriz(N, B);
	imprimir_Matriz(N, C);
	
	/* calculo de multiplicacion de matrices:
	 * se empleara el algoritmo clasico de filas x columnas
	 * se requiere que la manipulacion de las matrices sea por uso de 
	 * punteros
	 */
	for(int i=0; i< N; i++){
                for(int j=0; j< N; j++){
			/* punteros auxiliares y resultado de calc*/
			double *ptrauxA,*ptrauxB,Calc=0;
			/*se apuntan los punteros segun el algo*/
			ptrauxA=A+(N*i);
			ptrauxB=B+j;
			for(int k=0;k < N;k++,ptrauxA++,ptrauxB+=N){
				Calc+=(*ptrauxA * *ptrauxB);
			}
			C[i*N+j] = Calc;
		}
	}	
	
	
//	printf("%d \n",N);	
	
	printf("matriz resultado: \n");
	imprimir_Matriz(N, C);

	return 0;
}
