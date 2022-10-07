#include "globalVariables.h"
int pid;

void processHandle(int *backgroundID, int bgFlag, int sendCount){

    pid = fork();
    if(pid > 0){
        // executing parent process as background process
        // copying the name of the command executing as a background process
        if(bgFlag == 1){
            printf("%s started with PID %d\n", array[0], pid);
            for (int i = 0; i < 100; i++){
                if (backgroundID[i] == 0){
                    backgroundID[i] = pid;
                    strcpy(backCommand[i], array[0]);
                    break;
                }
            }
        }
        // executing parent process as foreground process
        // calculating time taken by foreground process
        else{
            time_t t;
            t = time(NULL);
            int status;
            waitpid(pid, &status, WUNTRACED);
            if(WIFSTOPPED(status))
            {
                for (int i = 0; i < 100; i++){
                    if (backgroundID[i] == 0){
                        printf("[%d] %s process with pid %d pushed to the background\n",i,backCommand[i],pid);
                        backgroundID[i] = pid;
                        strcpy(backCommand[i], array[0]);
                        break;
                    }
                }
            }
            time_t t1;
            t1 = time(NULL);
            timeTaken += (t1 - t);
            
        }
    }
    // child process found
    else if(pid == 0){
        if(bgFlag == 1){
            // setting pid of all background process as 0
            int pgid = +setpgid(0, 0);
            if(pgid == -1){
                perror("Child Process Error");
            }
        }
        // executing processes using execvp
        int execCall = execvp(array[0], array);
        if(execCall == -1) {
            perror("Execvp Error");
            exit(0);
        }
    }
    // no process found
    else if(pid < 0){
        perror("Error");
    }
}