[org 0x7c00]
KERNEL_LOCATION equ 0x1000
                                    

mov [BOOT_DISK], dl
                                    
xor ax, ax  ; set ax to 0
mov es, ax
mov ds, ax
mov bx, 0x7e00
mov sp, bp

mov bx, KERNEL_LOCATION
mov dh, 50 ; number of sector containing the kernel to read

mov ah, 0x02
mov al, dh 
mov ch, 0x00
mov dh, 0x00
mov cl, 0x02
mov dl, [BOOT_DISK]
int 0x13

cmp ah, 0x00
je pass ; test failure

mov bx, msg_failure ; A-Q : error from 01h to 11h
call print_string   ; si minuscule : la position de la lettre dans l'alphabet correspond au numero de dizaine de l'erreur (en hex)
cmp ah, 0x19
mov al, ah
jns high_failure

add al, 0x40
mov ah, 0x0e
int 0x10
jmp stop

high_failure:
shr al, 4
add al, 0x61
mov ah, 0x0e
int 0x10
jmp stop


stop:
    jmp stop

pass:

                                    
mov ah, 0x0
mov al, 0x3
int 0x10        ; text mode


CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start


mov ah, 0x0F    ; BIOS: Get current video mode
int 0x10        ; Call BIOS
mov [0x044A], ah  ; Store screen width (AH contains width)

cli
lgdt [GDT_descriptor]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp CODE_SEG:start_protected_mode

jmp $
                                    
BOOT_DISK: db 0

GDT_start:
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code:
        dw 0xffff ; size of the segment
        dw 0x0 ; base of the semgment
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GDT_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start


print_string:
    push ax
    mov ah, 0x0e
    print_string_loop:
        mov al, [bx]
        cmp al, 0
        je print_string_end
        int 0x10
        inc bx
        jmp print_string_loop
    
    print_string_end:
    
    pop ax
    ret


msg_failure: db "Loading failed, error : ", 0

[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000		; 32 bit stack base pointer
	mov esp, ebp

    jmp KERNEL_LOCATION

                                     
 
times 510-($-$$) db 0              
dw 0xaa55