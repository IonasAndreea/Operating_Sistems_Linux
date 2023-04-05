#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>


typedef struct section_headers
{
    char sect_name[9];
    char sect_type;
    int sect_offset;
    int sect_size;
}s_headers;

void listRec(const char *path, int recursive, const char *name_S, int size)
 {
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[700];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL) {
        perror("Could not open directory");
        return;
    }
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0
            && strcmp(entry->d_name, "..") != 0) {
                snprintf(fullPath, 700, "%s/%s", path, entry->d_name);
                    if(lstat(fullPath, &statbuf) == 0) {
                        if(strcmp(name_S, "NULL" )!=0){
                            if(strncmp(entry->d_name, name_S, strlen(name_S)) == 0){
                            printf("%s\n", fullPath);
                        }
                        }
                        else if(size != -1)
                        {
                            if(statbuf.st_size > size && S_ISREG(statbuf.st_mode))
                            printf("%s\n", fullPath);
                        }
                         else  {
                         printf("%s\n", fullPath);

                         }
                     if(S_ISDIR(statbuf.st_mode) && recursive == 1) {
                     listRec(fullPath, recursive, name_S,size);
                        }
                }
            }
    }
 closedir(dir);
 }

 void parse(const char* path)
 {
    int fd = -1;
    char version;
    char no_of_sections;
    short header_size;
    char magic;

    fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Could not open input file\n");
        return ;
    }

    lseek(fd,-3,SEEK_END);
    read(fd,&header_size,2);
   // printf("%d", header_size);//

    read(fd,&magic,1);
    if(magic != 'R')
    {
        printf("ERROR\n");
        printf("wrong magic\n");
        return ;
    }

    lseek(fd,-header_size, SEEK_CUR);
    read(fd,&version,1);
    if(version < 21 || version > 71)
    {
        printf("ERROR\n");
        printf("wrong version\n");
        return ;
    }

    read(fd,&no_of_sections,1);
    if(no_of_sections < 4 || no_of_sections > 17)
    {
        printf("ERROR\n");
        printf("wrong sect_nr\n");
        return ;
    }


    s_headers *s_heds = (s_headers*)malloc(no_of_sections * sizeof(s_headers)); 
    for(int i = 0; i < no_of_sections; i++)
    {
        read(fd, s_heds[i].sect_name,8);
        read(fd, &s_heds[i].sect_type,1);
        if(s_heds[i].sect_type != 60 && s_heds[i].sect_type != 52 && s_heds[i].sect_type != 73 && s_heds[i].sect_type != 17)
        {
            printf("ERROR\n");
            printf("wrong sect_types\n");
            return ;

        }
        read(fd, &s_heds[i].sect_offset,4);
        read(fd, &s_heds[i].sect_size,4);
    }

    printf("SUCCESS\n");
    printf("version=%d\n", version);
    printf("nr_sections=%d\n", no_of_sections);

    for(int i = 0; i < no_of_sections; i++)
    {
        printf("section%d: %s %d %d\n", i+1, s_heds[i].sect_name, s_heds[i].sect_type, s_heds[i].sect_size);

    }

        free(s_heds);
        close(fd);
    }


/*int list(const char *path)
 {
   DIR *dir = NULL;
   struct dirent *entry = NULL;
   char fullPath[512];
   struct stat statbuf;

   dir = opendir(path);
   if(dir == NULL) {
        printf("Could not open directory!");
        return -1;
    }
 while((entry = readdir(dir)) != NULL) {
    if(strcmp(entry->d_name, ".") != 0
    && strcmp(entry->d_name, "..") != 0) {
        snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
        if(lstat(fullPath, &statbuf) == 0) {
        printf("%s\n", fullPath);
        }
    }
 }
   closedir(dir);
   return 0;
 }*/


int main(int argc, char **argv){
    char path1[1000], path_p[1000];
    char p[10];
    char n[100], p1[100];
    char nameStart[100] = "NULL";
    int recursive = 0;
    int size = -1 ;
    if(argc >= 2){
        if(strcmp(argv[1], "variant") == 0){
            printf("35816\n");
        }
        if(strcmp(argv[1],"list") == 0)
        {
            for(int i = 2; i < argc; i++){
                if(strncmp(argv[i],"path=",5)==0){ 
                sscanf(argv[i],"%[^=]=%s", p, path1);}
                
                 if(strcmp(argv[i], "recursive") == 0){
                        recursive=1;
        			}
                if(strncmp(argv[i], "name_starts_with=",17) == 0){
                    sscanf(argv[i],"%[^=]=%s", n, nameStart);
                }
                if(strncmp(argv[i], "size_greater=",13) == 0){
                    size = atoi(argv[i]+13);
            }
        
        }
         printf("SUCCESS\n");
            listRec(path1, recursive, nameStart, size);
        }

        if(strcmp(argv[1],"parse") == 0)
        {
               if(strncmp(argv[2], "path=",5) == 0){
                    sscanf(argv[2], "%[^=]=%s",p1, path_p);
                
            }
            parse(path_p);
        }

         return 0;
    }
}

