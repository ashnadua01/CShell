#include "globalVariables.h"

void processHandle(int *backgroundID, int bgFlag, char *tkn){
    char *tknArray[PATH_MAX];
    int count = 0;
    tknArray[count++] = tkn;

    // tokenizing
    tkn = strtok(NULL, " ");
    while (tkn != NULL){
        tknArray[count++] = tkn;
        tkn = strtok(NULL, " ");
    }

    int pid = fork();
    if(pid > 0){
        // executing parent process as background process
        // copying the name of the command executinf as a background process
        if(bgFlag == 1){
            printf("%s started with PID %d\n", tknArray[0], pid);
            for (int i = 0; i < 100; i++){
                if (backgroundID[i] == 0){
                    backgroundID[i] = pid;
                    strcpy(backCommand[i], tknArray[0]);
                    break;
                }
            }
        }
        // executing parent process as foreground process
        // calculating time taken by foreground process
        else{
            time_t t;
            t = time(NULL);
            waitpid(pid, NULL, 0);
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
        int execCall = execvp(tknArray[0], tknArray);
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