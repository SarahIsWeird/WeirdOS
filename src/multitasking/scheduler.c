#include "scheduler.h"

#include "../video/text.h"

#define STACK_SIZE (4096)

static uint8_t stack1[STACK_SIZE];
static uint8_t stack2[STACK_SIZE];

static uint8_t userstack1[STACK_SIZE];
static uint8_t userstack2[STACK_SIZE];

static struct registers_s *states[2];

static int current_task = -1;

void task1() {
    // asm volatile("cli; hlt");
	while (1) {
        for (volatile int i = 0; i < 100000; i++);
		putchar('1');
	}
}

void task2() {
	while (1) {
        for (volatile int i = 0; i < 100000; i++);
		putchar('2');
	}
}

void task3() {
	while (1) {
        for (volatile int i = 0; i < 100000; i++);
		putchar('3');
	}
}

void init_multitasking() {
    states[0] = create_task(stack1, userstack1, task1);
    states[1] = create_task(stack2, userstack2, task2);
}

struct registers_s *create_task(uint8_t *stack, uint8_t *userstack, void *eip) {
    struct registers_s registers = {
        .eax = 0,
        .ecx = 0,
        .edx = 0,
        .ebx = 0,
        .ebp = 0,
        .esi = 0,
        .edi = 0,
        .eip = (uint32_t) eip,
        .cs = 0x18 | 0x03,
        .eflags = 0x202,
        .esp = (uint32_t) userstack + STACK_SIZE,
        .ss = 0x20 | 0x03,
        // .eip = (uint32_t) eip,
        // .cs = 0x08,
        // .eflags = 0x202,
    };

    struct registers_s *state = (struct registers_s *) &stack[STACK_SIZE - sizeof(struct registers_s)];
    *state = registers;

    return state;
}

struct registers_s *schedule_task(struct registers_s *old_state) {
    if (current_task >= 0) {
        states[current_task] = old_state;
    }

    current_task++;
    if (current_task == 2) current_task = 0;

    return states[current_task];
}
