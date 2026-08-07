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

        printTitle("Delete single MCS directory", 55);

        int rc = MCS_GetDirectoryEntryByNumber(current, &dir);

        int x = 0, y = 0;
        if (rc == 0) {
            x = 0, y += 20;
            if (dir->name[0] != '\0') {
                PrintMini(&x, &y, dir->name, 0, 0xFFFFFFFF, 0, 0, COLOR_BLUE, COLOR_WHITE, 1, 0);  // Name
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
            char buf[12];
            for (int i = 0; i < 4; i++) {
                ByteToHex((unsigned char)dir->flags[i], (unsigned char*)&buf[i * 3]);  // write 2 hex chars to 0, 3, 6, ...
                buf[(i * 3) + 2] = ' ';                                                // add space after 2 hex chars
            }
            buf[11] = '\0';

            strcpy(buffer, "Flags: ");
            strcat(buffer, buf);
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
        PrintMini(&x, &y, "[EXE] Delete Entry | [<] [>] Browse", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        //Bdisp_PutDisp_DD();
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
        if (key == KEY_CTRL_EXE) {
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
                    int x = 0, y = 0;
                    int count = dir->count;  // save orig count
                    // PrintMini(&x, &y, "Not empty", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
                    for (int i = 0; i < count; i++) {  // delete all items
                        MCS_GetDirectoryEntryByNumber(current, &dir);
                        TItemEntry* item = (TItemEntry*)dir->addr;

                        // x = 20, y += 20;
                        // PrintMini(&x, &y, item->name, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
                        rc = MCSDelVar2(dir->name, item->name);
                    }
                    int rc = MCS_DeleteDirectory(dir->name);  // delete empty dir now
                } else if (rc == 0x46) {
                    errorMsg("Error 0x46!", "System dir can't be deleted.");
                } else if (rc == 0xF0) {
                    errorMsg("Error 0xF0!", "Dir is empty.");
                } else if (rc == 0x40) {
                    errorMsg("Error 0x40!", "Dir doesn't exist.");
                } else if (rc == 0x47) {
                    errorMsg("Error 0x47!", "Certain dir flags are set.");
                }
            }
        }
    }
}