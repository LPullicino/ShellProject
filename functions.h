//
// Created by luke on 6/23/20.
//

#ifndef SHELLPROJECT_FUNCTIONS_H
#define SHELLPROJECT_FUNCTIONS_H

void initVariables();
void changeDir(char *args[]);
void echo(char *args[], int numOfTokens);
void setVariable(char *args[], char *line);
void unsetVariable(char *args[]);
void showEnv();
void forker(char *args[], char *line);
void piper(char *args[], char *line);
int countChar(char *line, char charToCount);

#endif //SHELLPROJECT_FUNCTIONS_H
