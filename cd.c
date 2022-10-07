#include "globalVariables.h"

void cd(int num){
    int per;
    if(num == 0){
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
    else if(num == 1){
        // printf("%s\n", array[0]);
        if(strcmp(array[0], ".") == 0){
            per = chdir(directory);
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
        else if(strcmp(array[0], "~") == 0){
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
        else if(strcmp(array[0], "..") == 0){
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
        else if(strcmp(array[0], "-") == 0){
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
            per = chdir(array[0]);
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
        printf("Too many arguments\n");
        perror("Arguments");
    }
    // printf("%s\n", prevDirectory);
}