#include "user/user.h" 
#include "kernel/types.h"
#include "kernel/stat.h"
int main(int argc,char* argv[]){
	if(argc==2){
		sleep(atoi(argv[1]));
		printf("sleep tick : %d",argv[1]);
		exit(0);
	}else{
		printf("arguements number is not correct");
		exit(1);
	}
	return  0;
}

