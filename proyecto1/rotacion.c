/*----------------------------------------------------------------
*
* Programación avanzada: Problema del barbero.
* Fecha: 23-Sep-2018
* Autor: 1201954   Alejandro Salmon Felix Diaz
*
*--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>


void rotacion(const char *directorio, const char *name, const int copies, const int timeout,char* program)
{
    DIR *w_dir;
    int rotation = 0, n;
    char original_log[strlen(directorio)+strlen(name) + 4];
    char old_log[strlen(directorio) + strlen(name) + 2];
    char new_log[strlen(directorio) + strlen(name) + 2];

    // Checar que el directorio existe
    if ((w_dir = opendir(directorio)) == NULL) {
        perror(directorio);
        exit(0);
    }
    closedir(w_dir);

    // crea path del log original
    sprintf(original_log, "%s/%s.log", directorio, name);

    // verifica si existe uno anteriormente
    if ( access(original_log, F_OK) == -1 ) {
        perror(name);
        exit(0);
    } 

    // ROTAR
    printf("\n\n");
    while (1) {
        sleep(timeout);
        printf("ROTATION %d\n", rotation+1);
        for (n=copies; n >= 0; n--){
            sprintf(old_log, "%s/%s.%d", directorio, name, n);
            old_log[strlen(old_log)] = '\0';
            sprintf(new_log, "%s/%s.%d", directorio, name, n+1);
            new_log[strlen(new_log)] = '\0';

            if (n > rotation){
                continue;
            }
            // borra archivos sobrantes
            if (n == copies){
                if (remove(old_log) == -1) {
                    perror(program);
                    exit(0);
                }
                printf("removed:\t %s.%d\n", name, n);
            } else if (n == 0){
            //  reenombrar .log a .1 y luego crear el nuevo .log
                if (rename(original_log, new_log) == -1){
                    perror(program);
                    exit(0);
                }
                printf("rotated:\t %s.log -> %s.%d\n", name, name, n + 1);
                if (creat(original_log, 0666) == -1){
                    perror(program);
                    exit(0);
                }
                printf("created:\t %s.log\n", name);
            } else {
            // se rota al siguiente numero si no hay log ni n
                if (rename(old_log, new_log) == -1) {
                    perror(program );
                    exit(0);
                }
                printf("rotated:\t %s.%d -> %s.%d\n", name, n, name, n+1);
            }
        }
        printf("\n");
        rotation++;
    }
}



int main(int argc, char* argv[]){
    int copies, timeout;
	char *directorio;
	char *name;
    if(argc != 5){
        printf("usage: %s [working_directory] [logname] [number_of_files] [timeout(seconds)]\n", argv[0]);
        if(atoi(argv[3]) < 0 || atoi(argv[4]) < 0){
            printf("Number of files and timeout in seconds must be positive integer numbers greater than zero.");
            return -1;
        }
        return -1;
    }

    directorio = argv[1];
    name = argv[2];
    copies = atoi(argv[3]);
    timeout = atoi(argv[4]);
    
    rotacion(directorio,name,copies,timeout,argv[0]);
    return 0;
}