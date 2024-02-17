#include "types.h"

char LetraTF (mode_t m);
char * ConvierteModo (mode_t m, char *permisos);
int TrocearCadena(char* cadena, char* trozos[]);

void *ObtenerMemoriaShmget (key_t clave, size_t tam, int *size);
void *Cmd_AlocateCreateShared (char *arg[], int *size);
void * MmapFichero (char * fichero, int protection, int *fd);
void *Cmd_AllocateMmap (char *arg[], int *fd);
ssize_t LeerFichero (char *fich, void *p, ssize_t n);
void Cmd_deletekey (char *args[]);
void Cmd_dopmap (char *args[]);
