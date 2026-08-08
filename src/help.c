#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

void help() {
    int key;

    const char* text[] = {
        "This add-in allows for simple ",
        "management of the MCS or the",
        "Main Content System directories.",
        "All add-ins have an internal name",
        "(@CBASIC for example) and a MCS ",
        "directory with the same name",
        "(limited to 8 chars) gets created",
        "when the add-in starts.",
        "",
        "There can only be a 147 (0x93)",
        "directory entries. If the limit",
        "is reached no new MCS directories",
        "can be created. Add-ins with new ",
        "internal names don't have a",
        "corresponding directory yet. The",
        "creation silently fails and the new",
        "add-in doesn't start.",
        "This utility helps to fix that.",
        "",
        "     == Create Directory Test ==",
        "You can enter a name and test if you ",
        "can create a new MCS directory. ",
        "The '@' symbol gets automatically",
        "added to the front of your input.",
        "After entering a name you can see",
        "the return code and the meaning",
        "of it:",
        "- 0: Success",
        "- 0x42: Already exists",
        "- 0x43: Dir space exhausted",
        "- anything else: Failure",
        "If you see 0x43 it means there is",
        "no space left and you should delete",
        "some directories to allow add-ins",
        "with new internal names to start",
        "again.",
        "",
        "      == Delete single directory ==",
        "This allows you to browse the list",
        "of all directories. Besides the name",
        "in blue, you can see the number of",
        "items in each dir, the starting",
        "address of the items in memory and ",
        "the flags. If there are items in the",
        "current directory you can view them",
        "with [F1]. The name, length in Bytes",
        "and the flags of each item are ",
        "shown.",
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
        "       == Delete all directories ==",
        "This will delete all directories",
        "that are empty. This is useful for",
        "quickly making space when the dir",
        "space is exhausted and new add-ins",
        "don't start.",
        "",
        "       == Directory space usage ==",
        "Shows the currently used amount ",
        "out of 147 directories. It's ",
        "impossible to have 0% usage as there",
        "are system directories that can't be",
        "deleted: system, library, main, ",
        "@REV3..@REV1, @LOCAL40..@LOCAL1, ",
        "@APLWORK" 
        "",
        "               == Test Mode ==",
        "Opens a secret menu that is usually",
        "accessed like this: Power off",
        "the calculator. After that hold",
        "down [OPTN] + [x10^] + [AC/ON] ",
        "together. Then on the popup enter",
        "the sequence 5, 9, 6, 3.",
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