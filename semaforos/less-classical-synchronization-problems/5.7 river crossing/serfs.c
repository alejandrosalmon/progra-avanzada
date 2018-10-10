#include "header.h"
#include <time.h>

void a_serf(char* program) {
	int semid, shmid, i, k, isCaptain = 0;
	key_t key;
	struct buffer *b;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(program);
		exit(-1);
	}
	
	if ( (semid = semget(key, 4, 0666))  < 0 ) {
		perror(program);
		exit(-1);
	}

	if ( (shmid = shmget(key, sizeof(struct buffer), 0666)) < 0 ) {
		perror("shmid");
		exit(-1);
	} 	
	b = (struct buffer *) shmat(shmid, (void *) 0, 0);
	
	mutex_wait(semid, MUTEX);
	b->serfs += 1;

	if (b->serfs == 4) {
		sem_signal(semid, SERFQUEUE, 4);
		b->serfs = 0;
		isCaptain = 1;
	} else if (b->hackers == 2 && b->serfs >= 2) {
		sem_signal(semid, HACKERQUEUE, 2);
		sem_signal(semid, SERFQUEUE, 2);
		b->serfs = 0;
		b->hackers -= 2;
		isCaptain = 1;
	} else {
		mutex_signal(semid, MUTEX);
	}

	sem_wait(semid, SERFQUEUE, 1);

	printf("Boarding a serf\n");

	sem_wait(semid, BARRIER, 1);

	if (isCaptain) {
		printf("Fugaaaa\n");
		mutex_signal(semid, MUTEX);
	}
	
	sem_signal(semid, BARRIER, 2);
	shmdt(b);
	exit(0);
}

int main(int argc, char* argv[]) {
	int amount_serfs = 0, semid, i, pid;
	key_t key;
	
	if (argc != 2) {
		printf("usage: %s amount_of_serfs amount_of_hackers\n", argv[0]);
		return -1;
	}
	
	amount_serfs = atoi(argv[1]);
	if (amount_serfs < 1) {
		printf("%s: The amount_of_serfs must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}
	
	for (i = 0; i < amount_serfs; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			a_serf(argv[0]);
		} else {
		}
	}

	return 0;
}
