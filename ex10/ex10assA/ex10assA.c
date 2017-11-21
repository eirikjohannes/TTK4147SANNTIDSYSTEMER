#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/mman.h> //Mutex?


dispatch_t              *dpp;
resmgr_attr_t           resmgr_attr;
dispatch_context_t      *ctp;
resmgr_connect_funcs_t  connect_funcs;
resmgr_io_funcs_t       io_funcs;
iofunc_attr_t           io_attr;

#define global_buf_size 100

volatile char GLOBAL_BUF[global_buf_size];

pthread_mutex_t buf_mutex;

int io_read(resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb);
int io_write(resmgr_context_t *ctp, io_write_t *msg, iofunc_ocb_t* ocb);
char buf[] = "Hello World\n";


void error(char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	printf("Start resource manager\n");
	sprintf(&GLOBAL_BUF,"HEI\n");
	// create dispatch.
	if (!(dpp = dispatch_create()))
		error("Create dispatch");

	pthread_mutex_init(&buf_mutex,NULL);

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
	if(!ocb->offset)
	{
		// set data to return
		pthread_mutex_lock(&buf_mutex);
		strlength=strlen(GLOBAL_BUF);
		SETIOV(ctp->iov, GLOBAL_BUF, strlength);
		pthread_mutex_unlock(&buf_mutex);
		_IO_SET_READ_NBYTES(ctp, strlength);

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
	//GLOBAL BUF iS ARR//
	//DRopping status and xtype

	//Number of bytes to retrn
	_IO_SET_WRITE_NBYTES (ctp,msg->i.nbytes);
	if(msg->i.nbytes>global_buf_size)
	{
		return ENOMEM; //of some sort
	}

	pthread_mutex_lock(&buf_mutex);
	resmgr_msgread(ctp,GLOBAL_BUF,msg->i.nbytes,sizeof(msg->i));
	GLOBAL_BUF[msg->i.nbytes]='\0';
	printf("Received %d bytes='%s'\n",msg->i.nbytes,GLOBAL_BUF);
	pthread_mutex_unlock(&buf_mutex);
	if(msg->i.nbytes>0){
		ocb->attr->flags |=IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;
	}
	return (_RESMGR_NPARTS(0));
}





























