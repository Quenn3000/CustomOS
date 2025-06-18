[bits 32]
[extern main]
call main

global load_idt
load_idt:

    lidt [0x3820]   ; warning : hardcoded value
    ;sti

    ret

jmp $