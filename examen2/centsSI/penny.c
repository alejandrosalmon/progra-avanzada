#include "header.h"

int main(int argc, char *argv[]) {
    int semid, shmid, i, j, k, blk_size, tamanio;
    key_t key;
    unsigned short final_values[6];
    struct buffer *b;

    if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}

	if ( (semid = semget(key, 3, 0666))  < 0 ) {
		perror(argv[0]);
		return -1;
	}

	if ( (shmid = shmget(key, sizeof(struct buffer), 0666)) < 0 ) {
		perror("shmid");
		return -1;
	}
	b = (struct buffer *) shmat(shmid, (void *) 0, 0);

	int flag = 0;
	while(1) {
	    if(flag == 0) {
	        printf("pennies wants liquour\n");
	        flag = 1;
	    }
	    sem_wait(semid, QUARTER, 1);
	    if(b->dime) {
	        b->dime = 0;
	        flag = 0;
	        printf("pennies buy liquour\n");
	        sleep(2);
	        sem_signal(semid, PROFESSOR, 1);
	    }


	}

    return 0;
}
