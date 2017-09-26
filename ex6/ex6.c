#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include <sched.h>
//#include <comedilib.h>
#include <native/task.h>
#include <native/timer.h>

#include "io.h"

#define wait_ms 1000000
#define wait_us 1000

#define delayTime 1*wait_us

int set_cpu(int cpu_number)
{
	// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),&cpu);
}

void xenTaskA(void *arg){
	rt_task_set_periodic(NULL,TM_NOW,delayTime);
	while(1){
		//timespec_add_us(&timera, delayTime);
		//clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &timera, NULL);
		if(!io_read(1)){
			io_write(1,0);
			usleep(5);
			io_write(1,1);
		}
		rt_task_wait_period(NULL);		
	}
}
void xenTaskB(void *arg){
	rt_task_set_periodic(NULL,TM_NOW,delayTime);
	while(1){
		//timespec_add_us(&timerb, delayTime);
		//clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&timerb,NULL);
		if(!io_read(2)){
			io_write(2,0);
			usleep(5);
			io_write(2,1);
		}
	}
}
void xenTaskC(void *arg){
	rt_task_set_periodic(NULL,TM_NOW,delayTime);
	while(1){
		//timespec_add_us(&timerc, delayTime);
		//clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&timerc,NULL);
		if(!io_read(3)){
			io_write(3,0);
			usleep(5);
			io_write(3,1);
		}
		rt_task_wait_period(NULL);
	}
}

void* stupidTest(void *arg){
	set_cpu(1);
	int i;
	while(1){
		i=1024*3;
	}
}


void periodicPrint(){
	rt_task_set_periodic(NULL, TM_NOW, 500*wait_ms);
	while(1){
		printf("Hello\n");
		rt_task_wait_period(NULL);
	}
}

int main(int argc, char **argv){
	int disturbance=1;
	io_init();
	printf("Initiated io\n");
	mlockall(MCL_CURRENT|MCL_FUTURE);
	RT_TASK task1;
	RT_TASK taskb;
	RT_TASK taskc;
	
	rt_task_create(&task1,"Response A",0,99,T_CPU(1));//|T_JOINABLE);
	rt_task_create(&taskb,"Response B",0,99,T_CPU(1));//|T_JOINABLE);
	rt_task_create(&taskc,"Response C",0,99,T_CPU(1));//|T_JOINABLE);

	rt_task_start(&task1,&xenTaskA,NULL);
	rt_task_start(&taskb,&xenTaskB,NULL);
	rt_task_start(&taskc,&xenTaskC,NULL);
	
	
	
	pthread_t pidARR[10];
	if(disturbance){
		int j=0;
		for(j=0; j<10;j++){
			pthread_create(&(pidARR[j]),NULL,stupidTest,NULL);
		}
		for(j=0;j<10;j++){
			pthread_join((pidARR[j]),NULL);
		}
	}

	
	while(1);
	
	return 0;
}
		
			
