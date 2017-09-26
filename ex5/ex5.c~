#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include "sched.h"
#include "io.h"


#define delayTime 1


pthread_t pidARR[13];

void timespec_add_us(struct timespec *t, long us){
	// add microseconds to timespecs nanosecond counter
	t->tv_nsec += us*1000;
	// if wrapping nanosecond counter, increment second counter
	if (t->tv_nsec > 1000000000)
	{
		t->tv_nsec = t->tv_nsec - 1000000000;
		t->tv_sec += 1;
	}
}

int set_cpu(int cpu_number){
	// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),
	&cpu);
}
void* vTaskA(void *arg){
	set_cpu(1);
	struct timespec timera;
	clock_gettime(CLOCK_REALTIME, &timera);
	while(1){
		timespec_add_us(&timera, delayTime);
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &timera, NULL);
		if(!io_read(1)){
			io_write(1,0);
			usleep(5);
			io_write(1,1);
		}		
	}
}
void* vTaskB(void *arg){
	set_cpu(1);
	struct timespec timerb;
	clock_gettime(CLOCK_REALTIME, &timerb);
	while(1){
		timespec_add_us(&timerb, delayTime);
		clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&timerb,NULL);
		if(!io_read(2)){
			io_write(2,0);
			usleep(5);
			io_write(2,1);
		}
	}
}
void* vTaskC(void *arg){
	set_cpu(1);
	struct timespec timerc;
	clock_gettime(CLOCK_REALTIME, &timerc);
	while(1){
		timespec_add_us(&timerc, delayTime);
		clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&timerc,NULL);
		if(!io_read(3)){
			io_write(3,0);
			usleep(5);
			io_write(3,1);
		}
	}
}

 
void* stupidTest(void *arg){
	set_cpu(1);
	int i;
	while(1){
		i=1024*3;
	}
}

int main(int argc, char **argv){

	io_init();
	io_write(1,1);
	io_write(2,1);
	io_write(3,1);
	
	pthread_create(&(pidARR[0]),NULL,vTaskA,NULL);
	pthread_create(&(pidARR[1]),NULL,vTaskB,NULL);
	pthread_create(&(pidARR[2]),NULL,vTaskC	,NULL);
	for (int j=0; j<10;j++){
		pthread_create(&(pidARR[3+j]),NULL,stupidTest,NULL);
	}

	for (int j=0;j<13;j++){
		pthread_join((pidARR[j]),NULL);
	}

	/*pthread_join((pidARR[1]),NULL);
	pthread_join((pidARR[2]),NULL);
	pthread_join((pidARR[3]),NULL);
	pthread_join((pidARR[4]),NULL);
	pthread_join((pidARR[5]),NULL);
	pthread_join((pidARR[6]),NULL);
	pthread_join((pidARR[7]),NULL);
	pthread_join((pidARR[8]),NULL);
	pthread_join((pidARR[9]),NULL);
	pthread_join((pidARR[10]),NULL);
	pthread_join((pidARR[11]),NULL);
	pthread_join((pidARR[2]),NULL);
	pthread_join((pidARR[2]),NULL);
	*/
	return 0;
}
		
			
