/*
Luca D'angelo Sabin : l.dangelo@udc.es
Antón Fernández Cerecedo : anton.fcerecedo
*/

#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <libgen.h>
#include "historic.h"

#define LONGITUD_MAX_COMMANDOS 256 //longitud máxima de los comandos y argumentos. Puesto así en el caso de que haya algun fichero con nombre muy grande
#define LONGITUD_MAX_ARGUMENTOS 8 //longitud máxima de caracteres de los argumentos disponibles
#define N_COMANDOS 14 //cantidad de comandos
#define LONGITUD_MAX_DIR 256
#define HISTORIC_MAX 4096
#define MAX_DIRS 128

#define NO_ID -1
#define NULL_ARG 'N'
#define SKIP 'S'

#define AUTHOR_1 "Luca D'angelo Sabin"
#define AUTHOR_2 "Antón Fernández Cerecedo"
#define LOGIN_1 "l.dangelo"
#define LOGIN_2 "anton.fcerecedo"
//------------- Funciones Dadas ----------------
int TrocearCadena(char* cadena, char* trozos[]);
char * ConvierteModo (mode_t m, char *permisos);
//----------------------------------------------
void imprimirPrompt();
void leerEntrada(char* comando, char* c_troceado[], int* n_palabras);
void procesarEntrada(char* init_comando, char* c_troceado[], const char* comandos_validos[], bool* terminado, int n_palabras, char* historic[]);
int validarComando(char* comando, const char* comandos_validos[]);
int validarArgumentos(char* argumento, const char* argumentos_validos[]);
//----------------- Comandos de la terminal P0 ----------------------------------------------------------------
void printAuthors(char* argumento);
void consoleGetpid();
void consoleGetppid();
void pwd();
void changeDir(char* argumento);
void getDate();
void getTime();
void getHistoric(char* comando, char* c_troceado[], char* argumento, char* historic[], const char* comandos_validos[], bool* terminado, int n_palabras);
void exit_console(bool* terminado, char* historic[]);
//----------------- Comandos de la terminal P1 ----------------------------------------------------------------
void shell_create(char* argumentos[], int n_palabras);
void shell_delete(char* argumentos[], int n_palabras);
void shell_list(char* argumentos[], int n_palabras);
void delete_file(char* path, bool rec);
void printFileInfo(char* path, char* d_name, bool hidden, bool long_format);
void listDir(char* path, bool hidden, bool long_format, bool show_dir, bool recursive);

//--------------------- Main -----------------------
int main() {
  const char* lista_comandos[] = {"authors","getpid","getppid","pwd","chdir","date","time","historic","quit","exit","end","create","delete","list"};
  char* c_troceado[LONGITUD_MAX_COMMANDOS];
  char* historic[HISTORIC_MAX] = {NULL};
  char comando[LONGITUD_MAX_COMMANDOS];

  int n_palabras; //numero de palabras en el comando
  bool terminado = false;

  while (!terminado) {
    imprimirPrompt();
    leerEntrada(comando, c_troceado, &n_palabras);
    addHistCommand(c_troceado, historic, n_palabras);
    procesarEntrada(comando, c_troceado, lista_comandos, &terminado, n_palabras, historic);
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

void procesarEntrada(char* init_comando, char* c_troceado[], const char* comandos_validos[], bool* terminado, int n_palabras, char* historic[]) {
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
      getHistoric(init_comando, c_troceado, argumentos[0], historic, comandos_validos, terminado, n_palabras);
      break;

    case 8://quit
      exit_console(terminado, historic);
      break;

    case 9://exit
      exit_console(terminado, historic);
      break;

    case 10://end
      exit_console(terminado, historic);
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

int validarArgumentos(char* argumento, const char* argumentos_validos[]) {

  if (strlen(argumento) < 1) return false;

  for (int i=0; argumentos_validos[i]!=NULL ; ++i) {  /*Si fue pasado con argumentos recorre la lista de argumentos
                                                       del comando para comprobar si es valido el argumento*/
    if (strcmp(argumento, argumentos_validos[i]) == 0) return i;
  }

  return -1;
}

//--------------------- Comandos P0 ------------------------------
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

void getHistoric(char* comando, char* c_troceado[], char* argumento, char* historic[], const char* comandos_validos[], bool* terminado, int n_palabras) {
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

      procesarEntrada(comando, c_troceado, comandos_validos, terminado, n_palabras, historic);
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
void printFileInfo(char* path, char* d_name, bool hidden, bool long_format) {
  struct stat fs;
  struct passwd *own;
  struct group *grp;
  struct tm *tm;
  char permisos[12];
  char w_days[7][4] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
  char m_days[12][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

  if (stat(path, &fs) < 0) return;
//datelastmodified inodenumber owner group mode drwx format size (number of links) name
  if (d_name [0] != '.' || hidden) {
    if (!long_format) printf("%9ld   %s\n",fs.st_size, d_name);
    else {
      own = getpwuid(fs.st_uid);
      grp = getgrgid(fs.st_gid);
      tm = localtime(&fs.st_mtime); //Mode and dt pendientes.
      printf("%s %s %d %d %7ld %10s %10s %9s %9ld (%3lu) %s\n", w_days[tm->tm_wday],
        m_days[tm->tm_mon], tm->tm_mday, tm->tm_year+1900, fs.st_ino, own->pw_name,
        grp->gr_name, ConvierteModo(fs.st_mode, permisos), fs.st_size, fs.st_nlink, d_name);
    }
  }
}

void listDir(char* path, bool hidden, bool long_format, bool show_dir, bool recursive) {
  struct dirent *dir, *directories[MAX_DIRS];
  struct stat is_dir;
  char full_path[LONGITUD_MAX_DIR];
  DIR *d;
  int dir_counter = 0, aux = 0;

  if (stat(path, &is_dir) < 0) { //Confirma si existe el archivo
    printf("Error al acceder a %s: %s\n", path, strerror(errno));
    return;
  }

  if (S_ISDIR(is_dir.st_mode)) {//si es un directorio lo abre
    d = opendir(path);

    if (!d) { //Checkea si hay errores de permisos.
      printf("Error al acceder a %s: %s\n", path, strerror(errno));
      return;
    }

    //
    if (!show_dir) printFileInfo(path, path, true, long_format); //si no se pidió listar los contenidos
    else { //si se pidió listar los contenidos
      printf("--------- %s\n", path); //carpeta en la que se encuentra en el momento
      while ((dir = readdir(d)) != NULL) {
        strcpy(full_path, path); //almacenamos en una variable el path entero para el directorio que tenemos que mostrar los contenidos, empezando por el path del directorio raiz
        strcat(full_path, "/"); //luego se añade una barra separadora
        strcat(full_path, dir->d_name); //y por ultimo el nombre del directorio al que queremos acceder
        stat(full_path, &is_dir);

        printFileInfo(full_path, dir->d_name, hidden, long_format);

        if (S_ISDIR(is_dir.st_mode) //si es un directorio
        && recursive //si se pidio recursividad
        && !(strcmp(".", dir->d_name)==0 || strcmp("..", dir->d_name)==0) //si no es "." ni ".."
        && dir_counter < MAX_DIRS) { //si no se alcanzó el máximo de directorios listables
          directories[dir_counter] = dir; //se almacena el puntero del directorio para una llamada recursiva futura
          dir_counter++;
        }
      }//bucle para listar ficheros

      while (aux < dir_counter) { //listar todos los subdirectorios del directorio
        dir = directories[aux];
        strcpy(full_path, path);
        strcat(full_path, "/");
        strcat(full_path, dir->d_name);
        listDir(full_path, hidden, long_format, show_dir, recursive);
        aux++;
      }
    closedir(d);
    }
  } else printFileInfo(path, path, true, long_format); //si se pasa como argumento un archivo oculto se imprime.
}

void shell_list(char* argumentos[], int n_palabras) {
  const char* argumentos_validos[] = {"-hid","-long","-dir","-rec", NULL};
  bool args_seleccionados[4] = {false, false, false, false};
  enum Args {hid, longlist, dir, rec}; //enum para facilitar los index de args_seleccionados
  int i=0, arg_id, path_n;
  char path[LONGITUD_MAX_DIR];

  while(i < n_palabras-1 && (arg_id = validarArgumentos(argumentos[i], argumentos_validos)) != NO_ID) { //Se recorre la lista de argumentos válidos pasados
    args_seleccionados[arg_id] = true; //en un array de "argumentos seleccionados" se guarda un true en el mismo índice del argumento seleccionado
    i++;//la i acaba en la posición en la que empiezan las rutas o se acaba el comando
  }
  // if (args_seleccionados[rec] == true) { //si se pide recursvidiad tambien lista directorios y ocultos, según el comportamiento de la shell dada de ejemplo.
  //   args_seleccionados[dir] = true;
  // }
  path_n = n_palabras - i - 1; //numero de archivos/directorios pasados como argumento.
  if (path_n == 0) strcpy(path, "."), args_seleccionados[dir] = true; //si es 0 se establece el directorio como  el directorio actual y -dir en true

  do {
    if (path_n > 0) {
      strcpy(path, argumentos[i]), path_n--, i++;//se copia a la variable path el argumento, y se actualizan las variables de control

      listDir(path, args_seleccionados[hid],
        args_seleccionados[longlist], args_seleccionados[dir], args_seleccionados[rec]);

    } else listDir(path, args_seleccionados[hid],
      args_seleccionados[longlist], args_seleccionados[dir], args_seleccionados[rec]);
  } while(path_n > 0);

}

void delete_file(char* path, bool rec) {
  struct dirent *dir;
  struct stat is_dir;
  char full_path[LONGITUD_MAX_DIR];
  DIR *d;

  if (stat(path, &is_dir) < 0) { //si el fichero o directorio no existe
    remove(path); //obtención de error
    perror("Imposible Borrar ");
    return;
  }

  if (S_ISDIR(is_dir.st_mode)) {
    if (remove(path) != 0) { //si el directorio está lleno
      if (!rec) perror("Imposible Borrar "); //si falla
      else { //si es un directorio y se pide recursividad
        d = opendir(path);

        if (!d) { //Checkea si hay errores de permisos.
          printf("Error al borrar %s: %s\n", path, strerror(errno));
          return;
        }

        while ((dir = readdir(d)) != NULL) {
          strcpy(full_path, path); //almacenamos en una variable el path entero para el directorio que tenemos que mostrar los contenidos, empezando por el path del directorio raiz
          strcat(full_path, "/"); //luego se añade una barra separadora
          strcat(full_path, dir->d_name); //y por ultimo el nombre del directorio al que queremos acceder
          stat(full_path, &is_dir);
          if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) //si el directorio no es "." o ".."
            delete_file(full_path, rec);
        } //bucle para borrar ficheros dentro del directorio
        closedir(d);
      }
      remove(path); //borra el directorio vacío
    }
  } else remove(path);
}

void shell_delete(char* argumentos[], int n_palabras){
  const char* argumentos_validos[] = {"-rec", NULL};
  bool rec = false;
  int path_n, i;
  char path[LONGITUD_MAX_DIR];

  if (n_palabras > 1 && (validarArgumentos(argumentos[0], argumentos_validos) != NO_ID)) { //Se mira si se pasó el argumento -rec
    rec = true; //si es cierto se actualiza una variable bool que marca la recursividad
    i = 1; //los archivos comienzan en la segunda posicion
  } else i = 0; //de lo contrario comienzan en la primera posicion
  //rec al ser un bool: false = 0 y true = 1
  path_n = n_palabras - rec - 1; //numero de archivos/directorios pasados como argumento.

  do {
    if (path_n > 0) {
      strcpy(path, argumentos[i]), path_n--, i++; //se copia a la variable path el argumento, y se actualizan las variables de control
      delete_file(path, rec);
    } else listDir(".", true, false, true, false); //si no se pasan argumentos se lista el directorio actual
  } while(path_n > 0);
}

void shell_create(char* argumentos[], int n_palabras){
  const char* argumentos_validos[] = {"-dir", NULL};
  FILE* fp;
  struct stat st;
  bool dir = false;

  //si el coando es "create" o "create -dir" se ejecuta listDir.
  if (n_palabras == 1 || (n_palabras == 2 && (validarArgumentos(argumentos[0], argumentos_validos) != NO_ID))) {
    listDir(".", true, false, true, false); //si no se pasan argumentos se lista el directorio actual
    return;
  }

  if (n_palabras > 2 && (validarArgumentos(argumentos[0], argumentos_validos) != NO_ID)) { //Se mira si se pasó el argumento -dir
    dir = true; //si es cierto se actualiza una variable bool que marca crear un directorio
  }

  if (stat(argumentos[0+dir], &st) == 0) { //Si ya existe el archivo o carpeta
    puts("Imposible crear: File exists"); //tira un error si son del mismo tipo
    return;
  }

  if (!dir) { //si es un archivo
    if((fp = fopen(argumentos[0+dir],"w")) == NULL) perror("Imposible crear ");
    else fclose(fp);
  }
  else //si es un directorio
    if (mkdir(argumentos[0+dir], 0777) < 0) perror("Imposible crear ");
  // puts("Imposible crear: File exists")

}
//---------------------------- Funciones dadas ---------------------------------
char LetraTF (mode_t m){
  switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
    case S_IFSOCK:
      return 's'; /*socket */
    case S_IFLNK:
      return 'l';
    /*symbolic link*/
    case S_IFREG:
      return '-';
    /* fichero normal*/
    case S_IFBLK:
      return 'b';
    /*block device*/
    case S_IFDIR:
      return 'd';
    /*directorio */
    case S_IFCHR:
      return 'c';
    /*char device*/
    case S_IFIFO:
      return 'p';
    /*pipe*/
    default:
      return '?';
    /*desconocido, no deberia aparecer*/
  }
}

char * ConvierteModo (mode_t m, char *permisos){
  strcpy (permisos,"---------- ");
  permisos[0]=LetraTF(m);
  if (m&S_IRUSR) permisos[1]='r';/*propietario*/
  if (m&S_IWUSR) permisos[2]='w';
  if (m&S_IXUSR) permisos[3]='x';
  if (m&S_IRGRP) permisos[4]='r';/*grupo*/
  if (m&S_IWGRP) permisos[5]='w';
  if (m&S_IXGRP) permisos[6]='x';
  if (m&S_IROTH) permisos[7]='r';/*resto*/
  if (m&S_IWOTH) permisos[8]='w';
  if (m&S_IXOTH) permisos[9]='x';
  if (m&S_ISUID) permisos[3]='s';/*setuid, setgid y stickybit*/
  if (m&S_ISGID) permisos[6]='s';
  if (m&S_ISVTX) permisos[9]='t';

  return permisos;
}

int TrocearCadena(char* cadena, char* trozos[]){
  int i=1;

  if((trozos[0] = strtok(cadena, " \n\t")) == NULL)
    return 0;

  while((trozos[i] = strtok(NULL, " \n\t")) != NULL)
    i++;
  return i;
}
