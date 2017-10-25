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
#define MAXS 100
#define key 9989


void fimHandler(int signal){
	int seg1;
	seg1 = shmget(key,sizeof(Proc), S_IRUSR | S_IWUSR);
	if(shmctl(seg1,IPC_RMID,NULL)==-1){
        printf("Nao foi possivel destruir a memoria\n");
        exit(1);
    }
	
}

int main (void){

	int i=0,seg1;
	int pid,status,com,t2;
	char cmd[MAXS], *a,*b;
	Proc *p;
	char *const arg[] = {"escalonador.c", NULL};
	
	seg1 = shmget(key,sizeof(Proc),IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	p=(Proc*)shmat(seg1,0,0);
	
	//printf("Interpretador, digite os programas - 0 para terminar:\n");

	if((pid=fork())<0)
	{
		printf("Erro na criação do processo");
		exit(1);
	}
	
	if(pid>0){

		/*LEITURA DOS COMANDOS*/
		while(1){		
			scanf(" %[^\n]", cmd);
			if(strcmp(cmd,"0")==0){
				printf("Terminado\n");
				exit(0);
			}
			a=strtok(cmd," ");
			while (a!= NULL){
				/*NOME*/
				if(a[0]=='<'){
			
					a[strlen(a)-1]='\0';
					strcpy(p->nome,&a[1]);
					//printf("%s\n",p->nome);
				}
				/*RAJADAS*/
				if(a[0]=='('){
					p->numR=0;
					b=strtok(a,",");
					while(b!=NULL ){
						if(b[0]=='('){
							p->raj[p->numR]=atoi(&b[1]);
						}
						else{
							p->raj[p->numR]=atoi(b);
						}
						p->numR++;
						b=strtok(NULL,",");
					}
					
				}
				a=strtok(NULL," ");
			}
			p->estado=novo;
			p->pid=0;
			p->fila=1;
			
			/*FIM DA QUEBRA*/
			
			printf(" Mandando o Sinal\n");

			//com=kill(pid,SIGUSR1);
			t2=kill(pid,SIGUSR2);
			/*
			if (com==-1|| t2==-1){
				printf("Erro no sinal\n");
				exit(-1);
			}
			printf("sinal :%d e %d\n",com,t2);
			*/

        }
        
        if(shmdt(p)==-1){
				printf("Erro no detach\n");
				exit(1);
		}
	
		
	}
	
	if(pid==0){
		sleep(4);
		execv("./escalonador", arg);
	}
	
	
	return 0;	
}
	

	


	
