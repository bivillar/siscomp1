#define MAX 21
#define RAJ 20

typedef enum{
	novo		=0,
	pronto 		=1,
	atual		=2,
	fila		=3
} Est;

typedef struct processo{
	int pid;
	int numR;
	char nome[MAX];
	int raj[RAJ];
	Est estado;
	int fila;  
} Proc;
