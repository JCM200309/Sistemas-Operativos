#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

padre(pid_hijo1,pid_hijo2){
    int msj = 0;
    bsend(pid_hijo1,msj);
    printf("Padre envia al Hijo_1 el valor %i: ", msj);
    while(msj < 50){
        msj = breceive(pid_hijo2);
        bsend(pid_hijo1,msj+1);
        printf("Padre envia al hijo el valor %i: ", msj+1);
    }
}

hijo1(pid_hijo2){
    int msj = 0;
    while(msj < 50){
        int msj = breceive(getppid());
        bsend(pid_hijo2,msj+1);
        printf("Hijo envia al Hijo_2 el valor %i: ", msj+1);
    }
}
hijo2(){
    int msj = 0;
    while(msj < 50){
        int msj = breceive(getppid());
        bsend(getppid(),msj+1);
        printf("Hijo envia al Padre el valor %i: ", msj+1);
    }
}

int main(){
    pid_t pid_hijo2 = fork();

    //Creo primero al hijo 2 para que el hijo1 tenga el pid del hijo2
    if(pid_hijo2 == 0){
        hijo2();
    }else{
        pid_t pid_hijo1 = fork();
        if(pid_hijo1 == 0){
            hijo1(pid_hijo2);
        }
        else{
            padre(pid_hijo1,pid_hijo2);
        }

    }
}