#include "idt.h"

#include "../../video/text.h"
#include "../../drivers/keyboard/kb.h"
#include "../../multitasking/scheduler.h"
#include "../gdt/gdt.h"

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

struct registers_s *common_interrupt_handler(int interrupt_number, struct registers_s *registers) {
    struct registers_s *new_registers = registers;

    if (interrupt_number < 0x20) {
        printf("Exception 0x%x (%s), halting kernel.\n", interrupt_number, exception_names[interrupt_number]);

        switch (interrupt_number) {
            case 0x6: // Invalid opcode
                printf("Invalid opcode at address 0x%x\n", registers->error_code);
                break;
            case 0xd: // GPF
                printf("The error code is %d. ", registers->error_code);
                
                if (registers->error_code & 1) {
                    print("The GPF origininated externally ");
                } else {
                    print("The GPF did not originate externally ");
                }

                printf("and the error code references descriptor %d in the %d", registers->error_code >> 3, (registers->error_code >> 1) & 3);

                if ((registers->error_code & 7) == 0) {
                    print("GDT.\n");
                } else if ((registers->error_code & 7) == 2) {
                    print("LDT.\n");
                } else {
                    print("IDT.\n");
                }

                break;
        }

        while (1) {
            asm volatile("cli; hlt");
        }
    }

    if (interrupt_number == 0x20) {
        new_registers = schedule_task(registers);
        get_tss()[1] = (uint32_t) (new_registers + 1);
        // registers = new_registers;
    }

    if (interrupt_number == 0x21) {
        on_keyboard_irq();
    }

    acknowledge_irq(interrupt_number);

    return new_registers;
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
