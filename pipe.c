#include "globalVariables.h"

void pipeline(int sendCount, int counterPipe, char* arrayFind[MAX_INPUT]){
    int index = 0;
    for(int i=0; i<sendCount; i++){
        if(strcmp(arrayFind[i], "|") == 0){
            break;
        }
        else{
            array[index++] = arrayFind[i];
        }
    }
    counterPipe--;
    array[index] = NULL;
    int andFlag = 0;
    for(int i=0; i<index; i++){
        if(strstr(array[i], "&") != NULL){
            printf("Syntax Error\n");
            return;
        }
    }

    int fd[2];
    int checker = pipe(fd);
    if(checker < 0){
        perror("Pipe Error");
        return;
    }
    int saveDup;
    saveDup = dup(STDOUT_FILENO);
    int d = dup2(fd[1], STDOUT_FILENO);
    if(d < 0){
        perror("Pipe Error");
        return;
    }
    commandCall(index, andFlag);
    dup2(saveDup, STDOUT_FILENO);
    close(fd[1]);

    int saveInDup;
    saveInDup = dup(STDIN_FILENO);
    int d2 = dup2(fd[0], STDIN_FILENO);
    if(d2 < 0){
        perror("Pipe Error");
        return;
    }
    int index2 = 0;
    for(int i=index+1; i<sendCount; i++){
        array[index2++] = arrayFind[i];
    }
    array[index2] = NULL;
    if(counterPipe == 0){
        andFlag = 0;
        for(int i=0; i<index2; i++){
            if(strstr(array[i], "&") != NULL){
                char *token;
                token = strtok(array[i], "&");
                andFlag = 1;
                array[i] = token;
                token = strtok(NULL, "&");
            }
        }
        commandCall(index2, andFlag);
    }
    else{
        pipeline(index2, counterPipe, array);
    }
    close(fd[0]);
    dup2(saveInDup, STDIN_FILENO);
    dup2(saveDup, STDOUT_FILENO);
}