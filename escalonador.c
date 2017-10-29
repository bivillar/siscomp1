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
pid_t pid_terminado=0;

void procHandler(int signal);
void fimHandler(int signal);
void RoundRobin(Fila *f, int t);
void IOHandler(int signal);
void CHLDHandler(int signal);
resultEx execProc(Proc *p, int t);
int indice_Rajada (Proc *p);
void criaProc (Proc *p, char *pC);

int main (int argc, char** argv){
   	
    fila1=fila_cria();
    fila2=fila_cria();
    fila3=fila_cria();
    filaIO=fila_cria();
    
    signal(SIGUSR1,procHandler);
    signal(SIGQUIT,fimHandler);
    signal(SIGCHLD,CHLDHandler);
    signal(SIGUSR2,IOHandler);
    
    for(EVER){
        if (fila_vazia(fila1) && fila_vazia(fila2) && fila_vazia(fila3) ){
            printf("(Escalonador pausado)\n");
            pause();
        }
        
    #ifdef DEBUG
        printf("\n-->Escalonador voltou\n");
        printf("--->filas vazias 1:%d 2:%d 3:%d\n", fila_vazia(fila1),fila_vazia(fila2),fila_vazia(fila3));
    #endif    
        
        RoundRobin(fila1,1);
        RoundRobin(fila2,2);
        RoundRobin(fila3,4);
    }
    
    return 0;
}

int PrimeiraFilaVazia(int t){
    if( (t==2 && ( !fila_vazia(fila1) || fila_vazia(fila2) )) || 
    	(t==4 && ((!fila_vazia(fila2) || !fila_vazia(fila1)) || fila_vazia(fila3))))
            return 0;
    return 1;
}

void RoundRobin(Fila *f, int t){
    Proc *p;
    int cond,fila;
    p=(Proc*)malloc(sizeof(Proc));
    
#ifdef DEBUG    
    printf("----->Entrou RR(%d)\n",t);
#endif    

    //Descobrir se filas de prioridade maior estao vazias
    
    if(PrimeiraFilaVazia(t)){
    
    #ifdef DEBUG
        printf("--->Entrou Verifica RR(%d) == 0\n",t);
        printf("--->Fila Vazia = %d\n",fila_vazia(f));
    #endif    
    	
    	if(t==4)
    		fila=3;
    	else
    		fila=t;
    	printf("FILA %d\n",fila);
    
        while(!fila_vazia(f)){
            p=fila_pop(f);
            cond=execProc(p,t);
            
        #ifdef DEBUG
            printf("--->cond=%d \n",cond);
        #endif
        
            if(cond==terminou)//TERMINOU
            {
            #ifdef DEBUG
                printf("--->Processo terminou \n");
            #endif
                free(p);
            }
            
            else if(cond==faltouTempo)//FALTOU TEMPO NA RAJADA
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
            
            else{//SOBROU TEMPO NA RAJADA
                if(t==1 || t==2)
                {
                    p->fila=1;
                }
                else{
                    p->fila=2;
                }
                
                fila_push(filaIO,p);
                
            #ifdef DEBUG
                printf("--->FILA IO TA VAZIA? %d\n", fila_vazia(filaIO));
            #endif
            
                kill(getpid(),SIGUSR2);
            }
        }
    }
    return;
}

resultEx execProc(Proc *p, int t){
    
    int indR;
    char *arg[]={NULL}, aux[]="./";
    
    /*sigset_t maskset, oldmaskset;
    
    sigemptyset(&maskset);
    sigaddset(&maskset, SIGUSR2);
    sigaddset(&maskset, SIGUSR1);
    sigaddset(&maskset, SIGCHLD);
    */
    
    indR = indice_Rajada(p);
    
#ifdef DEBUG
    printf("--->INDR =%d\n", indR);
#endif
    
    if (p->estado==novo){ //processo novo -> criar processo filho
    
  	#ifdef DEBUG
        printf("--->Entrou criar outro processo\n");
    #endif
    
        if((p->pid=fork())==0){ //processo filho
            //printf("--->Entrei no filho \n");
            strcat(aux,p->nome);
            execv(aux,arg);
        }
    }
    
#ifdef DEBUG   
    printf("--->Entrou execProc\n");
#endif    
    
    if(t >= p->raj[indR]){ //se eu tenho >= tempo pra executar do que a rajada que o processo ta
    #ifdef DEBUG
        printf("--->Vai executar pid %d\n",p->pid);
    #endif    
        //sigprocmask (SIG_BLOCK, &maskset, &oldmaskset);
        kill(p->pid,SIGCONT);
        sleep(p->raj[indR]); //executa durante esse tempo
        kill(p->pid,SIGSTOP);
        //sigprocmask (SIG_UNBLOCK, &maskset, &oldmaskset);
        
        p->raj[indR]=0; //zera o tempo da rajada
        if(indice_Rajada(p)==-1){//se nao tem mais rajada diferente de 0
        	kill(p->pid,SIGCONT);
            p->estado=terminado;
            return terminou;
        }
        else{ // tem mais rajada-> entra em espera de io
            p->estado=io;
        #ifdef DEBUG
            printf("--->tem que ir pra IO\n");
        #endif
            return sobrouTempo;
        }
    }
    //if(t < p->raj[indR]){//menos tempo do q precisa
    //sigprocmask (SIG_BLOCK, &maskset, &oldmaskset);
    kill(p->pid,SIGCONT);
    sleep(t); //executa durante t
    kill(p->pid,SIGSTOP);
    //sigprocmask (SIG_UNBLOCK, &maskset, &oldmaskset);
    p->raj[indR]-=t;
    
#ifdef DEBUG
    printf("--->TEMPO NA RAJADA: %d\n", p->raj[indR]);
#endif

    p->estado=fila;
    return faltouTempo;
    //}
}


int indice_Rajada (Proc *p){
    int i;
    for(i=0;i<p->numR;i++)
        if(p->raj[i]!=0)
            return i;
    return -1;
}

void IOHandler(int signal){
   	int id=fork();
    if (id==0){
    	
        Proc *p=fila_pop(filaIO);
        
    #ifdef DEBUG
        printf("--->Entrou no io processo:%d\n", getpid());
    #endif
    
        sleep(3);
        if (p->fila==1)
            fila_push(fila1,p);
        else
            fila_push(fila2,p);
            
    #ifdef DEBUG
        printf("--->Terminou IO\n");
        printf("--->IO: filas vazias 1:%d 2:%d 3:%d\n", fila_vazia(fila1),fila_vazia(fila2),fila_vazia(fila3));
    #endif
       
    }
}

void CHLDHandler(int signal){
    int pidE = waitpid(-1, NULL, 0);
	if (pidE==0)//Processo parou por um sinal
	{
	    return;//Nao faz nada;
	}
	else{
	    printf("Processo %d terminou\n",pidE);
	}
}

void  procHandler(int signal){
    Proc *p,*pNovo;
    int seg1,i;
    
    seg1 = shmget(key,sizeof(Proc),IPC_EXCL| S_IRUSR | S_IWUSR);
    p=(Proc*)shmat(seg1,0,0);
    if (!p){
        printf("Erro shm");
        exit(-1);
    }
    
    pNovo=(Proc*)malloc(sizeof(Proc));
    if (!pNovo){
    	printf("Falta Memoria");
    	exit(-1);
    }
    
    pNovo->fila=p->fila;
    pNovo->estado=p->estado;
    pNovo->pid=p->pid;
    strcpy(pNovo->nome,p->nome);
    pNovo->numR=p->numR;
    
    for(i=0;i<p->numR;i++){
        pNovo->raj[i]=p->raj[i];
	}

#ifdef DEBUG
    printf("--->Peguei processo %s da memoria\n\n", pNovo->nome);
#endif
    
    fila_push(fila1,pNovo);
    if (!fila_vazia(fila1))
    	printf("BOTOU FILA 1\n");
  	else
  		printf("NAO BOTOU\n");
    
}

void criaProc (Proc *p, char *pC){
    char *const arg[]= {pC, NULL}, pB[MAX]="./";
    
    strcat(pB,p->nome);
#ifdef DEBUG
    printf("--------->execv(%s, {%s,NULL})\n", pB,arg[0]);
#endif
    p->pid=fork();
    
    if(p->pid<0){printf("erro no fork");exit(-1);}
    
    if(p->pid==0){ //processo filho
    #ifdef DEBUG
        printf("--->entrei no filho \n");
    #endif
        execv(pB,arg);
    }
    if(p->pid!=1)
    	sleep(2);
}

void fimHandler(int signal){
    int seg1;
    seg1 = shmget(key,sizeof(Proc), S_IRUSR | S_IWUSR);
    
    
    if(shmctl(seg1,IPC_RMID,0)==-1){
        printf("Nao foi possivel destruir a memoria\n");
        exit(1);
   	}
    free(fila1);
    free(fila2);
    free(fila3);
    free(filaIO);
    
    printf("Terminando o escalonador\n");
    exit(0);
}
