#include "idt.h"

#include "../../video/text.h"

void idt_common_handler(int interrupt_number) {
    putchar('0' + interrupt_number);
}