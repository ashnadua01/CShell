#include "globalVariables.h"

void cd(int num){
    int per;
    if(num == 1){
        per = chdir(invokedDirectory);
        if(per == -1){
            perror("Directory Error");
        }
        else{
            char mid[PATH_MAX];
            strcpy(mid, invokedDirectory);
            if(strcmp(currDirectory, "~") != 0){
                char mid2[PATH_MAX];
                strncpy(mid2, currDirectory+1, strlen(currDirectory));
                strcat(mid, mid2);
            }
            strcpy(prevDirectory, mid);
        }
    }
    else if(num == 2){
        // printf("%s\n", array[0]);
        if(strcmp(array[1], ".") == 0){
            char sendDirec[PATH_MAX];
            getcwd(sendDirec, sizeof(sendDirec));
            per = chdir(sendDirec);
            if(per == -1){
                perror("Directory Error");
            }
            else{
                char mid[PATH_MAX];
                strcpy(mid, invokedDirectory);
                if(strcmp(currDirectory, "~") != 0){
                    char mid2[PATH_MAX];
                    strncpy(mid2, currDirectory+1, strlen(currDirectory));
                    strcat(mid, mid2);
                }
                strcpy(prevDirectory, mid);
            }
        }
        else if(strcmp(array[1], "~") == 0){
            per = chdir(invokedDirectory);
            if(per == -1){
                perror("Directory Error");
            }
            else{
                char mid[PATH_MAX];
                strcpy(mid, invokedDirectory);
                if(strcmp(currDirectory, "~") != 0){
                    char mid2[PATH_MAX];
                    strncpy(mid2, currDirectory+1, strlen(currDirectory));
                    strcat(mid, mid2);
                }
                strcpy(prevDirectory, mid);
            }
        }
        else if(strcmp(array[1], "..") == 0){
            if(strcmp(currDirectory, "~") == 0){
                per = chdir(invokedDirectory);
                if(per == -1){
                    perror("Directory Error");
                }
                else{
                    char mid[PATH_MAX];
                    strcpy(mid, invokedDirectory);
                    if(strcmp(currDirectory, "~") != 0){
                        char mid2[PATH_MAX];
                        strncpy(mid2, currDirectory+1, strlen(currDirectory));
                        strcat(mid, mid2);
                    }
                    strcpy(prevDirectory, mid);
                }
            }
            else{
                per = chdir("..");
                if(per == -1){
                    perror("Directory Error");
                }
                else{
                    char mid[PATH_MAX];
                    strcpy(mid, invokedDirectory);
                    if(strcmp(currDirectory, "~") != 0){
                        char mid2[PATH_MAX];
                        strncpy(mid2, currDirectory+1, strlen(currDirectory));
                        strcat(mid, mid2);
                    }
                    strcpy(prevDirectory, mid);
                }
            }
        }
        else if(strcmp(array[1], "-") == 0){
            // printf("%s\n", prevDirectory);
            per = chdir(prevDirectory);
            if(per == -1){
                perror("Directory Error");
            }
            else{
                char mid[PATH_MAX];
                strcpy(mid, invokedDirectory);
                if(strcmp(currDirectory, "~") != 0){
                    char mid2[PATH_MAX];
                    strncpy(mid2, currDirectory+1, strlen(currDirectory));
                    strcat(mid, mid2);
                }
                strcpy(prevDirectory, mid);
                pwd();
            }
        }
        else{
            per = chdir(array[1]);
            if(per == -1){
                perror("Directory Error");
            }
            else{
                char mid[PATH_MAX];
                strcpy(mid, invokedDirectory);
                if(strcmp(currDirectory, "~") != 0){
                    char mid2[PATH_MAX];
                    strncpy(mid2, currDirectory+1, strlen(currDirectory));
                    strcat(mid, mid2);
                }
                strcpy(prevDirectory, mid);
            }
        }
    }
    else{
        perror("Too many arguments");
    }
    // printf("%s\n", prevDirectory);
}