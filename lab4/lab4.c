/*
 * Programación Avanzada: Normalización de archivos 
 * Autor: Alejandro Salmon Felix Diaz
 * A01201954
 * Fecha de entrega: 9/sep/2018
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>

//el nombre lo dice todo
int normaliza(char *directorio, char *directorio2, char *programa) {
	DIR  *dir;
	struct dirent *dirent;
	struct stat input_info;
	struct stat output_info;
	FILE *file_in, *file_out;
	char filename_in[PATH_MAX + NAME_MAX + 1];
	char filename_out[PATH_MAX + NAME_MAX + 1];
	double max = INT_MIN;
	double min = INT_MAX;
	double new_size;
	double line[627];
	

	// verifica que sean directorios y  se puedan abrir 
	// abre directorio de entrada
	if (((dir = opendir(directorio)) == NULL ) || (opendir(directorio2) == NULL)){
		perror(programa);
		exit(-1);
	}
	//recorre el dir
	while((dirent = readdir(dir)) != NULL) {
		if(strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") !=0) {
			sprintf(filename_in, "%s/%s", directorio, dirent->d_name);
			sprintf(filename_out, "%s/%s", directorio2, dirent->d_name);

			//abre archivo de entrada en modo lectura
			if ((file_in = fopen(filename_in, "r")) == NULL) {
				perror(programa);
				exit(-1);
			}

			//abre archivo de salida, si no existe lo crea, en modo de escritura
			if ((file_out = fopen(filename_out, "w")) == NULL) {
				perror(programa);
				exit(-1);
			}

			//lee linea por linea el archivo que se abrio
			while (fscanf(file_in, "%lf,%lf", &line[0], &line[1]) != EOF) {
				//escribe primerso dos numeros en archivo de salida
				fprintf(file_out, "%.2lf,%.2lf", line[0], line[1]);
				//asigna max y min
				for(int i=2; i<627; i++){
					fscanf(file_in, ",%lf", &line[i]);
					if(line[i] > max) {
						max = line[i];
					} else if(line[i] < min){
						min = line[i];
					}
				}	
				fscanf(file_in, "\n");
				//normaliza y escribe cada entrada
				for(int i=2;i<627; i++) {
					fprintf(file_out, ",%.2lf", (line[i] - min)/(max-min)); 
				}
				fprintf(file_out, "\n");

				min = INT_MAX;
				max = INT_MIN;
			}
			fclose(file_in);
			fclose(file_out);
		}
	}
	closedir(dir);
	printf("Done. \n");	
}


int main(int argc, char* argv[]) {
	char *directorio, *directorio2;
	if (argc != 3) {
		printf("usage: %s input_directory output_directory\n", argv[0]);
		return -1;
	} else if (argc == 3) {
		directorio = argv[1];
		directorio2 = argv[2];
	}
	normaliza(directorio, directorio2, argv[0]);
	return 0;
}

