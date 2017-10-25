//
//  Fila.c
//  prog2_P3
//
//  Created by Livia Aloise on 13/06/16.
//  Copyright © 2016 Livia Aloise. All rights reserved.
//

#include "fila.h"
#include <stdlib.h>
#define N 50


struct fila{
    int n;
    int ini;
    int fim;
    Proc vet[N];
};


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
    f->vet[f->fim]=p;
    f->n++;
}
int fila_pop (Fila* f)
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

int fila_vazia (Fila* f)
{
    return (f->n==0);
}

void fila_libera (Fila* f)
{
    free(f);
}








