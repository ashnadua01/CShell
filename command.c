#include "globalVariables.h"

void commandCall(int sendCount, int checkerFlag){
    if(strcmp(array[0], "echo") == 0){
        echo(sendCount);
    }
    else if(strcmp(array[0], "pwd") == 0){
        pwd();
    }   
    else if(strcmp(array[0], "cd") == 0){
        cd(sendCount);
    }
    else if(strcmp(array[0], "history") == 0){
        history();
    }
    else if(strcmp(array[0], "ls") == 0){
        ls(sendCount);
    }
    else if(strcmp(array[0], "discover") == 0){
        discover(sendCount);
    }
    else if(strcmp(array[0], "pinfo") == 0){
        pinfo(sendCount);
    }
    else if(strcmp(array[0], "exit") == 0){
        exit(0);
    }
    else if(strcmp(array[0], "jobs") == 0){
        jobs(sendCount);
    }
    else if(strcmp(array[0], "sig") == 0){
        sig(sendCount);
    }
    else if(strcmp(array[0], "fg") == 0){
        fg(sendCount);
    }
    else if(strcmp(array[0], "bg") == 0){
        bg(sendCount);
    }
    else{
        processHandle(backgroundID, checkerFlag, sendCount);
    }
}