#include "header.h"
#include <time.h>

void a_hacker(char* program) {
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
	b->hackers += 1;

	if (b->hackers == 4) {
		sem_signal(semid, HACKERQUEUE, 4);
		b->hackers = 0;
		isCaptain = 1;
	} else if (b->hackers == 2 && b->serfs >= 2) {
		sem_signal(semid, HACKERQUEUE, 2);
		sem_signal(semid, SERFQUEUE, 2);
		b->hackers = 0;
		b->serfs -= 2;
		isCaptain = 1;
	} else {
		mutex_signal(semid, MUTEX);
	}

	sem_wait(semid, HACKERQUEUE, 1);

	printf("Boarding a hacker\n");

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
	int amount_hackers = 0, semid, i, pid;
	key_t key;
	
	if (argc != 2) {
		printf("usage: %s amount_of_serfs amount_of_hackers\n", argv[0]);
		return -1;
	}

	amount_hackers = atoi(argv[1]);
	if (amount_hackers < 1) {
		printf("%s: The amount_of_hackers must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}

	for (i = 0; i < amount_hackers; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			a_hacker(argv[0]);
		} else {
		}
	}

	return 0;
}
