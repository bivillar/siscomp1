#define MAX 21
#define RAJ 20

typedef enum{
	novo		=0,
	io          =1,
	terminado   =2,
	fila        =3
} Est;

typedef enum{
	faltouTempo =1,
	sobrouTempo =2,
	terminou    =4,
} resultEx;

typedef struct processo{
	int pid;
	int numR;
	char nome[MAX];
	int raj[RAJ];
	Est estado;
	int fila;  
} Proc;
