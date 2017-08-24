#include <stdio.h>
#include "sort.h"

int main(int argc, char *argv[]){
	int i;
	printf("Arguments before sort: ");
	for (i =1; i<argc;i++){
		printf("%s ",argv[i]);
	}
	printf("\n");
	sort(argc,argv);
	printf("Args after sort");
	for (i =1; i<argc;i++){
                printf("%s ",argv[i]);
        }
	return 0;
}

/*
int main(int argc, char* argv[]){
	while (argc--){
		printf("%s\n",*argv++);
	}
	return 0;

}*/
