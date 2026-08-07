#include <asm.h>

/* 
In asm.h change:
/* template for generic functions with global name */
#define _C_LABEL(x)	_ ## x	/* prefix fn name with _ */
/*#define _C_LABEL(x)	
x*/

SYSCALL(MCS_GetDirectoryEntryByNumber, 0x1533)