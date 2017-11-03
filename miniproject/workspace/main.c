/*
 * main.c
 *
 *  Created on: Oct 17, 2017
 *      Author: root
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>

#include "miniproject.h"
#define timercmp(a, b, CMP) \
(((a)->tv_sec == (b)->tv_sec) ? \
((a)->tv_nsec CMP (b)->tv_nsec) : \
((a)->tv_sec CMP (b)->tv_sec))

void sendToServer( char * data);
void* serverResponse(void *);
void* pidCtrl(void *);
void* udp(void *);

volatile TOTAL_RUNNING_THREADS=1;
pthread_mutex_t udp_send_mutex;
sem_t udp_received_y;
sem_t udp_received_signal;

pthread_t udp_thread, pid_thread, reply_thread;


struct udp_conn udp_connection;
volatile double y;
double Kp=10;
double Ki=800;
int main(){
	printf("Hello\n");


	if(udp_init_client(&udp_connection,9999,"192.168.0.1")==1){
		printf("Error during UDP_init\n");
		return 1;
	}
	else{
		printf("UDP ok\n");
	}

	pthread_mutex_init(&udp_send_mutex,NULL);
	sem_init(&udp_received_y,1,0);
	sem_init(&udp_received_signal,1,0);

	pthread_create(&udp_thread,NULL,udp,NULL);
	pthread_create(&pid_thread,NULL,pidCtrl,NULL);
	pthread_create(&reply_thread,NULL,serverResponse,NULL);


	pthread_join(pid_thread,NULL);
	TOTAL_RUNNING_THREADS=0;
	sendToServer("STOP");
	udp_close(&udp_connection);
	printf("CLosing down\n");

	return 0;
}

void * udp(void *r){ //thread
	char udp_buffer[50];

	while(1){
		if(udp_receive(&udp_connection,udp_buffer,50)>0){
			switch(udp_buffer[0]){
				case 'G':
					y=atof(&udp_buffer[8]);
					sem_post(&udp_received_y);
					break;
				case 'S':
					sem_post(&udp_received_signal);
					break;
			}
			if(TOTAL_RUNNING_THREADS==0){
				return r;
			}
		}
	}
}
void* serverResponse(void *r){
	char response[11]="SIGNAL_ACK";
	while(1){
		sem_wait(&udp_received_signal);
		sendToServer(response);
		if(TOTAL_RUNNING_THREADS==0){
			return r;
		}
	}

}
void sendToServer(char * data){
	pthread_mutex_lock(&udp_send_mutex);
	udp_send(&udp_connection,data,strlen(data)+1);
	pthread_mutex_unlock(&udp_send_mutex);
}

void* pidCtrl(void * r){ //thread
	struct timespec period, endtime;
	const double PERIOD= 0.0036; //Must match period [timespec)]
	const double PERIOD_time =1/PERIOD;
	char pid_Ctrl_buf[50];
	double error, integral, u, length;
	const double reference=1.0; //for step response?
	clock_gettime(CLOCK_REALTIME,&period);
	clock_gettime(CLOCK_REALTIME,&endtime);
	sendToServer("START");

	timespec_add_us(&endtime,500*1000);
	while(timercmp(&endtime,&period, >)==1){
		sendToServer("GET");
		sem_wait(&udp_received_y);

		error=reference-y;
		integral=integral+error*PERIOD;
		u=Kp*error+Ki*integral;

		sprintf(pid_Ctrl_buf,"SET:%f",u);
		sendToServer(pid_Ctrl_buf);
		timespec_add_us(&period,PERIOD*1000000);
		new_clock_nanosleep(&period);

	}
	return r;
}
