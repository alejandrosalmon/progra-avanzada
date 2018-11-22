#include "header.h"
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char* LOG = "archivo.log";
char ip[15];
char* working_dir;
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

char* append(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void logger(char* txt){
  time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	FILE *log;
	if(access(LOG, F_OK) != -1){
		log = fopen(LOG, "a");
	}else{
		log = fopen(LOG, "w+");
	}
	fseek(log, 0, SEEK_END);
	fprintf(log, "%d-%d-%d %d:%d:%d IP:%s %s\n",tm.tm_year+1900, tm.tm_mon+1,	tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, ip, txt);
	fflush(log);
	fclose(log);
}


int read_path(int sfd){
	long length, path;
	char* data;

	read(sfd, &length, sizeof(length));
	printf("Server: length = %li\n", length);
	data = (char*) malloc(length * sizeof(char));
	path = read(sfd, data, length * sizeof(char));
	printf("File path = %s\n", data);
	if (path != length) {
		perror("Path no es del tamano indicado\n\n");
		//exit(-1);
		data = "Error";
	}
	return data; 
}

int check_path(int sfd, char* path){
	char* log_msg;
	char codig[20];
	if((path[0] == '/') || (strstr(path, "..") != NULL)){
		pthread_mutex_lock(&mutex);
		sprintf(codig, "%d", SEND_FILE); 
		log_msg = append(append("Comando: ", codig),append(" Parametro: ", path));
		logger(log_msg);
		sprintf(codig, "%d", INTERNAL_ERR); 
		log_msg = append(append("Respuesta : ", codig), " Mensaje: Error interno del servidor.");
		logger(log_msg);
		pthread_mutex_unlock(&mutex);
		snd_msg(sfd, INTERNAL_ERR, "Error Interno\n");
		return -1;
	}
	return 0;
}

int send_file(int sfd, char* path){
	int f_origin, answer;
	char* response;
	char filename[PATH_MAX+NAME_MAX+1];
	struct stat info;
	char *log_msg;
	char codig[20];
	sprintf(filename, "%s%s", working_dir, path);

	pthread_mutex_lock(&mutex);
	log_msg = append("Comando: 101 ",append(" Parametro: ", path));
	logger(log_msg);
	log_msg = "";
	if(lstat(filename, &info)<0){
		answer = NOT_FOUND;
		response = "Archivo no encontrado";
		sprintf(codig, "%d", answer); 
		log_msg = append(append("Respuesta : ", codig), append(" Mensaje: ", response));
		logger(log_msg);
		pthread_mutex_unlock(&mutex);
	} else{
		if(S_ISDIR(info.st_mode)){
			answer = ROUTE_ISDIR;
			response = "La ruta es un directorio";
			sprintf(codig, "%d", answer); 
			log_msg = append(append("Respuesta : ", codig), append(" Mensaje: ", response));
			logger(log_msg);
			pthread_mutex_unlock(&mutex);
		}else{
			if(access(filename, R_OK) == 0){
				char buffer;
				char* target_filename = path+1;
				int fd_in, fd_out;
				fd_in = open(filename, O_RDONLY);
				fd_out = open(target_filename, O_WRONLY | O_TRUNC | O_CREAT, 0666);
				ssize_t nbytes;
				while((nbytes = read(fd_in, &buffer, sizeof(buffer))) != 0){
					write(fd_out, &buffer, nbytes);
				}
				answer = FILE_SENT;
				response = "Enviando archivo.";
				sprintf(codig, "%d", answer); 
				log_msg = append(append("Respuesta : ", codig), append(" Mensaje: ", response));
				logger(log_msg);
				pthread_mutex_unlock(&mutex);
				close(fd_in);
				close(fd_out);
			}else{
				answer = DENIED;
				response = "Permiso denegado.";
				sprintf(codig, "%d", answer); 
				log_msg = append(append("Respuesta : ", codig), append(" Mensaje: ", response));
				logger(log_msg);
				pthread_mutex_unlock(&mutex);
			}
		}
	}
	snd_msg(sfd, answer, response);
}

int show_dir(int sfd, char* path){
	int f_origin, answer;
	char* response;
	char filename[PATH_MAX+NAME_MAX+1];
	struct stat info;
	char *log_msg;
	char codig[20];
	sprintf(filename, "%s%s", working_dir, path);
	char* data;

	pthread_mutex_lock(&mutex);
	log_msg = append("Comando: 101 ",append(" Parametro: ", path));
	logger(log_msg);
	log_msg = "";
	if(lstat(filename, &info)<0){
		answer = DIR_NOT_FOUND;
		response = "Directorio no encontrado";
		sprintf(codig, "%d", answer); 
		log_msg = append(append("Respuesta : ", codig), append(" Mensaje: ", response));
		logger(log_msg);
		pthread_mutex_unlock(&mutex);
	} else{
		DIR* dir;
		if((dir = opendir(filename)) == NULL){
			answer = ROUTE_NOTDIR;
			response = "La ruta no es un directorio";
			sprintf(codig, "%d", answer); 
			log_msg = append(append("Respuesta : ", codig), append(" Mensaje: ", response));
			logger(log_msg);
			pthread_mutex_unlock(&mutex);
		}else{
			answer = PRINT_DIR;
			response = "Enviando directorio.";
			struct dirent* direntry;
			while((direntry = readdir(dir)) != NULL){
				if(strcmp(direntry->d_name, ".") != 0 &&
				strcmp(direntry->d_name, "..") != 0){
					data = append(data, direntry->d_name);
					data = append(data, "\n");
				}
			}
			sprintf(codig, "%d", answer); 
			log_msg = append(append("Respuesta : ", codig), append(" Mensaje: ", response));
			logger(log_msg);
			pthread_mutex_unlock(&mutex);
			response = data;
		}
	}
	snd_msg(sfd, answer, response);
}

/*

    Permiso denegado (código '201' y el mensaje de error).
    Archivo no encontrado (código '202' y el mensaje de error).
    Error interno (código '203' y el error generado).
    La ruta es un directorio (código '205' y el mensaje de error).
    Enviando archivo (código '301' y el contenido del archivo).
    Directorio no encontrado (código '206' y el mensaje de error).
    La ruta no es un directorio (código '207' y el mensaje de error).
    Enviando el contenido de un directorio (código '302' y el listado del directorio).
*/


void* serve_client(void* param){
	int code, flag =1;
	char* message, *path;
	long len;
	int nsfd = *( (int*) param);

	char* data_log = "Nueva conexión establecida con un cliente";
	pthread_mutex_lock(&mutex);
	logger(data_log);
	pthread_mutex_unlock(&mutex);

	snd_msg(nsfd, HELLO, "HOLA");
	srand(getpid());

	while(flag == 1){
		read(nsfd, &code, sizeof(code));
		printf("Server: codigo recibido %d \n\n", code);
		switch(code){
			case SEND_FILE:		
				path = read_path(nsfd);
				printf("%s\n", path);
				if(path == "Error"){
					snd_msg(nsfd, INTERNAL_ERR, "Error interno, tamano del buffer incorrecto");
				} else {
					if(check_path(nsfd, path) == 0){
						printf("Path: %s\n", path);
						send_file(nsfd, path);
						printf("funcion de send file\n\n");
					}
				}
				break;
			case SHOW_DIR:
				path = read_path(nsfd);
				if(path == "Error"){
					snd_msg(nsfd, INTERNAL_ERR, "Error interno, tamano del buffer incorrecto");
				} else {
					if(check_path(nsfd, path) == 0){
						show_dir(nsfd, path);
						printf("Path: %s\n", path);
						printf("funcion de send dir\n\n");
					}
				}
				break;
			case END_CONNECTION:
				flag = 0;
				close(nsfd);
				printf("END_CONNECTION\n\n");
				break;
			default:
				read_path(nsfd);
				message = "No existe tal comando\n\n";
				snd_msg(nsfd, UNKNOWN_COM, message);
		}
	}
}

void server(char* ip, int port, char* directory,char* program) {
	int sfd, nsfd, pid;
	pthread_t thread_id;
	struct sockaddr_in server_info, client_info;
	/* Apertura de un conector orientado a conexión de la familia 
	AF_INET. */
	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror(program);
		exit(-1);
	} 
	/* Publicidad de la dirección del servidor. 
	*/
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	server_info.sin_port = htons(port);
	if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0) {
		perror(program);
		exit(-1);
	}
	/* Declaración de un cola de 5 elementos para peticiones de
	conexiones. */
	listen(sfd, 1);
	while(1) {
		int len = sizeof(client_info);
		if ((nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0) {
			printf("nsfd no creado");
			perror(program);
			exit(-1);
		}
		pthread_create(&thread_id, NULL, serve_client, ((void *) &nsfd));
	}
}

int main(int argc, char *argv[]) {
	int semid;
	int port;
	
	//verifica numero correcto de parametros
	if (argc != 4) {
		printf("usage: %s ip port root_dir \n", argv[0]);
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
	working_dir = argv[3];
	if(working_dir[0] != '/'){
		printf("%s The path must be an absolute path\n", argv[0]);
		return -1;
	}
	DIR* dir = opendir(append(".",working_dir));
	if(!dir){
		perror(argv[0]);
		return -1;
	}
	
	server(ip, port, working_dir, argv[0]);
	
}
