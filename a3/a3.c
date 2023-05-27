#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include<string.h>
#include <string.h>
#include <sys/mman.h>


typedef struct section_headers
{
    char sect_name[9];
    char sect_type;
    int sect_offset;
    int sect_size;
}s_headers;


char buffer[100];
int dim;
char fileName[512];

int main(int argc, char**argv)
{
    if(mkfifo("RESP_PIPE_35816",0644) != 0)
    {
        perror("ERROR\ncannot create the response pipe\n");
        return 1;
    }
    
    int fd = -1;
    int fd2 = -1;

    fd = open("REQ_PIPE_35816",O_RDONLY);

    if(fd == -1) {
        perror("ERROR\ncannot open the request pipe\n");
        return 1;
    }

    fd2 = open("RESP_PIPE_35816",O_WRONLY);

    if(fd2 == -1) {
        perror("ERROR\ncannot open the request pipe\n");
        return 1;
    }

    int text = (int )write(fd2, "START#",6);
    if(text == 6)
    {
        printf("SUCCESS\n");
    }
  
   unsigned int nr;
   volatile unsigned int *sharedMem = NULL;
   unsigned int offset, value;
    while(1)
    {

        int i= 0 ;

        while(1)
        {
        read(fd, &buffer[i], 1);
            if(buffer[i] == '#' )
            {
                break;
            }
            i++;
        }
        
        if(strncmp(buffer, "EXIT#",5) == 0)
        {
            printf("EXIT\n");
            break;
        }

        if(strncmp(buffer, "VARIANT#",8) == 0)
        {
            
            write(fd2, "VARIANT#", 8);
            write(fd2, "VALUE#",6);
            unsigned int varianta = 35816;
            write(fd2, &varianta, sizeof(unsigned int));
        }

        if(strncmp(buffer,"CREATE_SHM#",11) == 0)
        {
            read(fd,&nr,sizeof(unsigned int));
            int shmFd = shm_open("/d5mpp1", O_CREAT | O_RDWR , 0664);
            if (shmFd < 0)
            {
                write(fd2, "CREATE_SHM#", 11);
                write(fd2, "ERROR#",6);
                return 1;
            }

            ftruncate(shmFd, 2420777);
            sharedMem = mmap(NULL, 2420777, PROT_READ | PROT_WRITE, MAP_SHARED,shmFd, 0);
            if (sharedMem == (void*)-1)
            {
                write(fd2, "CREATE_SHM#", 11);
                write(fd2, "ERROR#",6);
                shm_unlink("/d5mpp1");
                return 1;
            }

            write(fd2, "CREATE_SHM#", 11);
            write(fd2, "SUCCESS#",8);
        }

        if(strncmp(buffer,"WRITE_TO_SHM#",13) == 0)
        {
            read(fd, &offset, sizeof(unsigned int));
            read(fd, &value, sizeof(unsigned int));

            if(offset <0 || offset >=2420777)
            {
                write(fd2, "WRITE_TO_SHM#", 13);
                write(fd2, "ERROR#",6);
            }
            //*(sharedMem + offset)= value * sizeof(unsigned int);
            unsigned int* pozitie = (unsigned int*)((char*)sharedMem + offset);
            *pozitie = value;
            write(fd2, "WRITE_TO_SHM#", 13);
            write(fd2, "SUCCESS#",8);

        }

        if(strncmp(buffer, "MAP_FILE#",9) == 0)
        {
            int i= 0 ;
            while(1)
            {  
                read(fd, &fileName[i], 1);
                if(fileName[i] == '#' )
                    {   
                    break;
                    }
                i++;
            }
            int fd_f = open(fileName, O_RDONLY);
            if(fd_f == -1)
            {
                write(fd2, "MAP_FILE#", 9);
                write(fd2, "ERROR#",6);
            }

            write(fd2, "MAP_FILE#", 9);
            write(fd2, "SUCCESS#",8);

        }


    
    }
    munmap((void*)sharedMem, 2420777);
    shm_unlink("/d5mpp1");

    remove("RESP_PIPE_35816");
    remove("REQ_PIPE_35816");
    close(fd);
    close(fd2);
}


