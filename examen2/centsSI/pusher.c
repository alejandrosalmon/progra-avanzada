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

	if ( (semid = semget(key, 5, 0666))  < 0 ) {
		perror(argv[0]);
		return -1;
	}

	if ( (shmid = shmget(key, sizeof(struct buffer), 0666)) < 0 ) {
		perror("shmid");
		return -1;
	}
	b = (struct buffer *) shmat(shmid, (void *) 0, 0);

	srand( getpid() );
	while(1) {
    //printf("ENTRE al while\n");
  	i = rand() % 3;
    if (i == 0) {
      printf("The pusher arrives\n");
      b->quarter = 1;
      sem_signal(semid, PENNY, 1);
      sem_wait(semid, PROFESSOR, 1);
    } else if (i == 1) {
      printf("The pusher arrives\n");
      b->dime = 1;
      sem_signal(semid, QUARTER, 1);
      sem_wait(semid, PROFESSOR, 1);
    } else if (i == 2) {
      printf("The pusher arrives\n");
      b->penny = 1;
      sem_signal(semid, DIME, 1);
      sem_wait(semid, PROFESSOR, 1);
    }
  	printf("Pusher gets more coins\n\n");
  	sleep(2);
	}
    return 0;
}
