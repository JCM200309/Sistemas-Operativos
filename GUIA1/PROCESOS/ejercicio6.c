#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


void system2(const char *comando){

    //Basicamente lo que hace system es ejecutar el comando que nos pasaron en el shell

    //Por lo tanto voy a usar execl para abrir un shell y ejecutar el comando

    execl("/bin/sh", "sh" ,"-c", comando, (char *) NULL); //Pongo el path del shell para abrirlo y le mando como argumento mi comnando
    //Tengo que poner el -c para que lea los commandos del string y no en forma de estandar input
}

int main(){
    char comando[256];
    printf("Introduzca un comando \n");
    scanf("%255s", comando);
    system2 (comando);
    return 0;
}