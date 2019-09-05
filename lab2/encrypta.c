#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <libgen.h>
#include <sys/types.h>

//EXAMEN FINAL
//Esteban Quintana Cueto - a01184855

int main(int argc, char** argv) {

	int encriptado;
	int decriptado;

    char* crypted;
    char* unencrypted;

	unsigned char caracter;


	if (argc != 3) {
		printf("forma de uso: origen destino\n");
		return -1;
	}

    crypted = argv[1];

	if ((encriptado = open(crypted, O_RDONLY)) < 0) {
		printf("El archivo %s no existe.\n", crypted);
		return -1;
	}

	unencrypted = argv[2];

	if ((decriptado = open(unencrypted, O_WRONLY|O_CREAT|O_TRUNC, 0666)) < 0) {
        printf("Error al crear el archivo %s con el mensaje decriptado.\n",unencrypted);
		return -1;
	}


	while (read(encriptado, &caracter, 1)) {
        //hacer un shift de 4 localidades
		caracter = (caracter << 4) | (caracter >> 4);
		write(decriptado, &caracter, 1);
	}


	close(encriptado);

	close(decriptado);

	return 0;
}