#include <stdio.h>
#include <unistd.h>
#include <signal.h>
int main(){
   int i;
   for(i=0;i<2;i++){
   		printf("pid: %d\n",getpid());
   		sleep(1);
   }
   for(i=0;i<3;i++){
   		printf("pid: %d\n",getpid());
   		sleep(1);
   }
    for(i=0;i<4;i++){
        printf("pid: %d\n",getpid());
        sleep(1);
    }
    kill(getpid(),SIGCHLD);
	return 0;
} 
