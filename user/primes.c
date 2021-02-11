#include "kernel/types.h"
#include "user/user.h"



int main(){
    int pipedes[2];
    int global_fd;
    pipe(pipedes);
    //son
    if (!fork())
    {
    	close(pipedes[1]);
        global_fd=pipedes[0];
        
        int number;
   while(1)
   {
        if (read(global_fd, &number, 0x4) <=0||number<=0)
        {
          close(global_fd);  
          exit(0);
        }
        printf("prime %d\n", number);
        int prime=number;
        int son_pipes[2];
        pipe(son_pipes);
        if (fork())
        {
            close(son_pipes[0]);
            while(read(global_fd, &number, 0x4)==4)
            {
                if (number%prime)
                {
                    write(son_pipes[1],&number,4);
                }
            }
            close(global_fd);
            close(son_pipes[1]);
            wait(0);
            exit(0); 
        }
        else
        {
            close(son_pipes[1]);
            close(global_fd);
            global_fd=son_pipes[0];
        }
   }
    }
    else
    {
        close(pipedes[0]);
        for (int i=2;i<=31;i++)
        {
            write(pipedes[1],&i,4);
        }
        close(pipedes[1]);
        wait(0);
    } 
    exit(0);
}
