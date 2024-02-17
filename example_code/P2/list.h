




#include "f_dadas.h"

typedef struct {
  int fd;
  char name[MAX_LONGITUD]; //limitando el tamaño del struct.
} t_fich; //TIPO MMAP

typedef struct {
  int key;
} t_shared; //TIPO SHARED

typedef union {
  t_shared shared;
  t_fich fich; //sin embargo implicaría un mayor consumo de memoria innecesaria.
} t_varying_alloc; //CAMPO DE MEMORIA VARIABLE

typedef struct {
  void *address;
  int size;
  struct tm *timestamp;
} t_mem_field; //CAMPO DE MEMORIA EN COMUN

typedef enum {
  t_malloc,
  fichero,
  shared
} alloc_type; //CAMPO DE TIPO DE ALLOC

typedef struct {
  t_mem_field mem_field; //campos en comun de memoria
  alloc_type type; //tipo de alloc
  t_varying_alloc alloc_info; //informacion específica del allocacion
} t_memory_node; //STRUCT DE LA LISTA

void deleteElement(t_memory_node* list[], int index);
bool addElement(t_memory_node* list[], alloc_type type,
  t_varying_alloc alloc_type_info,
  void *address,
  int size,
  struct tm *e_time);
void iniciarLista(t_memory_node* list[]);
void printList(t_memory_node* list[], alloc_type type);
int searchMalloc(t_memory_node *list[], int size);
int searchMmap(t_memory_node *list[], char* fichero);
int searchKey(t_memory_node *list[], int key);
void freeAddress(t_memory_node *list[], char addr[]);
void clearMemList(t_memory_node* list[]);
