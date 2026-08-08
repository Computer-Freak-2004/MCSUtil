#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "../img/icon.h"
#include "util.h"

static int key;

void about() {
    Bdisp_AllClr_VRAM();
    EnableStatusArea(1);
    DisplayStatusArea();

    printTitle("About", 160);

    VRAM_CopySprite(icon, 20, 45, 92, 64);

    PrintXY(1, 2, "          MCS Utility", 0x20, TEXT_COLOR_BLUE);

    int x = 155, y = 50;
    PrintMini(&x, &y, "for Casio fx-CG50", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLUE, COLOR_WHITE, 1, 0);
    
    drawHLine(100);

    x = 0, y = 90;
    PrintMini(&x, &y, "Created by: Computer_Freak_2004", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    x = 0, y +=20;
    PrintMini(&x, &y, "Version: @", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    PrintMini(&x, &y, GIT_COMMIT, 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    x = 0, y +=20;
    PrintMini(&x, &y, "Date: 06.08. - 08.08.2026", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    x = 0, y +=20;
    PrintMini(&x, &y, "Buildtime: ", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    PrintMini(&x, &y, __DATE__, 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    PrintMini(&x, &y, " ", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    PrintMini(&x, &y, __TIME__, 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    x = 0, y +=25;
    PrintMiniMini(&x, &y, "https://github.com/Computer-Freak-2004/MCSUtil/", 0, TEXT_COLOR_BLUE, 0);

    Bdisp_PutDisp_DD();

    while (1) {
        GetKey(&key);
        if (key == KEY_CTRL_EXIT) {
            break;
        }
    }
}