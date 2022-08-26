CC=i686-elf-gcc
AS=nasm
LD=i686-elf-gcc

CFLAGS=-c -ffreestanding -O2 -Wall -Wextra
ASFLAGS=-felf32
LDFLAGS=-T linker.ld -ffreestanding -O0 -nostdlib -lgcc

CSRC=$(shell find src -name '*.c')
COBJ=$(patsubst %.c,%.c.o,$(subst src/,build/,$(CSRC)))

ASRC=$(shell find src -name '*.asm')
AOBJ=$(patsubst %.asm,%.asm.o,$(subst src/,build/,$(ASRC)))

.PHONY: clean

all: kernel

$(COBJ): build/%.c.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $<

$(AOBJ): build/%.asm.o: src/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

kernel: $(COBJ) $(AOBJ)
	$(LD) $(LDFLAGS) -o weirdos.bin $(COBJ) $(AOBJ)
	if grub-file --is-x86-multiboot weirdos.bin; then \
		echo Kernel is multiboot compatible.; \
	else \
		echo Kernel is NOT multiboot compatible!; \
	fi

iso: kernel
	mkdir -p iso/boot/grub
	cp weirdos.bin iso/boot/weirdos.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o weirdos.iso iso/

run: iso
	qemu-system-i386.exe -cdrom weirdos.iso

clean:
	rm -rf build/ iso/ kernel.bin weirdos.iso 2> /dev/null

