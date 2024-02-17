/*
Luca D'angelo Sabin : l.dangelo@udc.es : 47436242L
Antón Fernández Cerecedo : anton.fcerecedo :
*/
#include "types.h"
#include "f_dadas.h"
#include "mem_list.h"

void shell_memory(char* argumentos[], int n_args, t_memory_node* mem_list[]);
void allocate_memory(int n_args, char* argumentos[], t_memory_node* mem_list[]);
void deallocate_memory(int n_args, char* argumentos[], t_memory_node* mem_list[]);
void show_memory(int n_args, char* argumentos[], t_memory_node* mem_list[]);
void delete_shared_key(int n_args, char* argumentos[], t_memory_node* mem_list[]);
void memdump(char* args[], int n_args);
void memfill(char* args[], int n_args);
void readfile(char* args[], int n_args);
void writefile(char* args[], int n_args);
