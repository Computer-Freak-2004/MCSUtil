# MCSUtil

## Important Docs
- Addresses for CG-50: https://bible.planet-casio.com/simlo/chm/v20/fx_fxCG20_syscalls.htm
- MCS Function: https://bible.planet-casio.com/simlo/chm/v20/fx_legacy_MCS.htm
- MCS Structure: https://bible.planet-casio.com/simlo/chm/v20/fx_legacy_MCSstructure.HTM

- MCS_CreateDirectory: https://prizm.cemetech.net/Syscalls/MCS/MCS_CreateDirectory/
- MCS_DeleteDirectory: https://prizm.cemetech.net/Syscalls/MCS/MCS_DeleteDirectory/
- MCSDelVar2: https://prizm.cemetech.net/Syscalls/MCS/MCSDelVar2/

# Build
This project needs [PrizmSDK 0.6](https://github.com/Jonimoose/libfxcg) installed.

In (PrizmSDK Root)/include/asm.h change:
```
/* template for generic functions with global name */
/*#define _C_LABEL(x)   _ ## x*/    /* prefix fn name with _ */
#define _C_LABEL(x) x
```
to
```
/* template for generic functions with global name */
#define _C_LABEL(x)   _ ## x*/    /* prefix fn name with _
/*#define _C_LABEL(x) x */
```

To build it run `make` in the root dir.