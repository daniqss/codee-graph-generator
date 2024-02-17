#include "f_dadas.h"

#define LEERCOMPLETO ((ssize_t)-1)

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

/************************************************************************/
/************************************************************************/

void *ObtenerMemoriaShmget (key_t clave, size_t tam, int *size) {
  void * p;
  int aux,id,flags=0777;
  struct shmid_ds s;

  if (tam) /*si tam no es 0 la crea en modo exclusivo */
    flags=flags | IPC_CREAT | IPC_EXCL;
  /*si tam es 0 intenta acceder a una ya creada*/
  if (clave==IPC_PRIVATE) {
    errno=EINVAL;
    printf("Error no: %d", errno);
    return NULL;
  }
  /*no nos vale*/
  if ((id=shmget(clave, tam, flags))==-1) return (NULL);
  if ((p=shmat(id,NULL,0))==(void*) -1){
    aux=errno;
    /*si se ha creado y no se puede mapear*/
    if (tam)
    /*se borra */
      shmctl(id,IPC_RMID,NULL);
    errno=aux;
    printf("Error no2: %d", errno);
    return (NULL);
  }
  shmctl (id,IPC_STAT,&s);
  *size = s.shm_segsz;
  /* Guardar En Direcciones de Memoria Shared (p, s.shm_segsz, clave.....);*/
  return (p);
}

void *Cmd_AlocateCreateShared (char *arg[], int *size) /*arg[0] is the key
and arg[1] is the size*/{
  key_t k;
  size_t tam=0;
  void *p;
  if (arg[0]==NULL || arg[1]==NULL) {
    /*Listar Direcciones de Memoria Shared */
    puts("adhaidh");
    return NULL;}
  k=(key_t) atoi(arg[0]);

  if (arg[1]!=NULL)
    tam=(size_t) atoll(arg[1]);

  if ((p=ObtenerMemoriaShmget(k,tam,size))==NULL)
    perror ("Imposible obtener memoria shmget");
  else
    printf ("Memoria de shmget de clave %d asignada en %p\n",k,p);
  return p;
}

/************************************************************************/
/************************************************************************/

void * MmapFichero (char * fichero, int protection, int *fd) {

int df, map=MAP_PRIVATE,modo=O_RDONLY;struct stat s;
void *p;

if (protection&PROT_WRITE)
  modo=O_RDWR;
if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
  return NULL;
if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
  return NULL;
*fd = df;
return p;
}

void *Cmd_AllocateMmap (char *arg[], int *fd) /*arg[0] is the file name
and arg[1] is the permissions*/{
  printf("-> %s\n",arg[0]);
  char *perm;
  void *p;
  int protection=0;

  if (arg[0]==NULL) {/*Listar Direcciones de Memoria mmap;*/ return NULL;}
  if ((perm=arg[1])!=NULL && strlen(perm)<4) {
    if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
    if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
    if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
  }

  if ((p=MmapFichero(arg[0],protection, fd))==NULL)
    perror ("Imposible mapear fichero");
  else
    printf ("fichero %s mapeado en %p\n", arg[0], p);
  return p;
}

ssize_t LeerFichero (char *fich, void *p, ssize_t n) {
/*n=-1 indica que se lea todo*/
  ssize_t nleidos,tam=n;
  int df, aux;struct stat s;

  if (stat (fich,&s)==-1 || (df=open(fich,O_RDONLY))==-1)
    return ((ssize_t)-1);
  if (n==LEERCOMPLETO)
    tam=(ssize_t) s.st_size;
  if ((nleidos=read(df,p, tam))==-1){
    aux=errno;
    close(df);
    errno=aux;
    return ((ssize_t)-1);
  }
  close (df);
  return (nleidos);
}

/*********************************************************************/
/*********************************************************************/

void Cmd_deletekey (char *args[]) /*arg[0] points to a str containing the key*/{
  key_t clave;
  int id;
  char *key=args[0];

  if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
    printf ("rmkey clave_valida\n");
    return;
  }
  if ((id=shmget(clave,0,0666))==-1){
    perror ("shmget: imposible obtener memoria compartida");
    return;
  }
  if (shmctl(id,IPC_RMID,NULL)==-1)
    perror ("shmctl: imposible eliminar memoria compartida\n");
}

void Cmd_dopmap (char *args[]) /*no arguments necessary*/{
  pid_t pid;
  char elpid[32];
  char *argv[3]={"pmap",elpid,NULL};

  sprintf (elpid,"%d", (int) getpid());

  if ((pid=fork())==-1){
    perror ("Imposible crear proceso");
    return;
  }
  if (pid==0){
    if (execvp(argv[0],argv)==-1)
      perror("cannot execute pmap");
    exit(1);
  }
  waitpid (pid,NULL,0);
}

//---------------------------------------- P3 ----------------------------------------
char * NombreUsuario (uid_t uid){
  struct passwd *p;if ((p=getpwuid(uid))==NULL)
  if ((p=getpwuid(uid))==NULL)
    return (" ??????");
  return p->pw_name;
}


uid_t UidUsuario (char * nombre){
  struct passwd *p;
  if ((p=getpwnam (nombre))==NULL)
    return (uid_t) -1;
  return p->pw_uid;
}

void Cmd_getuid (char *tr[]){
uid_t real=getuid(), efec=geteuid();
printf ("Credencial real: %d, (%s)\n", real, NombreUsuario (real));
printf ("Credencial efectiva: %d, (%s)\n", efec, NombreUsuario (efec));
}

int Cmd_setuid (char *tr[]){
  uid_t uid;
  int u;

  if (tr[0]==NULL || (!strcmp(tr[0],"-l") && tr[1]==NULL)){
    Cmd_getuid(tr);
    return 0;
  }

  if (!strcmp(tr[0],"-l")){
    if ((uid=UidUsuario(tr[1]))==(uid_t) -1){
      printf ("Usuario no existente %s\n", tr[1]);
      return -1;
    }
  }

  else if ((uid=(uid_t) ((u=atoi (tr[0]))<0)? -1: u) ==(uid_t) -1){
    printf ("Valor no valido de la credencial %s\n",tr[0]);
    return -1;
  }

  if (setuid (uid)==-1) {
    printf ("Imposible cambiar credencial: %s\n", strerror(errno));
    return -1;
  }
  return 0;
}
