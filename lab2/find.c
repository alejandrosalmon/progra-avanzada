#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
void buscar(char *directorio, int rec, char *string, char *program) {
	DIR *dir;
	struct dirent *dirent;
	struct stat buf;
	char filename[PATH_MAX + NAME_MAX + 1];

	if ((dir = opendir(directorio)) == NULL ) {
		perror(program);
		exit(-1);
	} else {
		printf("Si existe %s", directorio);
	}	 
	//NameName: workspace
	//Path: /home/pperezm/
	//Last access: Wed Jul 18 08:35:27 2018
	//Last modification:: Wed Jul 18 08:35:27 2018
	if (rec) {

	}
}

int main(int argc, char* argv[]) {
	char dir_name[NAME_MAX + 1];
	char *directorio;
	char *str;
	int recursivo;		

	if(argc > 4 || argc < 3){
		printf("Usage: find str directory [-r]\n");
		return -1;
	}	
	
	//getcwd asigna a su primer parametro (un apuntador) que es un buffer 
	// de tamano del segundo parametro
	getcwd(dir_name, NAME_MAX);
	directorio = dir_name;
	recursivo = 0;
	if (argc == 4) {
		if(strcmp(argv[3], "-r") == 0) {
			recursivo = 1;
			directorio = argv[2];
			str = argv[1];
		} else {
			printf("find: invalid option, argument '%s' not recognized \nArguments: \n     -r             searches for str in directory recursively. \n", argv[3]);	
			return -1;
		}	
	} else if (argc == 3) {
		directorio = argv[2];
		str = argv[1];	
	}
	//si todas las entradas son correctas corre la funcion buscar
	// directorio (path en el cual buscar)
	// str (string a buscar en el directorio)
	// recursivo (si es o no recursiva la busqueda)
	buscar(directorio, recursivo, str, argv[0]);
	return 0;
}








