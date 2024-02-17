/*
Luca D'angelo Sabin : l.dangelo@udc.es : 47436242L
Antón Fernández Cerecedo : anton.fcerecedo :
*/
#include "c_P3.h"

//----------------------------- UID commands ---------------------------------

void cmd_getpriority(char* argumentos[], int n_palabras) {
  int pid, prio;

  if (!n_palabras) pid = getpid();
  else if (isnumber(argumentos[0])) pid = atoi(argumentos[0]);

  prio = getpriority(PRIO_PROCESS, pid);

  if (prio >= 0)
    printf("Prioridad de proceso %i: %i\n", pid, prio);
  else printf("Imposible obtener prioridad de proceso %i: %s\n", pid, strerror(errno));
}

void cmd_setpriority(char* argumentos[], int n_palabras) {
  int pid, prio, aux;

  if (!n_palabras) {
    cmd_getpriority(argumentos, n_palabras);
    return;
  }
  else if (isnumber(argumentos[0]) && n_palabras == 1) {
    prio = atoi(argumentos[0]);
    pid = getpid();
  }
  else if (n_palabras > 1 && isnumber(argumentos[0]) && isnumber(argumentos[1])) {
    pid = atoi(argumentos[0]);
    prio = atoi(argumentos[1]);
  }

  aux = setpriority(PRIO_PROCESS, pid, prio);

  if (aux == -1)
    printf("Imposible obtener prioridad de proceso %i: %s\n", pid, strerror(errno));
  else printf("Priority set\n");
}

//------------------------ fork, exec, background, foreground ---------------------------

void cmd_fork(char* argumentos[], int n_palabras, char *argv[]) {
  char* command[] = {argv[0], NULL};

  int pid;

  if ((pid = fork()) == 0) {
    execv(command[0], command);
  } else printf("Ejecutando proceso %i\n", pid);
  waitpid(pid, NULL, WUNTRACED);
}

void cmd_execute(char* argumentos[], int n_palabras, char* user) {
  char* aux[] = {"0", NULL};
  bool set_prio = false;
  int aux_prio;
  char* setuid[] = {"-l", user, NULL};

  if (user != NULL)
    if (Cmd_setuid(setuid) == -1) {
      printf("\tEjecutable debe ser setuid (rwsr-xr-x)\nNo ejecutado: %s\n", strerror(errno));
      return;
    }

  if (n_palabras >= 1){


    //priority check
    if (n_palabras >= 1 && (aux_prio = check_prio(argumentos, n_palabras)) > 0) {
      set_prio = true;
      aux[0] = argumentos[aux_prio]+1;
      argumentos[aux_prio] = NULL;
    }

    if (set_prio) cmd_setpriority(aux, 1);
    execvp(argumentos[0], argumentos);
  }
}

void foreground (char* argumentos[], int n_palabras, char* user) {
  int pid;
  bool set_prio=false;

  if (n_palabras >= 1){
    char* aux[] = {"0", NULL};
    char* setuid[] = {"-l", user, NULL};
    int aux_prio;

    //priority check
    if (n_palabras >= 1 && (aux_prio = check_prio(argumentos, n_palabras)) > 0) {
      set_prio = true;
      aux[0] = argumentos[aux_prio]+1;
      argumentos[aux_prio] = NULL;
    }

    if ((pid = fork()) == 0) {
      if (user != NULL)
        if (Cmd_setuid(setuid) == -1) {
          printf("\tEjecutable debe ser setuid (rwsr-xr-x)\nNo ejecutado: %s\n", strerror(errno));
          exit(0);
        }
      if (set_prio) cmd_setpriority(aux, 1);
      if (execvp(argumentos[0], argumentos) < 0)
        perror("No ejecutado");
      exit(0);
    }
    waitpid(pid, NULL, WUNTRACED);
  }
}

void listprocs (char* argumentos[], int n_palabras, tList* proc_list) {
  print_proc_list(proc_list, NULL);
}

void background (char* argumentos[], int n_palabras, tList* proc_list, char* user) {
  time_t rawtime = time (&rawtime);
  struct tm *e_time;
  bool set_prio = false, ampersand = false;
  int pid, aux_prio, state;
  char comando[LONGITUD_MAX_COMANDOS];
  char* prio = "0";
  char* aux[] = {prio, NULL};
  char* setuid[] = {"-l", user, NULL};

  //ampersand check
  if (argumentos[n_palabras-1][0] == '&' && strlen(argumentos[n_palabras-1]) < 3) {
    ampersand = true;
    argumentos[n_palabras-1] = NULL;
  }

  //priority check
  if (n_palabras >= 1 && (aux_prio = check_prio(argumentos, n_palabras-ampersand)) > 0) {
    set_prio = true;
    aux[0] = (prio = argumentos[aux_prio]+1);
    argumentos[aux_prio] = NULL;
  }


  if ((pid = fork()) == 0) {
    if (user != NULL)
      if (Cmd_setuid(setuid) == -1) {
        printf("\tEjecutable debe ser setuid (rwsr-xr-x)\nNo ejecutado: %s\n", strerror(EINVAL));
        exit(0);
      }

    if (set_prio) cmd_setpriority(aux, 1);
    if (execvp(argumentos[0], argumentos) == -1) perror("Cannot execute");
    exit(0);
  }

  combine(argumentos, n_palabras-set_prio-ampersand, comando);
  e_time = localtime (&rawtime);

  waitpid(pid, &state, WUNTRACED | WCONTINUED | WNOHANG);

  tItemL item = createItem(pid, atoi(prio), comando, e_time, state, 0);

  insertItem(item, NULL, proc_list);
}

void runAs (char* argumentos[], int n_palabras, tList* proc_list) {
  bool ampersand = false;

  if (n_palabras < 1) return;
  if (argumentos[n_palabras-1][0] == '&' && strlen(argumentos[n_palabras-1]) < 3) { // run-as command args... &
    ampersand = true;
  }

  if (ampersand) background(argumentos+1, n_palabras-1, proc_list, argumentos[0]);
  else foreground(argumentos+1, n_palabras-1, argumentos[0]);
}

void executeAs (char* argumentos[], int n_palabras) {
  if (n_palabras < 1) return;

  cmd_execute(argumentos+1, n_palabras-1, argumentos[0]);
}

void proc(char* argumentos[], int n_palabras, tList *proc_list) {
  const char* args_validos[] = {"-fg", NULL};
  int fg, status, pid;

  updateList(proc_list);

  if (n_palabras < 1) return;
  else if ((fg = validarArgumentos(argumentos[0], args_validos)) != NO_ID && n_palabras < 2) return;
    //findItem devuelve NULL si no encuentra pid
  if (fg == NO_ID) print_proc_list(proc_list, findItem(atoi(argumentos[0]), *proc_list));
  else {
    tPosL p = findItem(atoi(argumentos[1]), *proc_list);

    if (p == NULL || (p->itemL.prio == -1 && (pid = atoi(argumentos[1])))) {
      printf("Proceso de pid %d ya está finalizado\n", pid);
    } else {
      if (waitpid(p->itemL.pid, &status, WCONTINUED | WUNTRACED) == -1) perror("Error en proceso");
      else {
        if (WIFEXITED(status)) {
             printf("Process %d exited, status %d\n", pid, WEXITSTATUS(status));
         } else if (WIFSIGNALED(status)) {
             printf("Process %d killed by signal %d\n", pid, WTERMSIG(status));
         }
      }
      deleteItem(p->itemL.pid, proc_list);
      }
  }
}

void deleteProcs(char* argumentos[], int n_palabras, tList* proc_list) {
  const char* args_validos[] = {"-term", "-sig", NULL};
  bool args[] = {false, false};
  int arg_id;

  updateList(proc_list);

  if (n_palabras < 1) return;
  for (int i = 0; i<n_palabras; i++) { //opcion de eleccion de multiples argumentos...
    arg_id = validarArgumentos(argumentos[i], args_validos);
    if (arg_id != NO_ID) args[arg_id] = true;
  }

  if (args[0] || args[1]) deleteList(proc_list, args[0], args[1]); //opcion de borrar toda la lista si se quita la condicion
}
