#include "c_P2.h"

void allocate_memory(int n_args, char* argumentos[], t_memory_node* mem_list[]){
  const char* argumentos_validos[] = {"-malloc", "-mmap", "-shared", "-createshared", NULL};
  int arg_id, size;
  time_t rawtime = time (&rawtime);
  struct tm *e_time;
  t_varying_alloc aux;
  struct stat st;
  void *p;

  if ((arg_id = validarArgumentos(argumentos[1], argumentos_validos)) != NO_ID) {
    if (n_args > 3) {
      switch (arg_id) {
        case 0: //malloc
          if (isnumber(argumentos[2])) {
            p = malloc(atoi(argumentos[2]));
            size = atoi(argumentos[2]);
            if (p != NULL) printf("Asignados %d bytes en %p\n", size, p);
            else printf("Fallo en la asignación\n");
          }
          break;

        case 1: //mmap
          if (n_args > 3 && (p = Cmd_AllocateMmap(argumentos+2, &aux.fich.fd)) != NULL) {
            stat(argumentos[2], &st);
            size = st.st_size;
            strcpy(aux.fich.name, argumentos[2]);
          }
          break;

        case 2: //shared
          p = ObtenerMemoriaShmget(atoi(argumentos[2]), 0, &size);
          aux.shared.key = atoi(argumentos[2]);
          if (p != NULL) printf("Memoria compartida de clave %d en %p\n", aux.shared.key, p);
          else
            printf("Imposible asignar memoria compartida clave %d: %s\n", aux.shared.key, strerror(errno));
          break;

        case 3: //createshared
          if (n_args > 4 && isnumber(argumentos[3])) {
            p = Cmd_AlocateCreateShared(argumentos+2, &size);
            aux.shared.key = atoi(argumentos[2]);
          }
          break;
      }
      e_time = localtime (&rawtime);
      if (n_args > 4 || arg_id != 3)
        addElement(mem_list, (arg_id > 2) ? 2:arg_id /*tipo correspondiente a un enum, escrito en int*/ , aux, p, size, e_time);
    }
    else printList(mem_list, (arg_id > 2) ? 2:arg_id);
  } else puts("uso: allocate [-malloc|-shared|-createshared|-mmap] ....");
}

void deallocate_memory(int n_args, char* argumentos[], t_memory_node* mem_list[]){
  const char* argumentos_validos[] = {"-malloc", "-mmap", "-shared", NULL};
  int arg_id, key, index;
  char str_key[MAX_DIRS];

  if (n_args > 2 && ((arg_id = validarArgumentos(argumentos[1], argumentos_validos)) != NO_ID || (argumentos[1][0] == '0' && argumentos[1][1] == 'x'))) {
    if (n_args > 3) {
      switch(arg_id) {
        case 0:
          if (isnumber(argumentos[2])) {
            key = atoi(argumentos[2]);
            index = searchMalloc(mem_list, key);
          }
          break;

        case 1:
          strcpy(str_key, argumentos[2]);
          index = searchMmap(mem_list, str_key);
          break;

        case 2:
          if (isnumber(argumentos[2])) {
            key = atoi(argumentos[2]);
            index = searchKey(mem_list, key);
          }
          break;
      }
      deleteElement(mem_list, index);
    } else { //addr
      freeAddress(mem_list, argumentos[1]);
    }
  }
}

void show_memory(int n_args, char* argumentos[], t_memory_node* mem_list[]){
  const char* argumentos_validos[] = {"-malloc", "-mmap", "-shared", "-all", NULL};
  int arg_id = arg_id = validarArgumentos(argumentos[1], argumentos_validos);

  if (arg_id != NO_ID) {
    if (arg_id == 3) arg_id = NO_ID; //NO_ID = -1
    printList(mem_list, arg_id);
  } else printf("Opción %s no reconocida.\n", argumentos[1]);

}

void delete_shared_key(int n_args, char* argumentos[], t_memory_node* mem_list[]){
  Cmd_deletekey(argumentos+1);
}


void memdump(char* args[], int n_args) {
  int n = 25, i=0, j=0;
  char *p;

  if (n_args < 1) return; //no se dió dirección
  if (n_args > 1) n = atoi(args[1]);

  p = (char*) strtol(args[0], NULL, 16);

  for (i = 0; i < n; i=i+25){
    for (j = 0; j < 25 && i+j < n; ++j){
        printf(" %02x", p[i+j]);
    }
    printf("\n");
    for (j = 0; j < 25 && i+j < n; ++j){
        if (p[i+j] == '\n') printf(" \\n");
        else printf(" %2c", p[i+j]);
    }
    printf("\n");
  }

}

void memfill(char* args[], int n_args) {
  int n = 128;
  char *p, byte = 'A';

  if (n_args < 1) return; //no se dió dirección
  if (n_args < 3) {
    if (n_args > 1 && !isnumber(args[1])) byte = strtol(args[1], NULL, 16);
    else if (n_args > 1) n = atoi(args[1]);
  } else {
    n = atoi(args[1]);
    byte = strtol(args[2], NULL, 16);
  }

  p = (char*) strtol(args[0], NULL, 16);

  memset(p, byte, n);
}

void readfile(char* args[], int n_args) {
  ssize_t size;
  struct stat st;
  void* p;
  char addr[16], size_dump[16];
  char* args_memdump[2];

  if (n_args < 2) return;
  if (stat(args[0], &st) < 0) {
    perror("Archivo no encontrado");
    return;
  }
  p = (void*) strtol(args[1], NULL, 16);

  if (n_args > 2 && isnumber(args[2])) size = atoi(args[2]);
  else size = -1;

  size = LeerFichero(args[0], p, size); //read
  printf("%d bytes leídos de %s en %p\n",
    (int) size, args[0], p);

  strcpy(addr, args[1]); //se parsean los argumentos en una varaible auxiliar para llamar a memdump
  args_memdump[0] = addr;
  sprintf(size_dump, "%d", (int)size);
  args_memdump[1] = size_dump;
  printf("addr: %s size: %d\n", args_memdump[0], atoi(args_memdump[1]));
  memdump(args_memdump, 2); //print the read bytes
}

void writefile(char* args[], int n_args) {
  bool overwrite = false;
  struct stat st;
  size_t size;
  int fd;
  void* p;

  if (n_args < 3) return;
  if (n_args > 3 && strcmp(args[0], "-o")==0) overwrite = true;
  else if (n_args > 3) return;

  size = atoi(args[2+overwrite]); //overwrite argument offset
  p = (void*) strtol(args[1+overwrite], NULL, 16);

  if (stat(args[0+overwrite], &st) < 0) {
    fd = open(args[0+overwrite], O_RDWR | O_CREAT, 0777);
  } else {
    if (overwrite)
      fd = open(args[0+overwrite], O_RDWR);
    else fd = -1;
  }
  if (fd != -1)
    write(fd, p, size);
  else puts("Archivo ya existe, para realizar una sobreescitura llame writefile -o ...\n");
  close(fd);
}
