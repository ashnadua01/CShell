#include "globalVariables.h"

char* echoArray[MAX_INPUT];

// implementing echo
void echo(int sendCount){
    for(int i=1; i<sendCount; i++){
        printf("%s ", array[i]);
    }
    printf("\n");
}