#include "globalVariables.h"

char directorySend[PATH_MAX];
char fileSend[PATH_MAX];

// recurse function for printing all the files 
// and subdirectories of the given directory
void recurse(char dir[PATH_MAX], int flagF, int flagD){
    char string[PATH_MAX];
    struct dirent *dirContainer;
    DIR *dp = opendir(dir);
    // error handling
    if(dp == NULL){
        return;
    }
    if(recurseCall == 0){
        if(flagD == 1){
            printf("%s\n", dir);
            recurseCall++;
        }
    }
    while ((dirContainer = readdir(dp)) != NULL){
        // skipping if .. or . i.e. previous or current directory
        // including them will lead to an infinite loop
        if(strcmp(dirContainer->d_name, "..") == 0){
            continue;
        }
        else if(strcmp(dirContainer->d_name, ".") == 0){
            continue;
        }
        strcpy(string, dir);
        strcat(string, "/");
        strcat(string, dirContainer->d_name);
        // checking if -f flag is present
        if(flagF == 1){
            if(dirContainer->d_type != 4){
                printf("%s\n", string);
            }
        }
        // checking if -d flag is present
        if(flagD == 1){
            if(dirContainer->d_type == 4){
                printf("%s\n", string);
            }   
        }
        // calling the function again
        recurse(string, flagF, flagD);
    }
    closedir(dp);
}

// function for locating the file in the 
// given directory and printing it's path
void recurseFile(char file[PATH_MAX], char dir[PATH_MAX]){
    char string[PATH_MAX];
    struct dirent *dirContainer;
    DIR *dp = opendir(dir);
    if(dp == NULL){
        return;
    }
    while ((dirContainer = readdir(dp)) != NULL){
        // skipping if .. or . i.e. previous or current directory
        // including them will lead to an infinite loop
        if(strcmp(dirContainer->d_name, "..") == 0){
            continue;
        }
        else if(strcmp(dirContainer->d_name, ".") == 0){
            continue;
        }
        strcpy(string, dir);
        strcat(string, "/");
        strcat(string, dirContainer->d_name);
        char check[PATH_MAX];
        strcpy(check, "\"");
        strcat(check, dirContainer->d_name);
        strcat(check, "\"");
        // checking if file is present in the directory
        if(strcmp(check, file) == 0){
            printf("%s\n", string);
        }
        recurseFile(file, string);
    }
    closedir(dp);
}

// main discover function
void discover(char* tkn){
    // tokenizing
    tkn = strtok(NULL, " ");
    int count = 0;
    int flagD = 0;
    int flagF = 0;
    int checkFlag = 0;
    char directoryFlag = 0;
    char fileFlag = 0;
    while(tkn != NULL){
        // reading for flags
        if(strcmp(tkn, "-d") == 0){
            flagD = 1;
        }
        else if(strcmp(tkn, "-f") == 0){
            flagF = 1;
        }
        else if((strcmp(tkn, "-d") != 0) && (strcmp(tkn, "-f") != 0)){
            char string[PATH_MAX];
            strcpy(string, tkn);
            // checking if input is a file
            if(string[0] == '\"'){
                if(fileFlag == 0){
                    strcpy(fileSend, string);
                    count++;
                    fileFlag = 1;
                }
                // error handling
                else{
                    printf("Error: Too many files\n");
                    return;
                }
            }
            else{
                // checcking if more than 1 directory is already present
                if(directoryFlag == 0){
                    strcpy(directorySend, tkn);
                    count++;
                    directoryFlag = 1;
                }
                // error handling
                else{
                    printf("Error: Too many directories\n");
                    return;
                }
            }
        }
        tkn = strtok(NULL, " ");
    }
    // if -f or -d flag give when a file is given
    // error handles the wrong format
    if((fileFlag == 1 && directoryFlag == 0) || fileFlag == 1 && directoryFlag == 1){
        if(flagD == 1 || flagF == 1){
            printf("Error: Wrong format\n");
            return;
        }
    }
    if(count == 0){
        // sending current directorys
        if(flagD == 0 && flagF == 0){
            flagF = 1;
            flagD = 1;
        }
        char send[PATH_MAX];
        strcpy(send, ".");
        recurseCall = 0;
        recurse(send, flagF, flagD);
        checkFlag = 1;
    }
    else if(count == 1){
        if(fileFlag == 0){
            if(flagD == 0 && flagF == 0){
                flagD = 1;
                flagF = 1;
            }
            // checking if directory is .
            if(strcmp(directorySend, ".") == 0){
                char send[PATH_MAX];
                strcpy(send, ".");
                recurseCall = 0;
                recurse(send, flagF, flagD);
                checkFlag = 1;
            }
            // checking if directory is ..
            else if(strcmp(directorySend, "..") == 0){
                char send[PATH_MAX];
                strcpy(send, "..");
                recurseCall = 0;
                recurse(send, flagF, flagD);
                checkFlag = 1;
            }
            // checking if directory is ~
            else if(strcmp(directorySend, "~") == 0){
                char send[PATH_MAX];
                strcpy(send, invokedDirectory);
                recurseCall = 0;
                recurse(send, flagF, flagD);
                checkFlag = 1;
            }
            else{
                if(flagD == 0 && flagF == 0){
                    flagF = 1;
                    flagD = 1;
                }
                char current[PATH_MAX];
                getcwd(current, sizeof(current));
                int per = chdir(directorySend);
                // error handling
                if(per == -1){
                    perror("Directory Error");
                }
                else{
                    chdir(current);
                    recurseCall = 0;
                    recurse(directorySend, flagF, flagD);
                    checkFlag = 1;
                }
            }
        }
        else{
            char current[PATH_MAX];
            strcpy(current, ".");
            recurseFile(fileSend, current);
            checkFlag = 1;
        }
    }
    if(count == 2){
        if(checkFlag == 0){
            // checking if directory is .
            if(strcmp(directorySend, ".") == 0){
                char current[PATH_MAX];
                strcpy(current, ".");
                recurseFile(fileSend, current);
            }
            // checking if directory is ..
            else if(strcmp(directorySend, "..") == 0){
                char current[PATH_MAX];
                strcpy(current, "..");
                recurseFile(fileSend, current);
            }
            // checking if directory is ~
            else if(strcmp(directorySend, "~") == 0){
                char current[PATH_MAX];
                strcpy(current, invokedDirectory);
                recurseFile(fileSend, current);
            }
            else{
                char current[PATH_MAX];
                getcwd(current, sizeof(current));
                int per = chdir(directorySend);
                // error handling
                if(per == -1){
                    perror("Directory Error");
                }
                else{
                    chdir(current);
                    recurseFile(fileSend, directorySend);
                }
            }
        }
    }
}