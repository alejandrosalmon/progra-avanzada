#include "header.h"

int main(int argc, char *argv[]){
	int semid, shmid, i;
	key_t key;
	unsigned short final_values[5];
	struct buffer *b;

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}

	if ( (semid = semget(key, 5, 0666))  < 0 ) {
		perror(argv[0]);
		return -1;
	}

	if ((shmid = shmget(key, sizeof(struct buffer), 0666)) < 0){
		perror("shmid");
		return -1;
	}
	b = (struct  buffer *) shmat(shmid, (void *) 0 , 0);

	srand(getpid());
	
	while(1){
		//printf("Benji se mete al while\n");
		i = rand() % 3;
		switch(i){
			case 0:
				printf("Benji saca un billete(100) (Otakus se prenden mas \\o/)\n");
				b->indio = 1;
				sem_signal(semid, BENITO, 1);
				sem_wait(semid,BENJI, 1);
				break;
			case 1:
				printf("Benji saca un billete(50) (Otakus se prenden mas \\o/)\n");
				b->morelos = 1;
				sem_signal(semid, INDIO, 1);
				sem_wait(semid,BENJI, 1);
				break;
			case 2:
				printf("Benji saca un billete(20) (Otakus se prenden mas \\o/)\n");
				b->benito = 1;
				sem_signal(semid, MORELOS, 1);
				sem_wait(semid,BENJI, 1);
				break;
		}
		printf("Benji Saca su cartera (Otakus se prenden)\n");
		sleep(2);
	}

	return 0;
}