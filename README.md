# MCS Utility

This add-in allows for simple management of MCS (Main Content System) directories. All add-ins have an internal name (`@CBASIC` for example) and a MCS directory with the same name (limited to to 8 chars) is created when the add-in starts.

There can only be *147 (`0x93`) directory entries*. If the limit is reached, no new MCS directories can be created. Add-ins with new internal names don't have a corresponding directory yet. Its creation silently fails, causing the add-in not to start. 
This utility helps to fix that.

## Functions

### Create Directory Test

You can enter a name to test if a new MCS directory can be created. The `@` symbol gets automatically added to the front of your input.

After entering a name, the return code and its meaning are shown:
of it:

- `0`: Success
- `0x42`: Already exists
- `0x43`: Dir space exhausted
- anything else: Failure

If you see `0x43`, there is no space left. Delete some directories to allow add-ins with new internal names to start again.

### Delete single directory

This allows you to browse through and delete any directory.
- `[←]` and `[→]` browse one entry left or right
- `[↓]` and `[↑]` go to the previous/ next non-empty entry

Besides the directory name in blue, you can see its item count, the starting address of its items in memory, and its flags.
If the current directory contains items, you can view them with `[F1]`. The name, length in bytes, and flags of each item are shown.
Press `[EXE]` to delete the current directory entirely. The following return codes are possible:
- `0`: Success
- `0xF0`: Dir is null
- `0x40`: Dir doesn't exist
- `0x45`: Dir not empty
- `0x46`: Undeletable system dir
- `0x47`: Certain flags are set

### Delete all empty directories

This deletes all empty directories. This is useful for quickly freeing space when the directory space is exhausted and new add-ins don't start.

### Directory space usage

Shows how many of the 147 directory entries are currently in use. It's impossible to reach 0% usage as there are system directories that can't be deleted:
- `@APLWORK`
- `main`
- `@LOCAL1..@LOCAL40`
- `@REV3..@REV1`
- `library`
- `system`

### Test Mode

Opens a secret menu that is usually accessed like this:

1. Power off the calculator.
2. Hold down `[OPTN]` + `[x10^]` + `[AC/ON]` together.
3. On the popup enter the sequence `5, 9, 6, 3`
4. Press `[1]` to enter Valiable Manager. You can now browse through the directories and their items.


## Useful Docs
MCS Docs + Syscall Addresses:
- MCS Syscalls: https://bible.planet-casio.com/simlo/chm/v20/fx_legacy_MCS.htm
- MCS Structure: https://bible.planet-casio.com/simlo/chm/v20/fx_legacy_MCSstructure.HTM
- Syscall addresses for CG20+: https://bible.planet-casio.com/simlo/chm/v20/fx_fxCG20_syscalls.htm

Syscalls:
- MCS_CreateDirectory (wrong one): https://prizm.cemetech.net/Syscalls/MCS/MCS_CreateDirectory/
- MCS_DeleteDirectory: https://prizm.cemetech.net/Syscalls/MCS/MCS_DeleteDirectory/
- MCSDelVar2: https://prizm.cemetech.net/Syscalls/MCS/MCSDelVar2/

General Programming:
- Printing Text: https://prizm.cemetech.net/Tutorials/Printing_Text/
- Reading Input: https://prizm.cemetech.net/Tutorials/Reading_Input/
- Useful Routines: https://prizm.cemetech.net/Useful_Routines/
- Image Converter: https://www.cemetech.net/sc/

## Build
This project requires [PrizmSDK 0.6](https://github.com/Jonimoose/libfxcg).

In the PrizmSDK folder, open `/include/asm.h` and replace:
```
/* template for generic functions with global name */
/*#define _C_LABEL(x)   _ ## x*/    /* prefix fn name with _ */
#define _C_LABEL(x) x
```
with
```
/* template for generic functions with global name */
#define _C_LABEL(x)   _ ## x    /* prefix fn name with _ */
/*#define _C_LABEL(x) x */
```

Then run `make`.

## Manually added syscalls

Addresses only for fx-CG20/50 (probably won't work on fx-CG10):
- `0x1533: int MCS_GetDirectoryEntryByNumber(int dirno, TMainMemoryDirectoryEntry** pdir);`
- `0x1515: int MCS_CreateDirectory2(unsigned char* dir, char* dirno);`
- `0x1516: int MCS_DeleteDirectory(unsigned char* dir);`