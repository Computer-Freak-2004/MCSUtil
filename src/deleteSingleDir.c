#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "mcs_syscalls.h"
#include "util.h"

void deleteSingleDir() {
    TMainMemoryDirectoryEntry* dir;
    int current = 1;
    int key;
    while (1) {
        Bdisp_AllClr_VRAM();
        EnableStatusArea(1);
        DisplayStatusArea();

        printTitle("== Delete single MCS_Directory ==", 25);

        int rc = MCS_GetDirectoryEntryByNumber(current, &dir);

        int x = 0, y = 0;
        if (rc == 0) {
            x = 0, y += 20;
            PrintMini(&x, &y, dir->name, 0, 0xFFFFFFFF, 0, 0, COLOR_BLUE, COLOR_WHITE, 1, 0); // Name

            char buffer[64];
            char count_str[64];
            itoa(dir->count, count_str);
            strcpy(buffer, "Item count: ");
            strcat(buffer, count_str);

            x = 0, y += 20;
            PrintMini(&x, &y, buffer, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0); // Count
        }

        char buf[16];
        itoa(rc, (unsigned char*)buf);
        x = 0, y = 130;
        PrintMini(&x, &y, buf, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0); // Return Code

        memset(buf, 0, 16);
        x = 0, y += 20;
        itoa(current, (unsigned char*)buf);

        char num_str[32];
        strcpy(num_str, "No: ");
        strcat(num_str, buf);
        PrintMini(&x, &y, num_str, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0); // Dir Num

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
                PrintMini(&x, &y, "Delete this directory?", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
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