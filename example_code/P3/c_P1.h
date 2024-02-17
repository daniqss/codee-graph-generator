/*
Luca D'angelo Sabin : l.dangelo@udc.es : 47436242L
Antón Fernández Cerecedo : anton.fcerecedo :
*/
#include "types.h"
#include "f_dadas.h"

void shell_create(char* argumentos[], int n_palabras);
void shell_delete(char* argumentos[], int n_palabras);
void shell_list(char* argumentos[], int n_palabras);
void printFileInfo(char* path, char* d_name, bool hidden, bool long_format);
void listDir(char* path, bool hidden, bool long_format, bool show_dir, bool recursive);
void delete_file(char* path, bool rec);
