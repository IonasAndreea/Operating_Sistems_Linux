#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"

int main(){
    int  status2 = 0, status3 = 0, status6 = 0, status4 = 0, status5 = 0, status7 = 0;
    init();
    info(BEGIN, 1, 0);
    if(fork() == 0)
    {
        info(BEGIN, 2, 0);
        if(fork() == 0)
        {
            info(BEGIN, 3 , 0);
            if(fork() == 0)
            {
                info(BEGIN, 4, 0);
                if(fork() == 0)
                {
                    info(BEGIN, 6, 0);
                    info(END, 6, 0);
                }
                else
                { 
                    wait(&status6);
                    info(END, 4, 0);
        
                }
            }
            else
            {
                wait(&status4);
                info(END, 3, 0);

            }
        }
        else
        {
            if(fork() == 0)
            {
                info(BEGIN, 5, 0);
                info(END, 5, 0);
            }
            else
            {
                wait(&status3);
                wait(&status5);
                info(END, 2 , 0);

            }
            
        }
        
    }
    else{
        if(fork() == 0)
        {
            info(BEGIN, 7, 0);
            info(END, 7, 0);
        }
        else
        {
            wait(&status7);
            wait(&status2);
            info(END, 1, 0);
        }

   
    }
    
    return 0;
}