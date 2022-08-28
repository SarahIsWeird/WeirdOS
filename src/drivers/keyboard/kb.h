#ifndef KB_H
#define KB_H

void init_keyboard_driver();

char getchar();

extern void on_keyboard_irq();

#endif