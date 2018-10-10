#include "header.h"

int main(int argc, char* argv[])
{
	int semid, shmid, i;
	key_t key;
	
	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}
	
	if ( (semid = semget(key, 4, 0666 | IPC_CREAT))  < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	semctl(semid, BARRIER, SETVAL, 4);
	semctl(semid, MUTEX, SETVAL, 1);
	semctl(semid, SERFQUEUE, SETVAL, 0);
	semctl(semid, HACKERQUEUE, SETVAL, 0);
	
	if ( (shmid = shmget(key, sizeof(struct buffer), 0666 | IPC_CREAT)) < 0 ) {
		semctl(semid, 0, IPC_RMID, 0);
		perror("shmget");
		return -1;
	}

	struct buffer *b = (struct buffer *) shmat(shmid, (void *) 0, 0); 	
	b->hackers = 0;
	b->serfs = 0;
	shmdt(b);

	//semctl(semid, 0, IPC_RMID);

	return 0;
}
