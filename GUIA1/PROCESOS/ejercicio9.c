#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

//Dado un programa de dos procesos, padre e hijo, se quiere tener el siguiente comportamiento:
//Uno de los dos procesos debe escribir en pantalla ping y su número de PID. Automáticamente el otro
//proceso debe escribir pong con su número de PID. Se quiere repetir este comportamiento 3 veces.
//Luego de esto, se desea preguntar al usuario si quiere finalizar la ejecución o no. En caso que conteste
//que si, el padre debe terminar con la ejecución de su hijo y finalizar. En caso que se conteste que no,
//se vuelve a repetir el proceso antes dicho.

int ping = 0;
int pong = 0;

pid_t pid_hijo; 

void dormir(){
    pause();
}

void signal3_handler_padre(int sig){
    if(ping < 3){
        printf("Ping");
        printf("Mi numero de pid es: %i", getpid());
        ping++; 
        //Le mando la señal al hijo y lo duermo
        kill(getppid(),34);
        dormir();
    }
    else{
        //Salgo porque no quiero imprimir mas ping
        return;
    } 
}

void signal3_handler_hijo(int sig){
    if(pong < 3){
        printf("Pong \n");
        printf("Mi numero de pid es: %i \n", getpid());
        pong++;
        //Le mando la señal al padre y lo duermo
        kill(pid_hijo,34);
        dormir();
    }else{
        //Salgo porque no quiero imprimir mas pong
        return;
    }
}

int main(){

    //Proceso principal
    printf("Ping \n");
    printf("Mi numero de pid es: %i \n", getpid());
    ping = 1;
    pong = 0;
    signal(34,signal3_handler_hijo);
    pid_hijo = fork();
    
    
    
    //Tendria que establecer señales y handlers para asi enviar y recibir señales entre procesos

    
    //Si soy el padre, entonces quiero dormir el proceso

    if(pid_hijo != 0){
        //Linkeo una señal al padre
        

        dormir();
        exit(0);
    
    }else{
        //Soy el hijo

        //Linkeo una señal al hijo
        signal(34,signal3_handler_padre);


        printf("Pong \n");
        printf("Mi numero de pid es: %i \n", getpid());
        
        pong++;

        //Ahora tendria que mandar una señal al padre para despertarlo
        
        //Hijo envia señal al padre
        kill(getppid(),34);
        dormir();
        exit(0);
    }

    exit(0);
}