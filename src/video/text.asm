section .text

global disable_cursor
disable_cursor:
	mov al, 0x0a
	mov dx, 0x3d4
	out dx, al
	mov al, 0x20
	inc dx
	out dx, al
	ret