KEYCODE_BUFFER_SIZE equ 256

section .data

kb_error:	db 'Fatal keyboard error: expected a selftest return value of 0x55, but got 0x', 0

section .bss

keycode_buffer1:	resb KEYCODE_BUFFER_SIZE

section .text

extern print
extern putn

global read_keycode
read_keycode:
	xor eax, eax
	in al, 0x60
	ret

global init_keyboard_driver
init_keyboard_driver:
	xor eax, eax
	mov al, 0xaa
	out 0x64, al
	in al, 0x60
	cmp al, 0x55
	je .clr_o
	push eax
	push kb_error
	call print
	add esp, 4
	pop eax
	push 16
	push eax
	call putn
	add esp, 8
.hlt:	cli
	hlt
	jmp .hlt
.clr_o:	in al, 0x64
	test al, 1
	jz .clr_i
	in al, 0x60
	jmp .clr_o
.clr_i:	in al, 0x64
	test al, 2
	jnz .clr_i
	mov al, 0xf4
	out 0x60, al
	ret
