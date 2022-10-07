#include "globalVariables.h"

void bg(int sendCount){
    if(sendCount > 2){
        printf("Too many arguments\n");
        return;
    }
    else if(sendCount < 2){
        printf("Too few arguments\n");
        return;
    }
    int checker;
    int pid = atoi(array[1]);
    checker = kill(backgroundID[pid - 1], SIGCONT);
    if(checker < 0){
        perror("bg Error");
        return;
    }
}