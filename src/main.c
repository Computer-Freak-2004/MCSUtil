#include <fxcg/display.h>
#include <fxcg/file.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "createDirTest.h"

int main() {
    Bdisp_EnableColor(0);

    char* items[] = {
        "CreateDir Test",
        "Delete single Dir"
        "Delete all Dirs",
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

        for (int i = 0; i < 4; i++) {
            int x = 0, y = 20 + i * 20;
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

       
        if (key == KEY_CTRL_EXE && selected == 0) {
            createDirTest();
        }

         if (key == KEY_CTRL_EXE && selected == 1) {
            MCS_GetDirectoryEntryByNumber( 0, 1);
        }
        if (key == KEY_CTRL_EXE && selected == 3) {
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
        if (key == KEY_CTRL_EXE && selected == 4) {
            return 0;
        }
    }
}