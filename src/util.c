#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

void printTitle(char* str, int x) {
    int y = 0;
    PrintMini(&x, &y, str, 0, 0xFFFFFFFF, 0, 0, COLOR_BLUE, COLOR_WHITE, 1, 0);

    for (x = 0; x < LCD_WIDTH_PX; x++) {
        Bdisp_SetPoint_VRAM(x, 42, COLOR_BLACK);
    }
}
