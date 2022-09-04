#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../tables/idt/idt.h"

void init_multitasking();

struct registers_s *create_task(uint8_t *stack, uint8_t *userstack, void *eip);
struct registers_s *schedule_task(struct registers_s *old_state);

#endif