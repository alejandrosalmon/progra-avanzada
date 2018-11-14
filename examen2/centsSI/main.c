#include "header.h"

int main(int argc, char *argv[]) {
    int semid, shmid, i, j, k, blk_size, tamanio;
    key_t key;
    unsigned short final_values[6];
    struct buffer *b;

    if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}

	if ( (semid = semget(key, 5, 0666 | IPC_CREAT))  < 0 ) {
		perror(argv[0]);
		return -1;
	}
  /*Three engineering professors have gone to the faculty club to eat licorice. Each piece
  of licorice costs 36 cents. To buy a piece of licorice, a professor needs a quarter, a
  dime, and a penny (they do not give change, and they don’t take American Express).
  The first professor has a pocket full of pennies, the second a supply of quarters, and
  the third a supply of dimes. A wealthy alum walks up to the bar and lays down at
  random two of the three coins needed for a piece of licorice. The professor with the
  third coin takes the money and buys the licorice. The cycle then repeats. Show how to
  synchronize the professors and the alum.*


  /
    /*Este problema se basa en el de los cigarrete smokers problem, cada maestro equivale a un pusher, los ingredientes serian los centavos para comprar el licor y aleatoriamente se escogeran 2 centavos para ver si se peude comprar el licor o no
    se hacen 3 seaforos para representar los items con los que se cuentan, se incrementan o decrementan para asociar cuando se puede o no construir un cigarro, en este caso es para poder comprar alcohol.

    */

	semctl(semid, QUARTER, SETVAL, 0);
	semctl(semid, DIME, SETVAL, 0);
	semctl(semid, PENNY, SETVAL, 0);
	semctl(semid, PROFESSOR, SETVAL, 0);

	if ( (shmid = shmget(key, sizeof(struct buffer), 0666 | IPC_CREAT)) < 0 ) {
		semctl(semid, 0, IPC_RMID, 0);
		perror("shmid");
		return -1;
	}
	b = (struct buffer *) shmat(shmid, (void *) 0, 0);

    return 0;
}
