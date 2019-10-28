#include <stdio.h> 
#include <string.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <dirent.h>
#include <limits.h>
#include <time.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define MAX_LEN 128
#define COMMAND_NOTFOUND "Un(bear)able command. Type help and a little bear will show you the command list\0"

void print_Image();
void authUser();
void readUserInput();

int exitCommand = 0;
char userLoggedName[20]="";
  
int main()
{
    print_Image();
    printf("%s","Welcome to BearFiles. Please enter your bear name: \n");
    authUser();
    printf("Paw a command or type help and a little bear will show you a list with all commands available\n");
    do{
        readUserInput();
    }while(exitCommand==0);
    return 0;
}

char *split(char *str, const char *delim)
{
    char *p = strstr(str, delim);

    if (p == NULL) return NULL;     // delimiter not found

    *p = '\0';                      // terminate string after head
    return p + strlen(delim);       // return tail substring
}

static void list_dir (const char * dir_name, char * contructed_string, const char * file_name){
    DIR * d;

    /* Open the directory specified by "dir_name". */

    d = opendir (dir_name);

    /* Check it was opened. */
    if (! d) {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
    while (1) {
        struct dirent * entry;
        const char * d_name;

        /* "Readdir" gets subsequent entries from "d". */
        entry = readdir (d);
        if (! entry) {
            /* There are no more entries in this directory, so break
               out of the while loop. */
            break;
        }
        d_name = entry->d_name;
        
        if(strstr(d_name, file_name)){
            char name_and_dir[4096] = "";
            char name_formatter[4096] = "";
            char coloredName[100];
            char *tail;
            char tempColor[100] = "";
            strcat(tempColor, d_name);

            tail = split(tempColor, file_name);

            sprintf(coloredName,"%s%s%s%s%s",tempColor,KGRN,file_name,KWHT,tail);

            strcat(name_and_dir,dir_name);
            strcat(name_and_dir,"/");
            strcat(name_and_dir,d_name);

            
            strcat(name_formatter, coloredName);
            coloredName[0] = '\0';
            tempColor[0] = '\0';

            if(strlen(d_name)>=8){
                strcat(name_formatter,"\t");
            }else{
                strcat(name_formatter,"\t\t");
            }
            strcat(name_formatter, strstr(dir_name, "/tema_1"));
            
            strcat(name_formatter, "\t\t");

	        strcat(contructed_string, name_formatter);
            //printf(name_and_dir);

            struct stat sb;
            if (stat(name_and_dir, &sb) == -1) {
                perror("stat");
                exit(EXIT_FAILURE);
            }

            //file type
        switch (sb.st_mode & S_IFMT) {
            case S_IFBLK:  strcat(contructed_string, "block device\t");           break; 
            case S_IFCHR:  strcat(contructed_string, "character device\t");       break; 
            case S_IFDIR:  strcat(contructed_string, "directory\t");              break; 
            case S_IFIFO:  strcat(contructed_string, "FIFO/pipe\t");              break; 
            case S_IFLNK:  strcat(contructed_string, "symlink\t");                break; 
            case S_IFREG:  strcat(contructed_string, "regular file\t");           break; 
            case S_IFSOCK: strcat(contructed_string, "socket\t");                 break; 
            default:       strcat(contructed_string, "unknown\t");                break; 
            }

            
            //rights
            char modeval[10];
            mode_t perm = sb.st_mode;
                modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
                modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
                modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
                modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
                modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
                modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
                modeval[6] = (perm & S_IROTH) ? 'r' : '-';
                modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
                modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
                modeval[9] = '\t';


            //printf("%s\n",modeval);

            strcat(contructed_string, modeval);
            

            
            //size in bytes
            char temp[100];
            sprintf(temp , "%lld bytes\t", (long long) sb.st_size);
            strcat(contructed_string, temp);
            temp[0] = '\0';

            //Last modification
            char tempTime[100] = "";
            strcat(tempTime, ctime(&sb.st_mtime));
            tempTime[strlen(tempTime) - 1] = '\t';
            strcat(contructed_string, tempTime);

            //Last access
            strcat(contructed_string, ctime(&sb.st_atime));
            //strcat(contructed_string, "\n");
        }



        if (entry->d_type & DT_DIR) {

            /* Check that the directory is not "d" or d's parent. */
            
            if (strcmp (d_name, "..") != 0 &&
                strcmp (d_name, ".") != 0) {
                int path_length;
                char path[PATH_MAX];
 
                path_length = snprintf (path, PATH_MAX,
                                        "%s/%s", dir_name, d_name);
                //printf ("%s\n", path);
                if (path_length >= PATH_MAX) {
                    fprintf (stderr, "Path length has got too long.\n");
                    exit (EXIT_FAILURE);
                }
                /* Recursively call "list_dir" with the new path. */
                list_dir (path, contructed_string, file_name);
            }
	}
    }
    /* After going through all the entries, close the directory. */
    if (closedir (d)) {
        fprintf (stderr, "Could not close '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
}

void stat_dir(const char * dir_name, char * contructed_string, const char * file_name){
            char name_and_dir[1024] = "";
            char name_formatter[1024] = "";


            strcat(name_and_dir,dir_name);
            strcat(name_and_dir,"/");
            strcat(name_and_dir,file_name);

	        
            //printf(name_and_dir);
            //printf("%s\n",name_and_dir);
            struct stat sb;
            if (stat(name_and_dir, &sb) == -1) {
                strcat(contructed_string,"");
            }else{
                strcat(contructed_string, name_formatter);
                //file type
                switch (sb.st_mode & S_IFMT) {
                    case S_IFBLK:  strcat(contructed_string, "block device\t");           break; 
                    case S_IFCHR:  strcat(contructed_string, "character device\t");       break; 
                    case S_IFDIR:  strcat(contructed_string, "directory\t");              break; 
                    case S_IFIFO:  strcat(contructed_string, "FIFO/pipe\t");              break; 
                    case S_IFLNK:  strcat(contructed_string, "symlink\t");                break; 
                    case S_IFREG:  strcat(contructed_string, "regular file\t");           break; 
                    case S_IFSOCK: strcat(contructed_string, "socket\t");                 break; 
                    default:       strcat(contructed_string, "unknown\t");                break; 
                    }

                    
                    //rights
                    char modeval[10];
                    mode_t perm = sb.st_mode;
                        modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
                        modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
                        modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
                        modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
                        modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
                        modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
                        modeval[6] = (perm & S_IROTH) ? 'r' : '-';
                        modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
                        modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
                        modeval[9] = '\t';


                    //printf("%s\n",modeval);

                    strcat(contructed_string, modeval);
                    

                    
                    //size in bytes
                    char temp[100];
                    sprintf(temp , "%lld bytes\t", (long long) sb.st_size);
                    strcat(contructed_string, temp);
                    temp[0] = '\0';

                    //Last modification
                    char tempTime[100] = "";
                    strcat(tempTime, ctime(&sb.st_mtime));
                    tempTime[strlen(tempTime) - 1] = '\t';
                    strcat(contructed_string, tempTime);

                    //Last access
                    strcat(contructed_string, ctime(&sb.st_atime));
                    //strcat(contructed_string, "\n"); 
            }
}

void readUserInput(){
    
    int pid;
    int messageLength;
    int authenticated = 0;
    int sockp[2];
    char message[1024];
    char message2[1024];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockp) < 0) 
      { 
        perror("Err... socketpair"); 
        exit(1); 
      }
        switch(pid = fork()){
            case -1 :{
                perror("forkul nu a mers");exit(1);
            }
            case 0:{
                FILE * fp = NULL;
                char * line = NULL;
                size_t len = 0;
                ssize_t readB;
                int compare;
                size_t buffSize;

                
                if (read(sockp[0], message2, 1024) < 0) perror("[parinte]Err...read");
                //printf("%s\n",message2);

                fp = fopen("comenzi.txt", "r");
                    if (fp == NULL)
                        exit(1);

                    char* command = strtok(message2, " \n");
                    //printf("%s\n",command);
                    while ((readB = getline(&line, &len, fp)) != -1) {
                        if((compare = strncmp(command, line, strlen(command))) == 0){
                            char responce[1024] = "";
                            char read_string[1024];

                            //Command interpretter
                            if(strncmp(message2,"help",4) == 0){
                                //help
                                    FILE * fp2 = NULL;
                                    fp2 = fopen("help.txt", "r");
                                    if (fp2 == NULL) exit(1);
                                    while(fgets(read_string,sizeof(read_string),fp2) != NULL)
                                        strcat(responce, read_string);
                            }else if(strncmp(message2,"myfind",6) == 0){
                                //myfind [file]
                                char cwd[PATH_MAX];
                                getcwd(cwd, sizeof(cwd));
                                char temporary[1024] = "";
                                char name[100] = "";
                                strcat (name, message2+7);
                                name[strlen(name) - 1] = '\0';
                                //printf(name);
                                list_dir(cwd, temporary, name);

                                if (strlen(temporary) > 5){
                                    char formatter[1024] = "";
                                    sprintf(formatter,"%s\t%s\t%s\t%s\t%s\t\t%s\t\t\t%s\n","File Name", "Directory","File type", "File Perm","Size", "Last Modified", "Last Accessed");
                                    strcat(responce, formatter);
                                    strcat(responce, "________________________________________________________________________________________________________________________________________\n");
                                    strcat(responce, temporary);
                                }else{
                                    strcat(responce, "The teddy bear didn't find anything. Try again");
                                }

                            }else if(strncmp(message2,"mystat",6) == 0){
                                //mystat [file]
                                char cwd[PATH_MAX];
                                getcwd(cwd, sizeof(cwd));
                                char temporary[1024] = "";
                                char name[100] = "";
                                strcat (name, message2+7);
                                name[strlen(name) - 1] = '\0';
                                //printf(name);
                                stat_dir(cwd, temporary, name);

                                if (strlen(temporary) > 5){
                                    char formatter[1024] = "";
                                    sprintf(formatter,"%s\t%s\t%s\t\t%s\t\t\t%s\n","File type", "File Perm","Size", "Last Modified", "Last Accessed");
                                    strcat(responce, formatter);
                                    strcat(responce, "______________________________________________________________________________________________________________\n");
                                    strcat(responce, temporary);
                                }else{
                                    strcat(responce, "The teddy bear didn't find anything. Try again");
                                }
                            }
                            else{
                                if (write(sockp[0], "OK\0" , 1024) < 0) perror("Failed to send to child..Abord1");
                                exit(1);
                            }
                                responce[strlen(responce)] = '\0';
                                if (write(sockp[0], responce , 1024) < 0) perror("Failed to send to child..Abord2");
                                 exit(1);

                        }
                    }
                if (write(sockp[0], COMMAND_NOTFOUND, 1024) < 0) perror("Failed to send to child..Abord3");
                exit(1);
                
            }
            default:{
                //close(sockp[0]);
                printf("[%s~%s%s] ", KRED, userLoggedName, KWHT);
                fflush(stdout);
                fgets(message, sizeof(message), stdin);
                printf("\n");
                //printf("%s%d\n", message,strlen(message));
                message[strlen(message)] = '\0';

                if (write(sockp[1], message, 1024) < 0) perror("Failed to send to child..Abord");
                
                char readbuffer[1024];
                if (read(sockp[1], readbuffer, 1024) < 0) perror("[parinte]Err...read");
               // printf("%s%d\n", readbuffer, strlen(readbuffer));
                if(strncmp(readbuffer, "OK",strlen(readbuffer)-1)==0) {exitCommand = 1;};
                wait(1);
                printf("%s\n", readbuffer);
                //message[0] = '\0';
            }            
        }
}

void authUser(){
    char message[1024];
    int pid;
    int messageLength;
    int authenticated = 0;
    char file_descriptor_fifo1[] = "FIFO1",file_descriptor_fifo2[]="FIFO2";
    int fifo1Descriptor, fifo2Descriptor;

    do{
        switch (pid=fork()){
            case -1 :{
                perror("forkul nu a mers");exit(1);
            }
            case 0:{//copil
                //printf("%s","am ajuns aici\n");
                fifo1Descriptor = open("FIFO1", O_RDONLY);
                fifo2Descriptor = open("FIFO2", O_WRONLY);
                FILE * fp = NULL;
                char * line = NULL;
                size_t len = 0;
                ssize_t readB;
                int compare;
                
                if ((messageLength = read(fifo1Descriptor, message, 1024)) == -1)
                    perror("Eroare la citirea din FIFO!");
                else {
                    message[messageLength] = '\0';
                    //printf("S-au citit din FIFO %d bytes: \"%s\"\n", messageLength, message);
                }

                fp = fopen("users.txt", "r");
                if (fp == NULL)
                    exit(1);

                while ((readB = getline(&line, &len, fp)) != -1) {
                     if((compare = strncmp(message, line, strlen(message)-1)) == 0){
                         if ((messageLength = write(fifo2Descriptor, &compare, sizeof(int))) == -1)
                            perror("Problema la scriere in FIFO!");
                        exit(1);
                     }
                }
                compare = 1; //nu am gasit username-ul in fisier

                if ((messageLength = write(fifo2Descriptor, &compare, sizeof(int))) == -1)
                    perror("Problema la scriere in FIFO!");
                
                exit(1);
                //sleep(100);
            }
            default:{//parinte
                fifo1Descriptor = open("FIFO1", O_WRONLY);
                fifo2Descriptor = open("FIFO2", O_RDONLY);
                fgets(message, sizeof(message), stdin);
                if ((messageLength = write(fifo1Descriptor, message, strlen(message))) == -1)
                    perror("Problema la scriere in FIFO!");

                int resp;
                if ((messageLength = read(fifo2Descriptor, &resp, sizeof(int))) == -1)
                    perror("Eroare la citirea din FIFO!");
                else {
                    //printf("S-au citit din FIFO %d bytes: \"%d\"\n", messageLength, resp);
                    if(resp == 0) {authenticated = 1; strncat(userLoggedName, message, strlen(message)-1);}
                    else printf("Bear not found. Please enter a correct bear name:\n");
                }
                

                wait(1);
                close(fifo1Descriptor);
            }

        }
    }while(!authenticated);
}
 
void print_Image()
{

    FILE *fptr = NULL;
 
    if((fptr = fopen("bear.txt","r")) == NULL)
    {
        fprintf(stderr,"error opening %s\n","bear.txt");
        exit(1);
    }

    char read_string[MAX_LEN];
 
    while(fgets(read_string,sizeof(read_string),fptr) != NULL)
        printf("%s",read_string);

     fclose(fptr);
}