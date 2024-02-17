/*
Luca D'angelo Sabin : l.dangelo@udc.es : 47436242L
Antón Fernández Cerecedo : anton.fcerecedo :
*/

#include "proc_list.h"

struct SEN {
  char *nombre;
  int senal;
};

static struct SEN sigstrnum[]={
  {"HUP", SIGHUP},
  {"INT", SIGINT},
  {"QUIT", SIGQUIT},
  {"ILL", SIGILL},
  {"TRAP", SIGTRAP},
  {"ABRT", SIGABRT},
  {"IOT", SIGIOT},
  {"BUS", SIGBUS},
  {"FPE", SIGFPE},
  {"KILL", SIGKILL},
  {"USR1", SIGUSR1},
  {"SEGV", SIGSEGV},
  {"USR2", SIGUSR2},
  {"PIPE", SIGPIPE},
  {"ALRM", SIGALRM},
  {"TERM", SIGTERM},
  {"CHLD", SIGCHLD},
  {"CONT", SIGCONT},
  {"STOP", SIGSTOP},
  {"TSTP", SIGTSTP},
  {"TTIN", SIGTTIN},
  {"TTOU", SIGTTOU},
  {"URG", SIGURG},
  {"XCPU", SIGXCPU},
  {"XFSZ", SIGXFSZ},
  {"VTALRM", SIGVTALRM},
  {"PROF", SIGPROF},
  {"WINCH", SIGWINCH},
  {"IO", SIGIO},
  {"SYS", SIGSYS},
  /*senales que no hay en todas partes*/
  #ifdef SIGPOLL
  {"POLL", SIGPOLL},
  #endif
  #ifdef SIGPWR
  {"PWR", SIGPWR},
  #endif
  #ifdef SIGEMT
  {"EMT", SIGEMT},
  #endif
  #ifdef SIGINFO
  {"INFO", SIGINFO},
  #endif
  #ifdef SIGSTKFLT
  {"STKFLT", SIGSTKFLT},
  #endif
  #ifdef SIGCLD
  {"CLD", SIGCLD},
  #endif
  #ifdef SIGLOST
  {"LOST", SIGLOST},
  #endif
  #ifdef SIGCANCEL
  {"CANCEL", SIGCANCEL},
  #endif
  #ifdef SIGTHAW
  {"THAW", SIGTHAW},
  #endif
  #ifdef SIGFREEZE
  {"FREEZE", SIGFREEZE},
  #endif
  #ifdef SIGLWP
  {"LWP", SIGLWP},
  #endif
  #ifdef SIGWAITING
  {"WAITING", SIGWAITING},
  #endif
  {NULL,-1}};
/*fin array sigstrnum */



bool createNode(tPosL *p) {
    *p=malloc(sizeof(struct tNode));
    return *p!=NULL;
}

void createEmptyList(tList *L) {
    *L=NULL;
}

tItemL createItem(pid_t pid, int prio, char command[], struct tm *e_time,
  int proc_state, int termination) {

  tItemL item;

  item.pid = pid;
  item.prio = prio;
  item.proc_state = proc_state;
  item.timestamp = malloc(sizeof(*e_time));
  item.command = malloc(sizeof(char)*(strlen(command)+1));

  item.termination = termination;
  strcpy(item.command, command);
  *item.timestamp = *e_time;

  return item;
}

bool insertItem(tItemL d, tPosL p, tList *L) {

    tPosL q,r;

    if (!createNode(&q)) {
        return false;
    } else {

        q->itemL = d;
        q->next=NULL;

        if (*L == NULL)    // Si la lista está vacía.
            *L = q;
        else if (p==NULL) {    // Insertar en la última posicion.
            for (r=*L; r->next != NULL; r=r->next); // recorre los punteros de next hasta encontrar un nulo.
            r->next = q;
        } else if (p == *L) {       // insertar en la primera posición.
            q->next = *L;
            *L = q;
        } else {        // Insertar en cualquier otra posicion.
            q->itemL = p->itemL;
            p->itemL = d;
            q->next = p->next;
            p->next = q;
        }   // end if.
        return true;
    }
}   // end function

void updateList(tList *L) {
  tPosL p = *L;
  int state;

  while (p != NULL) {
    if (p->itemL.prio != -1) {
      p->itemL.changed = waitpid(p->itemL.pid, &state, WUNTRACED | WCONTINUED | WNOHANG);
      p->itemL.proc_state = state;
    }
    get_state(&p->itemL);
    p = p->next;
  }
}

bool isEmpty(tList L) {
    if (L==NULL) return true;
    else return false;
}

tPosL first(tList L) {
    return L;
}

tPosL last(tList L) {
    tPosL p;
    for (p=L; p->next!=NULL; p=p->next);
    return p;
}

tPosL next_pos(tPosL p, tList L) {
    return p->next;
}

tPosL previous(tPosL p, tList L) {
    tPosL q;
    if (p == L) return NULL;
    else {
        for (q = L; q->next != p; q = q->next);
        return q;
    }   // end if.
}

tPosL findItem(pid_t pid, tList L) {
    tPosL p;
    for (p = L; p != NULL && p->itemL.pid != pid; p = p->next);
    return p;
}

void deleteItem(pid_t pid, tList *L) {
    tPosL q, p;

    for (p = *L; p != NULL && p->itemL.pid != pid; p = p->next);
    if (p == NULL) return; //no se encontró el pid

    if (p == *L) {  // Cabeza.
        *L = (*L)->next;
    } else if(p->next == NULL) {   // Final de la lista.
        for (q=*L; q->next != p; q=q->next);
        q->next = p->next;
    } else {    // Cuerpo de la lista.
        q = p->next;
        p->itemL = q->itemL;
        p->next = q->next;
        p = q;
    } // end if.
    free(p->itemL.command);
    free(p->itemL.timestamp);
    free(p);
};

void deleteList(tList *L, bool term, bool sign) {
  int pids[MAX] = {0}, i=0;
  tPosL p;

  if (!term && !sign) { //borrar lsita entera
    while (*L != NULL) {
        p = *L;
        *L = (*L)->next;
        free(p->itemL.command);
        free(p->itemL.timestamp);
        free(p);
    }
    return;
  }

  for (p=*L; p != NULL; p = p->next) { //guarda los pids de los procesos a borrar
    if (p->itemL.prio == -1) {
      if (term && !WIFSIGNALED(p->itemL.termination)){
        pids[i] = p->itemL.pid;
        i++;
      }
      else if (sign && WIFSIGNALED(p->itemL.termination)) {
        pids[i] = p->itemL.pid;
        i++;
      }
    }
  }
  for (int j=0; pids[j] != 0; j++) deleteItem(pids[j], L); //borrado de todos los pids copiados a la lista
}

void print_proc_list(tList *list, tPosL p){
  tPosL pos;
  char* time_string;

  updateList(list);

  for (pos = *list; pos != NULL; pos = pos->next) {
    if (p == NULL || p == pos) {
      time_string = asctime(pos->itemL.timestamp);
      time_string[strlen(time_string)-1] = '\0';

      printf("%6d p=%d %s %s (%03d) %s\n",
      pos->itemL.pid, pos->itemL.prio,
      time_string, pos->itemL.str_state,
      pos->itemL.termination, pos->itemL.command);
    }
  }
}

/****************************** SENALES **********************************/

void get_state(tItemL *item) {
  if (item->changed > 0) {
    if (WIFEXITED(item->proc_state)) {

        strcpy(item->str_state, "TERMINADO");
        item->termination = WEXITSTATUS(item->proc_state);
        item->prio = -1;

     } else if (WIFSTOPPED(item->proc_state)) {

        strcpy(item->str_state, "PARADO");
        item->termination = WSTOPSIG(item->proc_state);

     } else if (WIFCONTINUED(item->proc_state)) {

        item->termination = 0;
        strcpy(item->str_state, "ACTIVO");

     } else if (WIFSIGNALED(item->proc_state)) {

        strcpy(item->str_state, "SENALADO");
        item->termination = WTERMSIG(item->proc_state);
        item->prio = -1;

     } else strcpy(item->str_state, "UNKNOWN");

  } else if (item->changed < 0) {

    strcpy(item->str_state, "TERMINADO");
    item->termination = 0;
    item->prio = -1;

  } else if (item->termination == 0) {
    strcpy(item->str_state, "ACTIVO");
  }
}

int Senal(char * sen) { /*devuelve el numero de senial a partir del nombre*/
  int i;

  for (i=0; sigstrnum[i].nombre!=NULL; i++)
    if (!strcmp(sen, sigstrnum[i].nombre))
      return sigstrnum[i].senal;
  return -1;
}

char *NombreSenal(int sen) { /*devuelve el nombre senal a partir de la senal*/
  /* para sitios donde no hay sig2str*/
  int i;

  for (i=0; sigstrnum[i].nombre!=NULL; i++)
    if (sen==sigstrnum[i].senal)
      return sigstrnum[i].nombre;
  return ("SIGUNKNOWN");
}
