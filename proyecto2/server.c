#include "header.h"
#include <pthread.h>

/*

1. Abrir el canal de comunicaciones e informar a la red tanto de la dirección a la que responderá como 
    de su disposición para aceptar peticiones de servicio.
2. Esperar a que un cliente le pida
    servicio en la dirección que él tiene declarada.
3. Cuando  recibe  una  petición  de  servicio,  si  es  un  servidor  interactivo,  atenderá  al  cliente.  Los 
    servidores  interactivos  se  suelen  implementar  cuando  la  respuesta  que  necesita  el  cliente  es 
    sencilla e implica poco tiempo de proceso.
    Si el servidor concurrente, creará un proceso mediante fork para que le dé servicio al cliente.
4. Volver al punto 2 para esperar nuevas peticiones de servicio.

*/

char* read_path(int sfd){
	long length, path;
	char* data;
	read(sfd, &length, sizeof(length));
	printf("length = %li\n", length);
	
	data = (char*) malloc(length * sizeof(char));
	path = read(sfd, data, length * sizeof(char));
	printf("File path = %s\n", data);
	free(data);
		
	printf("\n\n");
	
	if (path != length) {
		perror("Path no es del tamano indicado\n");
		exit(-1);
	} else {
		return data;
	}
	close(sfd);
}


void* serve_client(void* param){
	int code;
	char* message;
	long len;
	int nsfd = *( (int*) param);

	srand(getpid());
	read(nsfd, &code, sizeof(code));
	printf("%d codigo\n", code);
	switch(code){
		message = read_path(nsfd);
		case SEND_FILE:
			printf("SEND FILE\n");
			break;
		case SHOW_DIR:
		message = read_path(nsfd);
			printf("SHOW_DIR\n");
			break;
		case END_CONNECTION:
			printf("END_CONNECTION\n");
			break;
		default:
			message = "No existe tal comando\n";
			snd_msg(nsfd, UNKNOWN_COM, message);
	}
}

void server(char* ip, int port, char* program) {
	int sfd, nsfd, pid;
	pthread_t thread_id;
	struct sockaddr_in server_info, client_info;
	
	/* Apertura de un conector orientado a conexión de la familia 
	AF_INET. */
	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror(program);
		exit(-1);
	} 
	printf("Creo socket \n");
	/* Publicidad de la dirección del servidor. 
	*/
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	server_info.sin_port = htons(port);
	if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0) {
		perror(program);
		exit(-1);
	}
	printf("INICIO BIND\n");
	/* Declaración de un cola de 5 elementos para peticiones de
	conexiones. */
	listen(sfd, 1);
	while(1) {
		printf("Inicia while\n");
		int len = sizeof(client_info);
		if ((nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0) {
			printf("nsfd no creado");
			perror(program);
			exit(-1);
		}
		snd_msg(nsfd, HELLO, "HOLA");
		pthread_create(&thread_id, NULL, serve_client, ((void *) &nsfd));
		/* Código del proceso padre. */
	}
	
}

int main(int argc, char *argv[]) {
	int port, log_path;
	char ip[15];
	
	//verifica numero correcto de parametros
	if (argc != 4) {
		printf("usage: %s ip port log_path\n", argv[0]);
		return -1;
	}
	
	//valida que se use un puerto correcto
	port = atoi(argv[2]);
	if (port < 5000) {
		printf("%s: The port must be greater than 5000.\n", argv[0]);
		return -1;
	}
	
	if(strcpy(ip, argv[1]) < 0){
		printf("%s: Error with ip address.\n", argv[1]);
	}
	
		//verifica que se pueda abrir el log, si no existe lo crea
	if ( (log_path = open(argv[3], O_WRONLY | O_CREAT, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	server(ip, port, argv[0]);
	
}
