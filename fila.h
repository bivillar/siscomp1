//
//  Fila.h
//  prog2_P3
//
//  Created by Livia Aloise on 13/06/16.
//  Copyright © 2016 Livia Aloise. All rights reserved.
//

#include <stdio.h>
#include "def.h"

typedef struct fila Fila;
Fila* cria(void);
void insere (Fila* f, Proc *ṕ);
Proc* retira (Fila* f);
int vazia (Fila* f);
void libera (Fila* f);
