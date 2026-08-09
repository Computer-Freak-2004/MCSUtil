#ifndef UTIL_H
#define UTIL_H

typedef struct {
    char name[9];
    char length[12];
    char flags [12];
    char address[11];
} ItemDisplay;

void printTitle(char* str, int x);
void drawHLine(int y);
void fillArea(unsigned x, unsigned y, unsigned w, unsigned h, unsigned short col);
void errorMsg(char* errorcode, char* msg, int errorcolor);
void drawMenu(int x, int y, int width, int row_count, char* items_char[], ItemDisplay items_itemDisplay[], int item_count, int top_entry, int selected_entry, int useItemDisplay, int showAddress, int name_x, int length_x, int address_x, int flags_x, char arrow_symbol[2]);
void GetKeyMenu(int key, int *current, int* top_entry, int item_count, int row_count);

#endif