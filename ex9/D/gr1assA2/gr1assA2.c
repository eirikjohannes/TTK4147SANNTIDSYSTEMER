#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/dispatch.h>
#include <sys/mman.h>
#include <stdbool.h>
#define memloc "/sharedpid"
typedef struct pid_data{
	pthread_mutex_t pid_mutex;
	pid_t pid;
}pid_data_t;

pid_t thisPid;
pthread_mutexattr_t myattr;
pid_t PID0,PID1,PID2,PID3;

void* runningThread(void *args);
int set_priority(int priority);
int get_priority();


int main(int argc, char *argv[]) {
	printf("Welcome to client[s]\n");
	int fildescr=shm_open(memloc,O_RDWR,S_IRWXU);
	//ftruncate(fildescr,sizeof(thisPid));
	pid_data_t* ptr=mmap(0,sizeof(pid_data_t),PROT_READ|PROT_WRITE,MAP_SHARED,fildescr,0);

	pthread_mutexattr_init(&myattr);
	pthread_mutexattr_setpshared(&myattr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&ptr->pid_mutex,&myattr);
	set_priority(1);
	pthread_create(&PID0,NULL,runningThread,ptr);
	pthread_create(&PID1,NULL,runningThread,ptr);
	pthread_create(&PID2,NULL,runningThread,ptr);
	pthread_create(&PID3,NULL,runningThread,ptr);

	pthread_join(PID0,NULL);
	pthread_join(PID1,NULL);
	pthread_join(PID2,NULL);
	pthread_join(PID3,NULL);

	return EXIT_SUCCESS;
}

void* runningThread(void *args){
	pid_data_t *ptr=args;
	char databuffer[50]="Success, memloc OK\n";
	char replybuf[]="Received data";
	int statval;
	struct _msg_info info;
	info.pid=getpid();
	info.tid=gettid();
	if(info.tid>3){
		set_priority(10+info.tid);
	}
	else{
		set_priority(10-info.tid);
	}
	printf("This thread has tid: %i",info.tid);

	int channelID=0;
	while(1){
		if(pthread_mutex_trylock(&ptr->pid_mutex)!=EBUSY){
			thisPid=ptr->pid;
			pthread_mutex_unlock(&ptr->pid_mutex);

			printf("Read the PID: %u\n",thisPid);
			if(!channelID){
						channelID = ConnectAttach(0,thisPid,1,0,0);
					}
			if(channelID){
				statval=MsgSend(channelID,&databuffer,sizeof(databuffer),&replybuf,sizeof("Received data"));
			}
			else statval=-10;
			if (statval!=-1){
				printf("Sent from tid[%i] : ",info.tid);
				printf(databuffer);
				printf(" to the server, received ");
				printf(replybuf);
				printf(" \t with the status: %i\n",statval);
			}
		}

		//usleep(100000);
	};
}

int set_priority(int priority)
{
	int policy;
	struct sched_param param;
	// check priority in range
	if (priority < 1 || priority > 63) return-1;
	// set priority
	pthread_getschedparam(pthread_self(), &policy, &param);
	param.sched_priority = priority;
	return pthread_setschedparam(pthread_self(),policy, &param);
}
int get_priority()
{
	int policy;
	struct sched_param param;
	// get priority
	pthread_getschedparam(pthread_self(), &policy, &param);
	return param.sched_curpriority;
}

