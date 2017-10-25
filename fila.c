#include "fila.h"
#include <stdlib.h>

struct no{
	Proc *pr;
	No *prox;
};

struct fila{
	No *ini;
	No *fim;
};


Fila* fila_cria(void)
{
    Fila *f;
    f=(Fila*)malloc(sizeof(Fila));
    if(f==NULL){printf("Memoria Insuficiente\n");exit(1);}
    f->ini=NULL;
    f->fim=NULL;
    return f;
}

void fila_push (Fila* f, Proc *p)
{
	No *n;
	n = (No*) malloc (sizeof(No));
	n->pr = p;
	n->prox = NULL;
	if(!f->fim)
		f->ini = f->fim = n;
	else{
		f->fim->prox = n;
		f->fim = n;
	}
}

Proc* fila_pop (Fila* f)
{
	No *aux;
    Proc *p;
    if(fila_vazia(f))
    {
        printf("Fila Vazia");
        exit(1);
    }
    p = f->ini->pr;
	aux = f->ini;
	f->ini=aux->prox;
	free(aux);
    return p;
}

int fila_vazia (Fila* f)
{
    return (f->ini==NULL);
}

void fila_libera (Fila* f)
{
    No *n, *aux;
    n = f->ini;
    while(n){
    	aux = n;
    	n= aux->prox;
    	free(aux);
    }
    free(f);
}
