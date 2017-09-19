#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>

pthread_t PIDARR[2];
int globalvariabel=0;

void* kjooor(void *arg){
	int lokalvariable=0;
	for(int i=0;i<200000;i++){
		globalvariabel+=1;
		lokalvariable+=2;
	}
	printf("Lokal,global: %i,%i\n",lokalvariable,globalvariabel);
	return 0;
}

int main(int argc, char **argv){
       
        pthread_create(&(PIDARR[0]),NULL,kjooor,NULL);
        pthread_create(&(PIDARR[1]),NULL,kjooor,NULL);
	
	pthread_join((PIDARR[0]),NULL);
//        pthread_create(&(PIDARR[1]),NULL,kjooor,NULL);
	pthread_join((PIDARR[1]),NULL);
        return 0;
}
       
