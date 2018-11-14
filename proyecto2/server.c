#include "header.h"

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

int serve_client(int nsfd){
	int code;
	char* message;
	long len;

	srand(getpid());
	code = rcv_msg(nsfd);
	switch(code){
		case SEND_FILE:
			break;
		case SHOW_DIR:
			break;
		case END_CONNECTION:
			break;
		default:
			snd_msg(nsfd, UNKNOWN_COM, "No existe el comando");
	}
}

int main(int argc, char *argv[]) {
	int sfd, nsfd, pid, port;
	struct sockaddr_in server_info, cli_addr;
	int cli_addr_len;
	
	if (argc != 1) {
	    printf("usage: %s\n", argv[0]);
	    return -1;
	}
	
	/* Apertura de un conector orientado a conexión de la familia 
	AF_INET. */
	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror(argv[0]);
		return -1;
	} 
	printf("Creo socket");
	/* Publicidad de la dirección del servidor. 
	*/
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(DEFAULT_IP);
	server_info.sin_port = htons(DEFAULT_PORT);
	if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) == -1) {
		perror(argv[0]);
		return -1;
	}
	printf("INICIO BIND");
	/* Declaración de un cola de 5 elementos para peticiones de
	conexiones. */
	listen(sfd, 5);
	while(1) {
		printf("Inicia while");
		cli_addr_len = sizeof(cli_addr);
		if ((nsfd = accept(sfd, (struct sockaddr *) &cli_addr, &cli_addr_len)) == -1) {
			printf("nsfd no creado");
			perror(argv[0]);
			return -1;
		}
		if ((pid = fork()) == -1) { 
			perror(argv[0]);
			return -1;
		} else if (pid == 0) {
			/*
			Código del proceso hijo. 
			*/
			printf("proceso hijo");
			close(sfd);
			serve_client(nsfd);
			close(nsfd);
			return 0;
		}
		/* Código del proceso padre. */
		snd_msg(nsfd, HELLO, "HOLA");
		close(nsfd);
	}
}
