/*
 * Programación Avanzada: E/S de archivos
 * Autor: Alejandro Salmon Felix Diaz
 * A01201954
 * Fecha de entrega: 26/ago/2018
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define BLOCK 128

int main(int argc, char* argv[]){
	int rotacion, f_origin, f_destin;
	char buffer[BLOCK];

	//verifica que se den parametros correctos
	if (argc != 4) {
		printf("usage: encrypt number orign destination \n");
		return -1;
	}
	//verifica que primer argumento sea entre 1 y 128
       	else if ((rotacion = atoi(argv[1])) < 1 || (rotacion = atoi(argv[1])) > 128){
		printf("%s: the first parameter must be a positive integer number between 1 and 128\n", argv[0]);
		return -1;
	}
	rotacion = rotacion % 128;	
	//verifica que el archivo de origen exista
	if ((f_origin = open(argv[2], O_RDONLY))< 0) {
		printf("%s: File: \'%s\' doesn't exist in directory \n", argv[0], argv[2]);
		return -1;
	}
	//verifica que exista o se haya creado un nuevo archivo
	else if ((f_destin = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, 0666)) < 0 ) {
		printf("%s: Could not open destination file: %s", argv[0], argv[3]);
		return -1;
	} else {
		// Magia negra para encriptar por bloque 
		// ( ͡° ͜ʖ ͡°)
		while (read(f_origin, buffer, BLOCK) == BLOCK) {
			write(f_destin, &buffer[rotacion], (BLOCK - rotacion));
			write(f_destin, buffer, rotacion);
			printf("Wait for it... \n");
		}
		printf("Done (AKA: Listo calixto) \n");
	}

	//cerrar apuntadores a archivos
	close(f_origin);
	close(f_destin);
	return 0;

}

