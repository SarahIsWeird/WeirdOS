#include "text.h"

#include <stdarg.h>
#include <string.h>

struct {
    int row;
    int column;
    uint16_t color;
} terminal_info;

static char *digits = "0123456789abcdef";

static void scroll_terminal() {
    memmove(VMEM, VMEM + WIDTH, WIDTH * (HEIGHT - 1) * 2);
    memsetw(VMEM + WIDTH * 24, terminal_info.color | ' ', WIDTH * 2);
}

static void advance_row() {
    terminal_info.row++;

    if (terminal_info.row == TERM_HEIGHT) {
        terminal_info.column = 0;
        terminal_info.row--;
        scroll_terminal();
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

void print(const char *string) {
    while (*string != 0) {
        putchar(*(string++));
    }
}

void putn(unsigned int n, int base) {
    char buffer[128];
    char *buffer_ptr = buffer + 127;

    *(buffer_ptr--) = 0;

    do {
        *(buffer_ptr--) = digits[n % base];
        n /= base;
    } while (n > 0);

    print(++buffer_ptr);
}

void putsn(int n, int base) {
    if (n < 0) {
        putchar('-');
        n = -n;
    }

    putn(n, base);
}

void printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt != 0) {
        if (*fmt != '%') {
            putchar(*(fmt++));
            continue;
        }

        fmt++;

        switch (*fmt) {
            case 'c':
                putchar(va_arg(args, int));
                break;
            case 'd':
            case 'i':
                putsn(va_arg(args, int), 10);
                break;
            case 's':
                const char *str = va_arg(args, const char *);
                print(str);
                break;
            case 'o':
                putn(va_arg(args, unsigned int), 8);
                break;
            case 'u':
                putn(va_arg(args, unsigned int), 10);
                break;
            case 'p':
            case 'x':
            case 'X': // TODO: Add support for uppercase hexadecimal digits
                putn(va_arg(args, unsigned int), 16);
                break;
            case '%':
                putchar('%');
                break;
            default:
                putchar('%');
                putchar(*fmt);
                break;
        }

        fmt++;
    }
}
