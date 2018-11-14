#include "header.h"

int valor_servidor=0;

int write_var(int valor){
	valor_servidor = valor;
	if(valor_servidor == valor){
		return 1;
	} else {
		return 0;
	}
}


/*
	Funcion encargada de recibir y enviar codigos
*/
void* serves_client(void *param){
	int code;
	//int* data;
	long len;
	int resp, data;
	int nsfd = *( (int*) param);


	read(nsfd, &code, sizeof(code));
	printf("Recibio %i\n", code);
	srand(getpid());
	switch(code){
		case READ:
			printf("devuelve valor %d", valor_servidor);
			write(nsfd, &valor_servidor, sizeof(valor_servidor));
			break;
		case WRITE:
			read(nsfd, &data, sizeof(data));
			printf(" se escribe valor %d en la variable\n", data);
			valor_servidor = data;
			resp = write_var(data);
			write(nsfd, &resp, sizeof(resp));
			break;
		case BYE:
			printf("se cerro la conexion");
			break;
		default:

			break;
	}
	close(nsfd);
}

int main(int argc, char* argv[]) {
	int pid, port, sfd, nsfd, code, log;
	struct sockaddr_in server_info, cli_addr;
	int cli_addr_len;
	int valor = 0;
	pthread_t thread_id; 
	
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
	
	//crea el socket
	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	//verifica que se pueda abrir el log, si no existe lo crea
	if ( (log = open(argv[3], O_WRONLY | O_CREAT, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	//define caracteristicas de conexion
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(argv[1]);
	server_info.sin_port = htons(port);

	//bind para unir el socket a la conexion
	if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	//escuchamos en el socket definido con una cola de 10 peticiones
	listen(sfd, 25);
	while(1){
		cli_addr_len = sizeof(cli_addr);
		if ((nsfd = accept(sfd, (struct sockaddr *) &cli_addr, &cli_addr_len)) == -1) {
			printf("nsfd no creado\n");
			perror(argv[0]);
			return -1;
		}
		//crea un thread por cada peticion
		pthread_create(&thread_id, NULL, serves_client, ((void *) &nsfd));
	}
}
