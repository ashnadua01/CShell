#include "globalVariables.h"

void checkUsername(){
    // username retrieval
    register struct passwd *pw;
    register uid_t uid;
    uid = geteuid();
    pw = getpwuid(uid);
    strcpy(username, pw->pw_name);
}

void checkHostname(){
    // hostname / system name retrieval
    gethostname(hostname, sizeof(hostname));
}

void checkDirectory(){
    // current directory retrieval
    getcwd(currDirectory, sizeof(currDirectory));
    if(flag == 1){
        strcpy(invokedDirectory, currDirectory);
        strcpy(prevDirectory, invokedDirectory);
        readFile();
        flag = 0;
    }
    if(strcmp(invokedDirectory, currDirectory) == 0){
        char *src = "~";
        strcpy(currDirectory, src);
    }
    else if(strlen(currDirectory) < strlen(invokedDirectory)){
        strcpy(currDirectory, currDirectory);
    }
    else if(strlen(currDirectory) >= strlen(invokedDirectory)){
        if(strstr(currDirectory, invokedDirectory) == NULL){
            strcpy(currDirectory, currDirectory);
        }
        else{
            // changing path of the current directory
            char mid[PATH_MAX];
            strncpy(mid, currDirectory+strlen(invokedDirectory), strlen(currDirectory) - strlen(invokedDirectory) + 1);
            char *src = "~";
            // strcat(directory, mid);
            strcpy(currDirectory, src);
            strcat(currDirectory, mid);
        }
    }
}

void findID(){
    // iterating for all processes
    for (int i = 0; i < 1000; i++){
        if(backgroundID[i] > 0){
            int status;
            // checking return value of waitpid
            int waitCheck = waitpid(backgroundID[i], &status, WNOHANG);
            // if waitpid returns a value > 0, no error is encountered
            if(waitCheck > 0){
                int childProcessStatus = WIFEXITED(status);
                if(childProcessStatus != 0){
                    // printing in stderr file, and printing whether bg 
                    // process terminated normally or abnormally
                    fprintf(stderr, "%s with PID %d exited %s\n", backCommand[i], backgroundID[i], "normally");
                }
                else{
                    fprintf(stderr, "%s with PID %d exited %s\n", backCommand[i], backgroundID[i], "abnormally");
                }
                backgroundID[i] = 0;
            }
            // if waitpid returns -1, error is encountered
            else if(waitCheck == -1){
                perror("Process Error");
                return;
            }
        }
        else{
            continue;
        }
    }
}