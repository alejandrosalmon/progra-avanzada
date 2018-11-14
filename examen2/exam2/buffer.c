#include "header.h"

int main(int argc, char *argv[]){
	int semid, shmid, i, j, blk_size, tamanio;
	key_t key;
	unsigned short final_values[6];
	struct buffer *b;

	if ((key = ftok("/dev/null", 65)) == (key_t) -1) {
		perror(argv[0]);
		return -1;
	}

	if ((semid = semget(key,5, 0666 | IPC_CREAT)) < 0){
		perror(argv[0]);
		return -1;
	}

	semctl(semid, BENITO, SETVAL, 0);
	semctl(semid, MORELOS, SETVAL, 0);
	semctl(semid, INDIO, SETVAL, 0);
	semctl(semid, OTAKUS, SETVAL, 0);


	semctl(semid, 0, GETALL, final_values);

	printf("Valores: ");
	for(i = 0; i<6; i++) {
		printf("%3i", final_values[i]);
	}
	printf("\n");

	if(( shmid = shmget(key, sizeof(struct buffer), 0666 | IPC_CREAT)) < 0 ) {
		semctl(semid,0,IPC_RMID, 0);
		perror("shmid");
		return -1;
	}
	b = (struct buffer *) shmat(shmid, (void *) 0, 0);

	return 0;
}