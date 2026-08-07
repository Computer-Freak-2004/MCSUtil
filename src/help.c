#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

void help() {
    int key;

    char* text[] = {
        "Allows simple management of MCS ",
        "directories. All add-ins have an",
        "internal name (@SOMETHING) and a",
        "MCS directory with the same name",
        "gets created when the add-in",
        "starts.",
        " ",
        "There can only be a limited number",
        "of directory entries. If the limit",
        "is reached it cannot create a new",
        "MCS directory. That's why add-ins",
        "with new internal names do not",
        "start if they are ran for the ",
        "first time.",
        " ",
        "This utility helps to fix that.",
        "",
        "  == MCS_CreateDirectory Test =="
        "",
        "You can enter a name and test if",
        "you can create a new MCS",
        "directory. You see the return",
        "code and the meaning of it.",
        "The following return codes are",
        "possible:",
        "- 0: Success",
        "- 0x42: Already exists",
        "- 0x43: Dir space exhausted",
        "- anything else: Failure",
        "If you see 0x43 it means there",
        "is no space left and you should",
        "delete some of them to allow",
        "add-ins with new internal names",
        "to start again.",

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
    sb.barwidth = 5;

    Scrollbar(&sb);

    int scroll = 0;

    while (1) {
        Bdisp_AllClr_VRAM();
        EnableStatusArea(1);
        DisplayStatusArea();

        printTitle("Help", 160);

        for (int i = 0; i < visible_lines; i++) {
            int x = 0;
            int y = 20 + i * 20;
            PrintMini(&x, &y, text[scroll + i], 0, 360, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        }

        sb.indicatorpos = scroll;
        Scrollbar(&sb);
        Bdisp_PutDisp_DD();

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