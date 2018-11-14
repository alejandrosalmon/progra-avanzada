/*----------------------------------------------------------------
* Programación avanzada: Cifrado de un archivo
* Fecha: 26 de agosto de 2018
* Autor: Daniel Amezcua Sánchez A01234223
*--------------------------------------------------------------*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define BLOCKSIZE 128

int main(int argc, char* argv[]){
	int fd_origin, fd_destination, shift;
	char buffer[BLOCKSIZE];
	/* Verificar que se hayan recibido tres argumentos */
	if (argc != 4){
		printf("usage: %s number origin destination\n", argv[0]);
		return -1;
	}
	/* Verificar que el primer argumento sea un número positivo */
	if ((shift = atoi(argv[1])) < 1 ){
		printf("%s: the first parameter must be a positive integer number\n", argv[0]);
		return -1;
	}
	shift = shift % 128;

	/* Verificar que el archivo origen exista */
	if ((fd_origin = open(argv[2], O_RDONLY)) < 0 ) {
		printf("%s: No such file or directory\n", argv[0]);
		return -1;
	}

	/* Abrir y crear nuevo archivo */
	if ((fd_destination = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY)) < 0){
		printf("%s: Could not open destination file\n", argv[0]);
		return -1;
	}

	/* Leer por bloques de 128 bytes hasta el final del archivo */
	while (read(fd_origin, buffer, BLOCKSIZE) == BLOCKSIZE) {
		/* Escribir primera parte del nuevo buffer */
		write(fd_destination, &buffer[shift], (BLOCKSIZE - shift));

		/*Escribir segunda parte del buffer */
		write(fd_destination, buffer, shift);
	}
	printf("Done\n");
	/* Cerrar file descriptors */
	close(fd_origin);
	close(fd_destination);
	return 0;
}
