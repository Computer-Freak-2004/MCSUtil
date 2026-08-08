#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "about.h"
#include "createDirTest.h"
#include "deleteAllDirs.h"
#include "deleteSingleDir.h"
#include "dirSpaceUsage.h"
#include "help.h"
#include "mcs_syscalls.h"
#include "openTestMode.h"
#include "util.h"

static int key;

int main() {
    Bdisp_EnableColor(1);

    char* items[] = {
        "Create Directory Test                            ",
        "Delete single directory                          ",
        "Delete all empty directories                     ",
        "Directory space usage                            ",
        "Test Mode                                        ",
        "Help                                             ",
        "About                                            ",
        "Exit                                             "};

    int selected = 0;
    int count = sizeof(items) / sizeof(items[0]);

    while (1) {
        Cursor_SetFlashOff();
        Bdisp_AllClr_VRAM();

        EnableStatusArea(1);
        DisplayStatusArea();

        PrintXY(1, 1, "       MCS Utility", 0x20, TEXT_COLOR_BLUE);
        drawHLine(50);

        for (int i = 0; i < count; i++) {
            int x = 0, y = 30 + i * 20;
            if (i == selected) {
                PrintMini(&x, &y, "æ›  ", 0, 0xFFFFFFFF, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);
                x = 15;
                PrintMini(&x, &y, items[i], 0, 0xFFFFFFFF, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);
            } else {
                PrintMini(&x, &y, "   ", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
                x = 15;
                PrintMini(&x, &y, items[i], 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
            }
        }

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
            about();
        }
        if (key == KEY_CTRL_EXE && selected == 7) {  // Exit
            Bdisp_AllClr_VRAM();
            int x = 70, y = 60;
            PrintMini(&x, &y, "This add-in has exited.", 0, 0xFFFFFFFF, 0, 0, COLOR_WHITE, COLOR_RED, 1, 0);
            x = 18, y += 30;
            PrintMini(&x, &y, "Start another add-in and then this", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
            x = 125, y += 20;
            PrintMini(&x, &y, "one again. ;)", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

            return 0;
        }
    }
}