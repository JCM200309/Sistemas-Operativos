#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main(){

    pid_t pid_juan = fork();

    if(pid_juan == 0){

        //Proceso pid 10552

        printf("Soy Julieta\n");
        sleep(1);
        pid_t pid_jennifer = fork();

        if(pid_jennifer == 0){
            //Proceso pid 10557
            printf("Soy Jennifer\n");
            sleep(1);
            //Fin de Proceso pid 10557
            exit(0);


        }else{
            //Fin de Proceso pid 10552
            exit(0);
        }

    }else{

        //Proceso pid 10552

        printf("Soy Juan \n");
        sleep(1);

        int status;
        waitpid(-1, &status ,0);

        pid_t pid_jorge = fork();

        if(pid_jorge == 0){
            //Proceso pid 10558
            printf("Soy Jorge\n");
            sleep(1);
            //Fin de Proceso pid 10558
            exit(0);



        }else{
            //Fin de Proceso pid 10551
            exit(0);
        }

    }
    return 0;
}