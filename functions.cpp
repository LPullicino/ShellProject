//
// Created by luke on 6/23/20.
//

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>

#include "functions.h"



#define MAX_PATH 1024
#define MAX_STRING_LEN 255

extern char **environ;

void initVariables() {

    char *currWD = getcwd(NULL, 0);

    if (currWD != NULL) {

        setenv("CWD", currWD, 1);
        free(currWD);

    } else {

        perror("getcwd() error");

    }

}

void showEnv() {

    for (char **envVar = environ; *envVar; ++envVar) {

        std::cout << *envVar << '\n';

    }

}

void unsetVariable(char *args[]) {

    char* varToBeUnset = args[1];
    unsetenv(varToBeUnset);

}

void setVariable(char *args[], char *line) {

    char *token = NULL, *vars[2], *temp = nullptr;
    int index = 0;

    char varName[MAX_STRING_LEN];
    char varValue[MAX_STRING_LEN];

    temp = line;

    char *charLoc = strchr(temp, '=');
    int pos = charLoc - temp + 1;               //calculates the position of '=' as if it was an array

/*    printf("%d\n", pos);*/

    strncpy(varName, line, pos-1);          //copies the characters on the left hand side of the '=' symbol
    strcpy(varValue, line + pos);          //copies the characters on the right hand side of the '=' symbol

/*    printf("%s\n", line);
    printf("%s\n", varName);
    printf("%s\n", varValue);*/

    temp = varValue;

    if(strchr(temp, '$') != NULL) {

        char newVar[MAX_STRING_LEN];

        strcpy(newVar, varValue + 1);

        setenv(varName, getenv(newVar), 1);

    } else {

        setenv(varName, varValue, 1);

    }



}

void changeDir(char *args[]) {

    char* newDir = args[1];

    if(chdir(newDir) != 0){

        perror("Error when changing directory!");

    } else {

        char *currWD = getcwd(NULL, 0);

        setenv("CWD", currWD, 1);
        free(currWD);

    }


}

void echo(char *args[], int numOfTokens) {

    for(int i = 1; i < numOfTokens; i++) {

        char *dollarCheck = args[i];
        char *escapeCharCheck = args[i];

        if(strchr(dollarCheck, '$') != NULL /*&& strchr(escapeCharCheck, '\\') == NULL*/) {

            char *var = strtok(args[i], "$");
            std::cout << getenv(var);

        } else {

            std::cout << args[i];

        }

        std::cout << " ";

    }

    std::cout << std::endl;

}