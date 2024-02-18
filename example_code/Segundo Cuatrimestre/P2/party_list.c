/*
 * TITLE: Proyecto 2 de PRO2
 * SUBTITLE: Práctica 2
 * AUTHOR 1: Luca D'angelo Sabin LOGIN 1: l.dangelo
 * AUTHOR 2: Martin Vietes García LOGIN 2: martin.vieites
 * GROUP: 1.1
 * DATE: 07 / 05 / 2020
 */

#include "party_list.h"

bool createNode(tPosL *p) {

    *p=malloc(sizeof(struct tNode));
    return *p!=LNULL;
}

char* strLower(char* c) {   // Esa función fue creada porque en el compilador de Linux la función strlwr() no está por defecto.
    for (int i = 0; i<strlen(c); i++) {  // La función stricmp puede no estar recogida como una función por defecto tampoco, entonces ese optó por este método.
        c[i] = tolower(c[i]);
    }
    return c;
}

void createEmptyList(tList *L) {

    *L=LNULL;
}

bool insertItem(tItemL d, tList *L) {

    tPosL q;

    if (!createNode(&q)) {
        return false;
    } else {

        q->itemL = d;

        if (isEmptyList(*L))    // Si la lista está vacía.
            *L = q;
        else {  // insertar en la primera posición.
            tPartyName dLower;     // Variable para guardar el partido a ser añadido en minúsculas.
            tPartyName auxLower;   // Variable para guardar el nombre de los partidos en minúsculas.

            strLower(strcpy(dLower,d.partyName));    // Para que la comparación no distinga entre mayúsculas y minúsculas se almacena en una variable el nombre del partido a añadir en minúsculas.
            strLower(strcpy(auxLower,(*L)->itemL.partyName));   // Lo mismo se guarda en una variable auxiliar el nombre del primer partido en la lista, que se va a ir actualizando a medida que se avanza en la lista.

            tPosL i;
            // Se comprueba a qué posicion pertenece el elemento a insertar.
            for (i = *L; (strcmp(dLower, auxLower) > 0) && (i->next != LNULL); i = i->next, strLower(strcpy(auxLower,(i->itemL.partyName))));  // Se empieza desde el principio de la lista.

            if (i->next != LNULL || (i->next == LNULL && strcmp(dLower, auxLower) < 0) ) { // Si el elemento pertenece a cualquier posición menos depués del elemento final de la lista se procede a añadirlo.
                q->itemL = i->itemL;
                i->itemL = d;
                q->next = i->next;
                i->next = q;
            } else {    // Si el elemento pertenece al final de la lista, se le añade de una forma distinta..
                i->next = q;
                q->next = LNULL;
            }
        }   // end if.
        return true;
    }   // end if.
}   // end function
//precondición: lista esté creada y haya suficiente espacio en la memoria.
//poscondición: los elementos posteriores a la posición pueden cambiar.

void updateVotes(tNumVotes v, tPosL p, tList* L) {

    p->itemL.numVotes = p->itemL.numVotes + v;
}
// precondición: posicion válida de la lista.

bool isEmptyList(tList L) {

    if (L==LNULL) return true;  // Si la lista está vacía el puntero del principio de la lista L es nulo.
    else return false;
}

tItemL getItem(tPosL p, tList L) {

    tItemL item = p->itemL;
    return item;
}
//precondición: posicion válida de la lista.

tPosL first(tList L) {

    return L;
}
// precondición: lista no está vacía.

tPosL last(tList L) {

    tPosL p;

    for (p=L; p->next!=LNULL; p=p->next);
    return p;
}
// precondición: lista no está vacía.

tPosL next(tPosL p, tList L) {
    return p->next;
};
// precondición: p es una posicion válida de la lista.
// Si p es el último elemento de la lista se devuleve NULL.

tPosL previous(tPosL p, tList L) {
    tPosL q;

    if (q == L) return LNULL;
    else {
        for (q = L; q->next != p; q = q->next);
        return q;
    }   // end if.
}
// precondición: p es una posicion válida de la lista.
// Si p es el primer elemento de la lista se devuleve NULL.

tPosL findItem(tPartyName d, tList L) {
    tPosL p=LNULL;
    if (!isEmptyList(L)) {
        tPartyName dLower;     // Variable para guardar el partido a ser añadido en minúsculas.
        tPartyName auxLower;   // Variable para guardar el nombre de los partidos en minúsculas.

        strLower(strcpy(dLower,d));    // Para que la comparación no distinga entre mayúsculas y minúsculas se almacena en una variable el nombre del partido a añadir en minúsculas.
        strLower(strcpy(auxLower,L->itemL.partyName));   // Lo mismo se guarda en una variable auxiliar el nombre del primer partido en la lista, que se va a ir actualizando a medida que se avanza en la lista.

        for (p = L; p->next != LNULL && (strcmp(auxLower, dLower) < 0); p = p->next, strLower(strcpy(auxLower,p->itemL.partyName)));

        if (strcmp(auxLower, dLower) > 0) {
            p = LNULL;
        } else if (p->next == LNULL) {   // Como el bucle se salta la comparación con el último elemento, se verefica la última posición en la lista.
                strLower(strcpy(auxLower,p->itemL.partyName));
                if (strcmp(p->itemL.partyName, dLower) != 0) p = p->next;   // Si no es el elemento buscado, se devuelve LNULL,;
            }
        }
        return p;
}
// pre: la lista está creada.
// Si el elemento no es encontrado se devuleve NULL.

void deleteAtPosition(tPosL p, tList *L) {
    tPosL q;
    if (p == *L) {  // Caso si está en la cabeza.
        *L = (*L)->next;
    } else if(p->next == LNULL) {   // Caso si está al final de la lsita.
        for (q=*L; q->next != p; q=q->next);
        q->next = p->next;
    } else {    // Caso si está en el cuerpo de la lista.
        q = p->next;
        p->itemL = q->itemL;
        p->next = q->next;
        p = q;
    } // end if.
    free(p);
};
// precondición: posición es una posición válida.
// post: los siguientes elemtos de la lista al de la posicion eliminada pueden haber variado.

void deleteList(tList *L) {
    tPosL p;
    while (*L != LNULL) {   // Equivalente a deleteAtPosition(first(L),L)
        p = *L;
        *L = (*L)->next;
        free(p);
    }
}
// precondición: la lista esté previamente creada.

bool copyList (tList L, tList* m) { // Copia una lista L a una lista m.

    tPosL p,q,r;    // Punteros auxiliares. La p va iterando por la lista.
    bool ret=true;

    createEmptyList(m);

    if (!isEmptyList(L)) {
        p=L;
        while ((p!=LNULL) && (createNode(&r))) {
            r->itemL = p->itemL;  // Se copia el dato de la lista L a la m.
            r->next = LNULL;    // Se asigna LNULL al puntero next del último elementos de la lista m.
            if (p==L) {     // Ocurre solamente una vez. Si p es cabeza de la lista, es decir, la lista tiene solo un elemento.
                *m=r;
                q=r;
            } else {
                q->next = r;
                q = r;
            }   // end if.
            p = p->next;
        }   // end while.
    }   // end if.

    if (p!=LNULL) {
        ret = false;
        deleteList(m);  // Si la operacion no se cumple en su totalidad, se borra la lista creada.
    }   // end if.

    return ret;
}
//precondición: Hay suficiente memoria.
