#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

padre(pid_hijo){
    int msj = 0;
    bsend(pid_hijo,msj);
    printf("Padre envia al hijo el valor %i: ", msj);
    while(1){
        msj = breceive(pid_hijo);
        bsend(pid_hijo,msj+1);
        printf("Padre envia al hijo el valor %i: ", msj+1);
    }
}

hijo(){
    while(1){
        int msj = breceive(getppid());
        bsend(getppid(),msj+1);
        printf("Hijo envia al padre el valor %i: ", msj+1);
    }
}

int main(){
    pid_t pid_hijo = fork();

    if(pid_hijo == 0){
        hijo();
    }else{
        padre(pid_hijo);
    }
    
}