#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
pthread_t PIDARR[5];

bool running=1;
int var1=0, var2=0;




void* kjooor(void *arg){
//	pthread_t ID=pthread_self();
	while(running){

		var1=var1+1;
		var2=var1;
	}	
        return 0;
}
void* kjor2(void *arg){
	//lock mutex
//	pthread_t ID=pthread_self();
	for(int i=0;i<20;i++){
		printf("Number 1 is %i, number 2 is %i\n",var1,var2);
		usleep(100000);
	}
	running=0;
	//unlock mutex
	return 0;	
}
int main(int argc, char **argv){
//	bool running=1;
//	int var1=0, var2=0;
	//sem_init(&semaphore,0,3);
        pthread_create(&(PIDARR[0]),NULL,kjooor,NULL);
        pthread_create(&(PIDARR[1]),NULL,kjor2,NULL);

        pthread_join((PIDARR[0]),NULL);
        pthread_join((PIDARR[1]),NULL);

        return 0;
}

