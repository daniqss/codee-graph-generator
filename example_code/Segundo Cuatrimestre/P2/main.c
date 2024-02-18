/*
 * TITLE: Proyecto 2 de PRO2
 * SUBTITLE: Práctica 2
 * AUTHOR 1: Luca D'angelo Sabin LOGIN 1: l.dangelo
 * AUTHOR 2: Martin Vietes García LOGIN 2: martin.vieites
 * GROUP: 1.1
 * DATE: 07 / 05 / 2020
 */

#include  <stdio.h>
#include "types.h"
#include "center_list.h"

#define CABECERA "********************\n"
#define VOTE 1
#define CODE_LENGTH 2


void showHeader(char commandNumber[CODE_LENGTH+1], char command, char param1[NAME_LENGTH_LIMIT+1], char param2[NAME_LENGTH_LIMIT+1]);
void createNewCenter(tListC *L, char param1[], char param2[]);
void createNewParty(tListC *L, char param1[], char param2[]);
void voteParty(tListC *L, char param1[], char param2[]);
void removeCenters(tListC *L);
void showStats(tListC L);

void processCommand(char commandNumber[CODE_LENGTH+1], char command,char param1[NAME_LENGTH_LIMIT+1], char param2[NAME_LENGTH_LIMIT+1], tListC *L) {

    //printf("Read from input file: %s %c %s %s\n", commandNumber, command, param1, param2);

    switch(command) {
        case 'C': { // Create
            showHeader(commandNumber, command, param1, param2);
            createNewCenter(L, param1, param2);    // Crea un nuevo centro con 'n' (param2) totalvoters.
        }
        break;

        case 'N': { // New
            showHeader(commandNumber, command, param1, param2);
            createNewParty(L, param1, param2);  // Crea un nuevo partido en un centro.
        }
        break;

        case 'V': { // Vote
            showHeader(commandNumber, command, param1,param2);
            voteParty(L, param1, param2);    // Vota a un partido en un centro.
        }
        break;

        case 'R': { // Remove
            showHeader(commandNumber, command, param1, param2);
            removeCenters(L);   // Remover centros.
        }
        break;

        case 'S': { // Stats
            showHeader(commandNumber, command, param1, param2);
            showStats(*L);    // Mostrar stats de los centros y partidos.
        }
        break;

        default: {
            break;
        }
    }
}

void createNewCenter(tListC *L, char param1[], char param2[]) {
    tItemC itemC;   // Item que se va a añadir a la lista.
    strcpy(itemC.centerName, param1);   // Se pone nombre al partido a añadir.
    itemC.validVotes = 0;   // Se inicializa sus votos válidos.
    itemC.nullVotes = 0;    // Se inicializa sus votos nulos.
    itemC.totalVoters = atoi(param2);   // La función atoi transforma la cadena de carácteres en un int para copiar el contenido a los votantes totales del partido.

    // Se busca el elemento en la lista con su nombre con findItemC. Devuelve NULLC si el elemento no está en la lista.

    if (findItemC(param1, *L) == NULLC && insertItemC(itemC, L)) {  // <--- Se verefica si el elemento no existe en la lista, y la operacion de inserción es exitosa.
        // Por lo tanto, se crea un nuevo centro con el nombre dado.
        printf("* Create: center %s totalvoters %s\n",param1, param2); // Si no encuentra el elemento y consigue insertarlo en la lista, se procede a mostrar por pantalla.
    } else {
        printf("+ Error: Create not possible\n");   // <--- En el caso de que ya exista o no consiga insertalo, se devuelve un error.
    }

}
//Poscondición: Los elementos posteriores de la lista pueden cambiar de posición.

void createNewParty(tListC *L, char param1[], char param2[]) {

    tPosC pos = findItemC(param1,*L);   // Primero se busca el centro en la lista de centros.

    if (pos != NULLC) {     // Si el centro está en la lista, se procede a añadir el partido.

        tList auxPartyList; // Se crea la variable en la que se va a copiar la lista de partidos del centro.
        tItemL party;       // Se crea el partido a insertar.

        copyList(getItemC(pos, *L).partyList, &auxPartyList);   // Se copia la lista de partidos para modificarla.
        strcpy(party.partyName, param2);    // Se establece el nombre del partrido a añadir.
        party.numVotes = 0;                 // Se establece los votos del partido.

        if (findItem(param2,auxPartyList) == LNULL && insertItem(party, &auxPartyList)) {   // Si el partido ya no existe en la lista y la operacion de insertar es exitosa, se actualiza la lista en el itemC original.

            updateListC(auxPartyList, pos, L);                      // Se hace el cambio en la lista original.
            printf("* New: center %s party %s\n",param1,param2);    // Se muestra por pantalla el resultado exitoso.

        } else printf("+ Error: New not possible\n");   // Si ya se encuentra el partido en la lista de partidos del centro, o ocurre un error al añadir el partido, se muestra un error por pantalla.

    } else printf("+ Error: New not possible\n");   // Si el centro no está en la lista, se muestra un error.
}
// Sin precondiciones.

void voteParty(tListC *L, char param1[], char param2[]) {

    tPosC pos = findItemC(param1,*L);           // <--- Se obtiene la posicion del centro en la lista con su nombre. Devuelve NULLC si el centro no está en la lista.

    if (pos != NULLC) {                         // <--- Condicion de centro existente. Si no existe pos = NULLC.
        tItemC itemC = getItemC(pos, *L);       // <--- Se obtiene el itemC de la lista utilizando su posición.

        if (!isEmptyList(itemC.partyList) && findItem(param2, itemC.partyList) != LNULL) {  // Si la lista de partidos no está vacía y existe el partido en la lista, se procede a añadir el voto..

            tPosL partyPos = findItem(param2, itemC.partyList);        // <--- Se obtiene la posición del partido en la lista de partidos del centro.

            updateVotes(VOTE, partyPos, &itemC.partyList);             // <--- Se añade 1 voto al partido.
            updateValidVotesC(VOTE, pos, L);                           // <--- Luego se actualiza los votos válidos del centro.

            printf("* Vote: center %s party %s numvotes %d\n",param1, param2, getItem(partyPos,itemC.partyList).numVotes); // Se llama la función getItem para obtener los votos totales que tiene el partido
                                                                                                                        // ^ una vez actualizado con el voto para mostrarlo por pantalla.

        } else {    // <--- Si el partido no existe se devuelve un error y el voto es nulo.

            updateNullVotesC(VOTE, pos, L);     // Se actualizan los votos nulos del centro.

            printf("+ Error: Vote not possible. Party %s not found in center %s. NULLVOTE\n",param2,param1);    // Muestra el error por pantalla.

        }   // end if.

    } else printf("+ Error: Vote not possible.\n");     // <--- Si el centro no existe se devuelve un error.
}
// Sin precondiciones.

void removeCenters(tListC *L) {
    tPosC pos;
    tItemC itemC;
    int contador = 0; // Variable para guardar la cantidad de centros borrados.

    if (!isEmptyListC(*L)) {    // Si la lista no está vacía, se procede a recorrer la lista en busca de partidos con 0 votos válidos.

        for (pos = firstC(*L); pos != NULLC; pos = nextC(pos,*L)) {

            itemC = getItemC(pos, *L);

            if (itemC.validVotes == 0) {    // Si no tiene votos, se borra el centro.

                deleteAtPositionC(pos, L);  // Se borra el centro en la posición indicada.

                contador++;                 // Se actualiza el contador de centros borrados
                pos = previousC(pos, *L);   // Se actualiza la posición en la que se había parado el pivote para llevar en cuentra el centro que fue borrado.

                printf("* Remove: center %s\n",itemC.centerName);   // Se muestra por pantalla el centro borrado.
            } // end if.
        }   // end for.
    }   // end if.
    if (contador == 0) printf("* Remove: no centers removed\n");    // Si no se encontraron partidos a eliminar, se muestra un mensaje.
}
// No tiene precondiciones.
// poscondición: la posición de los elementos posteriores puede verse afectada.

void showStats(tListC L) {
    if (!isEmptyListC(L)) {
        for (tPosC pos = firstC(L); pos != NULLC; pos = nextC(pos,L)) { // Se recorre la lista de centros.

            tItemC itemC = getItemC(pos, L);    // Se obtiene el item del centro en la correspondiente posición.
            tNumVotes totalVotes = itemC.validVotes;    // Se guarda los votos válidos en una variable.

            printf("Center %s\n",itemC.centerName);

            if (totalVotes == 0) totalVotes = 1;    // Si los votos totales validos son 0, se cambia a 1 para evitar errores por divisiones entre 0.

            if (!isEmptyList(itemC.partyList)) {    // Si tiene partidos en el centro se muestra la información por partido.
                tList partyList = itemC.partyList;  // Se guarda la lista de partidos del centro en una variable.

                for (tPosL posL=first(partyList); posL != LNULL; posL = next(posL, partyList)) {    // Se recorre la lista de partidos dentro de cada centro.
                    tItemL itemL = getItem(posL, partyList);    // Se guarda el partido en la correspondiente posición en una variable.

                    printf("Party %s numvotes %d (%.2f%%)\n",itemL.partyName, itemL.numVotes, 100*(float)itemL.numVotes/totalVotes);// Por cada partido en la lista se muestra su información.
                }
            }
            printf("Null votes %d\n",itemC.nullVotes);  // Listar votos nulos.
            printf("Participation: %d votes from %d voters (%.2f%%)\n",itemC.validVotes+itemC.nullVotes, itemC.totalVoters, 100*(float)(itemC.validVotes+itemC.nullVotes)/itemC.totalVoters); // Listar participación.
            // Se hace un cambio explícito de tipo de dato a float para poder mostrar el porcentaje.
        }
    }
}
// No tiene precondiciones.

void readTasks(char *filename) {
    FILE *df;
    char commandNumber[CODE_LENGTH+1], command, param1[NAME_LENGTH_LIMIT+1], param2[NAME_LENGTH_LIMIT+1];

    tListC L;               // Aquí se crea la lista de centros.
    createEmptyListC(&L);   // Se inicializa la lista de centros.

    df = fopen(filename, "r");
    if (df != NULL) {
        while (!feof(df)) {
            char format[16];
            sprintf(format, "%%%is %%c ", CODE_LENGTH);
            fscanf(df, format, commandNumber, &command);
            if (command == 'S' || command == 'R') {
                param1[0] = '\0';
                param2[0] = '\0';
            } else {
                sprintf(format, "%%%is %%%is", NAME_LENGTH_LIMIT, NAME_LENGTH_LIMIT);
                fscanf(df, format, param1, param2);
            }
            processCommand(commandNumber, command, param1, param2, &L); // Se pasa la lista por referencia para poder modificarla.
        }
        fclose(df);
    } else {
        printf("Cannot open file %s.\n", filename);
    }
}

void showHeader(char commandNumber[CODE_LENGTH+1], char command, char param1[NAME_LENGTH_LIMIT+1], char param2[NAME_LENGTH_LIMIT+1]) { // Muestra la cabecera de cada operación por pantalla.

    printf(CABECERA);   // Primero se imprime el separador de 20 asteriscos.

    if (command == 'S' || command == 'R') printf("%s %c\n", commandNumber, command);  // Si el comando es Stats o Remove, no se tiene que mostrar ningún centro o partido en pantalla. Solamente el número y el código del comando.

    else printf("%s %c: center %s %s %s\n", commandNumber, command, param1,(command == 'C')?"totalvoters":"party", param2); // En el caso de que el comando se cualquier otro que no sea Stats o Remove,
                                                                                                                            // ^ se utilizaran los datos como el nombre del centro o partido, votos totales, etc.
}

int main(int nargs, char **args) {

    char *filename = "new.txt";

    if (nargs > 1) {
        filename = args[1];
    } else {
        #ifdef INPUT_FILE
        filename = INPUT_FILE;
        #endif
    }

    readTasks(filename);

    return 0;
}