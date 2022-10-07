#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/utsname.h>
#include<string.h>
#include<pwd.h>
#include<limits.h>
#include<ctype.h>
#include<dirent.h>
#include<sys/stat.h>
#include<grp.h>
#include<time.h>
#include<sys/wait.h>
#include<signal.h>

#include "pwd.h"
#include "echo.h"
#include "cd.h"
#include "display.h"
#include "history.h"
#include "ls.h"
#include "pinfo.h"
#include "discover.h"
#include "processHandling.h"

extern char username[300];
extern char hostname[300];
extern char invokedDirectory[PATH_MAX];
extern char currDirectory[PATH_MAX];
extern char prevDirectory[PATH_MAX];
extern char directory[PATH_MAX];
extern char *array[PATH_MAX];
extern char *tokenArray[MAX_INPUT];
extern char *processArray[MAX_INPUT];
extern char historyArray[20][MAX_INPUT];
extern int historyIndex;
extern int flag;
extern int recurseCall;
extern int backgroundID[1000];
extern char backCommand[100][100];
extern int timeTaken;