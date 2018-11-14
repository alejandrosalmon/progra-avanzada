/******************************************
 *  Programación Avanzada: Apuntadores y arreglos
 *  Fecha de entrega: 19-ago-2018
 *  Autor: Alejandro Salmón Félix Díaz 
 *
 * *************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef unsigned int uint;
void free_matrix();

typedef struct {
	double *data;
	uint nrows, ncols;
} Matrix;

void printm(Matrix *M) {
	/*
	 * printf("Here you have to display the matrix with the format:\n");
	 * printf(" a0,0 a0,1 ... a0,n-1,\n");
	 * printf(" a1,0 a1,1 ... a0,n-1,\n");
	 * printf(" ...\n");
	 * printf(" an-1,1 an-1,2 ... an-1,n-1\n");
	*/
	uint numrows = M-> nrows;
	uint numcols = M -> ncols;
	for(int i = 0; i < numrows; i++){
		for(int j = 0; j < numcols; j++){
			printf(" %.2f ", M -> data[(numcols * i) + j]);
		}
		printf("\n");
	}
	printf("\n");
}

Matrix* alloc_matrix(uint nrows, uint ncols) {
/****************************************
 * This procedure should request the amount of memory 
 * needed for the Matrix structure and for the storage 
 * of a matrix of the required dimensions. If you can 
 * not get that amount of memory, return NULL.
 * Check that nrows and ncols must be greater than 0.
 * If you have allocated memory and there is an error,
 * you must free all the memory allocated.
 ****************************************/
	Matrix* Mat = malloc(sizeof(Matrix));
	if(Mat != NULL){ 
		if(nrows  > 0 && ncols > 0) {
			Mat -> data = (double *)malloc(nrows * ncols * sizeof(double)); 
			Mat -> nrows = nrows;
			Mat -> ncols = ncols;
			return Mat;
		} else {
			printf("nrows and ncols must be greater than zero \n");
		}
	} else {
		printf("Matrix is NULL");
	}
	return NULL;
}

void set(Matrix *M, uint row, uint col, double val) {
/****************************************
 * Check that the row and column is valid for the matrix, 
 * if not, it displays an error. Otherwise, place the value 
 * in the correct cell. Check the following links:
 * https://stackoverflow.com/questions/2151084/map-a-2d-array-onto-a-1d-array
 * https://stackoverflow.com/questions/14015556/how-to-map-the-indexes-of-a-matrix-to-a-1-dimensional-array-c
 ****************************************/
	if(M == NULL){
		printf("the matrix must be valid \n");
	} else {
		uint ncols = M -> ncols;
		uint nrows = M -> nrows;
		if(row >= 0 && row < nrows){
			if(col >=0 && col < ncols){
				M -> data[(ncols * row) + col] = val;
			} else{
				printf("Invalid column \n");
			}
		} else {
			printf("Invalid row \n");
		}
	}
	return;
}

void matrix_mult(Matrix *A, Matrix *B, Matrix *C) {
/****************************************
 * It must be checked that the multiplication can be done 
 * (check the following links:
 * https://es.wikipedia.org/wiki/Multiplicaci%C3%B3n_de_matrices
 * https://www.geogebra.org/m/S6R8A2xD
 * ). If it can not be done, you must display an error message.
 * Remember check for null pointer. 
 * 
 * The multiplication is A x B = C
 ****************************************/
	if(( A != NULL) && (B != NULL)) {
		if((A -> nrows == C -> nrows) && (B -> ncols == C -> ncols)) {
			int crows = C -> nrows;
			int ccols = C -> ncols;
			float aux = 0;
			for(int i = 0; i < crows; i++) {
				for( int j = 0; j < ccols; j++) {
					for(int z = 0; z < B -> nrows; z++) {
						aux += (A -> data[((A -> ncols) * i) + z]) * (B -> data[((B -> ncols) * z) + j]);
					}
					C -> data[((C -> ncols) * i) + j] = aux;
					aux = 0;
				}
			}
		} else {
			printf("A -> rows must be equal to C->rows \n");
			printf("B -> cols must be equal to C->rows \n");
		}
	} else {
		printf("the matrixes must be valids \n");
	}
}

void free_matrix(Matrix *M) {
/****************************************
 * If the element that is received is different from NULL, 
 * the space assigned to the array and the structure must be 
 * freed.
 ****************************************/
	if(M != NULL){
		free(M -> data);
		free(M);
	} else {
		printf("the matrix must be a valid one \n");
	}
}


int main(int argc, char* argv[]) {
	printf("Creating the matrix A:\n");
	Matrix *A = alloc_matrix(3, 2);
	
	printf("Setting the matrix A:\n");
	set(A, 0, 0, 1.2);
	set(A, 0, 1, 2.3);
	set(A, 1, 0, 3.4);
	set(A, 1, 1, 4.5);
	set(A, 2, 0, 5.6);
	set(A, 2, 1, 6.7);
	printf("Printing the matrix A:\n");
	printm(A);
	
	printf("Creating the matrix B:\n");
	Matrix *B = alloc_matrix(2, 3);
	printf("Setting the matrix B:\n");
	set(B, 0, 0, 5.5);
	set(B, 0, 1, 6.6);
	set(B, 0, 2, 7.7);
	set(B, 1, 0, 1.2);
	set(B, 1, 1, 2.1);
	set(B, 1, 2, 3.3);
	printf("Printing the matrix B:\n");
	printm(B);
	
	printf("Creating the matrix C:\n");
	Matrix *C = alloc_matrix(3, 3);
	printf("A x B = C:\n");
	matrix_mult(A, B, C);
	printf("Printing the matrix C:\n");
	printm(C);
	
	printf("B x A = C:\n");
	matrix_mult(B, A, C);
	printf("Printing the matrix C:\n");
	printm(C);
	
	Matrix *D = NULL;
	printf("Setting a NULL matrix (D):\n");
	set(D, 0, 0, 10);
	
	printf("A x D(NULL) = C:\n");
	matrix_mult(A, D, C);
	printf("D(NULL) x A = C:\n");
	matrix_mult(D, A, C);
	
	printf("Allocating E with (0,0): \n");
	Matrix *E = alloc_matrix(0, 0);
	
	printf("Freeing A:\n");
	free_matrix(A);
	printf("Freeing B:\n");
	free_matrix(B);
	printf("Freeing C:\n");
	free_matrix(C);
	printf("Freeing D(NULL):\n");
	free_matrix(D);
	printf("Freeing E(NULL):\n");
	free_matrix(E);

	return 0;
}
