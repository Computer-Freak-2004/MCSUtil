#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "mcs_syscalls.h"
#include "util.h"

static int key;

void deleteAllDirs() {
    Bdisp_AllClr_VRAM();

    EnableStatusArea(1);
    DisplayStatusArea();
    printTitle("Delete all empty directories", 55);

    int confirm = 0;
    MsgBoxPush(3);
    while (1) {
        int x = 45, y = 60;
        PrintMini(&x, &y, "Delete ALL empty directories?", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        x = 45, y = y + 30;
        PrintMini(&x, &y, "[EXE] Yes | [EXIT] No", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

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
        Bdisp_AllClr_VRAM();
        EnableStatusArea(1);
        DisplayStatusArea();
        printTitle("Delete all empty directories", 55);

        MsgBoxPush(5);
        HourGlass();
        for (int i = 1; i <= MCS_SIZE; i++) {
            fillArea(60, 60, LCD_WIDTH_PX - 100, 45, COLOR_WHITE);

            int rc = MCS_GetDirectoryEntryByNumber(i, &dir);

            char name[9];
            if (dir->name[0] != '\0') {
                // create null terminated string otherwise there is garbage at the end when using full length
                memcpy(name, dir->name, 8);
                name[8] = '\0';

            } else {
                strcpy(name, "(Empty)");
            }

            int x = 65, y = 40;
            PrintMini(&x, &y, "Deleting ", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);  // Name
            if (strcmp(name, "(Empty)") == 0) {
                PrintMini(&x, &y, name, 0, 0xFFFFFFFF, 0, 0, COLOR_LIGHTGRAY, COLOR_WHITE, 1, 0);
            } else {
                PrintMini(&x, &y, name, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
            }

            x = 65, y = y + 20;
            char i_str[16];
            itoa(i, (unsigned char*)i_str);
            char max_str[16];
            itoa(MCS_SIZE, (unsigned char*)max_str);

            PrintMini(&x, &y, i_str, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);  // Progress i/max
            PrintMini(&x, &y, "/", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0); 
            PrintMini(&x, &y, max_str, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

            Bdisp_Rectangle(63, 98, 320, 118, COLOR_BLACK);
            ProgressBar2((unsigned char*)"", i, MCS_SIZE);

            if (rc == 0) {
                MCS_DeleteDirectory(dir->name);
            }
        }
    }
    MsgBoxPop();
}