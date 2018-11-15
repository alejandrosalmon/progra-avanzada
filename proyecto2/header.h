#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define CONTINUE        2
#define END             -2

// client
#define SEND_FILE       101
#define SHOW_DIR        102
#define END_CONNECTION  103

// server
#define HELLO		1
#define DENIED          201
#define NOT_FOUND       202
#define INTERNAL_ERR    203
#define UNKNOWN_COM     204
#define ROUTE_ISDIR     205
#define FILE_SENT       206
#define DIR_NOT_FOUND   207
#define ROUTE_NOTDIR    208

#define PRINT_DIR       302


void snd_msg(int sfd, int code, char* data) {
	long length = strlen(data);
	
	printf("enviando...\n");
	printf("codigo: %i\n", code);
	write(sfd, &code, sizeof(code));
	printf("length = %li\n", length);
	write(sfd, &length, sizeof(length));
	printf("text = %s\n", data);
	write(sfd, data, length * sizeof(char));
	printf("\n\n");
}

int rcv_msg(int sfd) {
	int code;
	long length, data_read;
	char* data;
	
	printf("recibiendo...\n");
	read(sfd, &code, sizeof(code));
	printf("codigo: %i\n", code);
	read(sfd, &length, sizeof(length));
	printf("lenght = %li\n", length);
	
	data = (char*) malloc(length * sizeof(char));
	data_read = read(sfd, data, length * sizeof(char));
	printf("text = %s\n", data);
	free(data);
	
	printf("\n\n");
	
	if (data_read != length) {
		return -1;
	} else {
		return code;
	}
}

#endif
