// MCS Create Dir Test by Computer_Freak_2004
// Enter a name for a MCS Directory and see the result, @ gets added to the front of the input
// 06.08.2026

#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include <stdlib.h>
#include <string.h>

#include "mcs_syscalls.h"
#include "util.h"

#define MAX_INPUT_LEN 7
char name[MAX_INPUT_LEN+1];
int rc = -1;
char result_dec[16];
char result_hex[16];

// Input
char* buffer;
int start = 0;
int cursor = 0;
static int key;

void printResult() {
    int x = 0, y = 60;
    if (rc != -1) {  // Input done
        char result[32];
        strcpy(result, result_dec);
        strcat(result, ", 0x");
        strcat(result, result_hex);

        PrintMini(&x, &y, result, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0); // Return Code
        x = 0, y = 80;
        if (rc == 0) {
            PrintMini(&x, &y, "Success", 0, 0xFFFFFFFF, 0, 0, COLOR_LIME, COLOR_WHITE, 1, 0);
        } else if (rc == 0x42) {
            PrintMini(&x, &y, "Already exists", 0, 0xFFFFFFFF, 0, 0, COLOR_ORANGE, COLOR_WHITE, 1, 0);
        } else if (rc == 0x43) {
            PrintMini(&x, &y, "Dir space exhausted", 0, 0xFFFFFFFF, 0, 0, COLOR_RED, COLOR_WHITE, 1, 0);
        } else {
            PrintMini(&x, &y, "Failure", 0, 0xFFFFFFFF, 0, 0, COLOR_RED, COLOR_WHITE, 1, 0);
        }
        char buffer[32];
        strcpy(buffer, "Input: ");
        strcat(buffer, name);

        x = 0, y = 170;

        PrintMini(&x, &y, buffer, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0); // Entred input
    } else if (rc == -1) {  // No input done
        x = 0, y = 170;
        PrintMini(&x, &y, "Input: ", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    }
}

void createDirTest() {
    buffer = malloc(MAX_INPUT_LEN+1);
    if (buffer == 0) return 1;

    buffer[0] = '\0';
    memset(buffer, 0, MAX_INPUT_LEN);

    start = 0;
    cursor = 0;
    rc = -1;

    Cursor_SetFlashOn(0);

    while (1) {
        Bdisp_AllClr_VRAM();

        EnableStatusArea(1);
        DisplayStatusArea();

        printTitle("Create Directory Test", 85);
        printResult();

        PrintXY(1, 2, "  Name: @", 0, TEXT_COLOR_BLACK);
        DisplayMBString((unsigned char*)buffer, start, cursor, 8, 2);
        Bdisp_Rectangle(104, 20, 260, 50, COLOR_BLACK);

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
            cursor = EditMBStringChar((unsigned char*)buffer, MAX_INPUT_LEN, cursor, key);
        else
            EditMBStringCtrl((unsigned char*)buffer, MAX_INPUT_LEN, &start, &cursor, &key, 1, 1);
    }
}