#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

pthread_t PIDARR[5];
pthread_mutex_t forks[5]={PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER};
sem_t waiter;

void* philosopher(void *arg){
	pthread_t PID=pthread_self();
	pthread_mutex_t* right;
	pthread_mutex_t* left;	
	int ID=-1;
	if(PID==(PIDARR[0])){
		right=&forks[1];
                left=&forks[0];
		ID=0;
       	}
	else if(PID==PIDARR[1]){
		right=&forks[2];
	        left=&forks[1];
        	ID=1;
	}
	else if(PID==PIDARR[2]){
		right=&forks[3];
                left=&forks[2];
        	ID=2;
	}
	else if(PID==PIDARR[3]){
                right=&forks[4];
                left=&forks[3];
        	ID=3;
	}
        else if(PID==PIDARR[4]){
                right=&forks[0];
                left=&forks[4];
        	ID=4;
	}
	/*	case PIDARR[3]:
			pthread_mutex_t* right=&forks[4];
                        pthread_mutex_t* left=&forks[3];
                        break;
		case PIDARR[4]:
			pthread_mutex_t* right=&forks[0];
			pthread_mutex_t* left=&forks[4];
			break;
		case default:
			prinft("FUUUCK");
			exit(1);
	}*/
	
	
	bool eating=0;
	while(1){
		if(eating){
			printf("Philosopher %u now eating\n",ID);
			usleep(10000);
			pthread_mutex_unlock(right);	
			printf("Philosopher %u laid down right fork\n",ID);
			pthread_mutex_unlock(left);
			printf("Philosopher %u laid down left fork\n",ID);
			eating=0;
			sem_post(&waiter);
		}
		else{
			printf("Philosopher %u now thinking \n",ID);
                        printf("Philosopher %u now wants to eat\n",ID);
                        sem_wait(&waiter);
			pthread_mutex_lock(left);
			printf("Philosopher %u picked up left fork\n",ID);
			pthread_mutex_lock(right);
			printf("Philosopher %u picked up right fork\n",ID);
			eating=1;
		}
	}
}

int main(int argc, char **argv){
	/*for(int i=0;i<5;i++){
        	forks[i]=PTHREAD_MUTEX_INITIALIZER;
	}*/
	sem_init(waiter,0,2);
	for(int i=0;i<5;i++){
	        pthread_create(&(PIDARR[i]),NULL,philosopher,NULL);
        }
	
        pthread_join((PIDARR[0]),NULL);
        pthread_join((PIDARR[1]),NULL);
        pthread_join((PIDARR[2]),NULL);
        pthread_join((PIDARR[3]),NULL);
        pthread_join((PIDARR[4]),NULL);
       
	return 0;
}
		
			
