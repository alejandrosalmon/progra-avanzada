/*----------------------------------------------------------------

*

* Programación avanzada: Manejando señales

* Fecha: 23-Sep-2018

* Autor: 1201954 Alejandro Salmon Felix Diaz

*

*--------------------------------------------------------------*/
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
#include <signal.h>

int n;
int count = 0;
int block = 0;

void signaling(int signal) {
    switch(signal){
        case SIGUSR1:
            if (block == 1){
                printf("SIGUSR1 - SIGUSR2 is now accepted\n");
                block = 0;
            } else {
                printf("SIGUSR1 - SIGUSR2 is now ignored\n");
                block = 1;
            }
            break;
        case SIGUSR2:
            if(block == 0){    
                count++;
                printf("SIGUSR2 - cont = %i - n = %i\n",count,n);
            }
            break;
    }
}

int main(int argc, char* argv[]){
    int x =0;
    if(argc == 1){
        n = 10;
    } else if(argc == 2){
        n = atoi(argv[1]);
        if (n < 1 || (atof(argv[1]) - atoi(argv[1])) != 0) {
            printf("%s: the parameter must be a positive integer number greater than zero.\n", argv[0]);
            return -1;
        }
    } else {
        printf("usage: %s [n]\n", argv[0]);
        return -1;
    }
    
    signal(SIGUSR1,signaling);
    signal(SIGUSR2,signaling);
    
    while(1){
        if(count == n){
            printf("Exiting...\n");
            exit(0);
            sleep(1);
        }
    }
    return 0;
}
