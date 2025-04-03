#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdint.h>


int main() {
    int server_socket;
    int client_socket;
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;
    unsigned int slen = sizeof(server_addr);
    unsigned int clen = sizeof(client_addr);
    int num;

    server_addr.sun_family = AF_UNIX;
    //Llena el path del unix domain con la palabra "unix_socket"
    strcpy(server_addr.sun_path, "unix_socket");
    unlink(server_addr.sun_path);

    //Creo el servidor del Socket
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    //bindeo el socket
    bind(server_socket, (struct sockaddr *) &server_addr, slen);

    //Setea el socket del servidor como un socket pasivo que recibira conexiones entrantes, con cola = 1
    listen(server_socket, 1);

    printf("Servidor: esperando conexión del cliente...\n");
    //Ahora voy a buscar leer el socket del cliente
    client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clen);
    while(1) {
    
        //Recibe del cliente un numero
        recv(client_socket, &num, sizeof(num), 0);
        printf("Servidor: recibí %d del cliente!\n", num);
        sleep(1);
        num++;

        //Envia al cliente
        send(client_socket, &num, sizeof(num), 0);

    }

    exit(0);
}