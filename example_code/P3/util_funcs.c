/*
Luca D'angelo Sabin : l.dangelo@udc.es : 47436242L
Antón Fernández Cerecedo : anton.fcerecedo :
*/
#include "util_funcs.h"

int validarArgumentos(char* argumento, const char* argumentos_validos[]) {

  if (strlen(argumento) < 1) return false;

  for (int i=0; argumentos_validos[i]!=NULL ; ++i) {  /*Si fue pasado con argumentos recorre la lista de argumentos
                                                       del comando para comprobar si es valido el argumento*/
    if (strcmp(argumento, argumentos_validos[i]) == 0) return i;
  }

  return -1;
}

bool isnumber(char* args) {
  int len = strlen(args);

  for (int i = 0; i<len; i++) {
    if (!isdigit(args[i])) return false;
  } return true;
}

int check_prio(char* argumentos[], int n) {
  for (int i = n-1; i>0 && i>n-3; i--) {
    if (argumentos[i][0] == '@') {
      if (isnumber(argumentos[i]+1)) {
        return i;
      }
    }
  }
  return -1;
}

void combine (char* input[], int n, char* output) {
  strcpy(output, "");
  for (int i=0; i<n && input[i] != NULL; i++) {
    strcat(output, input[i]);
    strcat(output, " ");
  }
}
