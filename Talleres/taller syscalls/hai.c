#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int contador = 0;

//handler SIGINT (padre)
void handler_sigint(){
	int status;
	wait(&status);
	contador++;
}

//handler SIGCHILD (padre)
void handler_sigchld(){
	exit(EXIT_SUCCESS);
}

//handler SIGURG (hijo)
void handler_sigurg(){
	printf("ya va!\n");
	contador++;
}

//codigo hijo
void hijo(char* argv[]){
	//hijo define handler
	signal(SIGURG, handler_sigurg);
	while(contador < 5){}
	kill(getppid(), SIGINT);
	//exec
	execvp(argv[1], argv+1);



	exit(EXIT_SUCCESS);
}


int main(int argc, char* argv[]) {
  	//variable contador
	//padre hace fork
	pid_t pid_hijo = fork();

	//padre define handler SIGINT y SIGCHILD
	signal(SIGINT, handler_sigint);
	signal(SIGCHLD, handler_sigchld);

	//sleep

	if (pid_hijo == 0) {
		hijo(argv);
		//hijo no llega acá
	}

	//ciclo 5 veces sup! ya va!
	for (int i = 0; i < 5; i++) {
		sleep(1);
		printf("sup!\n");
		kill(pid_hijo, SIGURG);

	}



	return 0;
}
