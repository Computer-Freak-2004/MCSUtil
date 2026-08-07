#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "createDirTest.h"
#include "deleteAllDirs.h"
#include "deleteSingleDir.h"
#include "dirSpaceUsage.h"
#include "help.h"
#include "mcs_syscalls.h"
#include "openTestMode.h"
#include "util.h"

int main() {
    Bdisp_EnableColor(1);

    char* items[] = {
        "MCS_CreateDirectory Test",
        "Delete single MCS directory",
        "Delete all empty MCS directories",
        "Directory space usage",
        "Test Mode",
        "Help",
        "About",
        "Exit"};

    int selected = 0;
    int count = sizeof(items) / sizeof(items[0]);
    int key;

    while (1) {
        Cursor_SetFlashOff();
        Bdisp_AllClr_VRAM();

        EnableStatusArea(1);
        DisplayStatusArea();

        PrintXY(1, 1, "       MCS Utility", 0x20, TEXT_COLOR_BLUE);
        drawHLine(52);

        for (int i = 0; i < count; i++) {
            int x = 0, y = 32 + i * 20;
            char menustr[64];
            if (i == selected) {
                strcpy(menustr, "> ");
                strcat(menustr, items[i]);
                PrintMini(&x, &y, menustr, 0, 0xFFFFFFFF, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);
            } else {
                strcpy(menustr, "  ");
                strcat(menustr, items[i]);
                PrintMini(&x, &y, menustr, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
            }
        }
        Bdisp_PutDisp_DD();

        GetKey(&key);
        if (key == KEY_CTRL_UP)
            selected = (selected + count - 1) % count;

        if (key == KEY_CTRL_DOWN)
            selected = (selected + count + 1) % count;
        if (key == KEY_CTRL_EXE && selected == 0)  // Create Dir test
            createDirTest();
        if (key == KEY_CTRL_EXE && selected == 1)  // Delete single dir
            deleteSingleDir();
        if (key == KEY_CTRL_EXE && selected == 2)  // Delete all dirs
            deleteAllDirs();
        if (key == KEY_CTRL_EXE && selected == 3) {  // Dir Space usage
           dirSpaceUsage();
        }

        if (key == KEY_CTRL_EXE && selected == 4) {  // Test Mode
            openTestMode();
        }
        if (key == KEY_CTRL_EXE && selected == 5) {  // Help
            help();
        }
        if (key == KEY_CTRL_EXE && selected == 6) {  // About
        }

        if (key == KEY_CTRL_EXE && selected == 7) {  // Exit
            return 0;
        }
    }
}