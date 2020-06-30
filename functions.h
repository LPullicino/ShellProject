//
// Created by luke on 6/23/20.
//

#ifndef SHELLPROJECT_FUNCTIONS_H
#define SHELLPROJECT_FUNCTIONS_H


#define MAX_ARGS 255
#define MAX_PATH 1024
#define MAX_STRING_LEN 255

void initVariables();
void changeDir(char *args[]);
void echo(char *args[], int numOfTokens);
void setVariable(char *args[], char *line);
void unsetVariable(char *args[]);
void showEnv();
void forker(char *args[], char *line);
void piper(char *args[], char *line);
int countChar(char *line, char charToCount);
void redirectionHandler(char *args[], char *line);
void redirectionReset();

#endif //SHELLPROJECT_FUNCTIONS_H
