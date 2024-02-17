/*
Luca D'angelo Sabin : l.dangelo@udc.es
Antón Fernández Cerecedo : anton.fcerecedo
*/

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pwd.h>
#include <grp.h>
#include <libgen.h>
#include <time.h>
#include <dirent.h>

#define LONGITUD_MAX_DIR 256
#define MAX_DIRS 128

void delete_file(char* path, bool rec);
char * ConvierteModo (mode_t m, char *permisos);
char LetraTF (mode_t m);
void listDir(char* path, bool hidden, bool long_format, bool show_dir, bool recursive);
void printFileInfo(char* path, char* d_name, bool hidden, bool long_format);

int main (int argc, char *argv[]) {
  int i=1, path_n; //el argv[0] es el propio ejecutable, por lo que comienza en la posicion 1
  bool rec;
  char path[LONGITUD_MAX_DIR];

  if (argc-1 > 1 && (strcmp(argv[1],"-rec") == 0)) { //Se mira si se pasó el argumento -rec
    rec = true; //si es cierto se actualiza una variable bool que marca la recursividad
    i++; //los archivos comienzan en la segunda posicion
  }

  path_n = argc - i; //numero de archivos/directorios pasados como argumento.

  do {
    if (path_n > 0) {
      strcpy(path, argv[i]), path_n--, i++;//se copia a la variable path el argumento, y se actualizan las variables de control

      delete_file(path, rec);
    } else listDir(".", true, false, true, false); //si no se pasan argumentos se lista el directorio actual
  } while(path_n > 0);

  return 0;
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

  if (remove(path) != 0 && !rec) perror("Imposible Borrar "); //si falla
  else if (S_ISDIR(is_dir.st_mode)) { //si es un directorio y se pide recursividad
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
    remove(path); //borrar directorio
    closedir(d);
  }
}

//--------------------------- List --------------------------

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
