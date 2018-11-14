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


int contar(char *directorio, char *programa) {
	DIR * dir;
	struct dirent *dirent;
	struct stat info;
	char filename[PATH_MAX + NAME_MAX + 1];
	int max = 0;
	int new_size = 0;
	int min = 0 ;
	int files = 0;
	long tot_size = 0;


	if ((dir = opendir(directorio)) == NULL ) {
		perror(programa);
		exit(-1);
	}
	//recorre el dir
	while((dirent = readdir(dir)) != NULL) {
		if(strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") !=0) {
			sprintf(filename, "%s/%s", directorio, dirent->d_name);
			if(lstat(filename, &info) >= 0){
				if(S_ISREG(info.st_mode)){
					files++;
					new_size = (int)info.st_size;
					tot_size += new_size;
					if(new_size < min || min == 0){
						min = new_size;
					} else if(new_size > max) {
						max = new_size;
					}
				} else if(S_ISDIR(info.st_mode)){
					tot_size +=	contar(filename, programa);
				}
			}

		}
	}
	printf("Directory: %s \n", directorio);
	printf("Occupied space: %ld Number of files: %d \n", tot_size, files);
	printf("Min size: %d  Max size: %d \n", min, max);
	return tot_size;	
}


int main(int argc, char* argv[]) {
	char dir_name[NAME_MAX + 1];
	char *source, *directorio;
	int min, max;
	if (argc != 2) {
		printf("usage: %s directory \n", argv[0]);
		return -1;
	} else if (argc == 2) {
	
		getcwd(dir_name, NAME_MAX);
		source = dir_name;
	
		directorio = argv[1];
	}
	contar(directorio, argv[0]);
	return 0;
}

