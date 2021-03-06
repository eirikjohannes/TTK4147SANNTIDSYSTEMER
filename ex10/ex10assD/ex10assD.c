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
#include "fifo.h"
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

volatile int global_blocked_list[255];

volatile char GLOBAL_BUF[global_buf_size];
volatile int GLOBALCOUNTER=0;
volatile int GLOBALCMD=0;

dispatch_t              *dpp;
resmgr_attr_t           resmgr_attr;
dispatch_context_t      *ctp;
resmgr_connect_funcs_t  connect_funcs;
resmgr_io_funcs_t       io_funcs;
iofunc_attr_t io_attr;

pthread_mutex_t buf_mutex, queue_mutex;
volatile fifo_t newQueue;

int io_read(resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb);
int io_write(resmgr_context_t *ctp, io_write_t *msg, iofunc_ocb_t* ocb);




char buf_D[255];

long counter = 0;
int blocking=0;
int nonblock = 1;
int client_id=0;

void error(char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	pthread_t t_id;
	printf("Start resource manager\n");
	// create dispatch.
	if (!(dpp = dispatch_create()))
		error("Create dispatch");
	init_fifo(&newQueue);

	pthread_mutex_init(&buf_mutex,NULL);
//	pthread_create(&t_id,NULL,&countingThread,NULL);

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

	if ( blocking ) {
		_IO_SET_READ_NBYTES(ctp, 0);
		return (_RESMGR_NPARTS(0));
	}


	if(!ocb->offset)
	{
		if ( fifo_status(&newQueue) ) {

			pthread_mutex_lock( &queue_mutex );
			fifo_rem_string( &newQueue, buf_D);
			printf( "Return %s\n", buf_D );
			SETIOV(ctp->iov, buf_D, strlen(buf_D));
			_IO_SET_READ_NBYTES(ctp, strlen(buf_D));

			ocb->offset = 1;
			pthread_mutex_unlock( &queue_mutex );
			return (_RESMGR_NPARTS(1));

		} else {
			if ( nonblock ) {
				printf( "Return nothing\n" );
				_IO_SET_READ_NBYTES(ctp, 0);
				return (_RESMGR_NPARTS(0));
			} else {
				printf( "Blocking\n" );
				blocking = 1;
				client_id = ctp->rcvid;
				return (_RESMGR_NOREPLY);
			}
		}
	}
	else
	{
		// return nothing
		_IO_SET_READ_NBYTES(ctp, 0);
		return (_RESMGR_NPARTS(0));
	}
}
int io_write(resmgr_context_t *ctp, io_write_t *msg, iofunc_ocb_t* ocb){

	printf( "Echo/Write command task D\n" );

	_IO_SET_WRITE_NBYTES (ctp, msg->i.nbytes);

	pthread_mutex_lock( &queue_mutex );
	resmgr_msgread(ctp, buf_D, msg->i.nbytes, sizeof(msg->i));
	buf_D [msg->i.nbytes] = '\0';
	fifo_add_string( &newQueue, buf_D );
	fifo_print(&newQueue);

	if ( blocking ) {
		printf( "Block released\n" );
		fifo_rem_string( &newQueue, buf_D);
		printf( "Sending %s\n", buf_D );
		MsgReply(client_id, 0, buf_D, strlen(buf_D)+1);
		blocking = 0;
	}
	pthread_mutex_unlock( &queue_mutex );

	if (msg->i.nbytes > 0)
		ocb->attr->flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;

	return (_RESMGR_NPARTS (0));
}























