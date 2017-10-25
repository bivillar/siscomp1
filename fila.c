//
//  Fila.c
//  prog2_P3
//
//  Created by Livia Aloise on 13/06/16.
//  Copyright © 2016 Livia Aloise. All rights reserved.
//

#include "Fila.h"
#include <stdlib.h>
#define N 50


struct fila{
    int n;
    int ini;
    int fim;
    Proc vet[N];
};


Fila* cria(void)
{
    Fila *f;
    f=(Fila*)malloc(sizeof(Fila));
    f->n=0;
    f->ini=0;
    f->fim=0;
    return f;
}

void insere (Fila* f, Proc *p)
{
    if(f->n==N)
    {
        printf("Fila cheia");
        exit(1);
    }
    f->fim++;
    f->vet[f->fim]=p;
    f->n++;
}
int retira (Fila* f)
{
    Proc *p;
    if(f->n==0)
    {
        printf("Fila Vazia");
        exit(1);
    }
    p=f->vet[f->ini];
    f->ini=(f->ini+1)%N;
    f->n--;
    return n;
}

int vazia (Fila* f)
{
    return (f->n==0);
}

void libera (Fila* f)
{
    free(f);
}








