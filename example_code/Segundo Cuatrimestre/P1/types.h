#ifndef TYPES_H
#define TYPES_H

#define NAME_LENGTH_LIMIT 20

typedef char tPartyName[NAME_LENGTH_LIMIT];
typedef int tNumVotes;

typedef struct tItemL {
    tPartyName partyName;
    tNumVotes numVotes;
} tItemL;

#endif //TYPES_H