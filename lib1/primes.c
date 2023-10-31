#include "user/user.h" 
#include "kernel/types.h"
#include "kernel/stat.h"

void rec(int mpipe[2]){
    int a[2];
    int first,ano;

    close(0);
    dup(mpipe[0]);
    close(mpipe[0]);
    close(mpipe[1]);
     if(read(0,&first,4)){//如果接受不到，代表结束，通知父进程不用递归
             printf("prime %d\n",first);
             pipe(a);
            if (fork()==0){//son
                     rec(a);
                }else{//father
                    while (read(0,&ano,4)){//循环写非first倍数
                        if (ano%first!=0) write(a[1],&ano,4);
                    }
                    close(0);
                    close(a[1]);
                    wait(0);
                } 

        }else{
             close(0);
        
        }
        exit(0);

     
}


void rec1(int mpipe[2]) {
    int primes, nums;
    int a[2];

    close(0); // 0的复用
    dup(mpipe[0]);
    close(mpipe[0]);
    close(mpipe[1]);

    if (read(0, &primes, 4)) {
        printf("prime %d\n", primes); // 打印由父进程传来的第一个数字

        pipe(a);
        if (fork() == 0) {
            rec(a); // 由子进程筛选下一批质数
        } // 思考：考虑子进程已经在读、但是父进程还没写完的情况，子进程会等吗，还是报错呢？
        else {
            while (read(0, &nums, 4)) { // 从父进程读取数据
                if (nums % primes != 0) { // 筛查，将符合条件的数字传给子进程
                    write(a[1], &nums, 4);
                }
            }
            close(a[1]);
            close(0);
            wait(0);
        }
    } else {
        close(0); // 递归出口：若父进程无数据输入，则说明筛查完毕
    }
    exit(0);
}
int main(){
    int mpipe[2];
    pipe(mpipe);
    //往pipe里写
    for (int i = 2; i <=33; i++) write(mpipe[1],&i,4);
    close(mpipe[1]);
    rec(mpipe);
   return 0;

}
