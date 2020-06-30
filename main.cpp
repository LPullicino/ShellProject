#include <cstring>

#include "linenoise.h"        //The Linenoise utility used is from https://github.com/antirez/linenoise
#include "functions.h"

int main() {

    char *line, ogLine[MAX_STRING_LEN], *token = NULL, *args[MAX_ARGS];
    int tokenIndex, numOfTokens;

    initVariables();

    while ((line = linenoise("$ ")) != NULL) {

        strcpy(ogLine, line);

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

        } else if (strcmp(args[0], "exit") == 0){

            return 0;

        } else {

            char *tempArg = args[0];
            char *tempLine = ogLine;


            if(strchr(tempArg, '=') != NULL) {

                setVariable(args, ogLine);


            } else if (strchr(tempLine, '|') != NULL) {

                piper(args, ogLine);

            } else if (tokenIndex != 1){

                forker(args, ogLine);

            }

        }

        linenoiseFree(line);

    }

}
