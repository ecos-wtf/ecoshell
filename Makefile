#PLATFORM=TCG300
PLATFORM=CG3700
TCPREFIX=gnutools/mipsisa32-elf/bin/mipsisa32-elf-
CC=$(TCPREFIX)gcc
OBJCOPY=$(TCPREFIX)objcopy
LDSCRIPT=$(PLATFORM)/payload.ld
CFLAGS=-march=mips32 -mabi=eabi -msoft-float -mno-abicalls -fno-builtin -nostdlib -nodefaultlibs -nostartfiles -T $(LDSCRIPT)

default: sample.bin reverseshell.bin reverseshell_thread.bin bindshell.bin bindshell_thread.bin

sample.elf: sample.c
	$(CC) sample.c -o $@ $(CFLAGS)

sample.bin: sample.elf
	$(OBJCOPY) -O binary -j .start -j .text -j .data -j .rodata $< $@

reverseshell.elf:
	$(CC) reverseshell.c -o $@ $(CFLAGS)

reverseshell.bin: reverseshell.elf
	$(OBJCOPY) -O binary -j .start -j .text -j .data -j .rodata $< $@

reverseshell_thread.elf:
	$(CC) reverseshell_thread.c -o $@ $(CFLAGS)

reverseshell_thread.bin: reverseshell_thread.elf
	$(OBJCOPY) -O binary -j .start -j .text -j .data -j .rodata $< $@

bindshell.elf: bindshell.c
	$(CC) bindshell.c -o $@ $(CFLAGS)

bindshell.bin: bindshell.elf
	$(OBJCOPY) -O binary -j .start -j .text -j .data -j .rodata $< $@

bindshell_thread.elf: bindshell_thread.c
	$(CC) bindshell_thread.c -o $@ $(CFLAGS)

bindshell_thread.bin: bindshell_thread.elf
	$(OBJCOPY) -O binary -j .start -j .text -j .data -j .rodata $< $@

.PHONY: clean

clean:
	rm -f *.elf *.bin
