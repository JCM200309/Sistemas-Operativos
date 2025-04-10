#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int calcular(const char *expresion) {
    int num1, num2, resultado;
    char operador;

    // Usamos sscanf para extraer los dos números y el operador de la expresión
    //El cliente va a tener que poner la expresion sin espacios (10+10)
    if (sscanf(expresion, "%d%c%d", &num1, &operador, &num2) != 3) {
        printf("Formato incorrecto\n");
        return 0;  // En caso de error, retornamos 0.
    }

    // Realizamos la operación según el operador
    switch (operador) {
        case '+':
            resultado = num1 + num2;
            break;
        case '-':
            resultado = num1 - num2;
            break;
        case '*':
            resultado = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                resultado = num1 / num2;
            } else {
                printf("Error: División por cero\n");
                return 0;  // Si hay división por cero, retornamos 0.
            }
            break;
        default:
            printf("Operador no reconocido\n");
            return 0;  // Si el operador no es válido, retornamos 0.
    }

    return resultado;
}

int hijo(int client_socket){
    
    int flag = 1;
    while(flag) {
        
        char expresion[100];
        //Recibe del cliente una expresion
        recv(client_socket, &expresion, sizeof(expresion), 0);

        //Si es cero son iguales, sino no 
        if(strcmp(expresion,"exit") == 0){
            
            flag = 0;

        }else{

            printf("Servidor: recibí %s del cliente!\n", expresion);
            int resultado = calcular(expresion);
            
            //Envia al cliente
            send(client_socket, &resultado, sizeof(resultado), 0);
    
        }
        
    }
    //Cierro el FD en el hijo
    close(client_socket);
    exit(EXIT_SUCCESS);
}



int main(){
     
    // COMPLETAR. Este es un ejemplo de funcionamiento básico.
    // La expresión debe ser recibida como un mensaje del cliente hacia el servidor.

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

    while(1){
        printf("Servidor: esperando conexión del cliente...\n");
        //Dado que el padre se va a bloquear hasta recibir un cliente
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clen);
        pid_t pid_hijo = fork();
        if(pid_hijo == 0){
            //Funcion hijo
            hijo(client_socket);
        }
        //Cierro el FD en el padre
        close(client_socket);
    }


    //const char *expresion = "10+5";  
    //int resultado = calcular(expresion);
    //printf("El resultado de la operación es: %d\n", resultado);
    exit(0);
}

