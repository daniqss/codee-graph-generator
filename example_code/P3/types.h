/*
Luca D'angelo Sabin : l.dangelo@udc.es : 47436242L
Antón Fernández Cerecedo : anton.fcerecedo :
*/

#define AUTHOR_1 "Luca D'angelo Sabin"
#define AUTHOR_2 "Antón Fernández Cerecedo"
#define LOGIN_1 "l.dangelo"
#define LOGIN_2 "anton.fcerecedo"

#define LONGITUD_MAX_DIR 256
#define MAX_DIRS 128
#define LONGITUD_MAX_COMANDOS 256 //longitud máxima de los comandos y argumentos. Puesto así en el caso de que haya algun fichero con nombre muy grande
#define LONGITUD_MAX_ARGUMENTOS 8 //longitud máxima de caracteres de los argumentos disponibles
#define COMANDOS {"authors","getpid","getppid","pwd",\
"chdir","date","time","historic","quit","exit","end","create","delete",\
"list", "memory", "memdump", "memfill", "readfile", "writefile", "recurse",\
"getpriority", "setpriority", "getuid", "setuid", "fork", "execute", "foreground",\
"background", "listprocs", "run-as", "execute-as", "proc", "deleteprocs"}
#define N_COMANDOS 33 //cantidad de comandos
#define LONGITUD_MAX_DIR 256
#define HISTORIC_MAX 4096
//ctes para list.c
#define MAX 4096
#define MAX_LONGITUD 200

#define NO_ID -1
#define NULL_ARG 'N'
#define SKIP 'S'

#include "util_funcs.h"
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <stddef.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <aio.h>
#include <sys/resource.h>
#include <signal.h>
