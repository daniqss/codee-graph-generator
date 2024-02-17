/*
Luca D'angelo Sabin : l.dangelo@udc.es : 47436242L
Antón Fernández Cerecedo : anton.fcerecedo :
*/
#include "types.h"
#include "f_dadas.h"

void printAuthors(char* argumento);
void consoleGetpid();
void consoleGetppid();
void pwd();
void changeDir(char* argumento);
void getDate();
void getTime();
void exit_console(bool* terminado);
//------historic---------
void initLista(char* historic[]);
bool addHistCommand(char* comando[], char* historic[], int n_palabras);
char* getCommand(char* n, char* historic[]);
char* next(int* n, char* historic[]);
void clearList(char* historic[]);
void showLista(char* historic[]);
void showListaN(char* n, char* historic[]);
