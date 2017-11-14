#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/dispatch.h>
#include <sys/mman.h>

#define memloc "/sharedpid"

typedef struct pid_data{
	pthread_mutex_t pid_mutex;
	pid_t pid;
}pid_data_t;


pid_t thisPID;
pthread_mutexattr_t myattr;


int main(int argc, char *argv[]) {
	printf("Welcome to the QNX Momentics IDE\n");
	int fildescr=shm_open(memloc,O_RDWR|O_CREAT,S_IRWXU);
	ftruncate(fildescr,sizeof(pid_data_t));
	pid_data_t *ptr=mmap(0,sizeof(pid_data_t),PROT_READ|PROT_WRITE,MAP_SHARED,fildescr,0);

	pthread_mutexattr_init(&myattr);
	pthread_mutexattr_setpshared(&myattr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&ptr->pid_mutex,&myattr);

	/*SERVER*/
	int channelID=ChannelCreate(0);





	thisPID=getpid();

	int msgID;
	pthread_mutex_lock(&ptr->pid_mutex);
	ptr->pid=thisPID;
	pthread_mutex_unlock(&ptr->pid_mutex);
	printf("Added %u to memloc\n",thisPID);

	char databuffer[50];
	char replyBuf="Received data";
	while(1){
		msgID=MsgReceive(channelID,&databuffer,sizeof(databuffer),NULL);
		if(msgID!=NULL&&(msgID!=EFAULT||msgID!=EINTR||msgID!=ESRCH||msgID!=ETIMEDOUT)){
			printf(databuffer);
			MsgReply(msgID,EOK,&replyBuf, sizeof(replyBuf));
		}

	};

	return EXIT_SUCCESS;
}
