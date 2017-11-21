#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/dispatch.h>
#include <sys/mman.h>

#define memloc "/sharedpid"

/* TO SHOW WHERE THIS IS LOCATED, OPEN TERMINAL:
	cd ../dev/shmem/sharedpid


C:\Users\stined\VirtualBox VMs\QNX Target
 */

typedef struct pid_data{
	pthread_mutex_t pid_mutex;
	pid_t pid;
}pid_data_t;


pid_t thisPID;
pthread_mutexattr_t myattr;

int set_priority(int priority);
int get_priority();

int main(int argc, char *argv[]) {
	printf("Welcome to the server\n");
	int fildescr=shm_open(memloc,O_RDWR|O_CREAT,S_IRWXU);
	ftruncate(fildescr,sizeof(pid_data_t));
	pid_data_t *ptr=mmap(0,sizeof(pid_data_t),PROT_READ|PROT_WRITE,MAP_SHARED,fildescr,0);

	pthread_mutexattr_init(&myattr);
	pthread_mutexattr_setpshared(&myattr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&ptr->pid_mutex,&myattr);

	/*SERVER*/
	//int channelID=ChannelCreate(0);
	int channelID=ChannelCreate(_NTO_CHF_FIXED_PRIORITY);



	struct _msg_info info;

	thisPID=getpid();

	int msgID;
	pthread_mutex_lock(&ptr->pid_mutex);
	ptr->pid=thisPID;
	pthread_mutex_unlock(&ptr->pid_mutex);
	printf("Added %u to memloc\n",thisPID);
	set_priority(10);
	char databuffer[50];
	char replyBuf="Received data";
	while(1){
		printf("Before, priority: %i",get_priority());
		msgID=MsgReceive(channelID,&databuffer,sizeof(databuffer),&info);
		printf("After, priority: %i",get_priority());
		if(msgID!=NULL&&(msgID!=EFAULT||msgID!=EINTR||msgID!=ESRCH||msgID!=ETIMEDOUT)){
			printf("\nReceived msg from PID: %i and TID: %i\n",info.pid,info.tid);
			MsgReply(msgID,EOK,&replyBuf, sizeof(replyBuf));
			printf(databuffer);
			printf("\n");
		}

	};

	return EXIT_SUCCESS;
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


