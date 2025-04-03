#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define VIVO 1
#define MUERTO 0

typedef struct{
    pid_t pid;
    int status;
}hijo_t;

int n;
int numero_maldito;

hijo_t *hijos;

int generate_random_number(){
    //Funcion para cargar nueva semilla para el numero aleatorio
    srand(time(NULL) ^ getpid());
    return (rand() % n);
}

void handler_SIGCHLD(int status){
    pid_t hijo_muerto = wait(NULL);
    //Marcar el muerto Recordar que estoy en padre
    for (int i = 0; i < n; i++){
        if (hijos[i].pid == hijo_muerto){
            hijos[i].status = MUERTO;
        }
    }
}

void handler_SIGTERM(int status){
    int numero_hijo = generate_random_number();
    if(numero_hijo == numero_maldito){
        printf("Estuvo bueno mientras duró.\n");
        exit(EXIT_SUCCESS);
    }
}



int main(int argc, char const *argv[]){
    

    
    n = atoi(argv[1]);
	int rondas = atoi(argv[2]);
	numero_maldito = atoi(argv[3]);
    hijos = malloc(sizeof(hijo_t)*n);

    signal(SIGCHLD,handler_SIGCHLD);

    

    //For para crear a los hijos

    for (int hijo_n = 0; hijo_n < n; hijo_n++){
        pid_t pid_hijo = fork();
        if (pid_hijo == 0){
            //Soy hijo y nunca vuelvo al for
            signal(SIGTERM,handler_SIGTERM); //Duda esto, se podria hacer una unica vez fuera del for????
            while(1){} //Este un pause criollo (No nos interesa que salga)
        }else{
            //Soy Padre
            //printf("el pid del hijo es %i.\n", pid_hijo);
            hijos[hijo_n].pid = pid_hijo;
            hijos[hijo_n].status = VIVO;
        }
    }

    //Otro for para las K rondas
    for (int r = 0; r < rondas; r++){
        //For para mandar a cada uno de los hijos el SIGTERM en una misma ronda
        for (int i = 0; i < n; i++){
            //Ahora vamos a querer mandar SIGTERM a cada uno de los hijos VIVOS
            if(hijos[i].status == VIVO){
                
                kill(hijos[i].pid,SIGTERM);
                sleep(1); //OJO WAIT
            }   
        }
    }
    
    //Finalizaron las rondas
    for(int i = 0; i < n; i++){
        if(hijos[i].status == VIVO){
            printf("Gane y soy el hijo: %i con pid: [%i].\n", i, hijos[i].pid);
            kill(hijos[i].pid,SIGKILL);
        }
    }
    
    


    free(hijos);
    exit(0);
}
