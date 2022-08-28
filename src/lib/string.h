#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

void memmove(void *dst, const void *src, size_t len);
void memset(void *dst, int value, size_t len);
void memsetw(void *dst, int value, size_t len);

#endif