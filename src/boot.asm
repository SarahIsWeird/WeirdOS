MBALIGN	equ 1
MEMINFO	equ 2
FLAGS	equ MBALIGN | MEMINFO
MAGIC	equ 0x1badb002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .bss
align 16
stack_bottom:
	resb 16384
stack_top:

section .text

extern kernel_main

global _start:function (_start.end - _start)
_start:
	mov esp, stack_top
	call kernel_main

	cli
.hang:	hlt
	jmp .hang
.end:
