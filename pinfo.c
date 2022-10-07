#include "globalVariables.h"

void pinfo(int sendCount){

    // tkn = strtok(NULL, " ");
    pid_t pid;
    char send[PATH_MAX];
    char process[MAX_INPUT];
    // if pid is not mentioned, concat self 
    // in the path for current process
    if(sendCount == 1){
        strcpy(send, "/proc/self");
        strcat(send, "/stat");
        pid = getpid();
    }
    else{
        // error handling
        if(sendCount > 2){
            printf("Error: Too many arguments\n");
            return;
        }
        strcpy(process, array[1]);
        pid = atoi(process);
        // error handling
        if(kill(pid, 0) != 0){
            printf("Error: Invalid Process ID\n");
            return;
        }
        else{
            // creating valid path to open file and read contents
            strcpy(send, "/proc");
            strcat(send, "/");
            strcat(send, process);
            strcat(send, "/stat");
        }
    }

    FILE* statusFile;
    statusFile = fopen(send, "r");
    // error handling
    if(statusFile == NULL){
        perror("File Error");
        return;
    }
    else{
        // reading from the file
        char arrayPinfo[1000][1000];
        for(int i=0; i<30; i++){
            fscanf(statusFile, "%s", arrayPinfo[i]);
        }
        fclose(statusFile);

        char status[1000];
        char memory[1000];

        // stroing status memory and process id
        strcpy(status, arrayPinfo[2]);
        strcpy(memory, arrayPinfo[22]);
        strcpy(process, arrayPinfo[0]);

        // appending status
        if(strcmp(arrayPinfo[4], arrayPinfo[7]) == 0){
            status[1] = '+';
            status[2] = 0;
        }
        else{
            status[1] = 0;
        }

        char executable[PATH_MAX];
        char id[300];

        // creating path to find executable path
        strcpy(executable, "/proc/");
        strcat(executable, process);
        strcat(executable, "/exe");

        char linker[PATH_MAX];
        
        // error handling
        // linker will store the executable path
        if(readlink(executable, linker, sizeof(linker)) <= 0){
            perror("Process Path Error");
            return;
        }

        char executablePath[PATH_MAX];
        // changing / modifying path of linker
        if(strlen(linker) < strlen(invokedDirectory)){
            strcpy(executablePath, linker);
        }
        else if(strlen(linker) >= strlen(invokedDirectory)){
            if(strstr(linker, invokedDirectory) == NULL){
                strcpy(executablePath, invokedDirectory);
            }
            else{
                strcpy(executablePath, "~");
                char mid[PATH_MAX];
                strncpy(mid, linker + strlen(invokedDirectory), strlen(linker) - strlen(invokedDirectory) + 1);
                strcat(executablePath, mid);
            }
        }

        printf("pid : %d\n", pid);
        printf("process status : %s\n", status);
        printf("memory : %s {Virtual Memory}\n", memory);
        printf("executable path : %s\n", executablePath);
    }
}