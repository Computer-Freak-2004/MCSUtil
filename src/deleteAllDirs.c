#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "mcs_syscalls.h"

void deleteAllDirs() {
    int key;

    while (1) {
        MsgBoxPush(3);

        int x = 50, y = 60;
        PrintMini(&x, &y, "Delete ALL directories?", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        x = 50, y = y + 30;
        PrintMini(&x, &y, "[EXE] Yes | [EXIT] No", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        Bdisp_PutDisp_DD();

        GetKey(&key);

        if (key == KEY_CTRL_EXE) {
            MsgBoxPop();
            TMainMemoryDirectoryEntry* dir;
            for (int i = 1; i < 0x93; i++) {
                Bdisp_AllClr_VRAM();

                EnableStatusArea(1);
                DisplayStatusArea();

                char buf[64];
                strcpy(buf, "Deleting ");
                strcat(buf, dir->name);

                int x = 50, y = 30;
                PrintMini(&x, &y, buf, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

                memset(buf, 0, 64);

                char i_str[16];
                itoa(i, i_str);
                strcpy(buf, i_str);

                strcat(buf, "/");

                char max_str[16];
                itoa(0x93, max_str);
                strcat(buf, max_str);

                x = 50, y = y + 30;
                PrintMini(&x, &y, buf, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

                ProgressBar2("", i, 0x93);
                Bdisp_PutDisp_DD();

                int rc = MCS_GetDirectoryEntryByNumber(i, &dir);
                if (rc == 0) {
                    MCS_DeleteDirectory(dir->name);
                }
            }
            break;

        } else if (key == KEY_CTRL_EXIT)
            break;
    }
}