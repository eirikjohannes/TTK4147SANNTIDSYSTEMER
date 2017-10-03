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
#include <rtdk.h>
//#include <comedilib.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <native/mutex.h>

#define wait_ms 1000000
#define wait_us 1000
#define rt_sem_wait rt_sem_p
#define rt_sem_signal rt_sem_v
#define delayTime 1*wait_us

RT_SEM awesomeSem;
RT_SEM resource;
RT_MUTEX mutexA,mutexB;
RT_TASK task1;
RT_TASK taskb;
RT_TASK taskc;
RT_TASK taskd;


/*
The base and current priority of the running task is printed together
with a provided message.
*/
void print_pri(RT_TASK *task, char *s)
{
	struct rt_task_info temp;
	rt_task_inquire(task, &temp);
	rt_printf("base:%i current:%i ", temp.bprio, temp.cprio);
	rt_printf(s);
}
int rt_task_sleep_ms(unsigned long delay){
	return rt_task_sleep(1000*1000*delay);
}
void busy_wait_ms(unsigned long delay){
	unsigned long count = 0;
	while (count <= delay*10){
		rt_timer_spin(1000*100);
		count++;
	}
}

void set_priority(RT_TASK *task,int n){
	rt_task_set_priority(task,n);
	rt_task_sleep(1);
}


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
	rt_printf("Task A now waiting for sempahortee\n");
	rt_sem_wait(&awesomeSem, TM_INFINITE);
	
	
	//Timed start
	rt_mutex_acquire(&mutexA,TM_INFINITE);
	//rt_sem_wait(&resource,TM_INFINITE);
	print_pri(&task1,"[A] low pri task locked mutexA\n");
	set_priority(&task1,4);
	busy_wait_ms(3);
	rt_mutex_acquire(&mutexB,TM_INFINITE);
	print_pri(&task1,"[A] low pri task locked mutexB\n");
	busy_wait_ms(3);
	print_pri(&task1,"Task A now releasing mutexA\n");
	rt_mutex_release(&mutexA);
	print_pri(&task1,"Task A now releasing mutexB\n");
	rt_mutex_release(&mutexB);
	set_priority(&task1,1);	
	print_pri(&task1,"Task A now released mutexB\n");
	
	busy_wait_ms(1);
	//rt_sem_signal(&resource);

}
void xenTaskB(void *arg){
	rt_printf("Task B now waiting for semaphore\n");
	rt_sem_wait(&awesomeSem, TM_INFINITE);
	
	//Timed start
	rt_task_sleep_ms(1);
	rt_mutex_acquire(&mutexB,TM_INFINITE);
	print_pri(&taskb,"[B] hi pri task locked mutexB\n");
	busy_wait_ms(1);
	rt_mutex_acquire(&mutexA,TM_INFINITE);
	print_pri(&taskb,"[A] lo pri task locked mutexA\n");
	busy_wait_ms(2);
	print_pri(&taskb,"Task B now releasing mutexA\n");
	rt_mutex_release(&mutexA);
	print_pri(&taskb,"Task B now releasing mutexB\n");
	rt_mutex_release(&mutexB);
	print_pri(&task1,"Task B now releasing mutexB\n");
	rt_printf("Task B now finnished\n");
}

void  xenTaskC(void *arg){
	rt_printf("Task C initialized, now waiting to broadcast sempahore\n");
	usleep(100000);
	rt_printf("2..\n");
	usleep(1000000);
	rt_printf("1..\n");
	usleep(1000000);
	rt_printf("RELEASSAAASEE\n");
	rt_sem_broadcast(&awesomeSem);
	usleep(100000);
}

int main(int argc, char **argv){
	rt_print_auto_init(1);
	int disturbance=1;
	rt_mutex_create(&mutexA,"A");
	rt_mutex_create(&mutexB,"B");
	rt_sem_create(&awesomeSem,"awesomeSem",0,S_FIFO);
	rt_sem_create(&resource,"resource",1,S_FIFO);
	rt_printf("Initiated io\n");
	mlockall(MCL_CURRENT|MCL_FUTURE);
	
	
	rt_task_create(&task1,"Response A",0,1,T_CPU(1)|T_JOINABLE);
	rt_task_create(&taskb,"Response B",0,2,T_CPU(1)|T_JOINABLE);
	rt_task_create(&taskc,"Response C",0,4,T_CPU(1)|T_JOINABLE);
	//rt_task_create(&taskd,"Response D",0,3,T_CPU(1)|T_JOINABLE);
	
	rt_task_start(&task1,&xenTaskA,NULL);
	rt_task_start(&taskb,&xenTaskB,NULL);
	rt_task_start(&taskc,&xenTaskC,NULL);
	//rt_task_start(&taskd,&xenTaskD,NULL);
	
	rt_task_join(&task1);
	rt_task_join(&taskb);
	rt_task_join(&taskc);
	//rt_task_join(&taskd);
	
	rt_sem_delete(&awesomeSem);
	rt_sem_delete(&resource);
	rt_mutex_delete(&mutexA);
	rt_mutex_delete(&mutexB);
	
	return 0;
}
		
			
