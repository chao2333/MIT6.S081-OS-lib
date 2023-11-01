#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc, char* argv[]){
    char buf[512];
    int length = read(0, buf, sizeof(buf));//从标准输入读数据
    //把参数复制到xargs中
    int xargc = 0;
    char* xargv[MAXARG];    
    for (int i = 1; i < argc; i++)
    {
        xargv[xargc] = argv[i];
        xargc++;
    }
    //把标准输入内容参数复制到xargs中
    char *p=buf;
    for (int i = 0; i < length; i++)
    {
        if (buf[i] == '\n')
        {
            if (fork()>0)
            {
                p = &buf[i+1];
                wait(0);
            }
            else
            {
                buf[i] = 0;
                xargv[xargc] = p;
                xargc++;
                xargv[xargc] = 0;
                xargc++;
                exec(xargv[0],xargv);
                exit(0);
            }
            
        }
              
    }
    exit(0);
}