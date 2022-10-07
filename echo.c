#include "globalVariables.h"

// implementing echo
void echo(char* token){
    token = strtok(NULL, " ");
    while(token != NULL){
        printf("%s ", token);
        token = strtok(NULL, " ");
    }
    printf("\n");
}