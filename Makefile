# --- STYLE DEFINITION ---

FONT_RED := $(shell tput setaf 1)
FONT_GREEN := $(shell tput setaf 2)
FONT_YELLOW := $(shell tput setaf 3)
FONT_BLUE := $(shell tput setaf 4)
FONT_PURPLE := $(shell tput setaf 5)
FONT_CYAN := $(shell tput setaf 6)
FONT_GRAY := $(shell tput setaf 7)
FONT_BLACK := $(shell tput setaf 8)
FONT_BOLD := $(shell tput bold)
FONT_RESET := $(shell tput sgr0)


# --- PRACTICAL PART ---
LIBGCC := $(shell $(CC) -m32 -print-libgcc-file-name)

SRC_PATH = src/
RES_PATH = res/
TESTS_PATH = $(RES_PATH)tests/
HEADER_PATH = include/
CFLAGS = -I $(HEADER_PATH) -ffreestanding -mgeneral-regs-only -m32 -g -fno-pie -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -fpermissive -fno-stack-protector -fno-threadsafe-statics
# -fno-threadsafe-statics : désactive la protection contre les accès concurrents lors de l'initialisation des variables statiques locales
# de toute façon il peut pas appeler les fonctions qui le permettent vu qu'elles n'existent pas chez moi HAHAHA
objects =	kernel_entry.o \
			kernel.o \
			math.o \
			ioport.o \
			utils.o \
			interrupt_descriptor_table.o \
			strings.o \
			interrupt_handlers.o \
			PCIController.o \
			keyboard.o \
			memory_management.o \
			process.o \
			switch_context.o \
			stdlib.o \
			time.o

objects_test =	tests.o \
				in_format.o \
				printf.o \
				process.o \
				math.o

# := $(shell ls src/tests -1 | sed -e 's/\..*$//')

objects_target = $(addprefix $(RES_PATH),$(objects))

objects_test_target = $(addprefix $(TESTS_PATH),$(objects_test))

all: $(RES_PATH)bin/OS.bin
	$(MAKE) success_msg


$(RES_PATH)boot.bin: $(SRC_PATH)bootloader.asm
	nasm -f bin $(SRC_PATH)bootloader.asm -o $(RES_PATH)boot.bin


$(RES_PATH)%.o: $(SRC_PATH)%.cpp
	g++ $(CFLAGS) -c $< -o $@

$(RES_PATH)%.o: $(SRC_PATH)%.asm
	nasm -f elf $< -o $@


$(RES_PATH)zeros.bin: $(SRC_PATH)zeros.asm
	nasm -f bin $(SRC_PATH)zeros.asm -o $(RES_PATH)zeros.bin



$(RES_PATH)full_kernel.bin: $(objects_target)
	echo $(objects_test)
	echo $(objects_target)
	ld -m elf_i386 -s -Ttext 0x1000 --oformat binary $(objects_target) $(LIBGCC) -o "$(RES_PATH)full_kernel.bin"


$(RES_PATH)full_os.bin: $(RES_PATH)boot.bin $(RES_PATH)full_kernel.bin
	cat $(RES_PATH)boot.bin $(RES_PATH)full_kernel.bin > $(RES_PATH)full_os.bin

$(RES_PATH)bin/OS.bin: $(RES_PATH)full_os.bin $(RES_PATH)zeros.bin
	cat $(RES_PATH)full_os.bin  $(RES_PATH)zeros.bin > $(RES_PATH)bin/OS.bin

success_msg:
		@printf "\n\n$(FONT_GREEN)\e[1m### FINISHED SUCCESSFULLY ###$(FONT_RESET)\e[0m\n\n"


clean:
	find res/. -type f -exec rm {} \;


tests:
	@mkdir -p $(TESTS_PATH)
	$(MAKE) all CFLAGS="$(CFLAGS) -D TEST" objects_target="$(objects_target) $(objects_test_target)"

run:
	qemu-system-x86_64 -drive format=raw,file="res/bin/OS.bin",index=0,if=floppy, -m 128M -serial stdio

debug:
	qemu-system-x86_64 -drive format=raw,file="res/bin/OS.bin",index=0,if=floppy, -m 128M -s -S -serial stdio

run_nographic:
	qemu-system-x86_64 -drive format=raw,file="res/bin/OS.bin",index=0,if=floppy, -m 128M -serial stdio -display curses