[bits 32]
[extern main]

call main

death:
    jmp death

; void load_idt(void* idt_ptr);
global load_idt
load_idt:
    push ebx
    mov ebx, esp
    add ebx, 8 ; get IDT* ; offset of 8 because of ebx push before (4 from push and 4 from function offset set by compiler)
    mov ebx, [ebx]

    lidt [ebx]   ; NO HARDCODED VALUE !!!!!!

    pop ebx

    ret

jmp $