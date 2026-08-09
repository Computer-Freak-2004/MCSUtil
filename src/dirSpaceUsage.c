#include <fxcg/display.h>
#include <fxcg/file.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "mcs_syscalls.h"
#include "util.h"

#define MAIN_MEM_SIZE 0xFA30

void checkUsage(int* usage, int* itemCount) {
    TMainMemoryDirectoryEntry* dir;
    *usage = 0;
    *itemCount = 0;

    int emptyDirs = 0;
    for (int i = 0; i <= MCS_SIZE; i++) {
        int rc = MCS_GetDirectoryEntryByNumber(i, &dir);
        if (rc != 0)
            continue;

        if (dir->name[0] == '\0')
            emptyDirs++;

        *itemCount += dir->count;
    }
    *usage = MCS_SIZE - emptyDirs;
}

void dirSpaceUsage() {
    int key;
    while (1) {
        Bdisp_AllClr_VRAM();

        EnableStatusArea(1);
        DisplayStatusArea();
        printTitle("Directory space usage", 80);

        int usage = 0;
        int itemCount = 0;
        checkUsage(&usage, &itemCount);
        int percent = (usage * 100) / MCS_SIZE;

        char buffer[32];

        char usage_str[16];
        char percent_str[16];
        itoa(usage, (unsigned char*)usage_str);
        itoa(percent, (unsigned char*)percent_str);
        strcpy(buffer, usage_str);
        strcat(buffer, "/");
        strcat(buffer, "147 (");
        strcat(buffer, percent_str);
        strcat(buffer, "%)");

        int width = strlen(buffer) * 12;
        int x = (LCD_WIDTH_PX - width) / 2;
        int y = 50;
        PrintMini(&x, &y, buffer, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);  // usage

        if (usage == MCS_SIZE) {
            x = 120, y += 20;
            PrintMini(&x, &y, "No space left!", 0, 0xFFFFFFFF, 0, 0, COLOR_RED, COLOR_WHITE, 1, 0);
        }

        x = 0, y = 150;
        char itemCount_str[32];
        itoa(itemCount, (unsigned char*)itemCount_str);
        PrintMini(&x, &y, "Total items: ", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);  // total Items
        PrintMini(&x, &y, itemCount_str, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        int mcs_start = 0;
        int mcs_bottom = 0;
        mcs_start = MCS_GetMainMemoryStart();
        MCS_GetCapa(&mcs_bottom);

        int mcs_usage = mcs_bottom - mcs_start;
        int mcs_free = MAIN_MEM_SIZE - mcs_usage;
        x = 0, y += 20;
        char mcsFree_str[32];
        itoa(mcs_free, (unsigned char*)mcsFree_str);
        PrintMini(&x, &y, "MCS free: ", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);  // MCS free
        PrintMini(&x, &y, mcsFree_str, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        PrintMini(&x, &y, " Byte", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        Bdisp_Rectangle(63, 98, 320, 118, COLOR_BLACK);
        ProgressBar2((unsigned char*)"", usage, MCS_SIZE);

        GetKey(&key);
        if (key == KEY_CTRL_EXIT) {
            break;
        }
    }
}