// MCS Create Dir Test by Computer_Freak_2004
// Enter a name for a MCS Directory and see the result, @ gets added to the front of the input
// 06.08.2026

#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "mcs_syscalls.h"

char name[21];
int rc = -1;
char result_dec[16];
char result_hex[16];

// Input
char* buffer;
int start = 0;
int cursor = 0;
int key;

void printTitle() {
    int x = 35, y = 0;
    PrintMini(&x, &y, "== MCS_CreateDirectory Test ==", 0, 0xFFFFFFFF, 0, 0, COLOR_BLUE, COLOR_WHITE, 1, 0);

    for (x = 0; x < LCD_WIDTH_PX; x++) {
        Bdisp_SetPoint_VRAM(x, 42, COLOR_BLACK);
    }
}

void printResult() {
    int x = 0, y = 60;
    if (rc != -1) {  // Input done
        char result[32];
        strcpy(result, result_dec);
        strcat(result, ", 0x");
        strcat(result, result_hex);

        PrintMini(&x, &y, result, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
        x = 0, y = 80;
        if (rc == 0) {
            PrintMini(&x, &y, "Success", 0, 0xFFFFFFFF, 0, 0, COLOR_GREEN, COLOR_WHITE, 1, 0);
        } else if (rc == 0x42) {
            PrintMini(&x, &y, "Already exists", 0, 0xFFFFFFFF, 0, 0, COLOR_ORANGE, COLOR_WHITE, 1, 0);
        } else if (rc == 0x43) {
            PrintMini(&x, &y, "Dir space exhausted", 0, 0xFFFFFFFF, 0, 0, COLOR_RED, COLOR_WHITE, 1, 0);
        } else {
            PrintMini(&x, &y, "Failure", 0, 0xFFFFFFFF, 0, 0, COLOR_RED, COLOR_WHITE, 1, 0);
        }

        x = 0, y = 170;
        PrintMini(&x, &y, name, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    } else if (rc == -1) {  // No input done
        x = 0, y = 170;
        PrintMini(&x, &y, "Enter a name for a MCS dir without @.", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    }
}

void createDirTest() {
    buffer = malloc(20);
    if (buffer == 0) return 1;

    buffer[0] = '\0';
    memset(buffer, 0, 21);

    start = 0;
    cursor = 0;
    rc = -1;

    Cursor_SetFlashOn(0);

    while (1) {
        Bdisp_AllClr_VRAM();

        EnableStatusArea(1);
        DisplayStatusArea();

        printTitle();
        printResult();

        DisplayMBString((unsigned char*)buffer, start, cursor, 1, 2);

        Bdisp_PutDisp_DD();

        GetKey(&key);

        if (key == KEY_CTRL_EXE) {
            name[0] = '@';
            strcpy(name + 1, buffer);

            int dirno;
            rc = MCS_CreateDirectory((unsigned char*)name, &dirno);

            itoa(rc, (unsigned char*)result_dec);
            LongToAscHex(rc, (unsigned char*)result_hex, 2);
        }

        if (key == KEY_CTRL_EXIT) {
            free(buffer);
            buffer = 0;
            break;
        }

        if (key && key < 30000)
            cursor = EditMBStringChar((unsigned char*)buffer, 20, cursor, key);
        else
            EditMBStringCtrl((unsigned char*)buffer, 20, &start, &cursor, &key, 1, 1);
    }
}