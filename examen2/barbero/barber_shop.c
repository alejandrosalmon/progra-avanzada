#include "header.h"

int main(int argc, char* argv[]){

	int chairs = 0, semid, i, shmid;
	key_t key;
	unsigned short sem_values[5];

	if (argc != 2) {
		printf("usage: %s size\n", argv[0]);
		return -1;
	}

	chairs = atoi(argv[1]);
	if (chairs < 1) {
		printf("%s: Chair number must be greater than 0\n", argv[0]);
		return -1;
	}

	if((key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}

	if( (semid = semget(key, 5, 0666 | IPC_CREAT)) < 0){
		perror(argv[0]);
		return -1;
	}


	semctl(semid, CHAIRS, SETVAL, chairs);
	semctl(semid, CUSTOMER, SETVAL, 0);
	semctl(semid, BARBER, SETVAL, 0);
	semctl(semid, CUSTOMERDONE, SETVAL, 0);
	semctl(semid, BARBERDONE, SETVAL, 0);


	semctl(semid, 0, GETALL, sem_values);

	printf("Valores: ");
	for(i = 0; i<5; i++) {
		printf("%3i", sem_values[i]);
	}
	printf("\n");

	if((shmid = shmget(key, sizeof(struct buffer), 0666 | IPC_CREAT)) < 0) {
		semctl(semid, 0, IPC_RMID, 0);
		perror("shmget");
		return -1;
	}

	struct buffer *b = (struct buffer *) shmat(shmid, (void *) 0, 0);
	b->customers = 0;
	shmdt(b);

	return 0;
}
