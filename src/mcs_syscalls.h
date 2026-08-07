#ifndef MCS_SYSCALLS_H
#define MCS_SYSCALLS_H

typedef struct {
    unsigned char name[8];
    void* addr;
    unsigned short count;
    char flags[2];
} TMainMemoryDirectoryEntry;
TMainMemoryDirectoryEntry MainMemoryDirectory[147];  // 0x93

int MCS_GetDirectoryEntryByNumber(
    int dirno, TMainMemoryDirectoryEntry** pdir);

int MCS_CreateDirectory(unsigned char* dir, char* dirno);

int MCS_DeleteDirectory(unsigned char* dir);

#endif