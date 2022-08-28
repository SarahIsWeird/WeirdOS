#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>

#define VMEM ((uint16_t *) 0xb8000)
#define WIDTH (80)
#define HEIGHT (25)

#define TERM_WIDTH 80
#define TERM_HEIGHT 25

enum color_e {
    BLACK = 0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GRAY,
    DARK_GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    YELLOW,
    WHITE
};

void init_terminal();
void clear_screen();
void set_color(enum color_e foreground, enum color_e background);
void putchar(char c);
void print(const char *string);
void putn(unsigned int n, int base);
void putsn(int n, int base);
void printf(const char *fmt, ...);

#endif
