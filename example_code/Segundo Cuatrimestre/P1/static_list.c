/*
 * TITLE: Proyecto 1 de PRO2
 * SUBTITLE: Práctica 1
 * AUTHOR 1: Luca D'angelo Sabin LOGIN 1: l.dangelo
 * AUTHOR 2: Martin Vietes García LOGIN 2: martin.vieites
 * GROUP: 1.1
 * DATE: 25 / 02 / 2020
 */

#include "static_list.h"

void createEmptyList(tList* L) {     // Paso por referencia.

    L->lastPos = LNULL;              // = (*L).lastPos
}

bool insertItem(tItemL d, tPosL p, tList* L) {

    tPosL i;
    if(L->lastPos==MAX-1) {
        return false;
    } else {
        L->lastPos++;
        if (p==LNULL) {   // Se diferencia la operacion de añadir (por el medio) e insertar (al final).
            L->itemL[L->lastPos] = d;
        } else {
            for (i=L->lastPos; i>=p+1; i--) {      // El cambio de posicion de elementos se hace del final al principio.
                L->itemL[i] = L->itemL[i-1];
            }   // end for.
            strcpy(L->itemL[p].partyName, d.partyName);
            L->itemL[p].numVotes = d.numVotes;
        }   // end if.
    }
    return true;
}
//precondición: lista esté creada.

void updateVotes(tNumVotes v, tPosL p, tList* L) {

    L->itemL[p].numVotes = L->itemL[p].numVotes + v;
}
// precondición: posicion es válida.

tPosL findItem(tPartyName d, tList L) {

    tPosL p=0;
    bool aux=false;

    if (L.lastPos == LNULL) return LNULL;
    else {
        do {
            if (strcmp(L.itemL[p].partyName, d) == 0) {
                aux = true;
                return p;
            }   // end if.
            else ++p;

        } while ((p<=L.lastPos) && !aux);
        return LNULL;
    }   // end if.
}
// precondición: lista está creada.
// Si el elemento no es encontrado se devuleve NULL.

bool isEmptyList(tList L) {

    return L.lastPos == LNULL;
}

tItemL getItem(tPosL p, tList L) {

    tItemL i;
    i = L.itemL[p];

    return i;
}
//precondición: posicion válida de la lista, y la lista está creada.

tPosL first(tList L) {
    return 0;
}
// precondición: lista no está vacía

tPosL last(tList L) {

    return L.lastPos;
}
// precondición: lista no está vacía

tPosL previous(tPosL p, tList L) {

    return --p;
}
// precondición: posición válida de la lista.

tPosL next(tPosL p, tList L) {

    if (p == L.lastPos) return LNULL;
    else return ++p;
}
// precondición: posición válida de la lista.

void deleteAtPosition(tPosL p, tList* L) {

    tPosL i;
    L->lastPos--;

    for (i=p; i<=L->lastPos; i++) {
        L->itemL[i] = L->itemL[i+1];
    }   // end for.
}
// precondición: posición válida de la lista.

void deleteList(tList* L) {

    while (L->lastPos != LNULL) {
        deleteAtPosition(first(*L),L);
        deleteAtPosition(last(*L),L);
    }
    L->lastPos=LNULL;
}
// precondición: la lista esté previamente creada.

bool copyList(tList L, tList* M) {
    tPosL p;
    for (p=0; p<=L.lastPos; p++) {
        M->itemL[p] = L.itemL[p];
    }
    M->lastPos = L.lastPos;
    return true;
}
// precondición: lista está creada previamente.