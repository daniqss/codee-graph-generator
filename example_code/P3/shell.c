/*
Luca D'angelo Sabin : l.dangelo@udc.es : 47436242L
Antón Fernández Cerecedo : anton.fcerecedo :
*/

#include "c_P0.h"
#include "c_P1.h"
#include "c_P2.h"
#include "c_P3.h"

//----------------------------------------------
void imprimirPrompt();
void leerEntrada(char* comando, char* c_troceado[], int* n_palabras);
void procesarEntrada(char* init_comando, char* c_troceado[], const char* comandos_validos[], bool* terminado,
  int n_palabras, char* historic[], t_memory_node* mem_list[], tList* proc_list, char *argv[]);
int validarComando(char* comando, const char* comandos_validos[]);
//----------------- Comandos de la terminal P0 ---------------------------------------------------------------
void getHistoric(char* comando, char* c_troceado[], char* argumento, char* historic[], const char* comandos_validos[],
bool* terminado, int n_palabras, t_memory_node* mem_list[], tList* proc_list, char *argv[]);
  //c_P0.h
//----------------- Comandos de la terminal P1 ---------------------------------------------------------------
  //c_P1.h
//------------------ Comandos de la terminal P2 --------------------------------------------------------------
void show_variables();
void show_funcs();
void shell_recurse(char* args[], int n_args);
void recurse (int n);
  //c_P2.h
//------------------ Comandos de la terminal P3 --------------------------------------------------------------
  //c_P3.h
//------------------------------------------------------------------------------------------------------------
int global1,global2,global3;

//--------------------- Main -----------------------
int main(int argc, char *argv[]) {
  const char* lista_comandos[] = COMANDOS;
  char* c_troceado[LONGITUD_MAX_COMANDOS];
  char* historic[HISTORIC_MAX] = {NULL};
  char comando[LONGITUD_MAX_COMANDOS];
  tList proc_list;
  t_memory_node* mem_list[MAX];
  createEmptyList(&proc_list);
  iniciarLista(mem_list);

  int n_palabras; //numero de palabras en el comando
  bool terminado = false;

  while (!terminado) {
    imprimirPrompt();
    leerEntrada(comando, c_troceado, &n_palabras);
    addHistCommand(c_troceado, historic, n_palabras);
    procesarEntrada(comando, c_troceado, lista_comandos, &terminado, n_palabras, historic, mem_list, &proc_list, argv);
  }

  clearList(historic);
  clearMemList(mem_list);
  deleteList(&proc_list, false, false);
  return 0;
}


void imprimirPrompt(){
  printf("> ");
}

void leerEntrada(char* cadena, char* c_troceado[], int* n_palabras){

    fgets(cadena, LONGITUD_MAX_COMANDOS, stdin);

    *n_palabras = TrocearCadena(cadena, c_troceado); // aqui se hace la llamada a trocear cadena
}

void procesarEntrada(char* init_comando, char* c_troceado[], const char* comandos_validos[],
  bool* terminado, int n_palabras, char* historic[], t_memory_node* mem_list[], tList* proc_list, char *argv[]) {

  char* comando = c_troceado[0];
  char* argumentos[LONGITUD_MAX_COMANDOS] = {NULL}; // establecida esta longitud para los casos en los que se pasan muchos argumentos
  char case_id;
  bool ampersand = false;


  if (n_palabras > 1){ //se copian las direcciones de los argumentos al array de argumentos.
    for (int j=1; j < n_palabras; j++) {
      argumentos[j-1] = c_troceado[j];
    }
  }

  if (comando != NULL) {
    case_id = validarComando(comando, comandos_validos); //verifica si es un comando de la shell

    if (case_id == NO_ID && n_palabras > 1 && c_troceado[n_palabras-1][0] == '&' && strlen(c_troceado[n_palabras-1]) < 3) { // command args... &
      ampersand = true;
      c_troceado[n_palabras-1] = NULL;
      n_palabras = n_palabras-1;
    }
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
      getHistoric(init_comando, c_troceado, argumentos[0], historic, comandos_validos, terminado, n_palabras, mem_list, proc_list, argv);
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

    case 15://memdump
      memdump(argumentos, n_palabras-1);
      break;

    case 16://memfill
      memfill(argumentos, n_palabras-1);
      break;

    case 17://readfile
      readfile(argumentos, n_palabras-1);
      break;

    case 18://writefile
      writefile(argumentos, n_palabras-1);
      break;

    case 19://recurse
      shell_recurse(argumentos, n_palabras-1);
      break;

    case 20://getpriority
      cmd_getpriority(argumentos, n_palabras-1);
      break;

    case 21://setpriority
      cmd_setpriority(argumentos, n_palabras-1);
      break;

    case 22://getuid
      Cmd_getuid(argumentos);
      break;

    case 23://setuid
      Cmd_setuid(argumentos);
      break;

    case 24://fork
      cmd_fork(argumentos, n_palabras-1, argv);
      break;

    case 25://execute
      cmd_execute(argumentos, n_palabras-1, NULL);
      break;

    case 26://foreground
      foreground(argumentos, n_palabras-1, NULL);
      break;

    case 27://background
      background(argumentos, n_palabras-1, proc_list, NULL);
      break;

    case 28://listprocs
      listprocs(argumentos, n_palabras-1, proc_list);
      break;

    case 29://run-as
      runAs(argumentos, n_palabras-1, proc_list);
      break;

    case 30://execute-as
      executeAs(argumentos, n_palabras-1);
      break;

    case 31://proc
      proc(argumentos, n_palabras-1, proc_list);
      break;

    case 32://deleteprocs
      deleteProcs(argumentos, n_palabras-1, proc_list);
      break;

    case SKIP://skip
      break;

    case NO_ID://command not found
      if (ampersand) background(c_troceado, n_palabras, proc_list, NULL);
      else foreground(c_troceado, n_palabras, NULL);
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
void getHistoric(char* comando, char* c_troceado[], char* argumento, char* historic[],
  const char* comandos_validos[], bool* terminado, int n_palabras, t_memory_node* mem_list[], tList* proc_list, char *argv[]) {
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

      procesarEntrada(comando, c_troceado, comandos_validos, terminado, n_palabras, historic, mem_list, proc_list, argv);
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
  char automatico[LONGITUD_MAX_COMANDOS];
  static char estatico[LONGITUD_MAX_COMANDOS];

  printf("Parámetro de n: %d en %p\n",n ,&n);
  printf("Static array en: %p\n", estatico);
  printf("Array automatico en: %p\n", automatico);
  puts("");
  n--;
  if (n>0) recurse(n);
}
