#include <stdint.h>

#include "video/text.h"
#include "tables/gdt/gdt.h"
#include "tables/idt/idt.h"
#include "drivers/keyboard/kb.h"

void kernel_main() {
	set_color(WHITE, CYAN);
	clear_screen();

	print("Initializing GDT...\n");
	init_gdt();
	print("Initializing IDT...\n");
	init_idt();
	print("Initializing keyboard driver...\n");
	init_keyboard_driver();

	asm volatile("sti");

	while (1);
}
