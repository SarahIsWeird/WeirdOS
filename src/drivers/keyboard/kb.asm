KEYCODE_BUFFER_SIZE equ 256

section .data

kc_buffer_w:	dd 0
kc_buffer_r:	dd 0
available_kc:	dd 0
kb_error:	db 'Fatal keyboard error: expected a selftest return value of 0x55, but got 0x', 0

section .bss

keycode_buffer:	resb KEYCODE_BUFFER_SIZE

section .text

extern putchar
extern print
extern putn

global on_keyboard_irq
on_keyboard_irq:
	mov ecx, dword [kc_buffer_w]
	in al, 0x60
	mov byte [keycode_buffer + ecx], al
	inc ecx
	cmp ecx, KEYCODE_BUFFER_SIZE
	je .reset
	mov dword [kc_buffer_w], ecx
	jmp .ok
.reset:	mov dword [kc_buffer_w], 0
.ok:	inc dword [available_kc]
	ret

global getchar
getchar:
.loop:	call getchar_nb
	cmp eax, 0
	je .loop
	ret

global getchar_nb
getchar_nb:
.wait:	mov eax, dword [available_kc]
	cmp eax, 0
	jne .ok
	ret
.ok:	dec dword [available_kc]
	xor eax, eax
	mov ecx, dword [kc_buffer_r]
	mov al, [keycode_buffer + ecx]

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
