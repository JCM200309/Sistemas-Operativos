#include <stdio.h>    // printf()
#include <stdlib.h>   // exit()
#include <unistd.h>   // fork() pipe() execlp() dup2() close()
#include <sys/wait.h> // wait()

// Constants 0 and 1 for READ and WRITE
enum { READ, WRITE };

// Debe ejecutar "ls -al"
void proceso_wc(int pipe_fd[]) {
	//ls no va a necesitar hacer write
	close(pipe_fd[WRITE]);

	dup2(pipe_fd[READ],STDIN_FILENO);

	//No voy a tener que leer ya que al mandar end of file 
	
	execlp("wc", "wc", "-l", NULL);
	exit(EXIT_SUCCESS);
}


void proceso_ls(int pipe_fd[]) {
	//ls no va a necesitar hacer read
	close(pipe_fd[READ]);

	//ahora quiero que el stdout del proceso vaya a write
	//Recordar que dup2(int oldfd, int newfd), se elimina la refencia al objeto apuntado por newfd y luego se apunta newfd al mismo objeto que oldfd
	//Como quiero ir desde stdout a write entonces pipe_fd[Write] es el viejo y el nuevo es stdout
	//Refs va a aumentar en 1 porque NO SE BORRA PIPE_FD[WRITE]
	dup2(pipe_fd[WRITE],STDOUT_FILENO);

	//Ahora cualquier salida va a ir al pipe

	//Ahora voy a buscar ejecutar ls. La salida va a ir por el pipe
	execlp("ls","ls","-l",NULL);
	exit(EXIT_SUCCESS);
}

int main() {

  	//Creo el pipe
	int pipe_fd[2]; // [0] = lectura, [1] = escritura
	pipe(pipe_fd);

	pid_t pid_hijo1 = fork();

	//Mi idea es en un fork hacer ls -al y en otro hacer wc -l 
	//El problema es que ls -al va a escribir en la shell y wc -l va a estar esperando recibir por stdin el teclado, asi que los tengo que duppear

	if(pid_hijo1 == 0){
		//Estoy en el hijo, este va a ser ls
		proceso_ls(pipe_fd);
	}else{
		pid_t pid_hijo2 = fork();
		if(pid_hijo2 == 0){
			//Estoy en el hijo2, este va a ser wc -l
			proceso_wc(pipe_fd);
		}
	}

	//Hay que cerrar el pipe
	close(pipe_fd[READ]);
    close(pipe_fd[WRITE]);

	wait(NULL);
	wait(NULL);
	exit(EXIT_SUCCESS);
}

