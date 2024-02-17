/*
Luca D'angelo Sabin : l.dangelo@udc.es
Antón Fernández Cerecedo : anton.fcerecedo
*/

#ifndef HISTORIC_C
#endif
#define HISTORIC_C
#define LONGITUD_MAX_COMMANDOS 256

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define HISTORIC_MAX 4096

void initLista(char* historic[]);
bool addHistCommand(char* comando[], char* historic[], int n_palabras);
char* getCommand(char* n, char* historic[]);
char* next(int* n, char* historic[]);
void clearList(char* historic[]);
void showLista(char* historic[]);
void showListaN(char* n, char* historic[]);
