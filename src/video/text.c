#include "text.h"

struct {
    int row;
    int column;
    uint16_t color;
} terminal_info;

static void advance_row() {
    terminal_info.row++;

    if (terminal_info.row == TERM_HEIGHT) {
        terminal_info.row = 0;
    }
}

static void advance_column() {
    terminal_info.column++;

    if (terminal_info.column == TERM_WIDTH) {
        terminal_info.column = 0;
        
        advance_row();
    }
}

void init_terminal() {
    terminal_info.row = 0;
    terminal_info.column = 0;

    set_color(WHITE, BLACK);
}

void set_color(enum color_e foreground, enum color_e background) {
    terminal_info.color = ((uint16_t) (background << 4 | foreground)) << 8;
}

void clear_screen() {
    for (int i = 0; i < TERM_WIDTH * TERM_HEIGHT; i++) {
        VMEM[i] = terminal_info.color | ' ';
    }
}

void putchar(char c) {
    if (c == '\n') {
        terminal_info.column = 0;
        advance_row();

        return;
    }

    VMEM[terminal_info.column + terminal_info.row * TERM_WIDTH] = terminal_info.color | c;

    advance_column();
}

void print(char *string) {
    while (*string != 0) {
        putchar(*(string++));
    }
}
