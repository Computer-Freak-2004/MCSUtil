#include <asm.h>

/* 
In asm.h change:
/* template for generic functions with global name */
#define _C_LABEL(x)	_ ## x	/* prefix fn name with _ */
/*#define _C_LABEL(x)	
x*/

SYSCALL_BARE(MCS_GetDirectoryEntryByNumber, 0x1533)
SYSCALL_BARE(MCS_CreateDirectory, 0x1515)

SYSCALL_ADDRESS