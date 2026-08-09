#include "deleteSingleDir.h"

#include <fxcg/display.h>
#include <fxcg/file.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "mcs_syscalls.h"
#include "util.h"

int x = 0, y = 0;
static int key;
TMainMemoryDirectoryEntry* dir;
ItemDisplay items[MCS_SIZE];

// Menu
int top_entry = 0;
int current = 0;

int row_count = 0;
int item_count = 0;

int items_top_entry = 0;
int items_current = 0;

int items_row_count = 0;
int items_item_count = 0;

void deleteItem() {
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
        int rc = MCS_GetDirectoryEntryByNumber(current + 1, &dir);
        if (rc != 0) {
            return;
        }

        rc = MCS_DeleteDirectory(dir->name);
        if (rc == 0x45) {  // directory not empty
            TItemEntry* items = (TItemEntry*)dir->addr;
            for (int i = dir->count - 1; i >= 0; i--) {
                MCSDelVar2(dir->name, items[i].name);
            }
            MCS_DeleteDirectory(dir->name);  // delete empty dir now
        } else if (rc == 0x46) {
            errorMsg("Error 0x46:", "System dir can't be deleted.", COLOR_RED);
        } else if (rc == 0xF0) {
            errorMsg("Error 0xF0:", "Directory is empty.", COLOR_RED);
        } else if (rc == 0x40) {
            errorMsg("Error 0x40:", "Directory doesn't exist.", COLOR_RED);
        } else if (rc == 0x47) {
            errorMsg("Error 0x47:", "Certain dir flags are set.", COLOR_RED);
        }
    }
}

void showItems() {
    int rc = MCS_GetDirectoryEntryByNumber(current + 1, &dir);
    if (rc != 0) {
        return;
    }

    if (dir->count == 0) {
        errorMsg("Error: ", "No items in this directory.", COLOR_RED);
        return;
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

    items_top_entry = 0;
    items_current = 0;

    items_row_count = 5;
    items_item_count = 0;

    MsgBoxPush(6);
    while (1) {
        fillArea(40, 85, 300, 109, COLOR_WHITE);

        int x = 40, y = 25;
        PrintMini(&x, &y, "Items:", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLUE, COLOR_WHITE, 1, 0);

        x = 40, y = 45;
        PrintMini(&x, &y, "Name", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        x = 152;
        PrintMini(&x, &y, "Len", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        x = 222;
        PrintMini(&x, &y, "Flags", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        drawMenu(40, 70, 300, items_row_count, 0, item_display, dir->count, items_top_entry, items_current, 1, 0, 0, 112, 0, 182);
        GetKey(&key);
        GetKeyMenu(key, &items_current, &items_top_entry, dir->count, items_row_count);
        if (key == KEY_CTRL_EXIT) {
            break;
        }
    }
    MsgBoxPop();
}

void getDirList() {
    for (int i = 1; i <= MCS_SIZE; i++) {
        int rc = MCS_GetDirectoryEntryByNumber(i, &dir);
        if (rc != 0) {
            strcpy(items[i - 1].name, "");
            continue;
        }

        // create null terminated string otherwise there is garbage at the end when using full length
        char name[9];
        memcpy(name, dir->name, 8);
        name[8] = '\0';
        if (name[0] != '\0') {
            strcpy(items[i - 1].name, name);  // Name
        } else {
            strcpy(items[i - 1].name, "(Empty)");  // Empty name
        }

        char len_str[5];
        itoa(dir->count, (unsigned char*)len_str);
        memcpy(items[i - 1].length, len_str, 5);
        items[i - 1].length[5] = '\0';  // Length

        char addr_str[11];
        char addr[8];
        LongToAscHex((int)dir->addr, (unsigned char*)addr, 8);
        strcpy(addr_str, "0x");
        strcat(addr_str, (char*)addr);
        addr_str[10] = '\0';
        memcpy(items[i - 1].address, addr_str, 11);  // Address

        char flag_str[7];
        for (int i = 0; i < 2; i++) {
            ByteToHex((unsigned char)dir->flags[i], (unsigned char*)&flag_str[i * 3]);  // write 2 hex chars to 0, 3
            flag_str[(i * 3) + 2] = ' ';                                                // add space after 2 hex chars
        }
        flag_str[6] = '\0';
        memcpy(items[i - 1].flags, flag_str, 6);  // Flags
    }
}

void deleteSingleDir() {
    Bdisp_AllClr_VRAM();

    current = 0;
    item_count = MCS_SIZE;
    row_count = 6;
    top_entry = 0;

    while (1) {
        Bdisp_AllClr_VRAM();
        EnableStatusArea(1);
        DisplayStatusArea();
        printTitle("Delete single directory", 75);

        getDirList();

        /*
        // Debug
        int x = 0, y = 20;
        char buf[32];
        char buf2[32];
        memset(buf, 0, 32);
        memset(buf2, 0, 32);
        itoa(top_entry, buf);
        itoa(current, buf2);
        PrintMini(&x, &y, "Top:", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        PrintMini(&x, &y, buf, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        PrintMini(&x, &y, " | Cur:", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        PrintMini(&x, &y, buf2, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        */

        x = 0, y = 20;
        PrintMini(&x, &y, "Name", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        x = 120;
        PrintMini(&x, &y, "Items", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        x = 190;
        PrintMini(&x, &y, "Address", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        x = 317;
        PrintMini(&x, &y, "Flags", 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        drawMenu(0, 43, 378, row_count, 0, items, item_count, top_entry, current, 1, 1, 0, 120, 190, 317);

        x = 0, y = 170;
        PrintMini(&x, &y, "[F1] Items | [EXE] Delete | [æ•][æ”] Browse", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        GetKey(&key);
        GetKeyMenu(key, &current, &top_entry, item_count, row_count);

        if (key == KEY_CTRL_EXIT) {
            break;
        }
        if (key == KEY_CTRL_EXE) {  // Delete item
            deleteItem();
        }
        if (key == KEY_CTRL_F1) {  // Show items
            showItems();
        }
    }
}