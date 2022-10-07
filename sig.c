#include "globalVariables.h"

void sig(int sendCount){
    if(sendCount > 3){
        printf("Too many arguments\n");
        return;
    }
    else if(sendCount < 3){
        printf("Too few arguments\n");
        return;
    }
    int index = atoi(array[1]);
    if(backgroundID[index-1] == 0){
        printf("No valid job number\n");
        return;
    }
    int pid = backgroundID[index - 1];
    int sign = atoi(array[2]);
    int check;
    check = kill(pid, sign);
    if(check < 0){
        perror("Sig Error");
        return;
    }
}