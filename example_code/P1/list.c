/*
Luca D'angelo Sabin : l.dangelo@udc.es
Antón Fernández Cerecedo : anton.fcerecedo
*/

#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pwd.h>
#include <grp.h>
#include <libgen.h>

#define LONGITUD_MAX_DIR 256
#define NO_ID -1
#define MAX_DIRS 128

char * ConvierteModo (mode_t m, char *permisos);
char LetraTF (mode_t m);
void listDir(char* path, bool hidden, bool long_format, bool show_dir, bool recursive);
void printFileInfo(char* path, char* d_name, bool hidden, bool long_format);
int validarArgumentos(char* argumento, const char* argumentos_validos[]);

int main (int argc, char *argv[]) {

  const char* argumentos_validos[] = {"-hid","-long","-dir","-rec", NULL};
  bool args_selec[4] = {false, false, false, false};
  enum Args {hid, longlist, dir, rec}; //enum para facilitar los index de args_seleccionados
  int i=1, arg_id, path_n; //el argv[0] es el propio ejecutable, por lo que comienza en la posicion 1
  char path[LONGITUD_MAX_DIR];

  while(i < argc && (arg_id = validarArgumentos(argv[i], argumentos_validos)) != NO_ID) { //Se recorre la lista de argumentos válidos pasados
    args_selec[arg_id] = true; //en un array de "argumentos seleccionados" se guarda un true en el mismo índice del argumento seleccionado
    i++;//la i acaba en la posición en la que empiezan las rutas o se acaba el comando
  }

  path_n = argc - i; //numero de archivos/directorios pasados como argumento.

  if (path_n == 0) strcpy(path, "."), args_selec[dir] = true; //si es 0 se establece el directorio como  el directorio actual y -dir en true

  do {
    if (path_n > 0) {
      strcpy(path, argv[i]), path_n--, i++;//se copia a la variable path el argumento, y se actualizan las variables de control

      listDir(path, args_selec[hid],
        args_selec[longlist], args_selec[dir], args_selec[rec]);

    } else listDir(path, args_selec[hid],
      args_selec[longlist], args_selec[dir], args_selec[rec]);
  } while(path_n > 0);

  return 0;
}

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

int validarArgumentos(char* argumento, const char* argumentos_validos[]) {

  if (strlen(argumento) < 1) return false;
  for (int i=0; argumentos_validos[i]!=NULL ; ++i) {  /*Si fue pasado con argumentos recorre la lista de argumentos
                                                     del comando para comprobar si es valido el argumento*/
    if (strcmp(argumento, argumentos_validos[i]) == 0) return i;
  }
  return -1;
}

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
