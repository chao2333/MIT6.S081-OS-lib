#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}
void find(char* fileName, char* path)
{
    int fd;
    char buf[512],*p;
    struct stat st;
    struct dirent de;


    fd = open(path, 0);
    if (fd < 0)  //尝试打开目录
    {
        fprintf(2, "cannot open %s", path);
        return;
    }
    strcpy(buf,path);
    p = buf+strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
        {
            continue;
        }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0)
        {
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        if (st.type == T_FILE)
        {
            char* nameBuf = fmtname(buf);
            for (int  i = 0; i < strlen(fileName); i++)
            {
                if (fileName[i]!=nameBuf[i])
                {
                    break;
                }
                else if (i == strlen(fileName)-1)
                {
                    printf("%s %d %d %d\n", buf, st.type, st.ino, st.size);
                }
            }
        }
        else if (st.type == T_DIR && strcmp(p,".") != 0 && strcmp(p,"..") != 0)
        {   
            find(fileName, buf);
        }
        
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(2, "arguement error");
        exit(0);
    }
    if (argc != 3)
    {
        fprintf(2, "arguement nums error");
    }
    else
    {
        find(argv[2], argv[1]);
    }

    exit(0);
}