#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

// Constants 0 and 1 for READ and WRITE
enum { READ, WRITE };



void padre(int pipe_fd[]){
    int msj = 1;
    write(pipe_fd[WRITE],&msj,sizeof(msj));
    printf("Padre envia al hijo el valor %i: \n", msj);
    sleep(1);
    while(1){
        read(pipe_fd[READ],&msj, sizeof(msj));
        sleep(1);
        msj++;
        write(pipe_fd[WRITE],&msj,sizeof(msj));
        printf("Padre envia al hijo el valor %i: \n", msj+1);
        sleep(1);
    }
}

void hijo(int pipe_fd[]){
    int msj;
    while(1){
        read(pipe_fd[READ],&msj, sizeof(msj));
        sleep(1);
        msj++;
        write(pipe_fd[WRITE],&msj,sizeof(msj));
        printf("Hijo envia al padre el valor %i: \n", msj+1);
        sleep(1);
    }
}

int main(){

    //Creo el pipe
    int pipe_fd[2];
    pipe(pipe_fd);
    
    pid_t pid_hijo = fork();

    if(pid_hijo == 0){
        hijo(pipe_fd);
    }else{
        padre(pipe_fd);
    }
    
}

//Lo que termina pasando es que el pipe no tiene una cola de mensajes de tamaño 1 por lo tanto no hay bloqueos entre los procesos