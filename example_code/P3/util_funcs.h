/*
Luca D'angelo Sabin : l.dangelo@udc.es : 47436242L
Antón Fernández Cerecedo : anton.fcerecedo :
*/
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int validarArgumentos(char* argumento, const char* argumentos_validos[]);
bool isnumber(char* args);
int check_prio(char* argumentos[], int n);
void combine (char* input[], int n, char* output);
