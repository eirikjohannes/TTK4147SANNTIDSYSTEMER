#include <stdlib.h>
#include <stdio.h>

void allocate(int value){
	int *ptr=NULL;
	ptr=malloc(1024*1024*1024*sizeof(char));
	if(ptr==NULL){
		//Error occured
		perror("Malloc failed to allocate more memory, exiting..\n");
		exit(1);
	}
	*ptr=value;
	printf("Test of allocated memory: %i\n", *ptr);
}

int main(int argc, char *argv[]){
	int i=0;
	while(1){
		allocate(i);
		i++;
	}

}
