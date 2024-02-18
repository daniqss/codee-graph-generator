#include <stdio.h>
#include <string.h>

#ifdef STATIC_LIST
#include "static_list.h"
#endif
#ifdef DYNAMIC_LIST
#include "dynamic_list.h"
#endif
#ifdef TEST_LIST
#include "list/list.h"
#endif

void print_list(tList list) {
    tPosL pos;
    tItemL item;

    printf("(");
    if (!isEmptyList(list)) {
        pos = first(list);
        while (pos != LNULL) {
            item = getItem(pos, list);
            printf(" %s numVotes %d", item.partyName, item.numVotes);
            pos = next(pos, list);
        }
    }
    printf(")\n");
}

int main() {
    tList list;
    tPosL pos;
    tItemL item1, item2;

    /* init */
    item1.numVotes = 0;


    /* create */
    createEmptyList(&list);
    print_list(list);


    /* insert */
    strcpy(item1.partyName, "party3");
    insertItem(item1, LNULL, &list);
    print_list(list);

    strcpy(item1.partyName, "party1");
    insertItem(item1, first(list), &list);
    print_list(list);

    strcpy(item1.partyName, "party5");
    insertItem(item1, LNULL, &list);
    print_list(list);

    strcpy(item1.partyName, "party2");
    insertItem(item1, next(first(list),list), &list);
    print_list(list);

    strcpy(item1.partyName, "party4");
    insertItem(item1, last(list), &list);
    print_list(list);


    /* find */
    pos = findItem("party33", list);
    if (pos == LNULL) {
        printf("party33 Not found\n");
    }

    pos = findItem("party3", list);
    item2 = getItem(pos, list);
    printf("%s\n", item2.partyName);


    /* update */
    updateVotes(1, pos, &list);
    item2 = getItem(pos, list);
    printf("%s\n", item2.partyName);
    print_list(list);


    /* remove */
    deleteAtPosition(next(first(list),list), &list);
    print_list(list);

    deleteAtPosition(previous(last(list),list), &list);
    print_list(list);

    deleteAtPosition(first(list),&list);
    print_list(list);

    deleteAtPosition(last(list),&list);
    print_list(list);

    deleteAtPosition(first(list),&list);
    print_list(list);

    insertItem(item1, LNULL, &list);
    print_list(list);

}
