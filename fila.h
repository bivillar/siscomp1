#include <stdio.h>
#include "def.h"

typedef struct no No;
typedef struct fila Fila;

Fila* fila_cria(void);

void fila_push(Fila* f, Proc *p);

Proc* fila_pop(Fila* f);

int fila_vazia(Fila* f);

void fila_libera(Fila* f);
