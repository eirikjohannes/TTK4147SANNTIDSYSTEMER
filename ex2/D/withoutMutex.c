

#include <stdlib.h>
#include <stdio.h>
#include <sys/times.h>
#include <pthread.h>
#include <unistd.h>

global bool running=1;
global int var1=0;
global int var2=0;

void* function1(void *arg){
	while(running){
		var1=var1+1;
		var2=var1;
	}
	return NULL;
}

void* function2(void *arg){
	for int i=1; i<=20;i++{
		printf("Number 1 is %i, number 2 is %i\n",var1,var2);
		usleep(100000);
	}
	return NULL;
}

int main(){
	pthread_t tid[2];
	pthread_create(&(tid[0]),NULL,function1,NULL);
	pthread_create(&(tid[1]),NULL,function2,NULL);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	return 0;	
}