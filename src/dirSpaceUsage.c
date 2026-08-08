#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "mcs_syscalls.h"
#include "util.h"

void dirSpaceUsage() {
    int key;
    while (1) {
        Bdisp_AllClr_VRAM();

        EnableStatusArea(1);
        DisplayStatusArea();
        printTitle("Directory space usage", 80);
        TMainMemoryDirectoryEntry* dir;
        int emptyDirs = 0;
        int itemCount = 0;
        for (int i = 0; i <= MCS_SIZE; i++) {
            int rc = MCS_GetDirectoryEntryByNumber(i, &dir);
            if (rc != 0)
                continue;

            if (dir->name[0] == '\0')
                emptyDirs++;

            itemCount += dir->count;
        }
        int usage = MCS_SIZE - emptyDirs;
        int percent = (usage * 100) / MCS_SIZE;

        char buffer[32];
        char usage_str[16];
        char percent_str[16];
        itoa(usage, usage_str);
        itoa(percent, percent_str);
        strcpy(buffer, usage_str);
        strcat(buffer, "/");
        strcat(buffer, "147 (");
        strcat(buffer, percent_str);
        strcat(buffer, "%)");

        int width = strlen(buffer) * 12;
        int x = (LCD_WIDTH_PX - width) / 2;
        int y = 50;
        PrintMini(&x, &y, buffer, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        if (usage == MCS_SIZE) {
            x = 120, y += 20;
            PrintMini(&x, &y, "No space left!", 0, 0xFFFFFFFF, 0, 0, COLOR_RED, COLOR_WHITE, 1, 0);
        }

        x = 0, y = 170;
        memset(buffer, 0, 32);
        strcpy(buffer, "Total items: ");
        char itemCount_str[32];
        itoa(itemCount, itemCount_str);
        strcat(buffer, itemCount_str);
        PrintMini(&x, &y, buffer, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);

        Bdisp_Rectangle(63, 98, 320, 118, COLOR_BLACK);
        ProgressBar2("", usage, MCS_SIZE);

        GetKey(&key);
        if (key == KEY_CTRL_EXIT) {
            break;
        }
    }
}