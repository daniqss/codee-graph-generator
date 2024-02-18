/*
 * TITLE: Proyecto 2 de PRO2
 * SUBTITLE: Práctica 2
 * AUTHOR 1: Luca D'angelo Sabin LOGIN 1: l.dangelo
 * AUTHOR 2: Martin Vietes García LOGIN 2: martin.vieites
 * GROUP: 1.1
 * DATE: 07 / 05 / 2020
 */

#ifndef PARTY_LIST_H
#define PARTY_LIST_H
#define LNULL NULL

#include "types.h"
#include <stdbool.h>
#include <stdlib.h>
#include "string.h"
#include "ctype.h"



typedef struct tNode* tPosL;
struct tNode {      // Cada celda de dato que contiene dato y un puntero.
    tItemL itemL;
    tPosL next;     // El puntero "next" que apunta al siguiente nodo.
};
typedef tPosL tList;


void createEmptyList(tList* L);
bool copyList(tList L, tList* M);
bool insertItem(tItemL d, tList* L);
void updateVotes(tNumVotes v, tPosL p, tList* L);
tPosL findItem(tPartyName d, tList L);
bool isEmptyList(tList L);
void deleteList(tList *L);
tItemL getItem(tPosL p, tList L);
tPosL first(tList L);
tPosL next(tPosL p, tList L);

#endif
