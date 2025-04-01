#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>


// Recordar que la cola de mensajes, unicamente acepta un solo valor
pid_t pid_derecha;
pid_t pid_izquierda;
int result;

void proceso_izquierda() {
    result = 0;
    while(1) {
        //Trato de enviar el mensaje hasta que me lo recivan
        while(send(pid_derecha, &result) == false){
            printf("La cola de menesajes esta llena, no se puede enviar mas de un mensaje a la vez");
        }
        result = cómputo_muy_difícil_1();
    }
}

void proceso_derecha() {
    while(1) {
        result = cómputo_muy_difícil_2();
        int left_result; 
        while(receive(pid_derecha,&left_result) == false){
            printf("La cola de mensajes esta vacia, no hay nada que recibir");
        }
        printf("%s %s", left_result, result);   
    }
}

//Pregunta b) Si se refiere al mismo comportamiento que en la secuencia del ejercicio12a) entonces tendria que tener cola de capacidad 1. Osea basicamente poder enviar el primer bsend y que no sea bloqueante para que pueda ejecutar computo_muy_dificil1 y computo_my_dificil2.