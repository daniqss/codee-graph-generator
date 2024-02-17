/*
Luca D'angelo Sabin : l.dangelo@udc.es : 47436242L
Antón Fernández Cerecedo : anton.fcerecedo :
*/
#include "proc_list.h"

void cmd_getpriority(char* argumentos[], int n_palabras);
void cmd_setpriority(char* argumentos[], int n_palabras);
void cmd_getuid(char* argumentos[], int n_palabras);
void cmd_setuid(char* argumentos[], int n_palabras);
void cmd_fork(char* argumentos[], int n_palabras, char *argv[]);
void cmd_execute(char* argumentos[], int n_palabras, char* user);
void runAs (char* argumentos[], int n_palabras, tList* proc_list);
void executeAs (char* argumentos[], int n_palabras);
void proc(char* argumentos[], int n_palabras, tList* proc_list);
void foreground (char* argumentos[], int n_palabras, char* user);
void background (char* argumentos[], int n_palabras, tList* proc_list, char* user);
void listprocs (char* argumentos[], int n_palabras, tList* proc_list);
void deleteProcs(char* argumentos[], int n_palabras, tList* proc_list);
