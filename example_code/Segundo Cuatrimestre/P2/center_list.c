/*
 * TITLE: Proyecto 2 de PRO2
 * SUBTITLE: Práctica 2
 * AUTHOR 1: Luca D'angelo Sabin LOGIN 1: l.dangelo
 * AUTHOR 2: Martin Vietes García LOGIN 2: martin.vieites
 * GROUP: 1.1
 * DATE: 07 / 05 / 2020
 */

#include "types.h"
#include "party_list.h"
#include "center_list.h"

char* strLower(char* c);    // Como la función strlwr() no está por defecto en el compilador de Linux, se creó esta función.
// La función stricmp puede no estar recogida como una función por defecto tampoco, entonces ese optó por este método.

void createEmptyListC(tListC* L) {   // Paso por referencia.

    L->lastPos = NULLC;
}

bool insertItemC(tItemC d, tListC* L) {

    tPosC i;

    if(L->lastPos==MAX_CENTERS-1) {     // Comprobar si la lista no está llena
        return false;
    } else {

        tList partyList;    // Lista de partidos que va a tener el centro.
        createEmptyList(&partyList); // La lista de partidos es inicializada.
        d.partyList = partyList;

        if (isEmptyListC(*L)) {   // Comprobar si la lista está vacía.
            L->lastPos++;
            L->itemC[L->lastPos] = d;   // En cuyo caso el elemento se añade sin más.

        } else {   // Para los demás casos, dado que la lista no está vacía.
            tPartyName dLower;     // Variable para guardar el centro a ser añadido en minúsculas.
            tPartyName auxLower;   // Variable para guardar el nombre de centros en minúsculas.

            strLower(strcpy(dLower,d.centerName));    // Para que la comparación no distinga entre mayúsculas y minúsculas se almacena en una variable el nombre del centro a añadir en minúsculas.
            strLower(strcpy(auxLower,L->itemC[L->lastPos].centerName));   // Lo mismo se guarda en una variable auxiliar el nombre del último centro en la lista, que se va a ir actualizando a medida que se avanza en la lista.

            L->lastPos++;

            for (i = L->lastPos; (strcmp(dLower, auxLower) < 0) && (i>0); i--, strLower(strcpy(auxLower,L->itemC[i-1].centerName))) { // El cambio de posicion de elementos se hace del final al principio.
                L->itemC[i] = L->itemC[i - 1];
            }   // end for.

            L->itemC[i] = d;    // Por fin, se añade el centro a la lista.

        } // end if.
        } // end if.
    return true;
} // end function.
//precondición: lista esté creada y el elemento ya no exista en la lista.
//poscondición: los elementos posteriores a la posición pueden cambiar.

void updateListC(tList L, tPosC p, tListC* C) {

    C->itemC[p].partyList = L;
}
// precondición: posicion es válida y lista está creada.

void updateValidVotesC(tNumVotes v, tPosC p, tListC* L) {

    L->itemC[p].validVotes = L->itemC[p].validVotes + v;
}
// precondición: posicion es válida y lista está creada.

void updateNullVotesC(tNumVotes v, tPosC p, tListC* L) {

    L->itemC[p].nullVotes = L->itemC[p].nullVotes + v;
}
// precondición: posicion es válida y lista está creada.

tPosC findItemC(tPartyName d, tListC L) {

    tPosC p;

    if (L.lastPos == NULLC) return NULLC;   // Comprobar si la lista está vacía.

    tPartyName dLower;     // Variable para guardar el centro a ser buscado en minúsculas.
    tPartyName auxLower;   // Variable para guardar nombre de centros en minúsculas.

    strLower(strcpy(dLower,d));   // Para que la comparación no distinga entre mayúsculas y minúsculas se almacena en una variable el nombre del centro a ser buscado en minúsculas.
    strLower(strcpy(auxLower,L.itemC[0].centerName));   // Lo mismo se guarda en una variable auxiliar el nombre del último centro en la lista, que se va a ir actualizando a medida que se avanza en la lista.

    for (p = 0; (strcmp(dLower, auxLower) > 0) && (p<L.lastPos); ++p, strLower(strcpy(auxLower,L.itemC[p].centerName)));  // Recorre la lista desde el principio para mirar si hay algun elemento que coincide.
        // Si encuentra algún elemento que es posterior al buscado la búsqueda cesa, dado que la lista está ordenada.
    if (strcmp(dLower, auxLower) == 0)  // Si la búsqueda se cesó debido a que se encontró un elemento coincidente, se devuelve la posición.
        return p;
    else return NULLC;  // En el caso de que haya sido porque no se encontró el elemento, se devuelve el valor nulo.
}
// precondición: lista está creada.
// Si el elemento no es encontrado se devuleve NULL.

bool isEmptyListC(tListC L) {

    return L.lastPos == NULLC;
}

tItemC getItemC(tPosC p, tListC L) {

    tItemC i;
    i = L.itemC[p];

    return i;
}
//precondición: posicion válida de la lista, y la lista está creada.

tPosC firstC(tListC L) {
    return 0;
}
// precondición: lista no está vacía

tPosC lastC(tListC L) {
    return L.lastPos;
}
// precondición: lista no está vacía

tPosC previousC(tPosC p, tListC L) {
    return --p;
}
// precondición: posición válida de la lista.

tPosC nextC(tPosC p, tListC L) {
    if (p == L.lastPos) return NULLC;
    else return ++p;
}
// precondición: posición válida de la lista.

void deleteAtPositionC(tPosC p, tListC* L) {

    tPosC i;
//    tPosL posL;     // variable de control de posición para borrar la lista de partidos del centro.
//    tList *partyList = &L->itemC[p].partyList;    // Se obtiene el puntero que apunta a la lista de partidos del centro.
    L->lastPos--;   // Se actualiza lastpos de la lista de centros.

//    while (*partyList != LNULL) {   // Se recorre la lista de partidos del centro para borrarla.
//        posL = *partyList;
//        deleteAtPosition(posL, partyList);
//        *partyList = next(posL,*partyList);
//    }

    deleteList(&L->itemC[p].partyList);

    for (i=p; i<=L->lastPos; i++) {
        L->itemC[i] = L->itemC[i+1];
    }   // end for.
}
// precondición: posición válida de la lista.