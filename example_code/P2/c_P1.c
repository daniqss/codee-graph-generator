#include "c_P1.h"

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
      tm = localtime(&fs.st_mtime);
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


//---------------------------- shell commands -------------------------------
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

  //si el comando es "create" o "create -dir" se ejecuta listDir.
  if (n_palabras == 1 || (n_palabras == 2 && (validarArgumentos(argumentos[0], argumentos_validos) != NO_ID))) {
    listDir(".", true, false, true, false); //si no se pasan argumentos se lista el directorio actual
    return;
  }

  if (n_palabras > 2 && (validarArgumentos(argumentos[0], argumentos_validos) != NO_ID)) { //Se mira si se pasó el argumento -dir
    dir = true; //si es cierto se actualiza una variable bool que marca crear un directorio
  }

  if (stat(argumentos[0+dir], &st) == 0) { //Si ya existe el archivo o carpeta con el mismo nombre
    puts("Imposible crear: File exists"); //tira un error
    return;
  }

  if (!dir) { //si no es un archivo
    if((fp = fopen(argumentos[0+dir],"w")) == NULL) perror("Imposible crear ");
    else fclose(fp);
  }
  else //si es un directorio
    if (mkdir(argumentos[0+dir], 0777) < 0) perror("Imposible crear ");
}
