#include <stdlib.h>
#include <stdio.h>
#include <sys/times.h>
#include <pthread.h>

void* sleep(){
	printf("Message\n");
	sleep(5);
}
int main(int argc, char **argv){
	pthread_create(&sleep);
	pthread_create(&sleep);
	return 0;



}
