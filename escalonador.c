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
#define EVER ;;


Fila *fila1, *fila2, *fila3,*filaIO;

void  procHandler(int signal);
void fimHandler(int signal);
void exibeProcHandler(int signal);
void RoundRobin(Fila *f,int t)
//int execProc(Proc *p, int t);

int main (int argc, char** argv){
   	
	fila1=fila_cria();
 	fila2=fila_cria();
 	fila3=fila_cria();

    signal(SIGUSR1,procHandler);
	signal(SIGQUIT,fimHandler);
	//signal(SIGUSR2,exibeProcHandler);

	
	for(EVER);

	return 0;	
}

void  procHandler(int signal){
	Proc *p,*pNovo;
	int seg1,i,pid;
	seg1 = shmget(key,sizeof(Proc),IPC_EXCL| S_IRUSR | S_IWUSR);
	p=(Proc*)shmat(seg1,0,0);
	
	pNovo=(Proc*)malloc(sizeof(Proc));
	
	pNovo->fila=p->fila;
	pNovo->estado=p->estado;
	pNovo->pid=p->pid;
	strcpy(pNovo->nome,p->nome);
	pNovo->numR=p->numR;
	
	 for(i=0;i<p->numR;i++){
        pNovo->raj[i]=p->raj[i];
    }
	
	fila_push(fila1,p);
	return;
	
}

void RoundRobin(Fila *f){
	Proc *p;
	int t,cond;
	p=(Proc*)malloc(sizeof(Proc));
	
	t=f->t;
	
	if(
	
	
	while(!fila_vazia(f)){
		filaProc=p->fila;
		
		if (filaProc==1){
			cond=execProc();
		}
	
	}	
}

void exibeProcHandler(int signal){
	Proc *p;
	int i;
   	p=(Proc*)malloc(sizeof(Proc));
	
	p=fila_pop(fila1);
	
	printf("p:%d  \n",p->pid); 
    printf("p:%s  \n",p->nome);
    printf("p:%d  \n",p->numR); 
    printf("p:%d  \n",p->estado);   
   
    for(i=0;i<p->numR;i++){
        printf("v[%d]=%d\n", i,p->raj[i]);
    }
}

void fimHandler(int signal){
	int seg1;
	seg1 = shmget(key,sizeof(Proc), S_IRUSR | S_IWUSR);
	
	if(shmctl(seg1,IPC_RMID,0)==-1){
        printf("Nao foi possivel destruir a memoria\n");
        exit(1);
    }
    printf("Terminando o escalonador\n");
    exit(0);
}



