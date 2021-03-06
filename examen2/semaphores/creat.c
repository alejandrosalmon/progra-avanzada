#include "header.h"

int main(int argc, char* argv[]) {
	int semid;
	key_t key;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}
	
	if ( (semid = semget(key, 2, 0666 | IPC_CREAT))  < 0 ) {
		perror(argv[0]);
		return -1;
	}

	semctl(semid, PROCESS1, SETVAL, 1);
	semctl(semid, PROCESS2, SETVAL, 0);
	// semctl(semid, 0, IPC_RMID);
}
