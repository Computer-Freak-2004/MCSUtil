#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
static int key;

void help() {
    const char* text[] = {
        "This add-in allows for simple ",
        "management of MCS (Main Content ",
        "System) directories. All add-ins",
        "have an internal name (@CBASIC ",
        "for example) and a MCS directory",
        "with the same name (limited to",
        "to 8 chars) is created when the ",
        "add-in starts.",
        "",
        "There can only be 147 (0x93) ",
        "directory entries. If the limit is ",
        "reached, no new MCS directories ",
        "can be created. Add-ins with new ",
        "internal names don't have a ",
        "corresponding directory yet. Its ",
        "creation silently fails, causing the ",
        "add-in not to start.",
        "This utility helps to fix that.",
        "",
        "     == Create Directory Test ==",
        "You can enter a name to test if a ",
        "new MCS directory can be created.",
        "The '@' symbol gets automatically",
        "added to the front of your input.",
        "After entering a name, the return ",
        "code and its meaning are shown:",
        "of it:",
        "- 0: Success",
        "- 0x42: Already exists",
        "- 0x43: Dir space exhausted",
        "- anything else: Failure",
        "If you see 0x43, there is no space ",
        "left. Delete some directories to ",
        "allow add-ins with new internal ",
        "names to start again.",
        "",
        "      == Delete single directory ==",
        "This allows you to browse through",
        "and delete any directory.",
        "- [Êê] and [Êë] browse one entry left",
        "   or right",
        "- [Êì] and [Êí] go to the previous/ ",
        "   next non-empty entry",
        "Besides the directory name in blue,",
        "you can see its item count, the",
        "starting address of its items in",
        "memory, and its flags.",
        "If the current directory contains",
        "items, you can view them with [F1].",
        "The name, length in bytes, and flags",
        "of each item are shown.",
        "Press [EXE] to delete the current",
        "directory entirely. The following",
        "return codes are possible:",
        "- 0: Success",
        "- 0xF0: Dir is null",
        "- 0x40: Dir doesn't exist",
        "- 0x45: Dir not empty",
        "- 0x46: Undeletable system dir ",
        "- 0x47: Certain flags are set",
        "",
        "   == Delete all empty directories ==",
        "This deletes all empty directories.",
        "This is useful for quickly freeing",
        "space when the directory space is",
        "exhausted and new add-ins don't",
        "start.",
        "",
        "       == Directory space usage ==",
        "Shows how many of the 147 directory",
        "entries are currently in use. It's",
        "impossible to reach 0% usage as",
        "there are system directories that",
        "can't be deleted:",
        "- @APLWORK",
        "- main",
        "- @LOCAL1..@LOCAL40 ",
        "- @REV3..@REV1",
        "- library",
        "- system",

        "",
        "               == Test Mode ==",
        "Opens a secret menu that is usually",
        "accessed like this: ",
        "1. Power off the calculator. ",
        "2. Hold down [OPTN] + [x10^] + ",
        "    [AC/ON] together. ",
        "3. On the popup enter the sequence",
        "    5, 9, 6, 3.",
        "Press [1] to enter Valiable",
        "Manager. You can now browse",
        "through the directories and their",
        "items.",
    };

    int total_lines = sizeof(text) / sizeof(text[0]);
    int visible_lines = 8;
    int scroll_pos = 0;
    struct scrollbar sb;
    sb.I1 = 0;
    sb.indicatormaximum = total_lines;
    sb.indicatorheight = visible_lines;
    sb.indicatorpos = scroll_pos;
    sb.I5 = 0;
    sb.barleft = 378;
    sb.bartop = 20;
    sb.barheight = 170;
    sb.barwidth = 6;

    Scrollbar(&sb);

    int scroll = 0;

    while (1) {
        Bdisp_AllClr_VRAM();
        EnableStatusArea(1);
        DisplayStatusArea();

        printTitle("Help", 160);

        for (int i = 0; i < visible_lines && scroll + i < total_lines; i++) {
            int x = 0;
            int y = 20 + i * 20;
            PrintMini(&x, &y, text[scroll + i], 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
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