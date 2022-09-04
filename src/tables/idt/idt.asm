section .data

idtr:	dw 2047
	dd idt

section .bss

idt:	resd 512

section .text

extern interrupt_handler
extern set_idt_entry

%assign i 0
%rep 256
extern interrupt_handler_%[i]
%assign i i+1
%endrep

global init_idt
init_idt:
%assign i 0
%rep 32
	push 0
	push 0xf
	push 8
	push interrupt_handler_%[i]
	push i
	push idt
	call set_idt_entry
	add esp, 24
%assign i i+1
%endrep
%rep 224
	push 0
	push 0xe
	push 8
	push interrupt_handler_%[i]
	push i
	push idt
	call set_idt_entry
	add esp, 24
%assign i i+1
%endrep
	lidt [idtr]
	call remap_pic
	ret

remap_pic:
	mov al, 0x11
	out 0x20, al
	mov al, 0x20
	out 0x21, al
	mov al, 0x04
	out 0x21, al
	mov al, 0x01
	out 0x21, al

	mov al, 0x11
	out 0xa0, al
	mov al, 0x28
	out 0xa1, al
	mov al, 0x02
	out 0xa1, al
	mov al, 0x01
	out 0xa1, al

	xor al, al
	out 0x21, al
	out 0xa1, al

	ret

global acknowledge_irq
acknowledge_irq:
	pop edx
	cmp edx, 0x20
	jl .done
	cmp edx, 0x2f
	jg .done
	mov al, 0x20
	out 0x20, al
	cmp edx, 0x28
	jl .done
	out 0xa0, al
.done:	ret
