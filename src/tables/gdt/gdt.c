#include "gdt.h"

#define GDT_SIZE 5

struct gdt_entry_s gdt[GDT_SIZE];

void init_gdt() {
    set_entry(0, 0, 0, 0, 0);
    set_entry(1, 0, 0xfffff, GDT_PRESENT | GDT_RING0 | GDT_CODE_SEGMENT | GDT_CODE_CONFORMING | GDT_CODE_READABLE, GDT_PROTECTED_MODE | GDT_GRANULARITY_4K);
    set_entry(2, 0, 0xfffff, GDT_PRESENT | GDT_RING0 | GDT_DATA_SEGMENT | GDT_DATA_WRITABLE, GDT_PROTECTED_MODE | GDT_GRANULARITY_4K);
    set_entry(3, 0, 0xfffff, GDT_PRESENT | GDT_RING3 | GDT_CODE_SEGMENT | GDT_CODE_CONFORMING | GDT_CODE_READABLE, GDT_PROTECTED_MODE | GDT_GRANULARITY_4K);
    set_entry(4, 0, 0xfffff, GDT_PRESENT | GDT_RING3 | GDT_DATA_SEGMENT | GDT_DATA_WRITABLE, GDT_PROTECTED_MODE | GDT_GRANULARITY_4K);

    set_gdt(gdt, sizeof(struct gdt_entry_s) * GDT_SIZE - 1);
    reload_segments();
}

void set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    struct gdt_entry_s *entry = &gdt[index];

    entry->limit_l = limit & 0xffff;
    entry->base_l = base & 0xffffff;
    entry->access = access;
    entry->limit_h = (limit >> 16) & 0xf;
    entry->flags = flags;
    entry->base_h = (base >> 24) & 0xff;
}
