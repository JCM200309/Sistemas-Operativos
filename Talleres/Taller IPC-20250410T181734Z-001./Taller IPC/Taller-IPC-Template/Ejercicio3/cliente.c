#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {

	int server_socket;
    struct sockaddr_un server_addr;

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "unix_socket");

    //Crea el socket del cliente
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    //Establece la coneccion
    if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error");
        exit(1);
    }
	int flag = 1;
    while(flag){
		char expresion[100];
		printf("Introduzca una expresion a evaluar, o exit para salir: ");
		scanf("%s",expresion);
		if(strcmp(expresion, "exit") == 0){
			flag = 0;
			write(server_socket,expresion,sizeof(expresion));

		}else{

			//ojo con PUNTERO EN EXPRESION
			write(server_socket, expresion, sizeof(expresion));
			
			//Lee es igual a usar recv
			int resultado;
			read(server_socket, &resultado, sizeof(resultado));
			printf("El resultado de la operación es: %d\n", resultado);
		}
        
    }
	close(server_socket);
	exit(0);
}
