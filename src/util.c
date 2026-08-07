#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

void printTitle(char* str, int x) {
    int y = 0;
    PrintMini(&x, &y, str, 0, 0xFFFFFFFF, 0, 0, COLOR_BLUE, COLOR_WHITE, 1, 0);

    drawHLine(42);
}

void drawHLine(int y) {
    for (int x = 0; x < LCD_WIDTH_PX; x++) {
        Bdisp_SetPoint_VRAM(x, y, COLOR_BLACK);
    }
}

void fillArea(unsigned x, unsigned y, unsigned w, unsigned h, unsigned short col) {
    unsigned short* s = (unsigned short*)GetVRAMAddress();
    s += (y * 384) + x;
    while (h--) {
        unsigned w2 = w;
        while (w2--)
            *s++ = col;
        s += 384 - w;
    }
}
