#include "scheduler.h"

#include "../video/text.h"

#define STACK_SIZE (16384)

static uint8_t stack1[STACK_SIZE];
static uint8_t stack2[STACK_SIZE];
static volatile struct registers_s *states[2];

static volatile int current_task = -1;

void task1() {
	while (1) {
		putchar('1');
	}
}

void task2() {
	while (1) {
		putchar('2');
	}
}

void init_multitasking() {
    states[0] = create_task(stack1, task1);
    states[1] = create_task(stack2, task2);
}

struct registers_s *create_task(uint8_t *stack, void *eip) {
    struct registers_s registers = {
        .eax = 0,
        .ecx = 0,
        .edx = 0,
        .ebx = 0,
        .esp2 = 0,
        .ebp = 0,
        .esi = 0,
        .edi = 0,
        .error_code = 0,
        .eip = (uint32_t) eip,
        .cs = 0x8,
        .eflags = 0x202,
        .esp = 0,
        .ss = 0x10
    };

    struct registers_s *state = &stack[STACK_SIZE - sizeof(struct registers_s)];
    *state = registers;

    return state;
}

struct registers_s *schedule_task(struct registers_s *old_state) {
    if (current_task != -1) {
        *states[current_task] = *old_state;
    }

    current_task++;
    current_task %= 2;

    return states[current_task];
}
