#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <pthread.h>


typedef struct{
    int proc;
    int thr;
    sem_t *sem;
}TH_STRUCT;

sem_t m3;
sem_t m4;


void *thread_function1 (void* arg)
{

    TH_STRUCT *s = (TH_STRUCT*)arg;

    if(s->proc == 3)
    {
       sem_wait(s->sem);

    }

    info(BEGIN,s->proc, s->thr);

    info(END, s->proc, s->thr);
    if(s->proc == 3)
    {
        sem_post(s->sem);
    
    }
    return NULL;
}

void *thread_f(void *arg)
{
    TH_STRUCT *s = (TH_STRUCT*)arg;
    if(s->proc == 2 && s->thr == 3)
    {
        sem_wait(&m3);
    
    }
    info(BEGIN, s->proc, s->thr);

    if(s->proc == 2 && s->thr == 4)
    {
        sem_post(&m3);
        sem_wait(&m4);

    }
    info(END, s->proc, s->thr);

    if (s->proc == 2 && s->thr == 3)
    {
        sem_post(&m4);
        
    }
    return NULL;
}

int main(){
    
    TH_STRUCT param[40];
    TH_STRUCT param2[5];
    TH_STRUCT param6[6];
    pthread_t tids6[6];
    pthread_t tids2[5];
    pthread_t tids[40];
    int  status2 = 0, status3 = 0, status6 = 0, status4 = 0, status5 = 0, status7 = 0;
    init();
    info(BEGIN, 1, 0);
    if(fork() == 0)
    {
        info(BEGIN, 2, 0);
        //sem_t sem2;
        sem_init(&m3,0,0);
        sem_init(&m4,0,0);


        for(int i = 0; i < 5; i++)
        {
            param2[i].proc = 2;
            param2[i].thr = i+1;

            //param2[i].sem = &sem2;
            /*if(i == 2)
            {
                pthread_mutex_init(&m3, NULL);
            }
            if(i == 3)
            {
                pthread_mutex_init(&m4, NULL);
            }*/
            pthread_create(&tids2[i],NULL, thread_f, &param2[i]);
        }

        for(int i = 0; i < 5; i++)
        {
            pthread_join(tids2[i], NULL);
        }
        sem_destroy(&m3);
        sem_destroy(&m4);
        


        if(fork() == 0)
        {
            info(BEGIN, 3 , 0);
            sem_t sem;
            sem_init(&sem,0,4);

            for(int i = 0; i < 40; i++)
            {
                param[i].proc = 3;
                param[i].thr = i+1;
                param[i].sem = &sem;
                pthread_create(&tids[i],NULL,thread_function1, &param[i]);
            }

            for(int i = 0; i < 40; i++)
            {
                pthread_join(tids[i], NULL);
            }
            sem_destroy(&sem);


            if(fork() == 0)
            {
                info(BEGIN, 4, 0);
                if(fork() == 0)
                {
                    info(BEGIN, 6, 0);
                    sem_t sem6;
                    sem_init(&sem6, 0, 1);

                    for(int i = 0; i < 6; i++)
                    {
                        param6[i].proc = 6;
                        param6[i].thr = i+1;
                        param6[i].sem = &sem6;
                        pthread_create(&tids6[i], NULL, thread_function1, &param6[i]);
                    }
                    for(int i = 0; i < 6; i++)
                    {
                        pthread_join(tids6[i],NULL);
                    }
                    sem_destroy(&sem6);
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