/*
 * TITLE: Proyecto 1 de PRO2
 * SUBTITLE: Práctica 1
 * AUTHOR 1: Luca D'angelo Sabin LOGIN 1: l.dangelo
 * AUTHOR 2: Martin Vietes García LOGIN 2: martin.vieites
 * GROUP: 1.1
 * DATE: 25 / 02 / 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define CODE_LENGTH 2
#define CABECERA "********************\n"
#define VOTE 1

#ifdef STATIC_LIST
#include "static_list.h"
#endif
#ifdef DYNAMIC_LIST
#include "dynamic_list.h"
#endif
#ifdef TEST_LIST
#include "list/list.h"
#endif

void showHeader (char command_number[CODE_LENGTH+1], char command, char param[NAME_LENGTH_LIMIT+1]);
void createNewParty (tList* L, char param[NAME_LENGTH_LIMIT+1]);
void voteParty (tList* L, char param[NAME_LENGTH_LIMIT+1], tNumVotes* nullVotes);
void illegalizeParty(tList* L, char param[NAME_LENGTH_LIMIT+1], tNumVotes* nullVotes);
void showStats (tList L, char param[NAME_LENGTH_LIMIT+1], tNumVotes nullVotes);
float countPartyVotes (tList L);


void processCommand(char command_number[CODE_LENGTH+1], char command, char param[NAME_LENGTH_LIMIT+1], tList* L, tNumVotes* nullVotes) {

    switch(command) {

        case 'N': { // New

            showHeader(command_number, command, param);
            createNewParty(L, param);

        }
        break;

        case 'V': { // Vote

            showHeader(command_number, command, param);
            voteParty(L, param, nullVotes);

        }
        break;

        case 'I': { // Illegalize

            showHeader(command_number, command, param);
            illegalizeParty(L, param, nullVotes);   // Se pasa por referencia la variable nullVotes.

        }
        break;

        case 'S': { // Stats

            showHeader(command_number, command, param);
            showStats(*L, param, *nullVotes);    // Aqui se pasa el valor de nullVotes, porque la variable es un puntero (tipo int*).

        }
        break;

        default: {
            break;
        }
    }
}

void readTasks(char *filename) {
    tList L;                // Se crea la variable lista.
    tNumVotes NullVotes;
    NullVotes = 0;          // Se crea la variable en donde se vana guardar los votos nulos y se inicializa a 0.

    createEmptyList(&L);    // Se crea la lista vacía antes de leer los ficheros de input y se pasa el parámetro por referencia.
    FILE *df;

    char command_number[CODE_LENGTH+1], command, param[NAME_LENGTH_LIMIT+1];

    df = fopen(filename, "r");
    if (df != NULL) {
        while (!feof(df)) {
            char format[16];
            sprintf(format, "%%%is %%c %%%is", CODE_LENGTH, NAME_LENGTH_LIMIT);
            fscanf(df,format, command_number, &command, param);
            processCommand(command_number, command, param, &L, &NullVotes); // Al llamar la funcion que va a leer el input y ejecutar las funciones se pasa
                                                                            // tanto la lista como la variable donde se guardan los votos nulos por referencia.
        }
        fclose(df);
    } else {
        printf("Cannot open file %s.\n", filename);
    }
}

void showHeader(char command_number[CODE_LENGTH+1], char command, char param[NAME_LENGTH_LIMIT+1]){    // imprime

    printf(CABECERA);
    printf("%s %c: %s %s\n", command_number, command, (command == 'S')?"totalvoters":"party", param);
}

void createNewParty(tList* L, char param[NAME_LENGTH_LIMIT+1]) {

    tItemL item;
    strcpy(item.partyName, param);
    item.numVotes = 0;

    tPosL pos = findItem(param,*L); // <--- Se obtiene la posicion del elemento en la lista con su nombre. Devuelve LNULL si el elemento no está en la lista.

    if (pos == LNULL) {             // <--- Si el elemento no existe la funcion findItem() devuelve LNULL.
        insertItem(item, LNULL, L); // <--- Por lo tanto, se crea un nuevo partido con el nombre dado al final de la lista pasando el parámetro LNULL.
        printf("* New: party %s\n",param);
    } else {                           // <--- En el caso de que ya exista se devuelve un error.
        printf("+ Error: New not possible\n");
    }
}
//Precondicion: Que el partido ya no exista.

void voteParty(tList* L, char param[NAME_LENGTH_LIMIT+1], tNumVotes* nullVotes){

    tPosL pos = findItem(param,*L);         // <--- Se obtiene la posicion del elemento en la lista con su nombre. Devuelve LNULL si el elemento no está en la lista.

    if (pos != LNULL) {                         // <--- Condicion de partido existente.

        tItemL item = getItem(pos, *L);         // <--- Se obtiene el item de la lista en la posicion deseada.

        updateVotes(VOTE, pos, L);              // <--- Se añade 1 voto al partido.
        printf("* Vote: party %s numvotes %i\n",param, item.numVotes+VOTE); // <--- Como el partido fue actualizado despues de haber conseguido su informacion es necesario sumarle el voto al mostrarlo por pantalla.

    } else {                                    // <--- Si el partido no existe se devuelve un error y el voto es nulo.
        printf("+ Error: Vote not possible. Party %s not found. NULLVOTE\n", param);

        *nullVotes += VOTE;        // <--- Su voto se guarda en la variable nullvotes.
    }
}
//Precondicion: Que el partido exista.

void illegalizeParty(tList* L, char param[NAME_LENGTH_LIMIT+1], tNumVotes* nullVotes){

    tPosL pos = findItem(param,*L);             // <--- Se obtiene la posicion del elemento en la lista con su nombre. Devuelve LNULL si el elemento no está en la lista.

    if (pos != LNULL) {                         // <--- Condicion de partido existente.

        tItemL item = getItem(pos, *L);
        *nullVotes += item.numVotes;

        printf("* Illegalize: party %s\n",param);
        deleteAtPosition(pos, L);               // El partido es ilegalizado, y se borra de la lista.

    } else {                                    // <--- Si el partido no existe se devuelve un error.
        printf("+ Error: Illegalize not possible\n");
    }
}
//Precondicion: Que el partido a ilegalizar exista.

float countPartyVotes(tList L) {

    float totalPartyVotes=0;

    for (tPosL i = first(L); (i != LNULL); i = next(i,L)) { // Recorre la lista de partidos y le suma los votos a la variable total.

        tItemL item = getItem(i, L);
        totalPartyVotes = totalPartyVotes + item.numVotes;

    }   // end for.

    if (totalPartyVotes==0) return 1; // Para evitar una division entre 0, la funcion devuelve 1 en el caso de que no existan votos.
    else return totalPartyVotes;      // Calcula el número total de votos efectuados entre todos los partidos
}


void showStats(tList L, char param[NAME_LENGTH_LIMIT+1], tNumVotes nullVotes) {

    float totalVotes = 0;     // Esa variable servirá para luego calcular la participacion. Se guardan todos los votos, incluidos nulos.

    if (!isEmptyList(L)) {

        float totalPartyVotes = countPartyVotes(L); // En esa variable se guardan los votos totales a partidos.

        for (tPosL p = first(L); (p != LNULL); p = next(p,L)) {         // <--- El bucle empieza en el primer elemento de la lista first().

            tItemL item = getItem(p,L);           // El bucle va recorriendo toda la lista, mostrando la informacion de cada partido,
            totalVotes = totalVotes +item.numVotes;          // En la variable total se van almacenando los votos totatel de los partidos para calcular la participacion.

            printf("Party %s numvotes %i (%.2f%%)\n", item.partyName, item.numVotes, 100*item.numVotes/totalPartyVotes);    // La funcion calculatePercentage esta en el main.c y calcula el porcentage de votos del partido pasado como parámetro.

        }   // end for.
    }
    printf("Null votes %i\n",nullVotes);            // Con esa informacion se mostra por pantalla los votos nulos.

    totalVotes = totalVotes + nullVotes;  // Al total se le suman los votos nulos al final.
    int voters = atoi(param);   // Con la funcion atoi de stdlib.h se transforma el parametro que era un string (char[]) a un int, y se le asigna a una variable float, aunque podría seguir siendo int sin interferir en la division.

    printf("Participation: %i votes from %i voters (%.2f%%)\n", (int) totalVotes, voters, 100*totalVotes/(float)voters);  // Se convierte explicitamente los tipos de datos de float a int para el printf, y de int a float para la division.
}

int main(int nargs, char **args) {

    char *file_name = "new.txt";

    if (nargs > 1) {
        file_name = args[1];
    } else {
        #ifdef INPUT_FILE
        file_name = INPUT_FILE;
        #endif
    }

    readTasks(file_name);

    return 0;
}
