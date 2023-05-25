#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include<string.h>


typedef struct section_headers
{
    char sect_name[9];
    char sect_type;
    int sect_offset;
    int sect_size;
}s_headers;


char buffer[100];
int dim;

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
        //buffer[i-1] = '\0';
        //size_t size = strlen(buffer);;
        //read(fd, &dim, 1 );
        //char* request = (char *) malloc(size* sizeof(char)) ;
        //read(fd, request, length);
        //if(size<= 0)
        //{
          //  break;
        //}

        if(strncmp(buffer, "EXIT#",5) == 0)
        {
            printf("EXIT\n");
            break;
        }

        if(strncmp(buffer, "VARIANT#",8) == 0)
        {
            //write(fd2, &dim, 1);
            write(fd2, "VARIANT#", 8);
            //write(fd2, &dim,1);
            write(fd2, "VALUE#",6);
            unsigned int varianta = 35816;
            write(fd2, &varianta, sizeof(unsigned int));
        }
        //free(request);
    }

    remove("RESP_PIPE_35816");
    remove("REQ_PIPE_35816");
    close(fd);
    close(fd2);
}


