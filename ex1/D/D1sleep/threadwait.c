#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
pthread_t tid[2];

void* sleeper(void *arg){
	pthread_t id =pthread_self();
	printf("Message from thread \n");
	sleep(5);
	printf("Message from thread\n");
	pthread_exit(1);
	return NULL;
}
int main(int argc, char **argv){
	pthread_create(&(tid[0]),NULL,sleeper,NULL);
	pthread_create(&(tid[1]),NULL,sleeper,NULL);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	return 0;
}
