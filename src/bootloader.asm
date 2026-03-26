[org 0x7c00]
KERNEL_LOCATION equ 0x1000 ; address of the kernel entry point location
                                    

mov [BOOT_DISK], dl


; --- load the kernel into the RAM ---
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
int 0x13 ;BIOS call, here is the doc : https://wiki.osdev.org/Disk_access_using_the_BIOS_(INT_13h)

cmp ah, 0x00
je .pass ; test failure

.failure:

    mov bx, msg_failure ; A-Q : error from 01h to 11h
    call .print_string   ; si minuscule : la position de la lettre dans l'alphabet correspond au numero de dizaine de l'erreur (en hex)
    cmp ah, 0x19
    mov al, ah
    jns .high_failure

    add al, 0x40
    mov ah, 0x0e
    int 0x10
    jmp .stop

.high_failure:
    shr al, 4
    add al, 0x61
    mov ah, 0x0e
    int 0x10
    jmp .stop


.stop:
    jmp .stop

.pass:


; --- change to text mode ---
mov ah, 0x0
mov al, 0x3
int 0x10    ; BIOS call, here is the doc : https://en.wikipedia.org/wiki/INT_10H


mov ah, 0x0F    ; BIOS: Get current video mode
int 0x10        ; Call BIOS
mov [0x044A], ah  ; Store screen width (AH contains width)


; --- here is the part who get memory mapping ---
mov ebx, 0

mov eax, 0xE820 ; function number to use. Here is the doc : http://www.uruk.org/orig-grub/mem64mb.html
mov ecx, 24 ; size of one MemoryMapEntry
mov di, 0x9000 ; address to store all MemoryMapEntries
mov edx, 0x534D4150 ; magic number
mov [memory_map_count], 0 ; set memory_map_count to 0

.get_memory_map:
    int 15h ; BIOS call
    jc .memory_mapped ; test if it crashed or it got an error
    cmp eax, 0x534D4150 ; test if the magic number is set into eax; it mean all worked
    jne .failure
    add es:di, 24; add 24 to the address to store the next MemoryMapEntry
    inc word [memory_map_count] ; increment memory_map_count
    cmp ebx, 0
    jne .get_memory_map


.memory_mapped:
    cmp word [memory_map_count], 0
    je .failure ; BIOS does not support E820 function

; --- description of the GlobalDescriptoTable and loading
CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

cli
lgdt [GDT_descriptor]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp CODE_SEG:.start_protected_mode

jmp $



BOOT_DISK: db 0

GDT_start:
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code: ; cette section sera le modèle de description
        dw 0xffff ; size of the segment
        dw 0x0 ; base of the semgment
        db 0x0 ; (this is also the base)
        db 0b10011010 ; 1 : present bit, 00 : privilege (here ring 0 : kernel), 1 : descriptor (code/data here), 1010 : executable, non-conforming, readable
        db 0b11001111 ; FLAGS : 4Kb granularity, 32 bits
        db 0x0 ; base again (hight bits)

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


msg_failure: db "Loading failed, error : ", 0
memory_map_count : dw 0x8FFE ; address of memory_map_counter (declared in memory_management.hpp)


; --- * print_string function ---
.print_string:
    push ax
    mov ah, 0x0e
    .print_string_loop:
        mov al, [bx]
        cmp al, 0
        je .print_string_end
        int 0x10
        inc bx
        jmp .print_string_loop
    
    .print_string_end:
    
    pop ax
    ret

; --- Switch into 32bits protected mode
[bits 32]
.start_protected_mode:
    mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000		; 32 bit stack base pointer
	mov esp, ebp

    jmp KERNEL_LOCATION

                                     
 
times 510-($-$$) db 0 ; fill the end with zeros      
dw 0xaa55 ; magic number for the OS to be detected