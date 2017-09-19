#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

pthread_t PIDARR[5];
int globalvariabel=0;
sem_t semaphore;

void resource(pthread_t threadInfo){
	unsigned int ID=(threadInfo);
	for(int i=0; i<4;i++){
		printf("Thread number: %i, number %i\n",ID,i);
		usleep(100000);
	}
}

void* kjooor(void *arg){
	pthread_t ID=pthread_self();
	sem_wait(&semaphore);
	resource(ID);
	sem_post(&semaphore);	
        return 0;
}

int main(int argc, char **argv){
	sem_init(&semaphore,0,3);
        pthread_create(&(PIDARR[0]),NULL,kjooor,NULL);
        pthread_create(&(PIDARR[1]),NULL,kjooor,NULL);
        pthread_create(&(PIDARR[2]),NULL,kjooor,NULL);
        pthread_create(&(PIDARR[3]),NULL,kjooor,NULL);
        pthread_create(&(PIDARR[4]),NULL,kjooor,NULL);

        pthread_join((PIDARR[0]),NULL);
        pthread_join((PIDARR[1]),NULL);
        pthread_join((PIDARR[2]),NULL);
        pthread_join((PIDARR[3]),NULL);
//        pthread_create(&(PIDARR[1]),NULL,kjooor,NULL);
        pthread_join((PIDARR[4]),NULL);
        return 0;
}

