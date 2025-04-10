#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "constants.h"

int generate_random_number(){
	return (rand() % 50);
}

int main(int argc, char **argv)
{	
	//Funcion para cargar nueva semilla para el numero aleatorio
	srand(time(NULL));

	int status, pid, n, start, buffer;
	n = atoi(argv[1]);
	buffer = atoi(argv[2]);
	start = atoi(argv[3]);

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}

	printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer, start);
    
  	/* COMPLETAR */
	int pipe_padrehijo[2];
	int pipe_hijopadre[2];
	int pipe_anillo[n][2];
	pipe(pipe_padrehijo);
	pipe(pipe_hijopadre);
	for (int i = 0; i < n; i++)
	{
		pipe(pipe_anillo[i]);
	}
	write(pipe_padrehijo[PIPE_WRITE], &buffer, sizeof(buffer));

	for (int i = 0; i < n; i++)
	{
		pid_t pid = fork();
		if (pid == 0){
			int valor_recibido;
			
			for (int k = 0; k < n; k++)
			{
				if (k != i){
					close(pipe_anillo[k][PIPE_WRITE]);
				}
				if (k != (i+n-1)%n){
					close(pipe_anillo[k][PIPE_READ]);
				}
			}
			

			if (i == start){
				int numero_magico = generate_random_number();
				printf("El numero magico es: %i \n", numero_magico);
				read(pipe_padrehijo[PIPE_READ], &valor_recibido, sizeof(valor_recibido));
				if (numero_magico < valor_recibido){numero_magico = numero_magico + valor_recibido;}
				while(valor_recibido < numero_magico){
					printf("Soy el proceso %i. El valor recibido es: %i \n", i, valor_recibido);
					valor_recibido = valor_recibido + 1;
					write(pipe_anillo[i][PIPE_WRITE], &valor_recibido, sizeof(valor_recibido));
					read(pipe_anillo[(i+n-1) % n][PIPE_READ], &valor_recibido, sizeof(valor_recibido));
				}	
				//se termina el juego
				close(pipe_hijopadre[PIPE_READ]); // no haria falta
				write(pipe_hijopadre[PIPE_WRITE], &valor_recibido, sizeof(valor_recibido));
				close(pipe_anillo[i][PIPE_WRITE]);
				close(pipe_anillo[(i+n-1)%n][PIPE_READ]);
				exit(EXIT_SUCCESS); // hay que hacer la cadena de EOF´s. este exit genera el dominó pues cierra el ultimo descriptor sobre el pipe[i][PIPE_WRITE]
			}else{
				while(1){
					int bytes_leidos = read(pipe_anillo[(i+n-1)%n][PIPE_READ], &valor_recibido, sizeof(valor_recibido));
					//esto propaga la cascada de EOF´s.
					if ( bytes_leidos <= 0){
						close(pipe_anillo[i][PIPE_WRITE]);
						close(pipe_anillo[(i+n-1)%n][PIPE_READ]);
						exit(EXIT_SUCCESS);

					}
					printf("Soy el proceso %i. El valor recibido es: %i \n", i, valor_recibido);
					valor_recibido = valor_recibido + 1;
					write(pipe_anillo[i][PIPE_WRITE], &valor_recibido, sizeof(valor_recibido));
				}

			}
			
	
		}

	}
	for (int i = 0; i < n; i++)
	{
		close(pipe_anillo[i][PIPE_READ]);
		close(pipe_anillo[i][PIPE_WRITE]);
	}
	
	int valor_final;
	for (int i = 0; i < n; i++)
	{
		wait(NULL);
	}
	
	read(pipe_hijopadre[PIPE_READ], &valor_final, sizeof(valor_final));
	
	printf("El resultado final es:%i\n", valor_final);
	
	exit(EXIT_SUCCESS);
}
