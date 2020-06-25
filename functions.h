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

#endif //SHELLPROJECT_FUNCTIONS_H
