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
void RoundRobin(Fila *f,int t);
void IOHandler(int signal);
int execProc(Proc *p, int t);

int main (int argc, char** argv){
   	int pid;
	fila1=fila_cria();
 	fila2=fila_cria();
 	fila3=fila_cria();

    signal(SIGUSR1,procHandler);
	signal(SIGQUIT,fimHandler);
	//signal(SIGUSR2,exibeProcHandler);
	
	if(pid!=0){//processo pai
		for(EVER){
			RoundRobin(fila1);
			RoundRobin(fila2);
			RoundRobin(fila3);
		}
	}
	if(pid==0)//processo filho que vai gerenciar a fila de io ao mesmo tempo

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

int verificaRR(int t){
	int i;
	
	if(t==2){
		if(!fila_vazia(fila1))
		return -1;
	}
	if(t==4){
		if(!fila_vazia(fila2))
			return -1;
	}
	return 0;
}



void RoundRobin(Fila *f){
	Proc *p;
	int t,cond;
	p=(Proc*)malloc(sizeof(Proc));
	
	t=fila_tempo(f);
	
	//Descobrir se filas de prioridade maior estao vazias

	if(verificaRR(t)){
	
		while(!fila_vazia(f)){
			
			p=fila_pop(f);
			cond=execProc(p,t);
			
			if(cond==0)//TERMINOU
			{
				printf("Processo terminou \n");
				free(p);
			}
			
			else if(cond==1)//Nao terminou e acabou o quantum
			{
				if(t==1){
					p->fila=2;
					fila_push(fila2,p);
				}
				else{
					p->fila=3;
					fila_push(fila3,p);
				}
			}
			
			else{//SOBROU TEMPO
				kill()
				if(t==1 || t==2)
				{
					p->fila=1;		
					fila_push(fila1,p);
				}
				else{
					p->fila=2;
					fila_push(fila2,p);
				}	
				
			}			
			
	}
	return;	
}

void IOHandler(int signal){
	
}

void CHLDHandler(int signal){
	
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

resultEx execProc(Proc *p, int t){
	char cmdC[MAX]="./" ;
	int indR;
	if (p->estado==novo){ //processo novo -> criar processo filho
		p->pid=fork();
		if(p->pid==0){ //processo filho
			strcat(cmdC,p->nome);
			kill(getpid(),SIGSTOP);
			execv(cmdC,NULL);
		}
	}
	indR = indice_Rajada(p);
	if(t >= p->raj[indR]){ //se eu tenho >= tempo pra executar do que a rajada que o processo ta
		kill(p->pid,SIGINT);
		sleep(p->raj[indR]); //executa durante esse tempo
		kill(p->pid,SIGSTOP);
		p->raj[indR]=0; //zera o tempo da rajada
		if(indice_Rajada(p)==-1){//se nao tem mais rajada diferente de 0
			p->estado=terminado;
			return terminou;
		}
		else{ // tem mais rajada-> entra em espera de io
			p->estado=io;
			return sobrouTempo;
		}
	}
	if(t < p->raj[indR]){//menos tempo do q precisa
		kill(p->pid,SIGINT);
		sleep(t); //executa durante t
		kill(p->pid,SIGSTOP);
		p->raj[indR]-=t;
		p->estado=fila;
		return faltouTempo;
	}
}

int indice_Rajada (Proc *p){
	int i;
	for(i=0;i<p->numR;i++)
		if(p->raj[i]!=0)
			return i;
	return -1; 
}


