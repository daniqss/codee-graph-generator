/*
Luca D'angelo Sabin : l.dangelo@udc.es : 47436242L
Antón Fernández Cerecedo : anton.fcerecedo :
*/

#include "c_P0.h"
#include "c_P1.h"
#include "c_P2.h"

//----------------------------------------------
void imprimirPrompt();
void leerEntrada(char* comando, char* c_troceado[], int* n_palabras);
void procesarEntrada(char* init_comando, char* c_troceado[], const char* comandos_validos[], bool* terminado, int n_palabras, char* historic[], t_memory_node* mem_list[]);
int validarComando(char* comando, const char* comandos_validos[]);
//----------------- Comandos de la terminal P0 ----------------------------------------------------------------
void getHistoric(char* comando, char* c_troceado[], char* argumento, char* historic[], const char* comandos_validos[], bool* terminado, int n_palabras, t_memory_node* mem_list[]);
//----------------- Comandos de la terminal P1 ---------------------------------------------------------------

//------------------ Comandos de la terminal P2 --------------------------------------------------------------
void show_variables();
void show_funcs();
void shell_recurse(char* args[], int n_args);
void recurse (int n);
//------------------------------------------------------------------------------------------------------------
int global1,global2,global3;

//--------------------- Main -----------------------
int main() {
  const char* lista_comandos[] = {"authors","getpid","getppid","pwd",
  "chdir","date","time","historic","quit","exit","end","create","delete",
  "list", "memory", "memdump", "memfill", "readfile", "writefile", "recurse"};
  char* c_troceado[LONGITUD_MAX_COMMANDOS];
  char* historic[HISTORIC_MAX] = {NULL};
  char comando[LONGITUD_MAX_COMMANDOS];
  t_memory_node* mem_list[MAX];
  iniciarLista(mem_list);

  int n_palabras; //numero de palabras en el comando
  bool terminado = false;

  while (!terminado) {
    imprimirPrompt();
    leerEntrada(comando, c_troceado, &n_palabras);
    addHistCommand(c_troceado, historic, n_palabras);
    procesarEntrada(comando, c_troceado, lista_comandos, &terminado, n_palabras, historic, mem_list);
  }
  clearList(historic);
  clearMemList(mem_list);
  return 0;
}


void imprimirPrompt(){
  printf("> ");
}

void leerEntrada(char* cadena, char* c_troceado[], int* n_palabras){

    fgets(cadena, LONGITUD_MAX_COMMANDOS, stdin);

    *n_palabras = TrocearCadena(cadena, c_troceado); // aqui se hace la llamada a trocear cadena
}

void procesarEntrada(char* init_comando, char* c_troceado[], const char* comandos_validos[], bool* terminado, int n_palabras, char* historic[], t_memory_node* mem_list[]) {
  char* comando = c_troceado[0];
  char* argumentos[LONGITUD_MAX_COMMANDOS] = {NULL}; // establecida esta longitud para los casos en los que se pasan muchos argumentos
  char case_id;



  if (n_palabras > 1){ //se copian las direcciones de los argumentos al array de argumentos.
    for (int j=1; j < n_palabras; j++) {
      argumentos[j-1] = c_troceado[j];
    }
  }

  if (comando != NULL) {
    case_id = validarComando(comando, comandos_validos);
  } else {
    case_id = SKIP;
  }

  switch (case_id) {
    case 0://authors
      printAuthors(argumentos[0]);
      break;

    case 1://getpid
      consoleGetpid();
      break;

    case 2://getppid
      consoleGetppid();
      break;

    case 3://pwd
      pwd();
      break;

    case 4://chdir
      changeDir(argumentos[0]);
      break;

    case 5://date
      getDate();
      break;

    case 6://time
      getTime();
      break;

    case 7://historic
      getHistoric(init_comando, c_troceado, argumentos[0], historic, comandos_validos, terminado, n_palabras, mem_list);
      break;

    case 8://quit
      exit_console(terminado);
      break;

    case 9://exit
      exit_console(terminado);
      break;

    case 10://end
      exit_console(terminado);
      break;

    case 11://create
      shell_create(argumentos, n_palabras);
      break;

    case 12://delete
      shell_delete(argumentos, n_palabras);
      break;

    case 13://list
      shell_list(argumentos, n_palabras);
      break;

    case 14://memory
      shell_memory(argumentos, n_palabras, mem_list);
      break;

    case 15:
      memdump(argumentos, n_palabras-1);
      break;

    case 16:
      memfill(argumentos, n_palabras-1);
      break;

    case 17:
      readfile(argumentos, n_palabras-1);
      break;

    case 18:
      writefile(argumentos, n_palabras-1);
      break;

    case 19:
      shell_recurse(argumentos, n_palabras-1);
      break;

    case SKIP://skip
      break;

    case NO_ID://command not found
      printf("%s no encontrado\n",comando);
      break;

    default:
      break;
  }
}

int validarComando(char* comando, const char* comandos_validos[]) {

  for (int i=0; i<N_COMANDOS; i++) {
    if (strcmp(comando, comandos_validos[i]) == 0) {
      return i; //comando bien, devuelve el índice del comando
    }
  }
  return NO_ID; //si el comando no fue encontrado devuelve -1 como índice para indicar que no es válido
}


//--------------------- Comandos P0 ------------------------------
void getHistoric(char* comando, char* c_troceado[], char* argumento, char* historic[], const char* comandos_validos[], bool* terminado, int n_palabras, t_memory_node* mem_list[]) {
  const char* argumentos_validos[] = {"-c", "-", "-r", NULL};
  char arg_case_letter;
  char aux[LONGITUD_MAX_ARGUMENTOS];

  if (argumento == NULL) arg_case_letter = NULL_ARG;
  else if (argumento[0] == '-' && (argumento[1] > '0' && argumento[1] <= '9')) arg_case_letter = argumento[0]; //para el caso de que el argumento sea -N
  else {
    strncpy(aux, argumento, 2); //Para el caso de que el argumento sea -rN
    aux[2] = '\0'; //copiamos el -rN (con el caracter \0 al final) a una variable auxiliar.

    if (validarArgumentos(aux, argumentos_validos) != NO_ID) arg_case_letter = argumento[1]; // para argumentos -rN y -c
    else arg_case_letter = SKIP;
  }

  switch(arg_case_letter){
    case 'c':
      clearList(historic);
      break;

    case 'r':
    {
      strtok(argumento, "r");
      strcpy(aux,strtok(NULL, "r")); //coger los numeros del argumento (depues del -r)
      char* aux_string = getCommand(aux, historic);//copia el comando a una variable auxiliar

      if (aux_string != NULL) strcpy(comando, aux_string); //lo copia al campo de comando
      else comando = NULL; //para no saltar error solo se copia si no es NULL, si no se le asigna el valor NULL

      n_palabras = TrocearCadena(comando, c_troceado);

      if (comando != NULL) {//comparacion para el caso de historic -rN
        if (strcmp(comando, "historic") == 0 && c_troceado[1] != NULL && c_troceado[1][0] == '-' && c_troceado[1][1] == 'r') c_troceado[1] = NULL; //Evitar un bucle infinito con historic -rN
      }

      if (c_troceado[0] == NULL) printf("No existe el elemento %s en el historico\n", aux);
      else printf("Ejecutando historic (%s): %s\n", aux, aux_string);

      procesarEntrada(comando, c_troceado, comandos_validos, terminado, n_palabras, historic, mem_list);
      break;
    }
    case '-':
      argumento = argumento + 1; //avanzar el puntero del argumento en 1 para ignorar el '-'

      showListaN(argumento, historic);
      break;

    case NULL_ARG:
      showLista(historic);
      break;

    case SKIP:
      break;

    default:
      break;
  }
}

//--------------------- Comandos P1 -------------------------------


//--------------------- Comandos P2 -------------------------------

void shell_memory(char* argumentos[], int n_args, t_memory_node* mem_list[]) {
  const char* argumentos_validos[] = {"-allocate","-dealloc","-show","-deletekey","-show-vars","-show-funcs","-dopmap",NULL};
  int arg_id;

  if (n_args > 1 && (arg_id = validarArgumentos(argumentos[0], argumentos_validos)) != NO_ID) {
    switch (arg_id) {
      case 0:
        if (n_args > 2)
          allocate_memory(n_args, argumentos, mem_list);
        break;

      case 1:
        if (n_args > 2)
          deallocate_memory(n_args, argumentos, mem_list);
        break;

      case 2:
        if (n_args > 2)
          show_memory(n_args, argumentos, mem_list);
        break;

      case 3:
        if (n_args > 2)
          delete_shared_key(n_args, argumentos, mem_list);
        break;

      case 4:
        show_variables();
        break;

      case 5:
        show_funcs();
        break;

      case 6:
        Cmd_dopmap(argumentos);
        break;
    }

  } else printList(mem_list, NO_ID);
}


void show_variables(){
  int local1, local2, local3;

  printf("Variables globales: %15p,\t%15p,\t%15p\n",
  (void*) &global1, (void*) &global2, (void*) &global3);
  printf("Variables locales:  %15p,\t%15p,\t%15p\n",
  (void*) &local1, (void*) &local2, (void*) &local3);
}

void show_funcs(){
  printf("Funciones del programa: %15p,\t%15p,\t%15p\n",
  &show_variables, &main, &pwd);
  printf("Funciones de librería:  %15p,\t%15p,\t%15p\n",
  &printf, &shmget, &isdigit);
}

void shell_recurse(char* args[], int n_args) {
  int n;

  if (n_args < 1) return;
  else n = atoi(args[0]);

  recurse(n);
}

void recurse (int n) {
  char automatico[LONGITUD_MAX_COMMANDOS];
  static char estatico[LONGITUD_MAX_COMMANDOS];

  printf("Parámetro de n: %d en %p\n",n ,&n);
  printf("Static array en: %p\n", estatico);
  printf("Array automatico en: %p\n", automatico);
  puts("");
  n--;
  if (n>0) recurse(n);
}
