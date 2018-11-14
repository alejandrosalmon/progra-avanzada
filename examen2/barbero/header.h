#ifndef SEMAFOROS_H
#define SEMAFOROS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define LLENO		0
#define VACIO		1
#define MUTEX		2

#define SIZE 		10
#define NUMSEM		3

#define CUSTOMER	0
#define CHAIRS 		1
#define BARBER 		2
#define CUSTOMERDONE 	3
#define BARBERDONE 	4


struct buffer {
	int next;
	int data[SIZE];
	int customers; 
};

int sem_wait(int semid, int sem_num, int val) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = -val;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

int sem_signal(int semid, int sem_num, int val) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = val;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

int mutex_wait(int semid, int sem_num){
	return sem_wait(semid, sem_num, 1);
}

int mutex_signal(int semid, int sem_num) {
	return sem_signal(semid, sem_num, 1);
}

#endif



