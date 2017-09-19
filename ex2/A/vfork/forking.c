#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

int globalvariabel=0;


int main(){
	int lokalvariabel=0;
	
	pid_t forkPid=vfork();
	for(int i=0;i<20;i++){
		globalvariabel+=1;
		lokalvariabel+=2;
		if (forkPid==0){
			printf("Child: %i, %i\n",globalvariabel, lokalvariabel);
		}
		else{
                        printf("Parent: %i, %i\n",globalvariabel, lokalvariabel);
		}
	}
	if(forkPid==0){
		
		printf("Childprocess: Lokalvariabel: %i\n",lokalvariabel);
		printf("ChildProcess Globalvariabel: %i\n",globalvariabel);
	}
	else{
		printf("Parentprocess: Lokalvariabel: %i\n",lokalvariabel);
                printf("ParentProcess Globalvariabel: %i\n",globalvariabel);

	}


	return 0;
}
