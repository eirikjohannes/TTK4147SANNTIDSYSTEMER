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
#include <native/sem.h>

#define wait_ms 1000000
#define wait_us 1000
#define rt_sem_wait rt_sem_p
#define delayTime 1*wait_us
RT_SEM awesomeSem;

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
	//rt_task_set_periodic(NULL,TM_NOW,delayTime);
	printf("Task A now waiting for sempahortee\n");
	rt_sem_wait(&awesomeSem, TM_INFINITE);
	printf("Task A now released by semaphore\n");
}
void xenTaskB(void *arg){
	printf("Task B now waiting for semaphore\n");
	rt_sem_wait(&awesomeSem, TM_INFINITE);
	printf("Task B now released by semaphore\n");
}
void  xenTaskC(void *arg){
	printf("Task C initialized, now waiting to broadcast sempahore\n");
	usleep(1000000);
	printf("5..\n");
	usleep(1000000);
	printf("4..\n");
	usleep(1000000);
	printf("3..\n");
	usleep(1000000);
	printf("2..\n");
	usleep(1000000);
	printf("1..\n");
	usleep(1000000);
	printf("RELEASSAAASEE\n");
	rt_sem_broadcast(&awesomeSem);
	usleep(100000);
	rt_sem_delete(&awesomeSem);
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
	
	printf("Initiated io\n");
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_sem_create(&awesomeSem,"awesomeSem",0,S_FIFO);
	RT_TASK task1;
	RT_TASK taskb;
	RT_TASK taskc;
	
	rt_task_create(&task1,"Response A",0,3,T_CPU(1)|T_JOINABLE);
	rt_task_create(&taskb,"Response B",0,1,T_CPU(1)|T_JOINABLE);
	rt_task_create(&taskc,"Response C",0,4,T_CPU(1)|T_JOINABLE);

	rt_task_start(&task1,&xenTaskA,NULL);
	rt_task_start(&taskb,&xenTaskB,NULL);
	rt_task_start(&taskc,&xenTaskC,NULL);
	
	rt_task_join(&task1);
	rt_task_join(&taskb);
	rt_task_join(&taskc);
	
	return 0;
}
		
			
