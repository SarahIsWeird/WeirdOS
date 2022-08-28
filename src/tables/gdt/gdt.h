#ifndef GDT_H
#define GDT_H

#include <stdint.h>

enum gdt_access_e {
    GDT_CODE_READABLE = 2,
    GDT_DATA_WRITABLE = 2,
    GDT_CODE_CONFORMING = 4,
    GDT_DATA_DIRECTION = 4,
    GDT_CODE_SEGMENT = 24,
    GDT_DATA_SEGMENT = 16,
    GDT_TSS_SEGMENT = 0,
    GDT_RING0 = 0,
    GDT_RING3 = 96,
    GDT_PRESENT = 128
};

enum gdt_flags_e {
    GDT_PROTECTED_MODE = 4,
    GDT_GRANULARITY_4K = 8
};

struct gdt_entry_s {
    uint32_t limit_l : 16;
    uint32_t base_l : 24;
    uint32_t access : 8;
    uint32_t limit_h : 4;
    uint32_t flags : 4;
    uint32_t base_h : 8;
} __attribute__((__packed__));

void init_gdt();

void set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);
void set_gdt(void *base, uint16_t limit);

#endif