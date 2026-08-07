#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "mcs_syscalls.h"
#include "util.h"

void deleteAllDirs() {
    Bdisp_AllClr_VRAM();

    EnableStatusArea(1);
    DisplayStatusArea();
    printTitle("Delete all empty MCS directories", 35);

    int key;
    int confirm = 0;
    MsgBoxPush(3);
    while (1) {
        int x = 45, y = 60;
        PrintMini(&x, &y, "Delete ALL empty directories?", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        x = 45, y = y + 30;
        PrintMini(&x, &y, "[EXE] Yes | [EXIT] No", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        Bdisp_PutDisp_DD();

        GetKey(&key);

        if (key == KEY_CTRL_EXE) {
            confirm = 1;
            break;
        } else if (key == KEY_CTRL_EXIT)
            break;
    }

    MsgBoxPop();
    if (confirm) {
        TMainMemoryDirectoryEntry* dir;
        for (int i = 1; i < MCS_SIZE; i++) {
            Bdisp_AllClr_VRAM();

            EnableStatusArea(1);
            DisplayStatusArea();

            printTitle("Delete all empty MCS_Directories", 35);

            char buf[64];
            strcpy(buf, "Deleting ");
            strcat(buf, dir->name);

            int x = 60, y = 30;
            PrintMini(&x, &y, buf, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);  // Name

            memset(buf, 0, 64);

            char i_str[16];
            itoa(i, i_str);
            strcpy(buf, i_str);

            strcat(buf, "/");

            char max_str[16];
            itoa(MCS_SIZE, max_str);
            strcat(buf, max_str);

            x = 60, y = y + 30;
            PrintMini(&x, &y, buf, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);  // Progress i/max

            Bdisp_Rectangle(63, 98, 318, 118, COLOR_BLACK);
            ProgressBar2("", i, MCS_SIZE);
            Bdisp_PutDisp_DD();

            int rc = MCS_GetDirectoryEntryByNumber(i, &dir);
            if (rc == 0) {
                MCS_DeleteDirectory(dir->name);
            }
        }
    }
}