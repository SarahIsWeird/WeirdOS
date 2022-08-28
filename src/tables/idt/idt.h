#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_entry_s {
        uint32_t offset_l : 16;
        uint16_t selector;
        uint8_t reserved1;
        uint8_t gate_type : 4;
        uint8_t reserved2 : 1;
        uint8_t ring : 2;
        uint8_t present : 1;
        uint32_t offset_h : 16;
} __attribute__((__packed__));

struct registers_s {
        uint32_t eax;
        uint32_t ecx;
        uint32_t edx;
        uint32_t ebx;
        uint32_t esp;
        uint32_t ebp;
        uint32_t esi;
        uint32_t edi;
} __attribute__((__packed__));

void common_interrupt_handler(int interrupt_number, struct registers_s registers, int error_code);

void set_idt_entry(struct idt_entry_s *idt, int index, uint32_t offset, uint16_t selector, int gate_type, int ring);

extern void init_idt();

#endif