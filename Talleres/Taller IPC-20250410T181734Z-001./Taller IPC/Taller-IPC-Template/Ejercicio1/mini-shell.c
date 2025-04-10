#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"
#include "mini-shell-parser.c"

static int run(char ***progs, size_t count)
{	
	int r, status;

	//Reservo memoria para el arreglo de pids
	//TODO: Guardar el PID de cada proceso hijo creado en children[i]
	pid_t *children = malloc(sizeof(*children) * count);

	//TODO: Pensar cuantos procesos necesito. -- NECESITAMOS COUNT PROCESOS
	//TODO: Pensar cuantos pipes necesito. -- NECESITAMOS (COUNT - 1) PROCESOS EL ULTIMO ESCRIBE EN STDOUT
	int pipes[count - 1][2];  
	for (int i = 0; i < count - 1; i++){
		pipe(pipes[i]);
	}
	// procesos
	for (int i = 0; i < count ; i++){
		children[i] = fork();
		if (children[i] == 0){
			//TODO: Para cada proceso hijo:
			if (i == 0){
				for (int k = 0; k < count - 1; k++){
					if(k != i){ 
						close(pipes[k][PIPE_WRITE]);  // cierro todos los writes salvo el que uso: pipes[i][WRITE]
					}
					close(pipes[k][PIPE_READ]); // cierro todos los reads salvo el que uso: pipes[i-1][READ]
					
				}
				dup2(pipes[i][PIPE_WRITE],STDOUT_FILENO);
				//OBS: Despues del dup, tenemos dos FDs que apuntan al extremo de escritura del pipe
				//No es necesario cerrarlo, pero esta bien tenerlo en cuenta.
				close(pipes[i][PIPE_WRITE]);
				execvp(progs[i][0], progs[i]); 
				exit(EXIT_FAILURE);
			}
			else if (i != 0 && i < (count - 1)){
				//proceso intermedio
				for (int k = 0; k < count - 1; k++){
					if(k != i){ 
						close(pipes[k][PIPE_WRITE]);  // cierro todos los writes salvo el que uso: pipes[i][WRITE]
					}
					if(k!=i-1){
						close(pipes[k][PIPE_READ]); // cierro todos los reads salvo el que uso: pipes[i-1][READ]
					}
				}
				dup2(pipes[i-1][PIPE_READ], STDIN_FILENO);
				dup2(pipes[i][PIPE_WRITE], STDOUT_FILENO);
				
				execvp(progs[i][0], progs[i]);
				//ESTO NO SE EJECUTA, NO PUEDO CERRAR PIPES ACA
				//PERO! LOS PIPES QUE USO NO QUEDAN ABIERTOS. LOS CIERRA EL PROG QUE SE EJECUTA CON EL EXEC.
				exit(EXIT_SUCCESS);
			}
			else if (i == (count - 1)){
				//proceso final
				for (int k = 0; k < count - 1; k++){
					close(pipes[k][PIPE_WRITE]);  // cierro todos los pipes, no llego a count - 1 porque no hay "ultimo pipe"
					if(k!=i-1){
						close(pipes[k-1][PIPE_READ]); // cierro todos los reads salvo el que uso: pipes[i-1][READ]
					}
				}
				dup2(pipes[i-1][PIPE_READ], STDIN_FILENO);
				execvp(progs[i][0], progs[i]);
				exit(EXIT_SUCCESS);
			
			//1. Redireccionar los file descriptors adecuados al proceso
			//2. Ejecutar el programa correspondiente
			}
		}
	}
	// CERRAMOS FD DEL PIPE PADRE. NO SE PUEDE HACER ANTES PORQUE SINO PERDEMOS LA REFERENCIA (COME CICLOS)
	for (int i = 0; i < count - 1; i++){  
		close(pipes[i][PIPE_READ]);
		close(pipes[i][PIPE_WRITE]);
	}
	

	//Espero a los hijos y verifico el estado que terminaron
	for (int i = 0; i < count; i++) {
		waitpid(children[i], &status, 0);

		if (!WIFEXITED(status)) {
			fprintf(stderr, "proceso %d no terminó correctamente [%d]: ",
			    (int)children[i], WIFSIGNALED(status));
			perror("");
			return -1;
		}
	}
	r = 0;
	free(children);

	return r;
}


int main(int argc, char **argv){
	if (argc < 2) {
		printf("El programa recibe como parametro de entrada un string con la linea de comandos a ejecutar. \n"); 
		printf("Por ejemplo ./mini-shell 'ls -a | grep anillo'\n");
		return 0;
	}
	int programs_count;
	char*** programs_with_parameters = parse_input(argv, &programs_count);

	printf("status: %d\n", run(programs_with_parameters, programs_count));

	fflush(stdout);
	fflush(stderr);

	return 0;
}
