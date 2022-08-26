extern idt_common_handler

%macro idt_handler 1
global idt_handler_%1
idt_handler_%1:
	push %1
	call idt_common_handler
	iret
%endmacro

%assign i 0
%rep 256
idt_handler i
%assign i i+1
%endrep
