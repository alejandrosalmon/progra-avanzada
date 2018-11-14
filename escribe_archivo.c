/*
 *
 *	Alejandro Salmón
 *	A01201954
 *	14/ago/2018
 *
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
	int fd_in, fd_out;
	
	//Verifica que se den 3 argumentos apra la función
	if (argc != 3) {
		printf("usage: %s origin destination\n", argv[0]);
		return -1;
	}
	
	//Verifica que el archvo de origen exista, en caso de que no devuelve errror
	if ((fd_in = open(argv[1], O_RDONLY)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	//Verifica que el archivo de destino existe, en caso de que no lo crea
	if ((fd_out = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	

	//Define un buffer para la lectura, mientras haya algo en el archivo de lectura
	//seguira escribiendolo en el archivo de destino desde el buffer byte por byte	
	char buffer[[2048];
	ssize_t nbytes;

	while (( nbytes = read(fd_in, &buffer, sizeof(buffer))) != 0) {
			write(fd_out, &buffer, 2048);
	}
	
	return 0;
}


