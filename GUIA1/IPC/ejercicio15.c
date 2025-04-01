#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

int main(){

    pid_t pid_hijo1 = fork();

    //Mi idea es en un fork hacer ls -al y en otro hacer wc -l 
    //El problema es que ls -al va a escribir en la shell y wc -l va a estar esperando recibir por stdin el teclado, asi que los tengo que duppear

    if(pid_hijo1 == 0){
        //Estoy en el hijo, este va a ser ls
        proceso_ls();
    }else{
        pid_t pid_hijo2 = fork();
        if(pid_hijo2 == 0){
            //Estoy en el hijo2, este va a ser wc -l
            proceso_wc();
        }else{
            //Estoy en el padre
        }
    }
}