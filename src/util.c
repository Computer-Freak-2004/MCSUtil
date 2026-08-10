#include "util.h"

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
    s += (y * LCD_WIDTH_PX) + x;
    while (h--) {
        unsigned w2 = w;
        while (w2--)
            *s++ = col;
        s += LCD_WIDTH_PX - w;
    }
}

void errorMsg(char* errorcode, char* msg, int errorcolor) {
    int key;
    MsgBoxPush(3);
    while (1) {
        int x = 50, y = 50;
        PrintMini(&x, &y, errorcode, 0, 0xFFFFFFFF, 0, 0, errorcolor, COLOR_WHITE, 1, 0);
        x = 50, y += 20;
        PrintMini(&x, &y, msg, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        x = 50, y += 30;
        PrintMini(&x, &y, "[EXE] OK", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        GetKey(&key);
        if (key == KEY_CTRL_EXE) break;
    }
    MsgBoxPop();
}

#define SPACE_WIDTH 7
#define FONT_HEIGHT 20
void drawMenu(int x, int y, int width, int row_count, char* items_char[], ItemDisplay items_itemDisplay[], int item_count, int top_entry, int selected_entry, int useItemDisplay, int showAddress, int name_x, int length_x, int address_x, int flags_x, char arrow_symbol[2]) {
    for (int i = 0; i < row_count && top_entry + i < item_count; i++) {
        int idx = top_entry + i;

        int x_pos = x;
        int y_pos = y + i * FONT_HEIGHT;

        if (useItemDisplay) {
            if (idx == selected_entry) {
                // fill width of selected entry with inverted spaces
                int spaces = (width / SPACE_WIDTH);
                char filler[spaces + 1];
                memset(filler, ' ', spaces);
                filler[spaces] = '\0';

                PrintMini(&x_pos, &y_pos, filler, 0, LCD_WIDTH_PX, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);
                x_pos = x + name_x;
                if (strcmp(items_itemDisplay[idx].name, "(Empty)") == 0) {
                    PrintMini(&x_pos, &y_pos, items_itemDisplay[idx].name, 0, LCD_WIDTH_PX, 0, 0, COLOR_LIGHTGRAY, COLOR_BLACK, 1, 0);
                } else {
                    PrintMini(&x_pos, &y_pos, items_itemDisplay[idx].name, 0, LCD_WIDTH_PX, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);
                }
                x_pos = x + length_x;
                PrintMini(&x_pos, &y_pos, items_itemDisplay[idx].length, 0, LCD_WIDTH_PX, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);

                if (showAddress) {
                    x_pos = x + address_x;
                    PrintMini(&x_pos, &y_pos, items_itemDisplay[idx].address, 0, LCD_WIDTH_PX, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);
                }
                x_pos = x + flags_x;
                PrintMini(&x_pos, &y_pos, items_itemDisplay[idx].flags, 0, LCD_WIDTH_PX, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);
            } else {
                x_pos = x + name_x;
                if (strcmp(items_itemDisplay[idx].name, "(Empty)") == 0) {
                    PrintMini(&x_pos, &y_pos, items_itemDisplay[idx].name, 0, LCD_WIDTH_PX, 0, 0, COLOR_LIGHTGRAY, COLOR_WHITE, 1, 0);
                } else {
                    PrintMini(&x_pos, &y_pos, items_itemDisplay[idx].name, 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
                }
                x_pos = x + length_x;
                PrintMini(&x_pos, &y_pos, items_itemDisplay[idx].length, 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

                if (showAddress) {
                    x_pos = x + address_x;
                    PrintMini(&x_pos, &y_pos, items_itemDisplay[idx].address, 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
                }

                x_pos = x + flags_x;
                PrintMini(&x_pos, &y_pos, items_itemDisplay[idx].flags, 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
            }
        } else {
            if (idx == selected_entry) {
                // fill width of selected entry with inverted spaces
                int spaces = (width / SPACE_WIDTH);
                if (arrow_symbol) {
                    char filler_arrow[spaces + 1];  // 2: arrow, 1: \0, 2 less spaces because of arrow
                    memcpy(filler_arrow, arrow_symbol, 2);
                    memset(filler_arrow + 2, ' ', spaces - 2);
                    filler_arrow[spaces] = '\0';
                    PrintMini(&x_pos, &y_pos, filler_arrow, 0, LCD_WIDTH_PX, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);

                    x_pos = x + 15;
                    PrintMini(&x_pos, &y_pos, items_char[idx], 0, LCD_WIDTH_PX, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);
                } else {
                    char filler[spaces + 1];
                    memset(filler, ' ', spaces);
                    filler[spaces] = '\0';
                    PrintMini(&x_pos, &y_pos, filler, 0, LCD_WIDTH_PX, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);

                    x_pos = x;
                    PrintMini(&x_pos, &y_pos, items_char[idx], 0, LCD_WIDTH_PX, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);
                }
            } else {
                if (arrow_symbol) {
                    x_pos = x + 15;
                    PrintMini(&x_pos, &y_pos, items_char[idx], 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
                } else {
                    x_pos = x;
                    PrintMini(&x_pos, &y_pos, items_char[idx], 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
                }
            }
        }
    }

    if (item_count > row_count) {
        struct scrollbar sb;

        sb.I1 = 0;
        sb.I5 = 0;
        sb.indicatormaximum = item_count;
        sb.indicatorheight = row_count;
        sb.indicatorpos = top_entry;

        sb.barleft = x + width;
        sb.bartop = y;
        sb.barheight = FONT_HEIGHT * row_count - 2;
        sb.barwidth = 6;

        Scrollbar(&sb);
    }
}

void GetKeyMenu(int key, int* current, int* top_entry, int item_count, int row_count) {
    if (key == KEY_CTRL_UP) {
        (*current)--;
        if (*current < 0) {  // menu loop
            *current = item_count - 1;
            if (item_count > row_count) {
                *top_entry = item_count - row_count;
            }
        }

        // if current atleast 1 above top_entry scroll list up
        if (*current - *top_entry < 0 && item_count > row_count) {
            (*top_entry)--;
        }
    }
    if (key == KEY_CTRL_RIGHT) {       // + 1 page
        if (item_count > row_count) {  // scrolling needed: jump ahead by row_count
            *current = (*current + row_count) % item_count;
            *top_entry = *current - (*current % row_count);

            if (*top_entry > item_count - row_count)  // always full screen
                *top_entry = item_count - row_count;
        } else {  // no scrolling needed: jump to start or end
            if (*current == *top_entry) {
                *current = item_count - 1;
            } else {
                *current = 0;
            }
        }
    }
    if (key == KEY_CTRL_LEFT) {        // - 1 page
        if (item_count > row_count) {  // scrolling needed: jump back by row_count
            // Add item_count because modulo can return neg. result
            *current = ((*current - row_count) + item_count) % item_count;
            *top_entry = *current - (*current % row_count);

            if (*top_entry > item_count - row_count)  // always full screen
                *top_entry = item_count - row_count;
        } else {  // no scrolling needed: jump to start or end
            if (*current == *top_entry) {
                *current = item_count - 1;
            } else {
                *current = 0;
            }
        }
    }
    if (key == KEY_CTRL_DOWN) {
        (*current)++;
        if (*current > item_count - 1) {  // menu loop
            *current = 0;
            *top_entry = 0;
        }

        // if current is bigger/at top_entry + RowCount scroll list down
        if (*current >= *top_entry + row_count) {
            (*top_entry)++;
        }
    }
}

void debugMenu(int current, int top_entry, int item_count, int row_count) {
    // Debug
    int x = 30, y = 0;
    char buf[32];
    char buf2[32];
    char buf3[32];
    char buf4[32];
    itoa(top_entry, (unsigned char*)buf);
    itoa(current, (unsigned char*)buf2);
    itoa(item_count, (unsigned char*)buf3);
    itoa(row_count, (unsigned char*)buf4);
    PrintMini(&x, &y, "Top:", 0x40, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    PrintMini(&x, &y, buf, 0x40, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    PrintMini(&x, &y, " | Cur:", 0x40, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    PrintMini(&x, &y, buf2, 0x40, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    PrintMini(&x, &y, " | Item:", 0x40, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    PrintMini(&x, &y, buf3, 0x40, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    PrintMini(&x, &y, " | Row:", 0x40, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    PrintMini(&x, &y, buf4, 0x40, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
}