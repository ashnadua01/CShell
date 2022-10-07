#include "globalVariables.h"

void readFile(){
    // reading from file and storing in the array
    FILE* historyFile = fopen("history.txt", "r");
    char string[100];

    while(fgets(string, 100, historyFile) != NULL){
        strcpy(historyArray[historyIndex], string);
        historyArray[historyIndex][strlen(historyArray[historyIndex]) - 1] = '\0';
        historyIndex++;
    }
    fclose(historyFile);
}

void history(){
    // printing 10 elements in the array
    if(historyIndex < 10){
        for(int i=0; i<historyIndex; i++){
            printf("%s\n", historyArray[i]);
        }
    }
    else{
        int count = historyIndex - 10;
        for(int i=count; i<historyIndex; i++){
            printf("%s\n", historyArray[i]);
        }
    }
}

void saveToFile(){
    // saving the contents of the array at each interval
    char dir[PATH_MAX];
    getcwd(dir, sizeof(dir));
    chdir(invokedDirectory);
    FILE* historyFile = fopen("history.txt", "w");
    for(int i=0; i<historyIndex; i++){
        fprintf(historyFile, "%s\n", historyArray[i]);
    }
    fclose(historyFile);
    chdir(dir);
}

void arrayChange(){
    // if number of commands in history array increase than 20, 
    // older commands are deleted and newer are added below
    for(int i=1; i<historyIndex; i++){
        strcpy(historyArray[i-1], historyArray[i]);
    }
    historyIndex--;
}