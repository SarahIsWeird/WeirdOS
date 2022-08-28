section .text

global memmove
memmove:
        push ebp
        mov ebp, esp
        push esi
        push edi
        mov edi, [esp + 16]
        mov esi, [esp + 20]
        mov ecx, [esp + 24]
        push ds
        push ds
        pop es
        cld
rep     movsb
        pop ds
        pop edi
        pop esi
        pop ebp
        ret

global memset:
memset:
        push ebp
        mov ebp, esp
        push edi
        mov edi, [esp + 12]
        mov eax, [esp + 16]
        mov ecx, [esp + 20]
        push ds
        push ds
        pop es
        cld
rep     stosb
        pop ds
        pop edi
        pop ebp
        ret

global memsetw:
memsetw:
        push ebp
        mov ebp, esp
        push edi
        mov edi, [esp + 12]
        mov eax, [esp + 16]
        mov ecx, [esp + 20]
        push ds
        push ds
        pop es
        cld
rep     stosw
        pop ds
        pop edi
        pop ebp
        ret