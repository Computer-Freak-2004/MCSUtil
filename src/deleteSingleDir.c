#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "createDirTest.h"
#include "mcs_syscalls.h"

void deleteSingleDir() {
    TMainMemoryDirectoryEntry* dir;
    int current = 1;
    int key;
    while (1) {
        Bdisp_AllClr_VRAM();
        EnableStatusArea(1);
        DisplayStatusArea();
        int x = 0, y = 0;

        int rc = MCS_GetDirectoryEntryByNumber(current, &dir);

        if (rc == 0) {
            PrintMini(&x, &y, dir->name, 0, 0xFFFFFFFF, 0, 0, COLOR_BLUE, COLOR_WHITE, 1, 0);
        }

        char buf[16];
        itoa(rc, (unsigned char*)buf);
        x = 0, y = 130;
        PrintMini(&x, &y, buf, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        memset(buf, 0, 16);
        x = 0, y += 20;
        itoa(current, (unsigned char*)buf);

        char num_str[32];
        strcpy(num_str, "No: ");
        strcat(num_str, buf);
        PrintMini(&x, &y, num_str, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        x = 0, y += 20;
        PrintMini(&x, &y, "[EXE] Delete Entry | [<] [>] Browse", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        Bdisp_PutDisp_DD();
        GetKey(&key);

        if (key == KEY_CTRL_EXIT) {
            break;
        }
        if (key == KEY_CTRL_LEFT) {
            if (current > 1)
                current--;
        }
        if (key == KEY_CTRL_RIGHT) {
            if (current < 0x93)
                current++;
        }
        if (key == KEY_CTRL_EXE) {
            while (1) {
                MsgBoxPush(3);

                x = 50, y = 60;
                PrintMini(&x, &y, "Delete this entry?", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
                x = 50, y = y + 30;
                PrintMini(&x, &y, "[EXE] Yes | [EXIT] No", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

                Bdisp_PutDisp_DD();

                GetKey(&key);
                MsgBoxPop();
                if (key == KEY_CTRL_EXE) {
                    MCS_DeleteDirectory(dir->name);
                    break;
                } else if (key == KEY_CTRL_EXIT)
                    break;
            }
        }
    }
}