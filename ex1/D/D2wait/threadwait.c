#include <stdlib.h>
#include <stdio.h>
#include <sys/times.h>
#include <pthread.h>
#include <unistd.h>
pthread_t tid[2];

void busy_wait_dealy(int seconds);

void* sleeper(void *arg){
	pthread_t id =pthread_self();
	printf("Message from thread \n");
	busy_wait_delay(5);
	printf("Message from thread\n");
	pthread_exit(1);
	return NULL;
}

void busy_wait_delay(int seconds)
{
	int i, dummy;
	int tps = sysconf(_SC_CLK_TCK);
	clock_t start;
	struct tms exec_time;
	times(&exec_time);
	start = exec_time.tms_utime;
	while( (exec_time.tms_utime - start) < (seconds * tps))
	{
		for(i=0; i<1000; i++)
		{
			dummy = i;
		}
		times(&exec_time);
	}
}

int main(int argc, char **argv){
	pthread_create(&(tid[0]),NULL,sleeper,NULL);
	pthread_create(&(tid[1]),NULL,sleeper,NULL);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	return 0;
}
