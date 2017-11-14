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

pid_t thisPid;
pthread_mutexattr_t myattr;

int main(int argc, char *argv[]) {
	printf("Welcome to the QNX Momentics IDE\n");
	int fildescr=shm_open(memloc,O_RDWR,S_IRWXU);
	//ftruncate(fildescr,sizeof(thisPid));
	pid_data_t* ptr=mmap(0,sizeof(pid_data_t),PROT_READ|PROT_WRITE,MAP_SHARED,fildescr,0);

	pthread_mutexattr_init(&myattr);
	pthread_mutexattr_setpshared(&myattr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&ptr->pid_mutex,&myattr);


	while(1){
		if(pthread_mutex_trylock(&ptr->pid_mutex)!=EBUSY){
			thisPid=ptr->pid;
			pthread_mutex_unlock(&ptr->pid_mutex);
			printf("Read the PID: %u\n",thisPid);
		}
		usleep(10000);
	};

	return EXIT_SUCCESS;
}
