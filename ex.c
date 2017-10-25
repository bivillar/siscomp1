#include <stdio.h>
#include <unistd.h>
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
	return 0;
} 
