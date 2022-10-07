#include "globalVariables.h"

char username[300];
char hostname[300];
char invokedDirectory[PATH_MAX];
char currDirectory[PATH_MAX];
char prevDirectory[PATH_MAX];
char directory[PATH_MAX];
char *array[PATH_MAX];
char historyArray[20][MAX_INPUT];
int historyIndex = 0;
int flag = 1;
int recurseCall = 0;
int backgroundID[1000];
char backCommand[100][100];
int timeTaken;
int fgFlag = 0;

int main(){
    while(1){
        checkUsername();
        checkHostname();
        checkDirectory();
        findID();
        if(fgFlag == 0){
            printf("<\033[0;32m%s@%s\033[0;37m:\033[0;34m%s\033[0;37m> ", username, hostname, currDirectory);
        }
        else{
            if(timeTaken < 1){
                printf("<\033[0;32m%s@%s\033[0;37m:\033[0;34m%s\033[0;37m> ", username, hostname, currDirectory);
            }
            else{
                printf("<\033[0;32m%s@%s\033[0;37m:\033[0;34m%s took %ds\033[0;37m> ", username, hostname, currDirectory, timeTaken);
                timeTaken = 0;
            }
        }
        char command[MAX_INPUT];
        // reading the command
        fgets(command, sizeof(command), stdin);
        if(strcmp(command, "\n") == 0){
            continue;
        }
        command[strlen(command) - 1] = '\0';
        // storing command in history
        if(historyIndex < 20){
            if(strcmp(historyArray[historyIndex-1], command) != 0){
                strcpy(historyArray[historyIndex], command);
                historyIndex++;
                saveToFile();
            }
        }
        else{
            if(strcmp(historyArray[historyIndex-1], command) != 0){
                arrayChange();
                strcpy(historyArray[historyIndex], command);
                historyIndex++;
                saveToFile();
            }
        }
        // tokenizing first on the basis of ;
        char *tokenArray[MAX_INPUT];
        char *mainToken;
        mainToken = strtok(command, ";");
        int countProcess = 0;
        // tokenArray[countProcess++] = mainToken;

        while(mainToken != NULL){
            tokenArray[countProcess++] = mainToken;
            mainToken = strtok(NULL, ";");
        }

        // for(int j=0; j<countProcess; j++){
        //     printf("%s\n", tokenArray[j]);
        // }

        for(int i=0; i<countProcess; i++){
            int counterAnd = 0;
            // tokenizing on the basis of &
            for(int j=0; j<strlen(tokenArray[i]); j++){
                if(tokenArray[i][j] == '&'){
                    counterAnd++;
                }
            }
            int count = 0;
            char *processArray[MAX_INPUT];
            char *token2 = strtok(tokenArray[i], "&");
            while(token2 != NULL){
                processArray[count++] = token2;
                token2 = strtok(NULL, "&");
            }
            for(int j=0; j<count; j++){
                // setting suitable flags for fg and bg processes
                int checkerFlag = 0;
                if(counterAnd == count){
                    checkerFlag = 1;
                    fgFlag = 0;
                }
                else{
                    if(j != count -1){
                        checkerFlag = 1;
                        fgFlag = 0;
                    }
                    else{
                        checkerFlag = 0;
                        fgFlag = 1;
                    }
                }
                char* token3;
                token3 = strtok(processArray[j], " ");
                // comparing and executing commands
                if(strcmp(token3, "echo") == 0){
                    echo(token3);
                }
                else if(strcmp(token3, "pwd") == 0){
                    pwd();
                }   
                else if(strcmp(token3, "cd") == 0){
                    int num = 0;
                    token3 = strtok(NULL, " ");
                    while(token3 != NULL){
                        array[num] = token3;
                        num++;
                        token3 = strtok(NULL, " ");
                    }
                    cd(num);
                }
                else if(strcmp(token3, "history") == 0){
                    history();
                }
                else if(strcmp(token3, "ls") == 0){
                    ls(token3);
                }
                else if(strcmp(token3, "discover") == 0){
                    discover(token3);
               }
                else if(strcmp(token3, "pinfo") == 0){
                    pinfo(token3);
                }
                else if(strcmp(token3, "exit") == 0){
                    exit(0);
                }
                else{
                    processHandle(backgroundID, checkerFlag, token3);
                }
            }

        }
    }
    return 0;
}