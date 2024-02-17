/*
 * TITLE: Proyecto 1 de PRO2
 * SUBTITLE: Práctica 1
 * AUTHOR 1: Luca D'angelo Sabin LOGIN 1: l.dangelo
 * AUTHOR 2: Martin Vietes García LOGIN 2: martin.vieites
 * GROUP: 1.1
 * DATE: 25 / 02 / 2020
 */

#include "dynamic_list.h"

bool createNode(tPosL *p) {

    *p=malloc(sizeof(struct tNode));
    return *p!=LNULL;
}

void createEmptyList(tList *L) {

    *L=LNULL;
}

bool insertItem(tItemL d, tPosL p, tList *L) {

    tPosL q,r;

    if (!createNode(&q)) {
        return false;
    } else {

        q->itemL = d;
        q->next=LNULL;

        if (*L == LNULL)    // Si la lista está vacía.
            *L = q;
        else if (p==LNULL) {    // Insertar en la última posicion.
            for (r=*L; r->next != LNULL; r=r->next); // recorre los punteros de next hasta encontrar un nulo.
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
//precondición: lista esté creada y haya suficiente espacio en la memoria.

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
    tPosL p;
    for (p = L; p != LNULL && (strcmp(p->itemL.partyName,d) != 0); p = p->next);
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
    } else {    // Casi si está en el cuerpo de la lista.
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
//precondición: Haya suficiente memoria.
