section .data
gdtr	dw 0
	dd 0

section .text

global set_gdt
set_gdt:
	mov eax, [esp + 4]
	mov [gdtr + 2], eax
	mov ax, [esp + 8]
	mov [gdtr], ax
	lgdt  [gdtr]
	jmp 0x08:.reload_cs_segment
.reload_cs_segment:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret

global set_tr
set_tr:
	mov eax, [esp + 4]
	ltr ax
	ret

global reload_segments
reload_segments:
	ret
