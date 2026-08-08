#include "deleteSingleDir.h"

#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "mcs_syscalls.h"
#include "util.h"

void errorMsg(char* errorcode, char* msg) {
    int key;
    MsgBoxPush(3);
    while (1) {
        int x = 50, y = 50;
        PrintMini(&x, &y, errorcode, 0, 0xFFFFFFFF, 0, 0, COLOR_RED, COLOR_WHITE, 1, 0);
        x = 50, y += 20;
        PrintMini(&x, &y, msg, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        x = 50, y = y += 30;
        PrintMini(&x, &y, "[EXE] OK", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        GetKey(&key);
        if (key == KEY_CTRL_EXE) break;
    }
    MsgBoxPop();
}

void deleteSingleDir() {
    TMainMemoryDirectoryEntry* dir;
    int current = 1;
    int key;
    while (1) {
        Bdisp_AllClr_VRAM();
        EnableStatusArea(1);
        DisplayStatusArea();

        printTitle("Delete single directory", 75);

        int rc = MCS_GetDirectoryEntryByNumber(current, &dir);

        int x = 0, y = 0;
        if (rc == 0) {
            x = 0, y += 20;
            // create null terminated string otherwise there is garbage at the end when using full length
            char name[9];
            memcpy(name, dir->name, 8);
            name[8] = '\0';
            if (dir->name[0] != '\0') {
                PrintMini(&x, &y, name, 0, 0xFFFFFFFF, 0, 0, COLOR_BLUE, COLOR_WHITE, 1, 0);  // Name
            } else {
                PrintMini(&x, &y, "(Empty)", 0, 0xFFFFFFFF, 0, 0, COLOR_LIGHTBLUE, COLOR_WHITE, 1, 0);  // empty Name
            }

            char buffer[64];
            char count_str[64];
            itoa(dir->count, count_str);
            strcpy(buffer, "Item count: ");
            strcat(buffer, count_str);

            x = 0, y += 20;
            PrintMini(&x, &y, buffer, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);  // Count

            x = 0, y += 20;
            memset(buffer, 64, 0);
            char addr_str[16];
            LongToAscHex(dir->addr, addr_str, 8);
            strcpy(buffer, "Address: 0x");
            strcat(buffer, addr_str);
            PrintMini(&x, &y, buffer, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);  // Address

            x = 0, y += 20;
            char flag_str[12];
            for (int i = 0; i < 4; i++) {
                ByteToHex((unsigned char)dir->flags[i], (unsigned char*)&flag_str[i * 3]);  // write 2 hex chars to 0, 3, 6, ...
                flag_str[(i * 3) + 2] = ' ';                                                // add space after 2 hex chars
            }
            flag_str[11] = '\0';

            strcpy(buffer, "Flags: ");
            strcat(buffer, flag_str);
            PrintMini(&x, &y, buffer, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);  // Flags
        }

        x = 0, y = 130;

        char buffer[32];
        char current_str[16];

        x = 0, y += 20;
        itoa(current, (unsigned char*)current_str);

        strcpy(buffer, current_str);
        strcat(buffer, "/");
        strcat(buffer, "147");
        PrintMini(&x, &y, buffer, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);  // Dir Num

        x = 0, y += 20;
        PrintMini(&x, &y, "[F1] Items | [EXE] Delete | [<][>] Browse", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        GetKey(&key);

        if (key == KEY_CTRL_EXIT) {
            break;
        }
        if (key == KEY_CTRL_LEFT) {
            if (current > 1)
                current--;
        }
        if (key == KEY_CTRL_RIGHT) {
            if (current < MCS_SIZE)
                current++;
        }
        if (key == KEY_CTRL_EXE) {  // Delete item
            MsgBoxPush(3);
            int confirm = 0;
            while (1) {
                x = 50, y = 60;
                PrintMini(&x, &y, "Delete this directory?", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
                x = 50, y = y + 30;
                PrintMini(&x, &y, "[EXE] Yes | [EXIT] No", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

                Bdisp_PutDisp_DD();

                GetKey(&key);
                if (key == KEY_CTRL_EXE) {
                    confirm = 1;
                    break;
                } else if (key == KEY_CTRL_EXIT)
                    break;
            }
            MsgBoxPop();

            if (confirm) {
                int rc = MCS_DeleteDirectory(dir->name);
                if (rc == 0x45) {  // directory not empty
                    TItemEntry* items = (TItemEntry*)dir->addr;
                    for (int i = dir->count - 1; i >= 0; i--) {
                        MCSDelVar2(dir->name, items[i].name);
                    }
                    MCS_DeleteDirectory(dir->name);  // delete empty dir now
                } else if (rc == 0x46) {
                    errorMsg("Error 0x46:", "System dir can't be deleted.");
                } else if (rc == 0xF0) {
                    errorMsg("Error 0xF0:", "Directory is empty.");
                } else if (rc == 0x40) {
                    errorMsg("Error 0x40:", "Directory doesn't exist.");
                } else if (rc == 0x47) {
                    errorMsg("Error 0x47:", "Certain dir flags are set.");
                }
            }
        }
        if (key == KEY_CTRL_F1) {  // Show items
            if (dir->count == 0) {
                errorMsg("Error: ", "No items in this directory.");
                continue;
            }

            // Get items
            TItemEntry* items = (TItemEntry*)dir->addr;
            ItemDisplay item_display[dir->count];
            for (int i = 0; i < dir->count; i++) {
                memcpy(item_display[i].name, items[i].name, 8);
                item_display[i].name[8] = '\0';

                char len_str[5];
                itoa(items[i].length, (unsigned char*)len_str);
                memcpy(item_display[i].length, len_str, 5);
                item_display[i].length[5] = '\0';

                char flag_str[12];
                for (int i = 0; i < 4; i++) {
                    ByteToHex((unsigned char)dir->flags[i], (unsigned char*)&flag_str[i * 3]);  // write 2 hex chars to 0, 3, 6, ...
                    flag_str[(i * 3) + 2] = ' ';                                                // add space after 2 hex chars
                }
                flag_str[11] = '\0';
                memcpy(item_display[i].flags, flag_str, 12);
            }

            int total_lines = dir->count;
            int visible_lines = 5;
            int scroll_pos = 0;
            struct scrollbar sb;
            sb.I1 = 0;
            sb.indicatormaximum = total_lines;
            sb.indicatorheight = visible_lines;
            sb.indicatorpos = scroll_pos;
            sb.I5 = 0;
            sb.barleft = 342;
            sb.bartop = 70;
            sb.barheight = 95;
            sb.barwidth = 6;

            Scrollbar(&sb);

            int scroll = 0;

            MsgBoxPush(6);
            while (1) {
                fillArea(40, 85, 300, 105, COLOR_WHITE);
                int x = 40, y = 25;
                PrintMini(&x, &y, "Items:", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLUE, COLOR_WHITE, 1, 0);
                y = 45;

                x = 40;
                PrintMini(&x, &y, "Name", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
                x = 152;
                PrintMini(&x, &y, "Len", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
                x = 222;
                PrintMini(&x, &y, "Flags", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
                for (int i = 0; i < visible_lines && scroll + i < total_lines; i++) {
                    int y = 70 + i * 20;
                    int x = 40;
                    PrintMini(&x, &y, item_display[scroll + i].name, 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);  // Name
                    x = 152;
                    PrintMini(&x, &y, item_display[scroll + i].length, 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);  // Length
                    x = 222;
                    PrintMini(&x, &y, item_display[scroll + i].flags, 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);  // Flags
                }

                sb.indicatorpos = scroll;
                Scrollbar(&sb);

                GetKey(&key);
                if (key == KEY_CTRL_EXIT) {
                    break;
                }
                if (key == KEY_CTRL_DOWN && scroll < total_lines - visible_lines)
                    scroll++;

                if (key == KEY_CTRL_UP && scroll > 0) {
                    scroll--;
                }
            }
        }
        MsgBoxPop();
    }
}