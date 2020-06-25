#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "linenoise.h"        //The Linenoise utility used is from https://github.com/antirez/linenoise
#include "functions.h"

#define MAX_ARGS 255
#define NUM_FUNCTIONS 1
#define MAX_STRING_LEN 255

int main() {

    char *line, ogLine[MAX_STRING_LEN], *token = NULL, *args[MAX_ARGS], *implementedFunctions[NUM_FUNCTIONS];
    int tokenIndex, numOfTokens;

    char *username = getenv("USER");

    initVariables();

    while ((line = linenoise("$ ")) != NULL) {

        strcpy(ogLine, line);

        //printf("%s\n", ogLine);

        token = strtok(line, " ");

        for(tokenIndex = 0; token != NULL && tokenIndex < MAX_ARGS - 1; tokenIndex++) {

            args[tokenIndex] = token;
            token = strtok(NULL, " ");

        }

        numOfTokens = tokenIndex;
        args[tokenIndex] = NULL;

        if(strcmp(args[0], "echo") == 0){

            echo(args, numOfTokens);

        } else if(strcmp(args[0], "cd") == 0){

            changeDir(args);

        } else if (strcmp(args[0], "showenv") == 0){

            showEnv();

        } else if (strcmp(args[0], "unset") == 0){

            unsetVariable(args);

        } else {

            char *temp = args[0];

            if(strchr(temp, '=') != NULL) {

                setVariable(args, ogLine);

            } else {

                //fork-plus-exec

            }

        }

        linenoiseFree(line);


    }

    return 0;

}
