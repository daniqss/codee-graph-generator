/*
Luca D'angelo Sabin : l.dangelo@udc.es : 47436242L
Antón Fernández Cerecedo : anton.fcerecedo :
*/
#include "c_P0.h"

void exit_console(bool* terminado) {
  *terminado = true;
}

void pwd() {
  char cwd[LONGITUD_MAX_DIR];
  getcwd(cwd, sizeof(cwd));
  printf("%s\n",cwd);
}

void getTime() {
  time_t rawtime;
  struct tm* time_info;

  time(&rawtime);
  time_info = localtime(&rawtime);

  printf ("%02d:%02d:%02d\n", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
}

void getDate() {
  time_t rawtime;
  struct tm* time_info;

  time(&rawtime);
  time_info = localtime(&rawtime);

  printf ("%02d/%02d/%04d\n", time_info->tm_mday, time_info->tm_mon, time_info->tm_year+1900);
}

void consoleGetpid() {
  int pid = getpid();
  printf("PID de la shell: %d\n", pid);
}

void consoleGetppid() {
  int pid = getppid();
  printf("PID del padre de la shell: %d\n", pid);
}

void printAuthors(char* argumento) {
  const char* argumentos_validos[] = {"-l", "-n", NULL};
  char arg_case_letter;

  if (argumento == NULL) arg_case_letter = NULL_ARG;
  else if (validarArgumentos(argumento, argumentos_validos) != NO_ID) arg_case_letter = argumento[1];
  else arg_case_letter = SKIP;


    switch(arg_case_letter){
      case 'l':
        printf("%s\n%s\n", LOGIN_1, LOGIN_2);
        break;

      case 'n':
        printf("%s\n%s\n", AUTHOR_1, AUTHOR_2);
        break;

      case NULL_ARG:
        printf("%s: %s\n%s: %s\n", AUTHOR_1, LOGIN_1, AUTHOR_2, LOGIN_2);
        break;

      case SKIP:

        break;

      default:
        break;
    }
}

void changeDir(char* argumento) {
  if (argumento == NULL) pwd();
  else if (chdir(argumento) != 0) {
    perror("No es posible cambiar el directorio");
  }
}

//----------------historic commands----------------------
bool addHistCommand(char* comando[], char* historic[], int n_palabras){
    int i = 0;

    for (; historic[i] != NULL; ++i); //llegar a la ultima posicion de la lista
    if (i == HISTORIC_MAX-1) return false; //si la lista está llena return false
    else if (comando[0] == NULL) return false;

    char* aux = (char*) malloc(sizeof(char)*LONGITUD_MAX_COMANDOS);
    *aux = '\0';  //incialización

    historic[i] = (char*) malloc(sizeof(char)*LONGITUD_MAX_COMANDOS); //añadir el puntero a la lista
    *historic[i] = '\0'; //incialización

    for (int j=0; j<n_palabras; ++j) { //copia el comando a la lista palabra a palabra
      strcpy(aux, comando[j]); //copiar palabra a variable auxiliar
      strcat(historic[i], aux); //concatenar palabra a la lista
      strcat(historic[i], " "); //concatenar un espacio que se pierde al concatenar
    }

    historic[i+1] = NULL; //avanzar la posicion de NULL

    free(aux);//libramos la memoria de la variable auxiliar
    return true;
}

char* getCommand(char* n, char* historic[]) {
  int i = atoi(n);
  int j = 0;

  for (; historic[j] != NULL; ++j); //llegar a la ultima posicion de la lista

  if (i<=j) return historic[i];
  else return NULL;
}

void clearList(char* historic[]){
  for (int i=0; historic[i] != NULL; ++i){
    free(historic[i]);//luego libramos la memoria del puntero
  }
  historic[0] = NULL;
}

void showLista(char* historic[]){
  for(int i=0; historic[i] != NULL; ++i){
    printf("%d\t%s\n",i,historic[i]);
  }
}

void showListaN(char* n, char* historic[]){
  int i = atoi(n);

  for(int j=0; historic[j] != NULL && j<i; ++j){
    printf("%d\t%s\n",j,historic[j]);
  }
}
