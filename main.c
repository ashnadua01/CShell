#include "globalVariables.h"

char username[300];
char hostname[300];
char invokedDirectory[PATH_MAX];
char currDirectory[PATH_MAX];
char prevDirectory[PATH_MAX];
char directory[PATH_MAX];
int inputFlag = 0;
int overwriteFlag = 0;
int appendFlag = 0;
char *array[PATH_MAX];
char historyArray[20][MAX_INPUT];
int historyIndex = 0;
int flag = 1;
int recurseCall = 0;
int backgroundID[1000];
char backCommand[100][100];
int timeTaken;
int fgFlag = 0;
int checkerFlag = 0;
extern int pid;
char command[MAX_INPUT];

void controlC(){
    return;
}
void control_Z()
{
    return;
}

int main(){
    signal(SIGINT, controlC);
    signal(SIGTSTP, control_Z);
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
        char *returnCheck;
        // reading the command
        returnCheck = fgets(command, sizeof(command), stdin);

        // check for cntrl D
        if(returnCheck == NULL){
            printf("\n");
            exit(0);
        }

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

        for(int i=0; i<countProcess; i++){
            int counterAnd = 0;
            int counterPipe = 0;
            // tokenizing on the basis of &
            for(int j=0; j<strlen(tokenArray[i]); j++){
                if(tokenArray[i][j] == '&'){
                    counterAnd++;
                }
                else if(tokenArray[i][j] == '|'){
                    counterPipe++;
                }
            }
            if(counterPipe == 0){
                int count = 0;
                char *andArray[MAX_INPUT];
                char *token2 = strtok(tokenArray[i], "&");
                while(token2 != NULL){
                    andArray[count++] = token2;
                    token2 = strtok(NULL, "&");
                }
                for(int j=0; j<count; j++){
                    // setting suitable flags for fg and bg processes
                    checkerFlag = 0;
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

                    // tokenizing on the basis of " "
                    char* token4;
                    char inputFile[PATH_MAX];
                    char overwriteFile[PATH_MAX];
                    char appendFile[PATH_MAX];

                    token4 = strtok(andArray[j], " ");
                    int sendCount = 0;
                    while(token4 != NULL){
                        if(strcmp(token4, "<") == 0){
                            if(inputFlag == 0){
                                inputFlag = 1;
                                token4 = strtok(NULL, " ");
                                strcpy(inputFile, token4);
                                token4 = strtok(NULL, " ");
                            }
                            else{
                                token4 = strtok(NULL, " ");
                            }
                        }
                        else if(strcmp(token4, ">") == 0){
                            if(overwriteFlag == 0){
                                overwriteFlag = 1;
                                token4 = strtok(NULL, " ");
                                strcpy(overwriteFile, token4);
                                token4 = strtok(NULL, " ");
                            }
                            else{
                                token4 = strtok(NULL, " ");
                            }
                        }   
                        else if(strcmp(token4, ">>") == 0){
                            if(appendFlag == 0){
                                appendFlag = 1;
                                token4 = strtok(NULL, " ");
                                strcpy(appendFile, token4);
                                token4 = strtok(NULL, " ");
                            }   
                            else{
                                token4 = strtok(NULL, " ");
                            }
                        }
                        else{
                            array[sendCount++] = token4;
                            token4 = strtok(NULL, " ");
                        }
                    }

                    array[sendCount] = NULL;

                    if(overwriteFlag == 1 && appendFlag == 1){
                        overwriteFlag = 0;
                        appendFlag = 0;
                        if(inputFlag == 1){
                            inputFlag = 0;
                        }
                        printf("Error: Too many arguments\n");
                        continue;
                    }

                    int inFD;
                    int saveInDup;
                    int saveDup;
                    int outFD;
                    int appFD;
                    if(inputFlag == 1){
                        inFD = open(inputFile, O_RDONLY, 0644);
                        if(inFD < 0){
                            perror("File Error");
                            inputFlag = 0;
                            continue;
                        }
                        else{
                            saveInDup = dup(STDIN_FILENO);
                            int d = dup2(inFD, STDIN_FILENO);
                            if(d < 0){
                                perror("Error");
                                inputFlag = 0;
                                continue;
                            }
                        }
                    }
                    if(overwriteFlag == 1){
                        outFD = open(overwriteFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if(outFD < 0){
                            perror("File Error");
                            overwriteFlag = 0;
                            continue;
                        }
                        else{
                            saveDup = dup(STDOUT_FILENO);
                            int d = dup2(outFD, STDOUT_FILENO);
                            if(d < 0){
                                perror("Error");
                                overwriteFlag = 0;
                                continue;
                            }
                        }
                    }
                    else if(appendFlag == 1){
                        appFD = open(appendFile, O_WRONLY | O_CREAT | O_APPEND, 0644);
                        if(appFD < 0){
                            perror("File Error");
                            appendFlag = 0;
                            continue;
                        }   
                        else{
                            saveDup = dup(STDOUT_FILENO);
                            int d = dup2(appFD, STDOUT_FILENO);
                            if(d < 0){
                                perror("Error");
                                appendFlag = 0;
                                continue;
                            }
                        }
                    }
                    commandCall(sendCount, checkerFlag);
                    if(inputFlag == 1){
                        inputFlag = 0;
                        dup2(saveInDup, STDIN_FILENO);
                        close(inFD);
                    }
                    if(overwriteFlag == 1){
                        overwriteFlag = 0;
                        dup2(saveDup, STDOUT_FILENO);
                        close(outFD);
                    }
                    else if(appendFlag == 1){
                        appendFlag = 0;
                        dup2(saveDup, STDOUT_FILENO);
                        close(appFD);
                    }
                }
            }
            else{
                char* token4;
                char inputFile[PATH_MAX];
                char overwriteFile[PATH_MAX];
                char appendFile[PATH_MAX];

                token4 = strtok(tokenArray[0], " ");
                int sendCount = 0;
                while(token4 != NULL){
                    if(strcmp(token4, "<") == 0){
                        if(inputFlag == 0){
                            inputFlag = 1;
                            token4 = strtok(NULL, " ");
                            strcpy(inputFile, token4);
                            array[sendCount++] = token4;
                            token4 = strtok(NULL, " ");
                        }
                        else{
                            token4 = strtok(NULL, " ");
                        }
                    }
                    else if(strcmp(token4, ">") == 0){
                        if(overwriteFlag == 0){
                            overwriteFlag = 1;
                            token4 = strtok(NULL, " ");
                            strcpy(overwriteFile, token4);
                            token4 = strtok(NULL, " ");
                        }
                        else{
                            token4 = strtok(NULL, " ");
                        }
                    }   
                    else if(strcmp(token4, ">>") == 0){
                        if(appendFlag == 0){
                            appendFlag = 1;
                            token4 = strtok(NULL, " ");
                            strcpy(appendFile, token4);
                            // array[sendCount++] = token4;
                            token4 = strtok(NULL, " ");
                        }   
                        else{
                            token4 = strtok(NULL, " ");
                        }
                    }
                    else{
                        array[sendCount++] = token4;
                        token4 = strtok(NULL, " ");
                    }
                }

                int inFD;
                int outFD;
                int appFD;
                int saveDup;
                int saveInDup;
                if(inputFlag == 1){
                    inFD = open(inputFile, O_RDONLY, 0644);
                    if(inFD < 0){
                        perror("File Error");
                        inputFlag = 0;
                        continue;
                    }
                    else{
                        saveInDup = dup(STDIN_FILENO);
                        int d = dup2(inFD, STDIN_FILENO);
                        if(d < 0){
                            perror("Error");
                            inputFlag = 0;
                            continue;
                        }
                    }
                }
                if(overwriteFlag == 1){
                    outFD = open(overwriteFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if(outFD < 0){
                    perror("File Error");
                        overwriteFlag = 0;
                        continue;
                    }
                    else{
                        saveDup = dup(STDOUT_FILENO);
                        int d = dup2(outFD, STDOUT_FILENO);
                        if(d < 0){
                            perror("Error");
                            overwriteFlag = 0;
                            continue;
                        }
                    }
                }
                else if(appendFlag == 1){
                    appFD = open(appendFile, O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if(appFD < 0){
                    perror("File Error");
                        appendFlag = 0;
                        continue;
                    }
                    else{
                        saveDup = dup(STDOUT_FILENO);
                        int d = dup2(appFD, STDOUT_FILENO);
                        if(d < 0){
                            perror("Error");
                            appendFlag = 0;
                            continue;
                        }
                    }
                }
                pipeline(sendCount, counterPipe, array);
                if(inputFlag == 1){
                    inputFlag = 0;
                    dup2(saveInDup, STDIN_FILENO);
                    close(inFD);
                }
                if(overwriteFlag == 1){
                    overwriteFlag = 0;
                    dup2(saveDup, STDOUT_FILENO);
                    close(outFD);
                }
                else if(appendFlag == 1){
                    appendFlag = 0;
                    dup2(saveDup, STDOUT_FILENO);
                    close(appFD);
                }
            }
        }
    }
    return 0;
}