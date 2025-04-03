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
    int client_socket1;
    int client_socket2;
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr1;
    struct sockaddr_un client_addr2;
    unsigned int slen = sizeof(server_addr);
    unsigned int clen = sizeof(client_addr1);
    unsigned int clen = sizeof(client_addr2);
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
    //Ahora voy a buscar leer el socket de ambos clientes
    client_socket1 = accept(server_socket, (struct sockaddr *) &client_addr1, &clen);
    client_socket2 = accept(server_socket, (struct sockaddr *) &client_addr2, &clen);

    
    //Le manda al cliente1 un 0
    int num = 0;
    send(client_socket1,&num,sizeof(0),0);
    while(1) {
        
        //Recibe del cliente1 un numero
        recv(client_socket1, &num, sizeof(num), 0);

        //Ahora quiero simular que el cliente1 le mando el numero al cliente2, entonces le mando ese mismo numero al cliente 2
        send(client_socket2,&num,sizeof(num),0);
        num++;

        //Envia al cliente
        send(client_socket, &num, sizeof(num), 0);

    }

    exit(0);
}