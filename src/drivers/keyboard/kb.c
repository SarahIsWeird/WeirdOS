#include "kb.h"

#include "../../video/text.h"

#define KEYCODE_BUFFER_SIZE 256

static volatile int kc_buffer_windex = 0;
static volatile int kc_buffer_rindex = 0;
static volatile int available_keycodes = 0;
static const char keymap[] = { 0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', 0, '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/' };
static const char keymap_shifted[] = { 0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', 0, '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?' };
static int caps_enabled = 0;

static const char *current_keymap = keymap;

static volatile uint8_t keycode_buffer[KEYCODE_BUFFER_SIZE];

void on_keyboard_irq() {
    keycode_buffer[kc_buffer_windex++] = read_keycode();
    kc_buffer_windex %= KEYCODE_BUFFER_SIZE;
    available_keycodes++;
}

char getchar() {
    while (1) {
        char c = getchar_nb();

        if (c != 0) return c;
    }
}

char getchar_nb() {
    if (available_keycodes == 0) return 0;

    unsigned char keycode = keycode_buffer[kc_buffer_rindex++];
    kc_buffer_rindex %= KEYCODE_BUFFER_SIZE;
    available_keycodes--;

    if (keycode & 0x80) {
        keycode &= 0x7f;

        if ((keycode != 0x2a) && (keycode != 0x36)) return 0;

        current_keymap = caps_enabled ? keymap_shifted : keymap;
        return 0;
    }

    keycode &= 0x7f;

    if (keycode == 0x3a) {
        caps_enabled = !caps_enabled;

        if (current_keymap == keymap) {
            current_keymap = keymap_shifted;
        } else {
            current_keymap = keymap;
        }
        
        return 0;
    }

    if (keycode == 0x2a || keycode == 0x36) {
        current_keymap = caps_enabled ? keymap : keymap_shifted;
        return 0;
    }

    if (keycode >= sizeof(keymap)) {
        return 0;
    }

    return current_keymap[keycode];
}
