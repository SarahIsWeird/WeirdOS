#include "idt.h"

#include "../../video/text.h"
#include "../../drivers/keyboard/kb.h"

const char *exception_names[] = {
    "Divide-by-zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Overflow",
    "Bound range exceeded",
    "Invalid opcode",
    "Device not available",
    "Double fault",
    "Coprocessor segment overrun (deprecated)",
    "Invalid TSS",
    "Segment not present",
    "Stack-segment fault",
    "General protection fault",
    "Page fault",
    "Reserved",
    "x87 floating-point exception",
    "Alignment check",
    "Machine check",
    "SIMD floating-point exception",
    "Virtualization exception",
    "Control protection exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor injection exception",
    "VMM communication exception",
    "Security exception",
    "Reserved"
};

static inline void outb(uint16_t port, uint8_t data) {
    asm volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

void common_interrupt_handler(int interrupt_number, struct registers_s registers, int error_code) {
    if (interrupt_number < 0x20) {
        printf("Exception 0x%d (%s), halting kernel.\n", interrupt_number, exception_names[interrupt_number]);

        while (1) {
            asm volatile("cli; hlt");
        }
    }
    
    printf("Interrupt 0x%x\n", interrupt_number);

    if (interrupt_number == 0x21) {
        on_keyboard_irq();
        print("Key pressed\n");
    }

    if (interrupt_number >= 0x20 && interrupt_number <= 0x2f) {
        if (interrupt_number >= 0x28) {
            outb(0xa0, 0x20);
        }

        outb(0x20, 0x20);
    }
}

void set_idt_entry(struct idt_entry_s *idt, int index, uint32_t offset, uint16_t selector, int gate_type, int ring) {
        struct idt_entry_s *entry = &idt[index];

        entry->offset_l = offset & 0xffff;
        entry->selector = selector;
        entry->gate_type = gate_type & 0xf;
        entry->ring = ring & 0b11;
        entry->present = 1;
        entry->offset_h = (offset >> 16) & 0xffff;
}
