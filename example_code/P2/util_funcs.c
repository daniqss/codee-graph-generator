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
