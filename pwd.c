#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/utsname.h>
#include<string.h>
#include<pwd.h>
#include<limits.h>

void pwd(){
    // retriving path of current directory
    char dir[PATH_MAX];
    getcwd(dir, sizeof(dir));
    printf("%s\n", dir);
}