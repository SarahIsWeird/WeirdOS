#ifndef KB_H
#define KB_H

#include <stdint.h>

void init_keyboard_driver();

char getchar();
char getchar_nb();

void on_keyboard_irq();

extern uint8_t read_keycode();

#endif