/*
 * TITLE: Proyecto 2 de PRO2
 * SUBTITLE: Práctica 2
 * AUTHOR 1: Luca D'angelo Sabin LOGIN 1: l.dangelo
 * AUTHOR 2: Martin Vietes García LOGIN 2: martin.vieites
 * GROUP: 1.1
 * DATE: 07 / 05 / 2020
 */

#ifndef CENTER_LIST_H
#define CENTER_LIST_H
#define MAX_CENTERS 10
#define NULLC -1

#include "types.h"
#include "stdbool.h"
#include "string.h"
#include "party_list.h"
#include "ctype.h"

typedef int tPosC;
typedef struct {
    tCenterName centerName;
    tNumVotes totalVoters;
    tNumVotes validVotes;
    tNumVotes nullVotes;
    tList partyList;
} tItemC;
typedef struct {
    tItemC itemC[MAX_CENTERS];
    tPosC lastPos;
} tListC;

bool isEmptyListC(tListC L);
void createEmptyListC(tListC* L);
bool insertItemC(tItemC d, tListC* L);
void updateListC(tList L, tPosC p, tListC* C);
void updateValidVotesC(tNumVotes v, tPosC p, tListC* L);
void updateNullVotesC(tNumVotes v, tPosC p, tListC* L);
tPosC findItemC(tPartyName d, tListC L);
tItemC getItemC(tPosC p, tListC L);
void deleteAtPositionC(tPosC p, tListC* L);
tPosC firstC(tListC L);
tPosC nextC(tPosC p, tListC L);
tPosC previousC(tPosC p, tListC L);
#endif
