#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/shm.h>
#include "def.h"
#define key 9999
#define N 50

typedef struct fila{
    int n;
    int ini;
    int fim;
    Proc *vProc;
} Fila;


Fila* fila_cria(void)
{
    Fila *f;
    f=(Fila*)malloc(sizeof(Fila));
    f->n=0;
    f->ini=0;
    f->fim=0;
    return f;
}

void fila_push (Fila* f, Proc *p)
{
    if(f->n==N)
    {
        printf("Fila cheia");
        exit(1);
    }
    f->fim++;
    f->vProc[f->fim]=*p;
    f->n++;
}

Fila *fila1, *fila2, *fila3;

void  procHandler(int signal){
	Proc *p;
	int seg1;
	seg1 = shmget(key,sizeof(Proc), S_IRUSR | S_IWUSR);
	p=(Proc*)shmat(seg1,0,0);
	
	printf("ola\n");
	fila_push(fila1,p);
}




int main (int argc, char** argv){
    
    int numP,i;
    int seg1;
	seg1 = shmget(key,sizeof(Proc), S_IRUSR | S_IWUSR);

    Proc *pNovo;
    
	fila1=fila_cria();
 	fila2=fila_cria();
 	fila3=fila_cria();
 	
    signal(SIGUSR1,procHandler);
   
   
    
    if(shmdt(pNovo)==-1){
				printf("Erro no detach");
				exit(1);
	}
	
	if(shmctl(seg1,IPC_RMID,NULL)==-1){
        printf("Nao foi possivel destruir a memoria\n");
        exit(1);
    }
	
	return 0;
	
}

