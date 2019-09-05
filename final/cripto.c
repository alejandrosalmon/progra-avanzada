/*
 * Programación Avanzada: Examen Final
 * Autor: Alejandro Salmon Felix Diaz
 * A01201954
 * Fecha de entrega: 27/nov/2018
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define BLOCK 8

int main(int argc, char* argv[]){
	int rotacion, f_origin, f_destin;

	unsigned char bit;

	if (argc != 3){
		printf("usage: %s origen destination\n", argv[0]);
		return -1;
	}

	if(( f_origin = open(argv[1], O_RDONLY))<0){
		printf("%s: no such file or directory.\n", argv[0]);
		return -1;
	}

	if((f_destin = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0666)) < 0){
		printf("%s: could not open destination file.\n", argv[0]);
		return -1;
	}

	while (read(f_origin, &bit, 1)){
		// cambia los primeros 4 bits del bit al final.
		// bitwise OR
		bit = (bit >> BLOCK/2) | (bit << BLOCK/2);
		write(f_destin, &bit, 1);
	}

	close(f_origin);
	close(f_destin);


	printf("done.\n\n");

	return 0;
}



