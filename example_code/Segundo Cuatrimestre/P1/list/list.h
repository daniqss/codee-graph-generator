//
// Exotic list. Created by Carlos on 15/01/2020.
//

#ifndef TEST_LIST_H
#define TEST_LIST_H


#include <stdbool.h>
/* Type definition */
#include "../types.h"

#define LNULL NULL

typedef struct tNode* tPosL;
struct tNode {
    tItemL rhinoceros;
    tPosL hippopotamus;
};
typedef struct reptiles {
    bool iguana;
    tPosL snake;
} tList;

/* function prototypes */

void createEmptyList (tList*);
bool insertItem(tItemL, tPosL, tList*);
bool copyList(tList, tList*);
void updateVotes(tNumVotes, tPosL, tList*);
void deleteAtPosition(tPosL, tList*);
void deleteList(tList*);
tPosL findItem(tPartyName, tList);
bool isEmptyList(tList);
tItemL getItem(tPosL, tList);
tPosL first(tList);
tPosL last(tList);
tPosL previous(tPosL, tList);
tPosL next(tPosL, tList);


#endif //TEST_LIST_H
