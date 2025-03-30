#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int bart(){
    printf("%s", "Soy bart \n");
    exit(0);
}

int maggie(){
    printf("%s", "Soy maggie \n");
    exit(0);
}

int lisa(){
    printf("%s", "Soy lisa \n");
    exit(0);
}

int homero(){
    //Estoy en Homero

    printf("%s", "Soy Homero \n");
    
    pid_t pid_lisa = fork();

    if(pid_lisa == 0){
        lisa();
    }
    
    waitpid(pid_lisa,NULL,0); //No creo a bart hasta que lisa alla muerto

    pid_t pid_bart = fork();

    if(pid_bart == 0){
        bart();
    }

    waitpid(pid_bart,NULL,0); //No creo a maggie hasta que bart alla muerto

    pid_t pid_maggie = fork();

    if(pid_maggie == 0){
        maggie();
    }

    waitpid(pid_maggie,NULL,0); //No salgo de homero hasta que maggie haya muerto
    exit(0);
}

int main(){

    //Estoy en Abraham

    printf("%s", "Soy Abraham \n");

    //Creo a Homero
    pid_t pid_homero = fork();

    if(pid_homero == 0){
        //Estoy en Homero
        homero();
    }

    waitpid(pid_homero,NULL,0);
    return 0;
}