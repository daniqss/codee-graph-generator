/*
Luca D'angelo Sabin : l.dangelo@udc.es : 47436242L
Antón Fernández Cerecedo : anton.fcerecedo :
*/

//linked list with head node
#include "f_dadas.h"
#define MAX_TERM_LENGTH 12

typedef struct tNode* tPosL;

typedef struct tItemL {
  pid_t pid;  //pid
  int prio; //priority
  char* command;  //command executes
  struct tm *timestamp; //timestamp
  int proc_state; //process state
  int termination; //termination status
  int changed; //if changed with waitpid
  char str_state[12]; //previous status stored as string
} tItemL;

struct tNode {
    tItemL itemL;
    tPosL next;
};
typedef tPosL tList; //head node

bool createNode(tPosL *p);
void createEmptyList(tList *L);
tItemL createItem(pid_t pid, int prio, char command[], struct tm *e_time, int proc_state, int termination);
bool insertItem(tItemL d, tPosL p, tList *L);
bool isEmpty(tList L);
void print_proc_list(tList* list, tPosL p);
tPosL first(tList L);
tPosL last(tList L);
tPosL next_pos(tPosL p, tList L);
tPosL previous(tPosL p, tList L);
tPosL findItem(pid_t pid, tList L);
void updateList(tList *L);
void deleteItem(pid_t pid, tList *L);
void deleteList(tList *L, bool term, bool sign);
void get_state(tItemL *item);
int Senal(char * sen);/*devuelve el numero de senial a partir del nombre*/
char *NombreSenal(int sen); /*devuelve el nombre senal a partir de la senal*/
