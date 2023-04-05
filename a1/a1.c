#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

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
    char path1[1000];
    char p[10];
    char n[100];
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

        }

         return 0;
    }
}

