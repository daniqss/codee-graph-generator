#include <unistd.h>
#include <time.h>
#include "lista.h"

#define LONGITUD_MAX_COMMANDOS 256 //longitud máxima de los comandos y argumentos. Puesto así en el caso de que haya algun fichero con nombre muy grande
#define LONGITUD_MAX_ARGUMENTOS 8 //longitud máxima de caracteres de los argumentos disponibles
#define N_COMANDOS 11 //cantidad de comandos
#define LONGITUD_MAX_DIR 256
#define HISTORIC_MAX 4096
#define NO_ID -1
#define GETPID_ID 1
#define NULL_ARG 'N'
#define SKIP 's'
#define ERROR 'x'

#define AUTHOR_1 "Luca D'angelo Sabin"
#define AUTHOR_2 "Antón Fernández Cerecedo"
#define LOGIN_1 "l.dangelo"
#define LOGIN_2 "anton.fcerecedo"

int TrocearCadena(char* cadena, char* trozos[]);


void imprimirPrompt();
void leerEntrada(char* comando, char* c_troceado[], int* n_palabras);
void procesarEntrada(char* c_troceado[], const char* comandos_validos[], bool* terminado, int n_palabras, char* historic[]);
int validarComando(char* comando, const char* comandos_validos[]);
bool validarArgumentos(char* argumento, const char* argumentos_validos[]);
//----------------- comandos de la terminal----------------------------------------------------------------
void printAuthors(char* argumento);
void consoleGetpid();
void consoleGetppid();
void pwd();
void changeDir(char* argumento);
void getDate();
void getTime();
void getHistoric(char* argumento, char* historic[], const char* comandos_validos[], bool* terminado, int n_palabras);
void exit_console(bool* terminado, char* historic[]);

int main() {
  const char* lista_comandos[] = {"authors","getpid","getppid","pwd","chdir","date","time","historic","quit","exit","end"};
  char* c_troceado[LONGITUD_MAX_COMMANDOS];
  char* historic[HISTORIC_MAX] = {NULL};
  char comando[LONGITUD_MAX_COMMANDOS];

  int n_palabras; //numero de palabras en el comando
  bool terminado = false;

  while (!terminado) {
    imprimirPrompt();
    leerEntrada(comando, c_troceado, &n_palabras);
    addHistCommand(c_troceado, historic, n_palabras);
    procesarEntrada(c_troceado, lista_comandos, &terminado, n_palabras, historic);
  }
  return 0;
}


void imprimirPrompt(){
  printf("> ");
}

void leerEntrada(char* cadena, char* c_troceado[], int* n_palabras){

    fgets(cadena, LONGITUD_MAX_COMMANDOS, stdin);

    *n_palabras = TrocearCadena(cadena, c_troceado); // aqui se hace la llamada a trocear cadena
}

void procesarEntrada(char* c_troceado[], const char* comandos_validos[], bool* terminado, int n_palabras, char* historic[]) {
  char* comando = c_troceado[0];
  char* argumento;
  char case_letter;



  if (n_palabras > 1) argumento = c_troceado[1];
  else argumento = NULL; //si no fue pasado ningun argumento, valdrá NULL

  if (comando != NULL) {
    int id_comando = validarComando(comando, comandos_validos);

    if (id_comando != NO_ID) {
      case_letter = comando[0];

    } else case_letter = ERROR;

  } else {
    case_letter = SKIP;
  }

  switch (case_letter) {
    case 'a'://authors
      printAuthors(argumento);
      break;

    case 'g'://getpid or getppid
      if (strcmp(comando, comandos_validos[GETPID_ID]) == 0) {
        consoleGetpid();
      } else consoleGetppid();
      break;

    case 'p'://pwd
      pwd();
      break;

    case 'c'://chdir
      changeDir(argumento);
      break;

    case 'd'://date
      getDate();
      break;

    case 't'://time
      getTime();
      break;

    case 'h'://historic
      getHistoric(argumento, historic, comandos_validos, terminado, n_palabras);
      break;

    case 'q'://quit
      exit_console(terminado, historic);
      break;

    case 'e'://exit or end
      exit_console(terminado, historic);
      break;

    case SKIP://skip
      break;

    case ERROR://command not found
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

bool validarArgumentos(char* argumento, const char* argumentos_validos[]) {

  if (strlen(argumento) < 2) return false; //si el argumento tiene un tamaño menor que 2, no es válido.

  for (int i=0; argumentos_validos[i]!=NULL ; ++i) {  /*Si fue pasado con argumentos recorre la lista de argumentos
                                                      del comando para comprobar si es valido el argumento*/
    if (strcmp(argumento, argumentos_validos[i]) == 0) return true;
  }

  return false;
}

//---------------------------------------------------
void exit_console(bool* terminado, char* historic[]) {
  clearList(historic);
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
  // int pid = getppid();
  // printf("PID del padre de la shell: %d\n", pid);
}

void printAuthors(char* argumento) {
  const char* argumentos_validos[] = {"-l", "-n", NULL};
  char arg_case_letter;

  if (argumento == NULL) arg_case_letter = NULL_ARG;
  else if (validarArgumentos(argumento, argumentos_validos)) arg_case_letter = argumento[1];
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

void getHistoric(char* argumento, char* historic[], const char* comandos_validos[], bool* terminado, int n_palabras) {
  const char* argumentos_validos[] = {"-c", "-", "-r", NULL};
  char* comando_a_procesar = (char*) malloc(sizeof(char)*LONGITUD_MAX_COMMANDOS);
  *comando_a_procesar = '\0';
  char* comando_procesado[LONGITUD_MAX_COMMANDOS];
  char arg_case_letter;
  char aux[LONGITUD_MAX_ARGUMENTOS];

  if (argumento == NULL) arg_case_letter = NULL_ARG;
  else if (argumento[0] == '-' && (argumento[1] > '0' && argumento[1] <= '9')) arg_case_letter = argumento[0]; //para el caso de que el argumento sea -N
  else {
    strncpy(aux, argumento, 2); //Para el caso de que el argumento sea -rN
    aux[2] = '\0';

    if (validarArgumentos(aux, argumentos_validos)) arg_case_letter = argumento[1]; // para argumentos -rN y -c
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
      char* aux_string = getCommand(aux, historic);

      if (getCommand(aux, historic) != NULL) {
        strcpy(comando_a_procesar, aux_string);
      }

      int k = strcmp(comando_a_procesar, "historic -r");  //variable de comapracion
      TrocearCadena(comando_a_procesar, comando_procesado);
      if (k>=1) comando_procesado[1] = NULL; //Evitar un bucle infinito con historic -rN


      if (comando_procesado[0] == NULL) printf("No existe el elemento %d en el historico\n", atoi(aux));
      else printf("Ejecutando historic (%d): %s\n",atoi(aux), aux_string);

      procesarEntrada(comando_procesado, comandos_validos, terminado, n_palabras, historic);
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
  free(comando_a_procesar);
}
//---------------------------------------------------
int TrocearCadena(char* cadena, char* trozos[]){
  int i=1;

  if((trozos[0] = strtok(cadena, " \n\t")) == NULL)
    return 0;

  while((trozos[i] = strtok(NULL, " \n\t")) != NULL)
    i++;
  return i;
}
