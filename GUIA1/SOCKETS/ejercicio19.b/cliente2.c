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
    int num = 0;
    while(1){
        //Escribe es igual a usar send
        write(server_socket, &num, sizeof(num));
        
        //Lee es igual a usar recv
        read(server_socket, &num, sizeof(num));
        printf("Cliente: recibí %d del servidor!\n", num);
        sleep(1);
        num++;
        
    }
    
}