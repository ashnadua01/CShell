#include "globalVariables.h"

void removeComm(int pid, int index){
    backgroundID[index - 1] = 0;
}

void fg(int sendCount){
    if(sendCount < 2){
        printf("Too few arguments\n");
        return;
    }
    else if(sendCount > 2){
        printf("Too many arguments\n");
        return;
    }
    int index = atoi(array[1]);
    int pid = backgroundID[index - 1];

    if(pid < 0){
        printf("Incorrect Process ID\n");
        return;
    }
    int status;
    setpgid(pid, getpgid(0));
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    tcsetpgrp(0, pid);
    int check;
    check = kill(pid, SIGCONT);
    if(check < 0){
        perror("fg");
        return;
    }
    waitpid(pid, &status, WUNTRACED);
    tcsetpgrp(0, getpgid(0));

    removeComm(pid, index);

    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
}