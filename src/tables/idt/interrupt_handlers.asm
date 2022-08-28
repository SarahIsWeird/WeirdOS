section .text

extern common_interrupt_handler

%macro exception_handler_with_code 1
global interrupt_handler_%1
interrupt_handler_%1:
	pushad
	push %1
	call common_interrupt_handler
	add esp, 4
	popad
	add esp, 4
	iret
%endmacro

%macro exception_handler_without_code 1
extern interrupt_handler_%1
interrupt_handler_%1:
	push 0
	pushad
	push %1
	call common_interrupt_handler
	add esp, 4
	popad
	add esp, 4
	iret
%endmacro

%macro interrupt_handler 1
extern interrupt_handler_%1
interrupt_handler_%1:
	push 0
	pushad
	push %1
	call common_interrupt_handler
	add esp, 4
	popad
	add esp, 4
	iret
%endmacro

exception_handler_without_code	 0 ; Divide-by-zero
exception_handler_without_code	 1 ; Debug
exception_handler_without_code	 2 ; Non-maskable interrupt
exception_handler_without_code	 3 ; Breakpoint
exception_handler_without_code	 4 ; Overflow
exception_handler_without_code	 5 ; Bound range exceeded
exception_handler_without_code	 6 ; Invalid opcode
exception_handler_without_code	 7 ; Device not available
exception_handler_with_code	 8 ; Double fault
exception_handler_without_code	 9 ; Coprocessor segment overrun (deprecated)
exception_handler_with_code	10 ; Invalid TSS
exception_handler_with_code	11 ; Segment not present
exception_handler_with_code	12 ; Stack-segment fault
exception_handler_with_code	13 ; General protection fault
exception_handler_with_code	14 ; Page fault
exception_handler_without_code	15 ; Reserved
exception_handler_without_code	16 ; x87 floating-point exception
exception_handler_with_code	17 ; Alignment check
exception_handler_without_code	18 ; Machine check
exception_handler_without_code	19 ; SIMD floating-point exception
exception_handler_without_code	20 ; Virtualization exception
exception_handler_with_code	21 ; Control protection exception
exception_handler_without_code	22 ; Reserved
exception_handler_without_code	23 ; Reserved
exception_handler_without_code	24 ; Reserved
exception_handler_without_code	25 ; Reserved
exception_handler_without_code	26 ; Reserved
exception_handler_without_code	27 ; Reserved
exception_handler_without_code	28 ; Hypervisor injection exception
exception_handler_with_code	29 ; VMM communication exception
exception_handler_with_code	30 ; Security exception
exception_handler_without_code	31 ; Reserved

%assign i 32
%rep 224
interrupt_handler i
%assign i i+1
%endrep
