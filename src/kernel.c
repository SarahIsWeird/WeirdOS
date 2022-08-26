#include <stdint.h>

#include "video/text.h"
#include "tables/gdt/gdt.h"
#include "tables/idt/idt.h"

void kernel_main() {
	set_color(WHITE, CYAN);
	clear_screen();

	print("uwu\nowo");

	init_gdt();
}
