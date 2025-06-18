SRC_PATH = src/
RES_PATH = res/
CFLAGS = -ffreestanding -mgeneral-regs-only -m32 -g -fno-pie -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -fpermissive -fno-stack-protector
objects = kernel_entry.o kernel.o math.o utils.o InterruptDescriptorTable.o strings.o interrupt_handlers.o
objects_target = $(addprefix $(RES_PATH),$(objects))

all: $(RES_PATH)bin/OS.bin


$(RES_PATH)boot.bin: $(SRC_PATH)bootloader.asm
	nasm -f bin $(SRC_PATH)bootloader.asm -o $(RES_PATH)boot.bin


$(RES_PATH)%.o: $(SRC_PATH)%.cpp
	g++ $(CFLAGS) -c $< -o $@

$(RES_PATH)kernel_entry.o: $(SRC_PATH)kernel_entry.asm
	nasm -f elf $(SRC_PATH)kernel_entry.asm -o $(RES_PATH)kernel_entry.o


$(RES_PATH)zeros.bin: $(SRC_PATH)zeros.asm
	nasm -f bin $(SRC_PATH)zeros.asm -o $(RES_PATH)zeros.bin



$(RES_PATH)full_kernel.bin: $(objects_target)
	ld -m elf_i386 -s -Ttext 0x1000 --oformat binary $(objects_target) -o "$(RES_PATH)full_kernel.bin"


$(RES_PATH)full_os.bin: $(RES_PATH)boot.bin $(RES_PATH)full_kernel.bin
	cat $(RES_PATH)boot.bin $(RES_PATH)full_kernel.bin > $(RES_PATH)full_os.bin

$(RES_PATH)bin/OS.bin: $(RES_PATH)full_os.bin $(RES_PATH)zeros.bin
	cat $(RES_PATH)full_os.bin  $(RES_PATH)zeros.bin > $(RES_PATH)bin/OS.bin


clean:
	find res/. -type f -exec rm {} \;

run:
	qemu-system-x86_64 -drive format=raw,file="res/bin/OS.bin",index=0,if=floppy, -m 128M


debug:
	qemu-system-x86_64 -drive format=raw,file="res/bin/OS.bin",index=0,if=floppy, -m 128M -s -S