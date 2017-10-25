#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
	int status;
    char *const arg[]={"escalonador.c",NULL};
	printf("exercicio 4 \n");
	
	if(fork()!=0){/* Comandos Pai */
		waitpid( -1, &status, 0);
	}
	else{ /*Comandos Filho*/
		execv("./e", arg);	
	}
	return 0;
} 
