





#include "list.h"

// FUNCIONES DE LISTA
void iniciarLista(t_memory_node* list[]) {
  list[0] = NULL;
}

bool addElement(t_memory_node* list[],
  alloc_type type,
  t_varying_alloc alloc_type_info,
  void *address,
  int size,
  struct tm *e_time){

    int i = 0;

    for (i=0; list[i] != NULL; ++i); //llegar a la ultima posicion de la lista
    if (i == MAX-1) return false; //si la lista está llena return false
    else if (address == NULL) return false;

    t_memory_node* aux = (t_memory_node*) malloc(sizeof(t_memory_node));

    aux->type = type;
    aux->mem_field.address = address;
    aux->mem_field.size = size;
    aux->mem_field.timestamp = e_time;
    aux->alloc_info = alloc_type_info;


    list[i] = aux;
    list[i+1] = NULL; //avanzar la posicion de NULL

    return true;
}

void deleteElement(t_memory_node* list[], int index){
  int i;
  t_memory_node *field;

  if (index == -1) return;

  field = list[index];

  if (field->type == t_malloc)
    free(list[index]->mem_field.address); //libramos la memoria del puntero guardado
  else if (field->type == shared) shmctl(field->alloc_info.shared.key, IPC_RMID, NULL); //se desmapea la memoria compartida con la clave
  else munmap(field->mem_field.address, field->mem_field.size);

  free(list[index]);

  for (i=index; list[i] != NULL; i++) {
    if (list[i+1] != NULL) list[i] = list[i+1];
    else list[i] = NULL;
  }
}

void printList(t_memory_node* list[], alloc_type type){
  t_memory_node *field;
  char* time_string;

  for (int i = 0; (field = list[i]) != NULL; i++) {
    time_string = asctime(field->mem_field.timestamp);
    time_string[strlen(time_string)-1] = '\0';

    if (type == -1 /*-all */ || field->type == type){

        printf("    %p %16d %s", field->mem_field.address, field->mem_field.size, time_string);
      if (field->type == shared)
        printf(" shared (key %d)\n", field->alloc_info.shared.key);
      else if (field->type == fichero)
        printf(" %s (descriptor %d)\n",field->alloc_info.fich.name, field->alloc_info.fich.fd);
      else printf(" malloc\n");
    }
  }
}

int searchMalloc(t_memory_node *list[], int size) { //busca una reserva con malloc del tamaño introducido
  for (int i = 0; list[i] != NULL; i++) {
    if (list[i]->type == t_malloc && list[i]->mem_field.size == size) {
      return i;
    }
  }
  puts("Ningún bloque de memoria de esa cantidad reservado con malloc.");
  return -1;
}

int searchMmap(t_memory_node *list[], char* fich) { //busca un fichero con el nombre introducido que esté mapeado
  for (int i = 0; list[i] != NULL; i++) {
    if (list[i]->type == fichero && strcmp(list[i]->alloc_info.fich.name, fich) == 0) {
      return i;
    }
  }
  printf("Ningún fichero con ese nombre %s mapeado.\n", fich);
  return -1;
}

int searchKey(t_memory_node *list[], int key) { //busca la clave que tenga memoria mapeada
  for (int i = 0; list[i] != NULL; i++) {
    if (list[i]->type == shared && list[i]->alloc_info.shared.key == key) {
      return i;
    }
  }
  puts("Ningún bloque de memoria compartida con esa clave mapeada en el proceso.");
  return -1;
}

void freeAddress(t_memory_node *list[], char addr[]){
  void* pointer = (void*) strtol(addr, NULL, 16);

  for (int i = 0; list[i] != NULL; i++) {
    if (list[i]->mem_field.address == pointer) {
      deleteElement(list, i);
      return;
    }
  }
  printf("Dirección %s no asignada con malloc, shared o mmap.\n", addr);
}

void clearMemList(t_memory_node* list[]) {
  int i;

  for (i = 0; list[i] != NULL; i++) free(list[i]);
}
