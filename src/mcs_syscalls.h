#ifndef MCS_SYSCALLS_H
#define MCS_SYSCALLS_H

typedef struct {
    unsigned char name[8];
    void* addr;
    unsigned short count;
    char flags[2];
} TMainMemoryDirectoryEntry;

typedef struct {
    unsigned char name[8];
    int offset;
    int length;
    char flags[4];
} TItemEntry;

#define MCS_SIZE 147

// TMainMemoryDirectoryEntry MainMemoryDirectory[147];  // 0x93

int MCS_GetDirectoryEntryByNumber(int dirno, TMainMemoryDirectoryEntry** pdir);

int MCS_CreateDirectory2(unsigned char* dir, char* dirno);

#endif