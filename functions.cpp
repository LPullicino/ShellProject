//
// Created by luke on 6/23/20.
//

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

#include "functions.h"


#define MAX_ARGS 255
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

void forker(char *args[], char *line) {

    pid_t pidNum = fork();
    char *temp = line;

    char *fileName = args[0];                   //can just input args[0] in execvp() but this makes the code slightly more readable

    if (pidNum == -1) {

        perror("fork failed");

    } else if (pidNum > 0) {

        int status;

        if (waitpid(pidNum, &status, 0) == -1) {

            perror("Error on wait");

        }

        if (WIFEXITED(status)) {

            printf("Child exited normally with exit status: %d\n", WEXITSTATUS(status));

        } else {

            printf("Child did not exit normally.\n");

        }

    }

    if(strchr(temp, '"') != NULL) {

        temp = line;
        char *firstLoc = strchr(temp, '"');
        int firstPos = firstLoc - temp + 1;

        temp = line;
        char *lastLoc = strrchr(temp, '"');
        int lastPos = lastLoc - temp + 1;

        char fileArgument[MAX_STRING_LEN];
        strncpy(fileArgument, line + firstPos, lastPos - firstPos - 1);

        char *toPass[3] = {fileName, fileArgument, NULL};

        if (execvp(fileName, toPass) == -1) {

            perror("Error when using execvp");

        }


    } else {

        redirectionHandler(args, line);

        if (execvp(fileName, args) == -1) {

            perror("Error when using execvp");

        }

        redirectionReset();

    }

}

void piper(char *args[], char *line) {

    pid_t id1, id2;
    int numOfPipes = countChar(line, '|'), fd[2*numOfPipes], tokenIndex;
    char *temp = line, *token = NULL;

    char stringBeforePipe[MAX_STRING_LEN];
    char stringAfterPipe[MAX_STRING_LEN];

    char *argsBeforePipe[MAX_ARGS];
    char *argsAfterPipe[MAX_ARGS];

    char *charLoc = strchr(temp, '|');
    int pos = charLoc - temp + 1;

    //Splitting the users input into two different parts which will then be tokenised

    strncpy(stringBeforePipe, line, pos-1);
    strcpy(stringAfterPipe, line + pos + 1);

    temp = stringAfterPipe;

    //Start tokenising

    token = strtok(stringBeforePipe, " ");

    for(tokenIndex = 0; token != NULL && tokenIndex < MAX_ARGS - 1; tokenIndex++) {

        argsBeforePipe[tokenIndex] = token;
        token = strtok(NULL, " ");

    }

    argsBeforePipe[tokenIndex] = NULL;

    token = strtok(stringAfterPipe, " ");

    for(tokenIndex = 0; token != NULL && tokenIndex < MAX_ARGS - 1; tokenIndex++) {

        temp = token;
        if (strchr(temp, '|')) {

            break;

        }

        argsAfterPipe[tokenIndex] = token;
        token = strtok(NULL, " ");

    }

    // Finish tokenising

    if(pipe(fd) < 0) {

        perror("Error when creating pipe");

    }

    if((id1 = fork()) < 0) {

        perror("Error when forking");

    } else if (id1 == 0) {

        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);

        if (execvp(argsBeforePipe[0], argsBeforePipe) == -1) {

            perror("Error when using execvp");

        }

    }

    if((id2 = fork()) < 0) {

        perror("Error when forking");

    } else if (id2 == 0) {

        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);

        if (execvp(argsAfterPipe[0], argsAfterPipe) == -1) {

            perror("Error when using execvp");

        }

    }

    close(fd[0]);
    close(fd[1]);

    int status;
    waitpid(id2, &status, 0);

}

void showEnv() {

    for (char **envVar = environ; *envVar; ++envVar) {                      //Loops through all the environment variables and prints them using the environ system variable

        std::cout << *envVar << '\n';

    }

}

void unsetVariable(char *args[]) {

    char* varToBeUnset = args[1];                       //Gets the variable that the system needs to unset
    unsetenv(varToBeUnset);

}

void setVariable(char *args[], char *line) {

    char *temp = nullptr;

    char varName[MAX_STRING_LEN];
    char varValue[MAX_STRING_LEN];

    temp = line;

    char *charLoc = strchr(temp, '=');
    int pos = charLoc - temp + 1;                           //calculates the position of '=' as if it was an array

    strncpy(varName, line, pos-1);                       //copies the characters on the left hand side of the '=' symbol
    strcpy(varValue, line + pos);                       //copies the characters on the right hand side of the '=' symbol

    temp = varValue;

    if(strchr(temp, '$') != NULL) {                      //handles the instances where the user wants to set a variable to the value of another variable

        char newVar[MAX_STRING_LEN];

        strcpy(newVar, varValue + 1);

        setenv(varName, getenv(newVar), 1);

    } else {

        setenv(varName, varValue, 1);

    }

}

void changeDir(char *args[]) {

    char* newDir = args[1];                                                 //Checks which directory the user wants to go to

    if(chdir(newDir) != 0){

        perror("Error when changing directory!");

    } else {

        char *currWD = getcwd(NULL, 0);

        setenv("CWD", currWD, 1);                              //Updates the variable that contains our current working directory
        free(currWD);

    }


}

void echo(char *args[], int numOfTokens) {

    for(int i = 1; i < numOfTokens; i++) {

        char *dollarCheck = args[i];
        char *escapeCharCheck = args[i];

        if(strchr(dollarCheck, '$') != NULL /*&& strchr(escapeCharCheck, '\\') == NULL*/) {                             //Handles the translation from environment variables to its value

            char *var = strtok(args[i], "$");
            std::cout << getenv(var);

        } else {

            std::cout << args[i];

        }

        std::cout << " ";

    }

    std::cout << std::endl;

}

int countChar(char *line, char charToCount) {

    int numOfOccurences = 0;

    for(int i = 0; i < sizeof(line)/sizeof(line[0]); i++) {

        if(line[0] == charToCount) {

            numOfOccurences++;

        }

    }

    return numOfOccurences;

}

void redirectionHandler(char *args[], char *line) {

    char fileName[MAX_STRING_LEN], *tempGreater = line, *tempSmaller = line, *mode, *charLoc;
    bool RorW = false, redirectionFound = false;                                                                        //RorW - Read or Write where "false" is read and "true" is write
    int pos;

    if ((charLoc = strchr(tempGreater, '>')) != NULL) {

        pos = charLoc - tempGreater + 1;

        if(countChar(line, '>') == 1) {

            mode = "w";                                                                                                 //freopen() has multiple different modes in this case we're setting it to "w" which is the overwrite mode.

        } else {

            mode = "w+";                                                                                                //Setting mode to append
            pos += 1;

        }

        RorW = true;
        redirectionFound = true;

    } else if (strchr(tempSmaller, '<') != NULL) {

        pos = charLoc - tempSmaller + 1;

        mode = "r";                                                                                                     //Setting mode to read
        redirectionFound = true;

    }

    if (redirectionFound) {

        strcpy(fileName, line + pos);

    }

    if (RorW && redirectionFound) {

        freopen(fileName, mode, stdout);                                //redirects program output to the given filename

    } else if (redirectionFound) {

        freopen(fileName, mode, stdin);                                 //redirects contents of the given file to the input stream

    }

}

void redirectionReset() {

    //since freopen() redirects our input/output streams we have to redirect them back to our terminal

    freopen("dev/tty", "w", stdout);
    freopen("dev/tty", "w", stdin);

}