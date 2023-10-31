#include "user/user.h" 
#include "kernel/types.h"
#include "kernel/stat.h"
int main(int argc, char *argv[]){

    int fd1[2],fd2[2];

    pipe(fd1);
    pipe(fd2);
    char c[1];
    int pid=fork();

    if (pid>0)//father
    {
        write(fd1[1]," ",1);
        wait(0);
        read(fd2[0],c,1);

        printf("%d: received pong\n",getpid());
         exit(0);
    }else if(pid==0){//son

        printf("%d: received ping\n",getpid());
        if(read(fd1[0],c,1)==0) printf("%d: received ping",getpid());;
        write(fd2[1]," ",1);

       exit(0);
    }
    return 0;
}