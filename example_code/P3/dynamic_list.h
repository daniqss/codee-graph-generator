/*
 * TITLE: Proyecto 1 de PRO2
 * SUBTITLE: Práctica 1
 * AUTHOR 1: Luca D'angelo Sabin LOGIN 1: l.dangelo
 * AUTHOR 2: Martin Vietes García LOGIN 2: martin.vieites
 * GROUP: 1.1
 * DATE: 25 / 02 / 2020
 */

#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H

#include "types.h"

#include <stdbool.h>
#include <stdlib.h>
#include "string.h"
#define LNULL NULL

typedef struct tNode* tPosL;
struct tNode {      // Cada celda de dato que contiene dato y un puntero.
    tItemL itemL;
    tPosL next;     // El puntero "next" que apunta al siguiente nodo.
};
typedef tPosL tList;


void createEmptyList(tList* L);
bool copyList(tList L, tList* M);
void deleteList(tList* L);
bool insertItem(tItemL d, tPosL p, tList* L);
void updateVotes(tNumVotes v, tPosL p, tList* L);
tPosL findItem(tPartyName d, tList L);
bool isEmptyList(tList L);
tItemL getItem(tPosL p, tList L);
tPosL first(tList L);
tPosL last(tList L);
tPosL previous(tPosL p, tList L);
tPosL next(tPosL p, tList L);
void deleteAtPosition(tPosL p, tList* L);

#endif
