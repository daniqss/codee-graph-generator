#ifndef TYPES_H
#define TYPES_H

#define NAME_LENGTH_LIMIT 32

typedef char tPartyName[NAME_LENGTH_LIMIT];
typedef char tCenterName[NAME_LENGTH_LIMIT];
typedef int tNumVotes;

typedef struct tItemL {
    tPartyName partyName;
    tNumVotes numVotes;
} tItemL;

#endif //TYPES_H