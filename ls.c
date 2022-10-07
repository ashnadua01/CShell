#include "globalVariables.h"

int prFlag = 0;

// checking if file
int ifFile(char* name){
    char path[PATH_MAX];
    struct stat check;
    getcwd(path, sizeof(path));
    strcat(path, "/");
    strcat(path, name);
    stat(path, &check);
    if(S_ISREG(check.st_mode) == 1){
        return 1;
    }
    return 0;
}

// checking if directory
int ifDirectory(char* name){
    char path[PATH_MAX];
    struct stat check;
    getcwd(path, sizeof(path));
    strcat(path, "/");
    strcat(path, name);
    stat(path, &check);
    if(S_ISDIR(check.st_mode) == 1){
        return 1;
    }
    return 0;
}

//checking if executable file
int ifExecutable(char* name){
    char path[PATH_MAX];
    struct stat check;
    getcwd(path, sizeof(path));
    strcat(path, "/");
    strcat(path, name);
    // stat(path, &check);
    if((stat(path, &check) >= 0) && (check.st_mode > 0) && (S_IEXEC & check.st_mode)){
        return 1;
    }
    return 0;
}

//sorting the array using bubble sort
void sort(int count, char sortArray[count][300]){
    char temp[300];
    for(int i=0; i<count - 1; i++){
        for(int j=0; j<count - 1 - i; j++){
            if(strcmp(sortArray[j], sortArray[j+1]) > 0){
                strcpy(temp, sortArray[j]);
                strcpy(sortArray[j], sortArray[j+1]);
                strcpy(sortArray[j+1], temp);
            }
        }
    }
}

// implementing ls (without any flag)
void lsHidden(char dir[PATH_MAX]){
    struct dirent *dirContainer;
    DIR *dp = opendir(dir);
    // error handling
    if(dp == NULL){
        perror("Directory/File Error");
    }
    else{
        char sortArray[PATH_MAX][300];
        int count = 0;
        while((dirContainer = readdir(dp)) != NULL){
            // skipping hidden files
            if(dirContainer->d_name[0] == '.'){
                continue;
            }            
            strcpy(sortArray[count++], dirContainer->d_name);
	    }
        sort(count, sortArray);
        for(int i=0; i<count; i++){
            // adjusting colour for directory
            if(ifDirectory(sortArray[i]) == 1){
                printf("\033[0;34m");
            }
            // adjusting colour for executable file
            else if(ifExecutable(sortArray[i]) == 1){
                printf("\033[0;32m");
            }
            // adjusting colour for regular file
            else if(ifFile(sortArray[i]) == 1){
                printf("\033[0;37m");
            }
            printf("%s\n", sortArray[i]);
            printf("\033[0;37m");
        }
    }
    closedir(dp);
}

// implementing ls -a
void lsNotHidden(char dir[PATH_MAX]){
    struct dirent *dirContainer;
    DIR *dp = opendir(dir);
    // error handling
    if(dp == NULL){
        perror("Directory/File Error");
    }
    else{
        char sortArray[PATH_MAX][300];
        int count = 0;
        while((dirContainer = readdir(dp)) != NULL){         
            strcpy(sortArray[count++], dirContainer->d_name);
	    }
        sort(count, sortArray);
        for(int i=0; i<count; i++){
            // adjusting colour for directory
            if(ifDirectory(sortArray[i]) == 1){
                printf("\033[0;34m");
            }
            // adjusting colour for executable file
            else if(ifExecutable(sortArray[i]) == 1){
                printf("\033[0;32m");
            }
            // adjusting colour for regular file
            else if(ifFile(sortArray[i]) == 1){
                printf("\033[0;37m");
            }
            printf("%s\n", sortArray[i]);
            printf("\033[0;37m");
        }
    }
    closedir(dp);
}

// implementing ls -l for directories
void lsLHidden(char dir[PATH_MAX]){
    struct dirent *dirContainer;
    DIR *dp = opendir(dir);
    struct stat check;
    struct passwd* check2;
    struct group* check3;
    struct tm* check4;
    int link, sze;
    // error handling
    if(dp == NULL){
        perror("Directory/File Error");
    }
    else{
        char sortArray[PATH_MAX][300];
        int count = 0;
        int sum = 0;
        while((dirContainer = readdir(dp)) != NULL){    
            if(dirContainer->d_name[0] == '.'){
                continue;
            }    
            strcpy(sortArray[count++], dirContainer->d_name);
            char path[PATH_MAX];
            strcpy(path, dir);
            strcat(path, "/");
            strcat(path, sortArray[count-1]);
            stat(path, &check);
            // couting block size for total
            sum += check.st_blocks;
	    }
        sort(count, sortArray);
        sum = sum / 2;
        // printing total
        printf("total %d\n", sum);
        // printing content for each file present in the directory
        for(int i=0; i<count; i++){
            char path[PATH_MAX];
            strcpy(path, dir);
            strcat(path, "/");
            strcat(path, sortArray[i]);
            stat(path, &check);
            // checking and printing all permissions
            if(ifDirectory(sortArray[i]) == 1){
                printf("d");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IRUSR){
                printf("r");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IWUSR){
                printf("w");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IXUSR){
                printf("x");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IRGRP){
                printf("r");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IWGRP){
                printf("w");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IXGRP){
                printf("x");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IROTH){
                printf("r");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IWOTH){
                printf("w");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IXOTH){
                printf("x");
            }
            else{
                printf("-");
            }
            // number of hard links to file
            link = check.st_nlink;
            // owner name
            check2 = getpwuid(check.st_uid);
            if(check2 == NULL){
                perror("Error Owner Name");
            }
            // group name
            check3 = getgrgid(check.st_gid);
            if(check3 == NULL){
                perror("Error Group Name");
            }
            // size of file
            sze = check.st_size;
            // time of creation / modification
            check4 = localtime(&check.st_mtime);
            printf("%3d", link);
            printf("%9s", check2->pw_name);
            printf("%7s", check3->gr_name);
            printf("%7d", sze);
            int date = check4->tm_mday;
            printf("%3d", date);
            int month = check4->tm_mon;
            // checking month of creation / modification
            if(month == 0){
                printf(" Jan");
            }
            else if(month == 1){
                printf(" Feb");
            }
            else if(month == 2){
                printf(" Mar");
            }
            else if(month == 3){
                printf(" Apr");
            }
            else if(month == 4){
                printf(" May");
            }
            else if(month == 5){
                printf(" Jun");
            }
            else if(month == 6){
                printf("  Jul");
            }
            else if(month == 7){
                printf(" Aug");
            }
            else if(month == 8){
                printf(" Sep");
            }
            else if(month == 9){
                printf(" Oct");
            }
            else if(month == 10){
                printf(" Nov");
            }
            else if(month == 11){
                printf(" Dec");
            }
            int hour = check4->tm_hour;
            int min = check4->tm_min;
            printf(" %.2d:%.2d",check4->tm_hour,check4->tm_min);
            // adjusting colour for directory
            if(ifDirectory(sortArray[i]) == 1){
                printf("\033[0;34m");
            }
            // adjusting colour for executable file
            else if(ifExecutable(sortArray[i]) == 1){
                printf("\033[0;32m");
            }
            // adjusting colour for regular file
            else if(ifFile(sortArray[i]) == 1){
                printf("\033[0;37m");
            }
            printf(" %s\n", sortArray[i]);
            printf("\033[0;37m");
        }
    }
    closedir(dp);
}

// implementing ls -l for file
void lsLFile(char dir[PATH_MAX], char file[PATH_MAX]){
    struct dirent *dirContainer;
    DIR *dp = opendir(dir);
    struct stat check;
    struct passwd* check2;
    struct group* check3;
    struct tm* check4;
    int link, sze;
    // error handling
    if(dp == NULL){
        perror("Directory/File Error");
    }
    else{
        char path[PATH_MAX];
        strcpy(path, dir);
        strcat(path, "/");
        strcat(path, file);
        stat(path, &check);
        // printing content for the file
        // checking and printing all permissions
        if(ifDirectory(file) == 1){
            printf("d");
        }
        else{
            printf("-");
        }
        if(check.st_mode & S_IRUSR){
            printf("r");
        }
        else{
            printf("-");
        }
        if(check.st_mode & S_IWUSR){
            printf("w");
        }
        else{
            printf("-");
        }
        if(check.st_mode & S_IXUSR){
            printf("x");
        }
        else{
            printf("-");
        }
        if(check.st_mode & S_IRGRP){
            printf("r");
        }
        else{
            printf("-");
        }
        if(check.st_mode & S_IWGRP){
            printf("w");
        }
        else{
            printf("-");
        }
        if(check.st_mode & S_IXGRP){
            printf("x");
        }
        else{
            printf("-");
        }
        if(check.st_mode & S_IROTH){
            printf("r");
        }
        else{
            printf("-");
        }
        if(check.st_mode & S_IWOTH){
            printf("w");
        }
        else{
            printf("-");
        }
        if(check.st_mode & S_IXOTH){
            printf("x");
        }
        else{
            printf("-");
        }
        // number of hard links to file
        link = check.st_nlink;
        // owner name
        check2 = getpwuid(check.st_uid);
        if(check2 == NULL){
            perror("Error Owner Name");
        }
        // group name
        check3 = getgrgid(check.st_gid);
        if(check3 == NULL){
            perror("Error Group Name");
        }
        // size of file
        sze = check.st_size;
        // time of creation / modification
        check4 = localtime(&check.st_mtime);
        printf("%3d", link);
        printf("%9s", check2->pw_name);
        printf("%7s", check3->gr_name);
        printf("%7d", sze);
        int date = check4->tm_mday;
        printf("%3d", date);
        int month = check4->tm_mon;
        // checking month of creation / modification
        if(month == 0){
            printf(" Jan");
        }
        else if(month == 1){
            printf(" Feb");
        }
        else if(month == 2){
            printf(" Mar");
        }
        else if(month == 3){
            printf(" Apr");
        }
        else if(month == 4){
            printf(" May");
        }
        else if(month == 5){
            printf(" Jun");
        }
        else if(month == 6){
            printf("  Jul");
        }
        else if(month == 7){
            printf(" Aug");
        }
        else if(month == 8){
            printf(" Sep");
        }
        else if(month == 9){
            printf(" Oct");
        }
        else if(month == 10){
            printf(" Nov");
        }
        else if(month == 11){
            printf(" Dec");
        }
        int hour = check4->tm_hour;
        int min = check4->tm_min;
        printf(" %.2d:%.2d",check4->tm_hour,check4->tm_min);
        // adjusting colour for directory
        if(ifDirectory(file) == 1){
            printf("\033[0;34m");
        }
        // adjusting colour for executable file
        else if(ifExecutable(file) == 1){
            printf("\033[0;32m");
        }
        // adjusting colour for regular file
        else if(ifFile(file) == 1){
            printf("\033[0;37m");
        }
        printf(" %s\n", file);
        printf("\033[0;37m");
    }
}

// implementing ls -l -a
void lsLNotHidden(char dir[PATH_MAX]){
    struct dirent *dirContainer;
    DIR *dp = opendir(dir);
    struct stat check;
    struct passwd* check2;
    struct group* check3;
    struct tm* check4;
    int link, sze;
    // error handling
    if(dp == NULL){
        perror("Directory/File Error");
    }
    else{
        char sortArray[PATH_MAX][300];
        int count = 0;
        int sum = 0;
        while((dirContainer = readdir(dp)) != NULL){    
            strcpy(sortArray[count++], dirContainer->d_name);
            char path[PATH_MAX];
            strcpy(path, dir);
            strcat(path, "/");
            strcat(path, sortArray[count-1]);
            stat(path, &check);
            // couting block size for total
            sum += check.st_blocks;
	    }
        sort(count, sortArray); 
        sum = sum / 2;
        // printing total
        printf("total %d\n", sum);
        // printing content for each file present in the directory
        for(int i=0; i<count; i++){
            char path[PATH_MAX];
            strcpy(path, dir);
            strcat(path, "/");
            strcat(path, sortArray[i]);
            stat(path, &check);
            // checking and printing all permissions
            if(ifDirectory(sortArray[i]) == 1){
                printf("d");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IRUSR){
                printf("r");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IWUSR){
                printf("w");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IXUSR){
                printf("x");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IRGRP){
                printf("r");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IWGRP){
                printf("w");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IXGRP){
                printf("x");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IROTH){
                printf("r");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IWOTH){
                printf("w");
            }
            else{
                printf("-");
            }
            if(check.st_mode & S_IXOTH){
                printf("x");
            }
            else{
                printf("-");
            }
            // number of hard links to file
            link = check.st_nlink;
            // owner name
            check2 = getpwuid(check.st_uid);
            if(check2 == NULL){
                perror("Error Owner Name");
            }
            // group name
            check3 = getgrgid(check.st_gid);
            if(check3 == NULL){
                perror("Error Group Name");
            }
            // size of file
            sze = check.st_size;
            // time of creation / modification
            check4 = localtime(&check.st_mtime);
            printf("%3d", link);
            printf("%9s", check2->pw_name);
            printf("%7s", check3->gr_name);
            printf("%7d", sze);
            int date = check4->tm_mday;
            printf("%3d", date);
            int month = check4->tm_mon;
            // checking month of creation / modification
            if(month == 0){
                printf(" Jan");
            }
            else if(month == 1){
                printf(" Feb");
            }
            else if(month == 2){
                printf(" Mar");
            }
            else if(month == 3){
                printf(" Apr");
            }
            else if(month == 4){
                printf(" May");
            }
            else if(month == 5){
                printf(" Jun");
            }
            else if(month == 6){
                printf("  Jul");
            }
            else if(month == 7){
                printf(" Aug");
            }
            else if(month == 8){
                printf(" Sep");
            }
            else if(month == 9){
                printf(" Oct");
            }
            else if(month == 10){
                printf(" Nov");
            }
            else if(month == 11){
                printf(" Dec");
            }
            int hour = check4->tm_hour;
            int min = check4->tm_min;
            printf(" %.2d:%.2d",check4->tm_hour,check4->tm_min);
            // adjusting colour for directory
            if(ifDirectory(sortArray[i]) == 1){
                printf("\033[0;34m");
            }
            // adjusting colour for executable file
            else if(ifExecutable(sortArray[i]) == 1){
                printf("\033[0;32m");
            }
            // adjusting colour for regular file
            else if(ifFile(sortArray[i]) == 1){
                printf("\033[0;37m");
            }
            printf(" %s\n", sortArray[i]);
            printf("\033[0;37m");
        }
    }
    closedir(dp);
}

// checking if file exists in directory
int checkFileExists(char path[PATH_MAX], char file[PATH_MAX]){
    struct dirent *dirContainer;
    DIR *dp = opendir(path);
    if(dp == NULL){
        perror("Directory/File Error");
    }
    int ans = 0;
    while((dirContainer = readdir(dp)) != NULL){
        if(strcmp(dirContainer->d_name, file) == 0){
            ans = 1;
            break;
        }
    }
    return ans;
}

// main ls function
void ls(char* tkn){
    int flagL = 0;
    int flagA = 0;
    int count = 0;
    // tokenizing
    tkn = strtok(NULL, " ");
    if(tkn == NULL){
        // calling ls
        char dir[PATH_MAX];
        getcwd(dir, sizeof(dir));
        lsHidden(dir);
    }
    else{
        while(tkn != NULL){
            // reading for flags
            if(strcmp(tkn, "-l") == 0){
                flagL = 1;
            }
            else if(strcmp(tkn, "-a") == 0){
                flagA = 1;
            }
            else if(strcmp(tkn, "-al") == 0){
                flagA = 1;
                flagL = 1;
            }
            else if(strcmp(tkn, "-la") == 0){
                flagA = 1;
                flagL = 1;
            }
            // reading directory / file names
            else{
                array[count++] = tkn;
            }
            tkn = strtok(NULL, " ");
        }
        // calling of ls
        if(flagA == 0 && flagL == 0){
            int flag1 = 0;
            if(count == 1){
                // checking if directory is .
                if(strcmp(array[0], ".") == 0){
                    char send[PATH_MAX];
                    getcwd(send, sizeof(send));
                    lsHidden(send);
                    flag1 = 1;
                }
                // checking if directory is ..
                if(strcmp(array[0], "..") == 0){
                    char dir[PATH_MAX];
                    getcwd(dir, sizeof(dir));
                    chdir("..");
                    char send[PATH_MAX];
                    getcwd(send, sizeof(send));
                    lsHidden(send);
                    chdir(dir);
                    flag1 = 1;
                }
                // checking if directory is ~
                if(strcmp(array[0], "~") == 0){
                    char dir[PATH_MAX];
                    getcwd(dir, sizeof(dir));
                    chdir(invokedDirectory);
                    char send[PATH_MAX];
                    getcwd(send, sizeof(send));
                    lsHidden(send);
                    chdir(dir);
                    flag1 = 1;
                }
                // checking if file
                if(ifFile(array[0]) == 1){
                    char current[PATH_MAX];
                    getcwd(current, sizeof(current));
                    int ch = checkFileExists(current, array[0]);
                    if(ch == 0){
                        int per = chdir(array[0]);
                        if(per == -1){
                            perror("Directory/File Error");
                        }
                    }
                    else{
                        printf("%s\n", array[0]);
                    }
                    flag1 = 1;
                }
            }
            if(flag1 == 0){
                int entFlag = 0;
                for(int i=0; i<count; i++){
                    // checking if file
                    if(ifFile(array[i]) == 1){
                        entFlag = 1;
                        char current[PATH_MAX];
                        getcwd(current, sizeof(current));
                        int ch = checkFileExists(current, array[i]);
                        if(ch == 0){
                            int per = chdir(array[0]);
                            if(per == -1){
                                perror("Directory/File Error");
                                continue;
                            }
                        }
                        else{
                            printf("%s\n", array[i]);
                            continue;
                        }
                    }
                    if(count > 1){
                        if(entFlag == 1){
                            printf("\n");
                            entFlag = 0;
                        }
                        printf("\033[0;37m%s:\n", array[i]);
                    }
                    char dir[PATH_MAX];
                    getcwd(dir, sizeof(dir));
                    int per = chdir(array[i]);
                    // error handling
                    if(per == -1){
                        perror("Directory/File Error");
                    }
                    else{
                        char send[PATH_MAX];
                        getcwd(send, sizeof(send));
                        lsHidden(send);
                        chdir(dir);
                    }
                    if(i < count - 1){
                        printf("\n");
                    }
                }
            }
        }
        // calling of ls -a
        if(flagA == 1 && flagL == 0){
            int flag2 = 0;
            if(count == 0){
                char send[PATH_MAX];
                getcwd(send, sizeof(send));
                lsNotHidden(send);
            }
            else if(count == 1){
                // checking if directory is .
                if(strcmp(array[0], ".") == 0){
                    char send[PATH_MAX];
                    getcwd(send, sizeof(send));
                    lsNotHidden(send);
                    flag2 = 1;
                }
                // checking if directory is ..
                if(strcmp(array[0], "..") == 0){
                    char dir[PATH_MAX];
                    getcwd(dir, sizeof(dir));
                    chdir("..");
                    char send[PATH_MAX];
                    getcwd(send, sizeof(send));
                    lsNotHidden(send);
                    chdir(dir);
                    flag2 = 1;
                }
                // checking if directory is ~
                if(strcmp(array[0], "~") == 0){
                    char dir[PATH_MAX];
                    getcwd(dir, sizeof(dir));
                    chdir(invokedDirectory);
                    char send[PATH_MAX];
                    getcwd(send, sizeof(send));
                    lsNotHidden(send);
                    chdir(dir);
                    flag2 = 1;
                }
                // checking if file
                if(ifFile(array[0]) == 1){
                    char current[PATH_MAX];
                    getcwd(current, sizeof(current));
                    int ch = checkFileExists(current, array[0]);
                    if(ch == 0){
                        int per = chdir(array[0]);
                        if(per == -1){
                            perror("Directory/File Error");
                        }
                    }
                    else{
                        printf("%s\n", array[0]);
                    }
                    flag2 = 1;
                }
            }
            if(flag2 == 0){
                int entFlag = 0;
                for(int i=0; i<count; i++){
                    // checking if file
                    if(ifFile(array[i]) == 1){
                        entFlag = 1;
                        char current[PATH_MAX];
                        getcwd(current, sizeof(current));
                        int ch = checkFileExists(current, array[i]);
                        if(ch == 0){
                            int per = chdir(array[0]);
                            if(per == -1){
                                perror("Directory/File Error");
                                continue;
                            }
                        }
                        else{
                            printf("%s\n", array[i]);
                            continue;
                        }
                    }
                    if(count > 1){
                        if(entFlag == 1){
                            printf("\n");
                            entFlag = 0;
                        }
                        printf("\033[0;37m%s:\n", array[i]);
                    }
                    char dir[PATH_MAX];
                    getcwd(dir, sizeof(dir));
                    int per = chdir(array[i]);
                    // error handling
                    if(per == -1){
                        perror("Directory/File Error");
                    }   
                    else{
                        char send[PATH_MAX];
                        getcwd(send, sizeof(send));
                        lsNotHidden(send);
                        chdir(dir);
                    }
                    if(i < count - 1){
                        printf("\n");
                    }
                }
            }
        }
        // calling of ls -l
        if(flagA == 0 && flagL == 1){
            int flag3 = 0;
            if(count == 0){
                char send[PATH_MAX];
                getcwd(send, sizeof(send));
                lsLHidden(send);
            }
            else if(count == 1){
                // checking if directory is .
                if(strcmp(array[0], ".") == 0){
                    char send[PATH_MAX];
                    getcwd(send, sizeof(send));
                    lsLHidden(send);
                    flag3 = 1;
                }
                // checking if directory is ..
                if(strcmp(array[0], "..") == 0){
                    char dir[PATH_MAX];
                    getcwd(dir, sizeof(dir));
                    chdir("..");
                    char send[PATH_MAX];
                    getcwd(send, sizeof(send));
                    lsLHidden(send);
                    chdir(dir);
                    flag3 = 1;
                }
                // checking if directory is ~
                if(strcmp(array[0], "~") == 0){
                    char dir[PATH_MAX];
                    getcwd(dir, sizeof(dir));
                    chdir(invokedDirectory);
                    char send[PATH_MAX];
                    getcwd(send, sizeof(send));
                    lsLHidden(send);
                    chdir(dir);
                    flag3 = 1;
                }
                // checking if file
                if(ifFile(array[0]) == 1){
                    char current[PATH_MAX];
                    getcwd(current, sizeof(current));
                    int ch = checkFileExists(current, array[0]);
                    if(ch == 0){
                        int per = chdir(array[0]);
                        if(per == -1){
                            perror("Directory/File Error");
                        }
                    }
                    else{
                        char current[PATH_MAX];
                        getcwd(current, sizeof(current));
                        lsLFile(current, array[0]);
                    }
                    flag3 = 1;
                }
            }
            if(flag3 == 0){
                int entFlag = 0;
                for(int i=0; i<count; i++){
                    // checking if file
                    if(ifFile(array[i]) == 1){
                        entFlag = 1;
                        char current[PATH_MAX];
                        getcwd(current, sizeof(current));
                        int ch = checkFileExists(current, array[i]);
                        if(ch == 0){
                            int per = chdir(array[i]);
                            if(per == -1){
                                perror("Directory/File Error");
                                continue;
                            }
                        }
                        else{
                            char current[PATH_MAX];
                            getcwd(current, sizeof(current));
                            lsLFile(current, array[i]);
                            continue;
                        }
                    }
                    if(count > 1){
                        if(entFlag == 1){
                            printf("\n");
                            entFlag = 0;
                        }
                        printf("\033[0;37m%s:\n", array[i]);
                    }
                    char dir[PATH_MAX];
                    getcwd(dir, sizeof(dir));
                    int per = chdir(array[i]);
                    // error handling
                    if(per == -1){
                        perror("Directory/File Error");
                    }
                    else{
                        char send[PATH_MAX];
                        getcwd(send, sizeof(send));
                        lsLHidden(send);
                        chdir(dir);
                    }
                    if(i < count - 1){
                        printf("\n");
                    }
                }
            }
        }
        // calling of ls -l -a
        if(flagA == 1 && flagL == 1){
            int flag4 = 0;
            if(count == 0){
                char send[PATH_MAX];
                getcwd(send, sizeof(send));
                lsLNotHidden(send);
            }
            else if(count == 1){
                // checking if directory is .
                if(strcmp(array[0], ".") == 0){
                    char send[PATH_MAX];
                    getcwd(send, sizeof(send));
                    lsLNotHidden(send);
                    flag4 = 1;
                }
                // checking if directory is ..
                if(strcmp(array[0], "..") == 0){
                    char dir[PATH_MAX];
                    getcwd(dir, sizeof(dir));
                    chdir("..");
                    char send[PATH_MAX];
                    getcwd(send, sizeof(send));
                    lsLNotHidden(send);
                    chdir(dir);
                    flag4 = 1;
                }
                // checking if directory is ~
                if(strcmp(array[0], "~") == 0){
                    char dir[PATH_MAX];
                    getcwd(dir, sizeof(dir));
                    chdir(invokedDirectory);
                    char send[PATH_MAX];
                    getcwd(send, sizeof(send));
                    lsLNotHidden(send);
                    chdir(dir);
                    flag4 = 1;
                }
                // checking if file
                if(ifFile(array[0]) == 1){
                    char current[PATH_MAX];
                    getcwd(current, sizeof(current));
                    int ch = checkFileExists(current, array[0]);
                    if(ch == 0){
                        int per = chdir(array[0]);
                        if(per == -1){
                            perror("Directory/File Error");
                        }
                    }
                    else{
                        char current[PATH_MAX];
                        getcwd(current, sizeof(current));
                        lsLFile(current, array[0]);
                    }
                    flag4 = 1;
                }
            }
            if(flag4 == 0){
                int entFlag = 0;
                for(int i=0; i<count; i++){
                    // checking if file
                    if(ifFile(array[i]) == 1){
                        entFlag = 1;
                        char current[PATH_MAX];
                        getcwd(current, sizeof(current));
                        int ch = checkFileExists(current, array[i]);
                        if(ch == 0){
                            int per = chdir(array[0]);
                            if(per == -1){
                                perror("Directory/File Error");
                                continue;
                            }
                        }
                        else{
                            char current[PATH_MAX];
                            getcwd(current, sizeof(current));
                            lsLFile(current, array[i]);
                            continue;
                        }
                    }
                    if(count > 1){
                        if(entFlag == 1){
                            printf("\n");
                            entFlag = 0;
                        }
                        printf("\033[0;37m%s:\n", array[i]);
                    }
                    char dir[PATH_MAX];
                    getcwd(dir, sizeof(dir));
                    int per = chdir(array[i]);
                    // error handling
                    if(per == -1){
                        perror("Directory/File Error");
                    }
                    else{
                        char send[PATH_MAX];
                        getcwd(send, sizeof(send));
                        lsLNotHidden(send);
                        chdir(dir);
                    }
                    if(i < count - 1){
                        printf("\n");
                    }
                }
            }
        }
    }
}