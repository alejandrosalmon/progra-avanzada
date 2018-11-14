#ifndef SEMAFOROS_H
#define SEMAFOROS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

struct buffer {
	int quarter;
	int dime;
	int penny;
};


#define QUARTER 	0
#define DIME    	1
#define PENNY   	2
#define STUDENT 	3
#define PROFESSOR 4


int mutex_wait(int semid, int sem_num) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = -1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int mutex_signal(int semid, int sem_num) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = 1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int sem_wait(int semid, int sem_num, int val) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = -val;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int sem_signal(int semid, int sem_num, int val) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = val;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

#endif
