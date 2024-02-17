/*
Luca D'angelo Sabin : l.dangelo@udc.es
Antón Fernández Cerecedo : anton.fcerecedo
*/

#include "historic.h"

// void initLista(char* historic[]) {
//   historic[0] = NULL;
// }

bool addHistCommand(char* comando[], char* historic[], int n_palabras){
    int i = 0;

    for (; historic[i] != NULL; ++i); //llegar a la ultima posicion de la lista
    if (i == HISTORIC_MAX-1) return false; //si la lista está llena return false
    else if (comando[0] == NULL) return false;

    char* aux = (char*) malloc(sizeof(char)*LONGITUD_MAX_COMMANDOS);
    *aux = '\0';  //incialización

    historic[i] = (char*) malloc(sizeof(char)*LONGITUD_MAX_COMMANDOS); //añadir el puntero a la lista
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
