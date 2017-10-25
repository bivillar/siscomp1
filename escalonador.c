#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/shm.h>
#include "fila.h"
#define key 9989


Fila *fila1, *fila2, *fila3;

void  procHandler(int signal){
	Proc *p;
	int seg1;
	seg1 = shmget(key,sizeof(Proc), S_IRUSR | S_IWUSR);
	p=(Proc*)shmat(seg1,0,0);
	
	printf("ola\n");
	fila_push(fila1,p);
	return;
}

void  testeHandler(int signal){
	printf("Sinal recebido\n");
}




int main (int argc, char** argv){
    
    int numP,i,pid;
    int seg1;
	seg1 = shmget(key,sizeof(Proc), S_IRUSR | S_IWUSR);
	Proc *pNovo; 
	
    
	fila1=fila_cria();
 	fila2=fila_cria();
 	fila3=fila_cria();
 	
    //signal(SIGUSR1,procHandler);
	signal(SIGUSR2,testeHandler);
	
	pid = getpid();
	printf("pid= %d\n",pid);
	
    //printf("numR: %d\n",pNovo->numR);
   
   	printf("oi\n");
   	printf("pid %d\n",getpid());
    
    if(shmdt(pNovo)==-1){
				printf("Erro no detach\n");
				exit(1);
	}
	
	
	if(shmctl(seg1,IPC_RMID,NULL)==-1){
        printf("Nao foi possivel destruir a memoria\n");
        exit(1);
    }
	
	return 0;
	
}

