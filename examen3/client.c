#include "header.h"

#define MAXTHREADS		5
#define MAXITERATIONS	5

void read_value(int sfd) {
	int value, code = READ;
	
	write(sfd, &code, sizeof(code));
	read(sfd, &value, sizeof(value));
	printf("pid % i - read value %i\n", getpid(), value);
}

void write_value(int sfd, int num) {
	int answer, value = num, code = WRITE;
	
	write(sfd, &code, sizeof(code));
	write(sfd, &value, sizeof(value));
	read(sfd, &answer, sizeof(answer));
	
	printf("pid % i - write value %i - modified %i\n", getpid(), value, answer);
}

void one_process(char* ip, int port, int id) {
	int i, sfd, num, val;
	struct sockaddr_in server_info;
	
	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("socket");
		exit(-1);
	}
	
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	server_info.sin_port = htons(port);
	if ( connect(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror("connect");
		exit(-1);
	}
	
	srand(getpid());
	for (i = 1; i <= MAXITERATIONS; i++) {
		num = rand() % 2;
		switch(num) {
			case 0 : read_value(sfd); break;
			default: 
				val = (id * 10) + (rand() % 10);
				write_value(sfd, val);
				break;
		}
		sleep( rand() % 3 );
	}
	val = BYE;
	write(sfd, &val, sizeof(val));
	close(sfd);
	
	printf("pid %i ended\n", getpid());
}
	

int main(int argc, char* argv[]) {
	int i, pid, port;
		
	if (argc != 3) {
	    printf("usage: %s ip port\n", argv[0]);
	    return -1;
	}
	
	port = atoi(argv[2]);
	if (port < 5000) {
		printf("%s: The port must be greater than 5000.\n", argv[0]);
		return -1;
	}
	
	for (i = 0; i < MAXTHREADS; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			exit(-1);
		} else if (pid == 0) {
			one_process(argv[1], port, (i + 1));
		} else {
			//do nothing
		}
	}
	
	while (i > 0) {
		wait(NULL);
		i--;
	} 
	return 0;
}
