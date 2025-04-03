#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

// Constants 0 and 1 for READ and WRITE
enum { READ, WRITE };




void ejecutarNieto(int pipe_hijo[],int numero_padre){
    //El nieto va a calcular el numero para escribirlo luego en el pipe y asi hacer que el padre lo reciba
    int resultado = calcular(numero_padre);

    //Ahora que ya tengo el resultado lo que voy a hacer es que el nieto lo escriba en el pipe que cree con el hijo

    write(pipe_hijo[WRITE],&resultado,size_of(resultado));
    exit(EXIT_SUCCESS);
}

void ejecutarHijo (int n, int i, int pipes[][2]) {
    
    //Como quiero enviarle el numero al nieto para que pueda calcular el numero voy a leer del padre el numero que me envio
    int numero_padre;
    read(pipes[i][READ],&numero_padre,sizeof(numero_padre));
    
    //Creo el subproceso nieto que tendra que calcular 
    int pid_nieto = fork();

    //Para que el nieto le avise al padre voy a crear un nuevo pipe para almacernar el resultado y hacer que el padre espere sigchild y desp handlearla para que cuando la reciba lea del pipe
    int pipe_hijo[2];
    pipe(pipe_hijo);

    

    
    if(pid_nieto == 0){
        //Preguntar si se puede pasar numero_padre o si hay que pasarlo por un pipe por el tema de que los PROCESOS NO COMPARTEN MEMORIA
        ejecutarNieto(pipe_hijo,numero_padre);
    }
    else{
        //Voy a hacer que el hijo espere al nieto para despues recibir por el pipe el valor que este calculo
        wait(NULL);

        //Ahora se que el nieto escribio en el pipe, por lo tanto lo leo
        int rta_nieto;
        read(pipe_hijo[READ],rta_nieto,sizeof(rta_nieto));

        //Ahora en teoria el hijo tendria que notificar al padre que ya termino, por lo tanto tendra que escribir en el segundo pipe del hijo i
        write(pipes[n+i][WRITE],&i,sizeof(i));
        write(pipes[n+i][WRITE], &rta_nieto,sizeof(rta_nieto));
    }
    exit(EXIT_SUCCESS);
}
int main(int argc, char* argv[]){
    if (argc< 2) {
        printf ("Debe ejecutar con la cantidad de hijos como parametro\n");
    return 0; }
    int N = atoi(argv[1]);
    int pipes[N*2][2];
    for ( int i=0; i< N*2; i++){
        pipe(pipes[i]); 
    }
    for (int i=0; i< N; i++) {
        int pid = fork () ;
        if (pid==0) {
            ejecutarHijo(N,i,pipes);
            return 0;
        }else{
            int numero = dameNumero(pid) ;
            write(pipes[i][1], &numero, sizeof(numero)); 
        } 
    }
    int cantidadTerminados = 0;
    char hijoTermino [N] = {0};
    while (cantidadTerminados < N) {
        for ( int i=0; i< N; i++) {
            if (hijoTermino[i]) {
                continue; 
            }
            char termino = 0;
            write(pipes[i][1], &termino, sizeof(termino));
            read(pipes[N+i][0], &termino, sizeof(termino));
            if (termino) {
                int numero;
                int resultado ;
                read(pipes[N+i][0], &numero, sizeof(numero));
                read(pipes[N+i][0], &resultado, sizeof(resultado));
                informarResultado(numero, resultado);
                hijoTermino[i] = 1;
                cantidadTerminados++; 
            } 
        } 
    }
    wait(NULL);
    return 0; 
}