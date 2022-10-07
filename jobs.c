#include "globalVariables.h"
char commands[100][100];
int ids[1000];

void bubbleSort(int index){
    for(int i=0; i<index-1; i++){
        for(int j=0; j<index-i-1; j++){
            if(strcmp(commands[j], commands[j+1]) > 0){
                char temp[PATH_MAX];
                int tempInt;
                strcpy(temp, commands[j]);
                strcpy(commands[j], commands[j+1]);
                strcpy(commands[j+1], temp);
                tempInt = ids[j];
                ids[j] = ids[j+1];
                ids[j+1] = tempInt;
            }
        }
    }
}

void jobs(int sendCount){
    int sFlag = 0;
    int rFlag = 0;
    if(sendCount == 1){
        sFlag = 1;
        rFlag = 1;
    }
    else if(sendCount == 2){
        if(strcmp(array[1], "-r") == 0){
            rFlag = 1;
        }
        else if(strcmp(array[1], "-s") == 0){
            sFlag = 1;
        }
        else{
            printf("Wrong Symbol\n");
            return;
        }
    }
    else if(sendCount == 3){
        if(strcmp(array[1], "-r") == 0){
            rFlag = 1;
        }
        else if(strcmp(array[1], "-s") == 0){
            sFlag = 1;
        }
        else{
            printf("Wrong Symbol\n");
            return;
        }
        if(strcmp(array[2], "-r") == 0){
            rFlag = 1;
        }
        else if(strcmp(array[2], "-s") == 0){
            sFlag = 1;
        }
        else{
            printf("Wrong Symbol\n");
            return;
        }
    }
    else{
        printf("Too many arguments\n");
        return;
    }
    int index = 0;
    for(int i=0; i<100; i++){
        if(backgroundID[i] == 0){
            continue;
        }
        else{
            ids[index] = backgroundID[i];
            strcpy(commands[index], backCommand[i]);
            index++;
        }
    }
    if(index > 1){
        bubbleSort(index);
    }
    for(int i=0; i<index; i++){
        char processFile[PATH_MAX];
        char id[PATH_MAX];
        sprintf(id, "%d", ids[i]);
        strcpy(processFile, "/proc/");
        strcat(processFile, id);
        strcat(processFile, "/stat");

        FILE* pF = fopen(processFile, "r");

        if(pF == NULL){
            perror("File Error");
            return;
        }

        char status;
        
        char arrayJobs[1000][1000];
        for(int j=0; j<10; j++){
            fscanf(pF, "%s", arrayJobs[j]);
        }
        fclose(pF);

        char st[PATH_MAX];
        if(strcmp(arrayJobs[2], "T") == 0){
            strcpy(st, "Stopped");
        }
        else{
            strcpy(st, "Running");
        }

        if(sFlag == 1 && (strcmp(st, "Stopped") == 0)){
            printf("[%d] %s %s [%d]\n", i+1, st, commands[i], ids[i]);
        }
        if(rFlag == 1 && (strcmp(st, "Running") == 0)){
            printf("[%d] %s %s [%d]\n", i+1, st, commands[i], ids[i]);
        }
    }
}