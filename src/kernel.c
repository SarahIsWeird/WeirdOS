#include <stdint.h>

#include "video/text.h"
#include "tables/gdt/gdt.h"
#include "tables/idt/idt.h"
#include "drivers/keyboard/kb.h"
#include "multitasking/scheduler.h"

void kernel_main() {
	init_terminal();

	print("Initializing GDT...\n");
	init_gdt();
	print("Initializing IDT...\n");
	init_idt();
	print("Initializing keyboard driver...\n");
	init_keyboard_driver();

    print("Initializing scheduler...\n");
    init_multitasking();

    print("Enabling interrupts...\n");
	asm volatile("sti");

    while (1) {
        for (volatile int i = 0; i < 10000; i++);
        putchar('c');
    }

    // while (1) {
    //     putchar(getchar());
    // }

    while (1);
}
