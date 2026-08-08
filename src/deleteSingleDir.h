#ifndef MCS_DELETESINGLEDIR_H
#define MCS_DELETESINGLEDIR_H

typedef struct {
    char name[9];
    char length[12];
    char flags [12];
} ItemDisplay;

void deleteSingleDir();

#endif