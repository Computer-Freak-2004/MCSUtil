// MCS Test by Computer_Freak_2004
// Enter a name for a MCS Directory and see the result, @ gets added to the front of the input
// 06.08.2026

#include <fxcg/display.h>
#include <fxcg/file.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "createDirTest.h"

int main() {
    Bdisp_EnableColor(0);
    int selected = 0;
    int key;

    char* items[] = {
        "CreateDir Test",
        "Delete all Dirs",
        "About",
        "Exit"};

    while (1) {
        Cursor_SetFlashOff();
        Bdisp_AllClr_VRAM();

        EnableStatusArea(1);
        DisplayStatusArea();

        for (int i = 0; i < 4; i++) {
            int x = 0, y = 20 + i * 20;
            if (i == selected)
                PrintMini(&x, &y, items[i], 0, 0xFFFFFFFF, 0, 0, COLOR_BLUE, COLOR_WHITE, 1, 0);
            else
                PrintMini(&x, &y, items[i], 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        }

        Bdisp_PutDisp_DD();

        GetKey(&key);

        if (key == KEY_CTRL_UP)
            selected = (selected + 3) % 4;

        if (key == KEY_CTRL_DOWN)
            selected = (selected + 1) % 4;

        if (key == KEY_CTRL_EXE && selected == 0) {
            createDirTest();
        }
        if (key == KEY_CTRL_EXE && selected == 2) {
            while (1) {
                Bdisp_AllClr_VRAM();
                EnableStatusArea(1);
                DisplayStatusArea();

                int x = 0, y = 0;
                PrintMini(&x, &y, "About Text", 0, 0xFFFFFFFF, 0, 0, COLOR_BLUE, COLOR_WHITE, 1, 0);
                GetKey(&key);
                if (key == KEY_CTRL_EXIT) {
                    break;
                }
            }
        }
        if (key == KEY_CTRL_EXE && selected == 3) {
            return 0;
        }
    }
}