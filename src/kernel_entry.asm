[bits 32]
[extern main]
call main

global load_idt
load_idt:
    push ebx
    mov ebx, esp
    add ebx, 8
    mov ebx, [ebx]

    lidt [ebx]   ; NO ENCODED VALUE !!!!!!

    pop ebx

    ret

jmp $