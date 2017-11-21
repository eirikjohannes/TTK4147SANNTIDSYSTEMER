#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/mman.h> //Mutex?
#include <pthread.h>

dispatch_t              *dpp;
resmgr_attr_t           resmgr_attr;
dispatch_context_t      *ctp;
resmgr_connect_funcs_t  connect_funcs;
resmgr_io_funcs_t       io_funcs;
iofunc_attr_t           io_attr;

#define global_buf_size 100

#define CMDSTOP 0
#define CMDUP 1
#define CMDDOWN -1

volatile char GLOBAL_BUF[global_buf_size];
volatile int GLOBALCOUNTER=0;
volatile int GLOBALCMD=0;

pthread_mutex_t buf_mutex;
pthread_mutex_t counter_mutex, cmd_mutex;

int io_read(resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb);
int io_write(resmgr_context_t *ctp, io_write_t *msg, iofunc_ocb_t* ocb);
char buf[] = "Hello World\n";




void* countingThread(void*arg);

void error(char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	pthread_t t_id;
	printf("Start resource manager\n");
	sprintf(&GLOBAL_BUF,"HEI\n");
	// create dispatch.
	if (!(dpp = dispatch_create()))
		error("Create dispatch");

	pthread_mutex_init(&cmd_mutex,NULL);
	pthread_mutex_init(&counter_mutex,NULL);
	pthread_mutex_init(&buf_mutex,NULL);
	pthread_create(&t_id,NULL,&countingThread,NULL);

	// initialize resource manager attributes.
	memset(&resmgr_attr, 0, sizeof(resmgr_attr));
	resmgr_attr.nparts_max = 1;
	resmgr_attr.msg_max_size = 2048;

	// set standard connect and io functions.
	iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs, _RESMGR_IO_NFUNCS, &io_funcs);
	iofunc_attr_init(&io_attr, S_IFNAM | 0666, 0, 0);

	// override functions
	io_funcs.read = io_read;
	io_funcs.write = io_write;

	// establish resource manager
	if (resmgr_attach(dpp, &resmgr_attr, "/dev/myresource", _FTYPE_ANY, 0, &connect_funcs, &io_funcs, &io_attr) < 0)
		error("Resmanager attach");
	ctp = dispatch_context_alloc(dpp);

	// wait forever, handling messages.
	while(1)
	{
		if (!(ctp = dispatch_block(ctp)))
			error("Dispatch block");
		dispatch_handler(ctp);
	}

	exit(EXIT_SUCCESS);
}

int io_read(resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb)
{
	int strlength;
	char buffer2[255];
	int i;
	if(!ocb->offset)
	{

		// set data to return
		/*
		pthread_mutex_lock(&buf_mutex);
		strlength=strlen(GLOBAL_BUF);
		SETIOV(ctp->iov, GLOBAL_BUF, strlength);
		pthread_mutex_unlock(&buf_mutex);
		*/


		pthread_mutex_lock(&counter_mutex);
		sprintf(buffer2,"%i\n",GLOBALCOUNTER);
		//strcat(buffer2,"\n");
		pthread_mutex_unlock(&counter_mutex);
		SETIOV(ctp->iov,buffer2,strlen(buffer2));
		_IO_SET_READ_NBYTES(ctp,strlen(buffer2));
		printf("IO READ reads: %s\n",buffer2);
		// increase the offset (new reads will not get the same data)
		ocb->offset = 1;

		// return
		return (_RESMGR_NPARTS(1));
	}
	else
	{
		// set to return no data
		_IO_SET_READ_NBYTES(ctp, 0);

		// return
		return (_RESMGR_NPARTS(0));
	}
}

int io_write(resmgr_context_t *ctp, io_write_t *msg, iofunc_ocb_t* ocb){
	int status;
	char buffer[20];
	//GLOBAL BUF iS ARR//
	//DRopping status and xtype

	//Number of bytes to retrn
	_IO_SET_WRITE_NBYTES (ctp,msg->i.nbytes);
	if(msg->i.nbytes>global_buf_size)
	{
		return ENOMEM; //of some sort
	}


	resmgr_msgread(ctp,buffer,msg->i.nbytes,sizeof(msg->i));
	buffer[msg->i.nbytes]='\0';
	printf("Received %d bytes=%s",msg->i.nbytes,buffer);

	int currentCMD=0;
	if(strstr(buffer,"up")){
		currentCMD=1;
		printf("Mapped to UP\n");
	}
	else if(strstr(buffer,"down")){
		currentCMD=-1;
		printf("Mapped to DOWN\n");
	}
	else if (strstr(buffer,"stop")){
		currentCMD=0;
		printf("Mapped to STOP\n");
	}
	else
		printf("COULDNT MAP, WAAAH\n");


	pthread_mutex_lock(&cmd_mutex);
	GLOBALCMD=currentCMD;
	pthread_mutex_unlock(&cmd_mutex);

	if(msg->i.nbytes>0){
		ocb->attr->flags |=IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;
	}
	return (_RESMGR_NPARTS(0));
}


void* countingThread(void* arg){
	//CMD initialized to zero
	while(1){
		pthread_mutex_lock(&cmd_mutex);
		switch(GLOBALCMD){
		case CMDSTOP:
			//DO nothing
			break;
		case CMDUP:
			pthread_mutex_lock(&counter_mutex);
			GLOBALCOUNTER++;
			printf("Counting up: %i\n",GLOBALCOUNTER);
			pthread_mutex_unlock(&counter_mutex);
			break;
		case CMDDOWN:
			pthread_mutex_lock(&counter_mutex);
			GLOBALCOUNTER--;
			printf("Counting down: %i\n",GLOBALCOUNTER);
			pthread_mutex_unlock(&counter_mutex);
			break;
		default:
			break;
		}
		pthread_mutex_unlock(&cmd_mutex);

		delay(100);
	}
}


























