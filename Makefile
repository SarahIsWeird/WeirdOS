CC=i686-elf-gcc
AS=nasm
LD=i686-elf-gcc

CFLAGS=-c -ffreestanding -fno-stack-protector -Wall -Wextra -Wno-void-pointer-to-int-cast -O2 -Isrc/lib
ASFLAGS=-felf32
LDFLAGS=-T linker.ld -ffreestanding -nostdlib -lgcc -O2

CFLAGS_D=-c -ffreestanding -Wall -Wextra -O0 -g -Isrc/lib
ASFLAGS_D=-felf32 -g
LDFLAGS_D=-T linker.ld -ffreestanding -nostdlib -lgcc -O0 -g

CSRC=$(shell find src -name '*.c')
COBJ=$(patsubst %.c,%.c.o,$(subst src/,build/,$(CSRC)))
COBJ_D=$(patsubst %.c,%.c.d.o,$(subst src/,build/,$(CSRC)))

ASRC=$(shell find src -name '*.asm')
AOBJ=$(patsubst %.asm,%.asm.o,$(subst src/,build/,$(ASRC)))
AOBJ_D=$(patsubst %.asm,%.asm.d.o,$(subst src/,build/,$(ASRC)))

.PHONY: clean

all: kernel

$(COBJ): build/%.c.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $<

$(AOBJ): build/%.asm.o: src/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(COBJ_D): build/%.c.d.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_D) -o $@ $<

$(AOBJ_D): build/%.asm.d.o: src/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS_D) -o $@ $<

kernel: $(COBJ) $(AOBJ)
	$(LD) $(LDFLAGS) -o weirdos.bin $(COBJ) $(AOBJ)
	@if grub-file --is-x86-multiboot weirdos.bin; then \
		echo "\033[32mKernel is multiboot compatible.\033[0m"; \
	else \
		echo "\033[31mKernel is NOT multiboot compatible.\033[0m"; \
		false; \
	fi

iso: kernel
	mkdir -p iso/boot/grub
	cp weirdos.bin iso/boot/weirdos.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o weirdos.iso iso/

run: iso
	qemu-system-i386 -cdrom weirdos.iso

kernel_debug: $(COBJ_D) $(AOBJ_D)
	$(LD) $(LDFLAGS_D) -o weirdos.bin $(COBJ_D) $(AOBJ_D)
	@if grub-file --is-x86-multiboot weirdos.bin; then \
		echo "\033[32mKernel is multiboot compatible.\033[0m"; \
	else \
		echo "\033[31mKernel is NOT multiboot compatible.\033[0m"; \
		false; \
	fi

nooptrun: kernel_debug
	qemu-system-i386 -no-reboot -no-shutdown -kernel weirdos.bin

debugrun: kernel_debug
# -d cpu,int -s -S
	qemu-system-i386 -no-reboot -no-shutdown -kernel weirdos.bin -d cpu,int -s -S

clean:
	rm -rf build/ iso/ weirdos.bin weirdos.iso 2> /dev/null

