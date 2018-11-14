#include "header.h"

int main(int argc, char *argv[]) {
	int semid, shmid;
	key_t key;
	unsigned short final_values[6];
	struct buffer *b;

	if((key = ftok("/dev/null", 65)) == (key_t) -1) {
		perror(argv[0]);
		return -1;
	}
	if ((semid = semget(key, 3, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}	
	if ((shmid = shmget(key,sizeof(struct buffer), 0666)) < 0) {
		perror("shmid");
		return -1;
	}
	b = (struct buffer *) shmat(shmid, (void *) 0, 0);

	int bandera = 0;
	while(1){
		if(bandera == 0) {
			printf("Otaku con benitos(pobres) quiere Pocky :( \n");
			bandera = 1;
		}
		sem_wait(semid,INDIO, 1);
		if(b->morelos) {
			b->morelos = 0;
			bandera = 0;
			printf("Otaku con benitos tiene Pocky :)\n");
			sleep(2);
			sem_signal(semid,BENJI, 1);
		}
	}

	return 0;
}